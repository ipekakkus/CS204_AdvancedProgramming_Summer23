#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <limits>
//#include "DynStack.cpp"

using namespace std;

struct WordFrequency {
    string word;
    int frequency;
    WordFrequency(const string& w, int f) : word(w), frequency(f) {}
};
void countWordOccurrences(const string& filename, stack<WordFrequency>& frequencyStack) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cout << "Failed to open the file: " << filename << endl;
        return;
    }

    string line;
    stack<WordFrequency> tempFrequencyStack;  // Temporary stack to hold updated frequencies

    while (getline(inputFile, line)) {  // Read the file line by line
        istringstream iss(line);
        string word;
        while (iss >> word) {  // Tokenize using whitespace (space or tab)
            bool found = false;
            while (!frequencyStack.empty()) {
                WordFrequency wordFreq = frequencyStack.top();
                frequencyStack.pop();

                if (wordFreq.word == word) { // When alread existing word is encountered again
                    wordFreq.frequency++;
                    tempFrequencyStack.push(wordFreq);
                    found = true;
                    break;
                }
                tempFrequencyStack.push(wordFreq);  // Push non-matching word back onto temp stack
            }
            // Restore the original order of elements in frequencyStack
            while (!tempFrequencyStack.empty()) {
                frequencyStack.push(tempFrequencyStack.top());
                tempFrequencyStack.pop();
            }
            if (!found) {
                frequencyStack.push(WordFrequency(word, 1));  // Push new word with frequency 1
            }
        }
    }
    inputFile.close();
}
void printWordFrequencies(stack<WordFrequency> frequencyStack) {
    stack<WordFrequency> reverseStack;
    while (!frequencyStack.empty()) {
        reverseStack.push(frequencyStack.top());
        frequencyStack.pop();
    }
    while (!reverseStack.empty()) {
        WordFrequency wordFreq = reverseStack.top();
        reverseStack.pop();
        cout << wordFreq.word << ": " << wordFreq.frequency << " occurrences" << endl;
    }
}
void compareWordFrequencies(stack<WordFrequency>& primaryStack, stack<WordFrequency>& secondaryStack) {
    stack<WordFrequency> commonWordsStack;

    // Create a temporary stack to store and manipulate secondary frequencies
    stack<WordFrequency> tempSecondaryStack = secondaryStack;

    // Iterate through the primary stack of word frequencies
    while (!primaryStack.empty()) {
        WordFrequency wordFreqPrimary = primaryStack.top();
        primaryStack.pop();

        bool found = false; // Flag to track if a common word is found

        stack<WordFrequency> tempStack;  // Temporary stack to hold unmatched words

        // Search for matching word in the tempSecondaryStack
        while (!tempSecondaryStack.empty()) {
            WordFrequency wordFreqSecondary = tempSecondaryStack.top();
            tempSecondaryStack.pop();

            if (wordFreqSecondary.word == wordFreqPrimary.word) {
                found = true;
                int minFrequency;
                // Calculate the common frequency as the minimum of the two frequencies
                if (wordFreqPrimary.frequency < wordFreqSecondary.frequency) {
                    minFrequency = wordFreqPrimary.frequency;
                }
                else {
                    minFrequency = wordFreqSecondary.frequency;
                }
                commonWordsStack.push(WordFrequency(wordFreqPrimary.word, minFrequency));
                break;
            }
            else {
                tempStack.push(wordFreqSecondary);  // Push non-matching word back onto temp stack
            }
        }

        // Restore the original order of elements in tempSecondaryStack
        while (!tempStack.empty()) {
            tempSecondaryStack.push(tempStack.top());
            tempStack.pop();
        }

        // If not found, push the word back onto the primary stack
        if (!found) {
            tempSecondaryStack = secondaryStack; // Restore tempSecondaryStack for the next iteration
        }
    }

    // Display the comparison results
    while (!commonWordsStack.empty()) {
        WordFrequency commonWordFreq = commonWordsStack.top();
        commonWordsStack.pop();
        cout << "The word \"" << commonWordFreq.word << "\" occurred at least " << commonWordFreq.frequency << " time(s) in both files." << endl;
    }
}


void askInput(int number, string& filename) {
    ifstream file;
    string fileorder;

    if (number == 1) {
        fileorder = "first";
    }
    else if (number == 2) {
        fileorder = "second";
    }

    while (!file.is_open()) {
        cout << "Enter the " << fileorder << " file name: " << endl;
        cin >> filename;
        file.open(filename);
    }
}

int main() {

    cout << "This program finds the common words of two files using stacks." << endl << "---" << endl;

    string filename1, filename2;

    askInput(1, filename1);
    askInput(2, filename2);

    stack<WordFrequency> frequencyStackFile1;
    countWordOccurrences(filename1, frequencyStackFile1);

    stack<WordFrequency> frequencyStackFile2;
    countWordOccurrences(filename2, frequencyStackFile2);
    /*
    cout << "Word frequencies in file 1:" << endl;
    printWordFrequencies(frequencyStackFile1);

    cout << "Word frequencies in file 2:" << endl;
    printWordFrequencies(frequencyStackFile2);
    */

    int menuchoice; 
    do {
        cout << "Choose with respect to which file the result will be sorted to (1: first file, 2: second file):" << endl;
        if (cin >> menuchoice) {
            if (menuchoice == 1 || menuchoice == 2) {
                cout << "---" << endl;
                if (menuchoice == 1) {
                    compareWordFrequencies(frequencyStackFile1, frequencyStackFile2); 
                }
                else if (menuchoice == 2) { 
                    compareWordFrequencies(frequencyStackFile2, frequencyStackFile1); 
                }
                break;  // Exit the loop if a valid choice is entered
            }
            else {
                cout << "---" << endl << "Invalid choice " << endl << "---" << endl;
            }
        }
        else {
            cout << "---" << endl << "Invalid choice " << endl << "---" << endl;
            cin.clear(); // Clear the error state 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input 
        }
    } while (true);

    
    return 0;
}
