// CS204_THE2_IPEKAKKUS_DLL.cpp 
// IPEK AKKUS 30800 
// Doubly Linked Lists Take Home Exam

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

//STRUCTS
struct stockNode {
    string resourceName;
    int resourceQuantity;
    stockNode* next;
    stockNode* prev;
};
struct consumpNode {
    char buildType;
    vector<int> consumpQtys;
    consumpNode* next;
    consumpNode* prev;
};
struct colonyNode {
    char buildType;
    int emptyBlocks2TheLeft;
    colonyNode* next;
    colonyNode* prev;
};

// FUNCTIONS TO READ AND STORE THE FILES
stockNode* readStockData() {
    stockNode* head = NULL;
    stockNode* tail = NULL;

    ifstream file; 
    string filename; 

    cout << "Please enter the stock file name: " << endl;
    cin >> filename; 

    while (!file.is_open()) { 

        file.open(filename); 

        if (!file.is_open()) { 
            cout << "Unable to open the file " << filename << ". Please enter the correct stock file name: " << endl;
            cin >> filename; 
        }
        else {
            break; // Exit the loop if the file is successfully opened
        }

    }
    string line;
    while (getline(file, line)) {
        // Split the line into resourceName and resourceQuantity
        string resourceName;
        int resourceQuantity;
        stringstream ss(line);
        ss >> resourceName >> resourceQuantity;

        // Create a new stock node
        stockNode* newNode = new stockNode;
        newNode->resourceName = resourceName;
        newNode->resourceQuantity = resourceQuantity;
        newNode->next = NULL;
        newNode->prev = tail;
        // Update the tail and head pointers
        if (tail == NULL) {
            head = newNode;
        }
        else {
            tail->next = newNode;
        }
        tail = newNode;
    }
    file.close();
    return head;
}
consumpNode* readConsumpData() {
    
    consumpNode* head = NULL;
    consumpNode* tail = NULL;

    ifstream file; 
    string filename; 

    cout << "Please enter the consumption file name: " << endl;
    cin >> filename; 

    while (!file.is_open()) { 

        file.open(filename); 

        if (!file.is_open()) { 
            cout << "Unable to open the file " << filename << ". Please enter the correct consumption file name: " << endl;
            cin >> filename; 
        }
        else {
            break; // Exit the loop if the file is successfully opened
        }

    }

    string line;
    while (getline(file, line)) {
        // Split the line into buildType and consumption quantities
        char buildType;
        vector<int> consumpQtys;
        stringstream ss(line);
        ss >> buildType;

        int qty;
        while (ss >> qty) { // since the second element is a vector, this part is different from the readStockData function
            consumpQtys.push_back(qty);
        }

        // Create a new consumption node
        consumpNode* newNode = new consumpNode;
        newNode->buildType = buildType;
        newNode->consumpQtys = consumpQtys;
        newNode->next = NULL;
        newNode->prev = tail;

        // Update the tail and head pointers
        if (tail == NULL) {
            head = newNode;
        }
        else {
            tail->next = newNode;
        }
        tail = newNode;
    }

    file.close();
    return head;
}
colonyNode* readColonyData() { // const string& filename
    
    colonyNode* head = nullptr;
    colonyNode* tail = nullptr;
    colonyNode* newNode = nullptr;

    ifstream file;
    string filename;

    cout << "Please enter the colony file name: " << endl; 
    cin >> filename; 

    while (!file.is_open()) { 

        file.open(filename); 

        if (!file.is_open()) { 
            cout << "Unable to open the file " << filename << ". Please enter the correct colony file name: " << endl;
            cin >> filename; 
        }
        else {
            break; // Exit the loop if the file is successfully opened
        }
              
    }

    string line;
    int currentIndex = 0;
    int emptyBlocks2TheLeft = 0;
    char buildType = ' ';

    while (getline(file, line)) {
        for (char c : line) {
            if (c == '-') {
                emptyBlocks2TheLeft++;
                currentIndex++;
            }
            else {
                if (buildType != c) {
                    // Create a new colony node when the building type changes
                    buildType = c;
                    newNode = new colonyNode;
                    newNode->buildType = buildType;
                    newNode->emptyBlocks2TheLeft = emptyBlocks2TheLeft;
                    newNode->next = nullptr;
                    newNode->prev = tail; 

                    if (tail == nullptr) {
                        head = newNode; 
                    } 
                    else {
                        tail->next = newNode; 
                    }
                    tail = newNode; 
                    emptyBlocks2TheLeft = 0;
                }
                else {
                    // Create a new colony node even if the building type is the same
                    newNode = new colonyNode;
                    newNode->buildType = buildType;
                    newNode->emptyBlocks2TheLeft = emptyBlocks2TheLeft;
                    newNode->next = nullptr;
                    newNode->prev = tail;

                    if (tail == nullptr) {
                        head = newNode;
                    }
                    else {
                        tail->next = newNode;
                    }
                    tail = newNode;
                    emptyBlocks2TheLeft = 0;
                }
                currentIndex++;
            }
        }
    }
    file.close();
    return head;
}

// OUTPUT FUNCTIONS
void displayRemainingStock(stockNode* stockHead) {
    cout << "Remaining Stock after Calculations:" << endl;
    stockNode* current = stockHead;
    while (current != nullptr) {
        cout << "Resource Name: " << current->resourceName << ", Remaining Quantity: " << current->resourceQuantity << endl;
        current = current->next;
    }
}
void displayMenu() {
    cout << "1. Construct a new building on the colony " << endl
        << "2. Destruct/Disassemble a building from the colony " << endl
        << "3. Print the colony " << endl
        << "4. Print the colony in reverse " << endl
        << "5. Print the colony while showing inner empty blocks " << endl
        << "6. Print the colony while showing inner empty blocks in reverse " << endl
        << "7. Print the stock " << endl
        << "8. Exit" << endl;
} // Function to display the menu
void printColony(colonyNode* head) {
    colonyNode* current = head;
    while (current != NULL) {
        cout << current->buildType;
        current = current->next;
    }
    cout << endl;
}
void printColonywithParanthesis(colonyNode* head) {
    colonyNode* current = head;
    while (current != NULL) {
        cout << "(" << current->emptyBlocks2TheLeft << ")" << current->buildType;
        current = current->next;
    }
    cout << endl;
}
void printColonyReverse(colonyNode* tail) {
    colonyNode* current = tail;
    while (current != nullptr) {
        cout << current->buildType;
        current = current->prev;  // Move to the previous node
    }
    cout << endl;  // Add a newline after printing the reversed colony
}
void printColonyWithEmpty(colonyNode* head) {
    colonyNode* current = head;
    while (current != nullptr) {
        // Print the dash lines (empty blocks)
        for (int i = 0; i < current->emptyBlocks2TheLeft; ++i) {
            cout << "-";
        }
        cout << current->buildType;  // Print the building type
        current = current->next;
    }
    cout << endl; // Add a newline after printing the colony
}
void printColonyReverseWithEmpty(colonyNode* tail) {
    colonyNode* current = tail;
    while (current != nullptr) {
        cout << current->buildType;  // Print the building type
        // Print the dash lines (empty blocks)
        for (int i = 0; i < current->emptyBlocks2TheLeft; ++i) {
            cout << "-";
        }
        current = current->prev;
    }
    cout << endl; // Add a newline after printing the colony
}
void printRemainingStock(stockNode* stockHead) {
    stockNode* current = stockHead;
    while (current != nullptr) {
        cout << current->resourceName << "(" << current->resourceQuantity << ")" << endl;
        current = current->next;
    }
}

// FUNCTION TO RELEASE THE MEMORY
void releaseAllMemory(stockNode* stockHead, consumpNode* consumpHead, colonyNode* colonyHead) { 
    stockNode* stockCurrent = stockHead; 
    while (stockCurrent != nullptr) {
        stockNode* temp = stockCurrent;
        stockCurrent = stockCurrent->next;
        delete temp;
    }
    consumpNode* consumpCurrent = consumpHead;
    while (consumpCurrent != nullptr) {
        consumpNode* temp = consumpCurrent;
        consumpCurrent = consumpCurrent->next;
        delete temp;
    }
    colonyNode* colonyCurrent = colonyHead;
    while (colonyCurrent != nullptr) {
        colonyNode* temp = colonyCurrent;
        colonyCurrent = colonyCurrent->next;
        delete temp;
    }
}

//FUNCTION FOR PERFORMING CALCULATIONS
stockNode* getNodeAt(stockNode* head, int index) {
    stockNode* current = head;
    int count = 0;
    while (current != nullptr) {
        if (count == index) {
            return current;
        }
        count++;
        current = current->next;
    }
    return nullptr;
}
bool performCalculations(stockNode* stockHead, consumpNode* consumpHead, colonyNode* colonyHead) {
    colonyNode* currentColonyNode = colonyHead;
    while (currentColonyNode != nullptr) {
        char currentBuildType = currentColonyNode->buildType;
        // Find the corresponding consumpNode for the current buildType
        consumpNode* currentConsumpNode = consumpHead;
        while (currentConsumpNode != nullptr && currentConsumpNode->buildType != currentBuildType) {
            currentConsumpNode = currentConsumpNode->next;
        }
        if (currentConsumpNode == nullptr) {
            cout << "No consumption data found for building type: " << currentBuildType << endl;
            currentColonyNode = currentColonyNode->next;
            continue;
        }
        for (size_t i = 0; i < currentConsumpNode->consumpQtys.size(); ++i) {
            stockNode* currentStockNode = getNodeAt(stockHead, i);
            int consumption = currentConsumpNode->consumpQtys[i];
            currentStockNode->resourceQuantity -= consumption;
            if (currentStockNode->resourceQuantity < 0) {
                cout << "Insufficient resource " << currentStockNode->resourceName << endl;
                return false; // Return false indicating insufficient resources
            }
        }
        currentColonyNode = currentColonyNode->next;
    }

    return true; // Return true indicating sufficient resources
}

//FUNCTIONS FOR THE CONSTRUCTIONS
void updateEmptyBlocks(colonyNode* colonyHead, int position) {
    colonyNode* temp = colonyHead;
    while (temp != NULL && position > 1) {
        temp = temp->next;
        position--;
    }
    if (temp != NULL && temp->next != NULL && temp->next->emptyBlocks2TheLeft > 0) {
        temp->next->emptyBlocks2TheLeft--; // Decrement the next node's emptyBlocks2TheLeft 
    }
} 
bool isValidBuildingType(char buildingType, consumpNode* consumpHead) {
    consumpNode* currentConsumpNode = consumpHead;
    while (currentConsumpNode != nullptr) {
        if (currentConsumpNode->buildType == buildingType) {
            return true; // Valid building type found
        }
        currentConsumpNode = currentConsumpNode->next;
    }
    return false; // No valid building type found
}
colonyNode* addNodeToBeginning(colonyNode* colonyHead, char buildType) {  
    colonyNode* newNode = new colonyNode; 
    newNode->prev = nullptr; 
    newNode->buildType = buildType; 
    newNode->emptyBlocks2TheLeft = 0;  // There is no empty cell before the FIRST cell
    newNode->next = nullptr; 
    newNode->next = colonyHead;
    if (colonyHead != NULL) {
        colonyHead->prev = newNode;
    }
    colonyHead = newNode;
    return colonyHead;
}
colonyNode* addBuildingNodeAtTPosition(colonyNode* colonyHead, char buildType, int targetPosition) { // Function to add a new building node at a specified position
    colonyNode* newNode = new colonyNode;
    
    newNode->prev = nullptr;  
    newNode->buildType = buildType; 
    newNode->emptyBlocks2TheLeft = 0;  // There is no empty cell before the FIRST cell
    newNode->next = nullptr;
     
    if (targetPosition != 1) {
        colonyNode* temp = colonyHead; 
        int emptyCellCount = 0;
        while (temp != nullptr && emptyCellCount < targetPosition - 1) { 
            emptyCellCount += temp->emptyBlocks2TheLeft;   // Count the empty cells indicated by currentEmptyCells
            temp = temp->next;               // Move to the next node
        }
        if (temp != nullptr) { 
            newNode->prev = temp; 
            newNode->next = temp->next; 
            if (temp->next != nullptr) { 
                temp->next->prev = newNode; 
            }
            temp->next = newNode; 
        } 
        else {
            cout << "Position " << targetPosition << " not found." << endl; 
            delete newNode; 
        }
    }
    else if (targetPosition == 1) {
        colonyNode* temp = colonyHead; 
        while (temp != nullptr){
            if (temp->next->emptyBlocks2TheLeft = 0) {   
                temp = temp->next;  
            }
            else {
                colonyHead = addNodeToBeginning(colonyHead, buildType); 
                break;
            }
            temp = temp->next;                  // Move to the next node
        }    
    }
    return colonyHead;
}
colonyNode* addBuildingNodeAtPosition(colonyNode* colonyHead, char buildType, int targetPosition) {
    colonyNode* newNode = new colonyNode;

    newNode->prev = nullptr;
    newNode->buildType = buildType;
    newNode->emptyBlocks2TheLeft = 0; // There is no empty cell before the FIRST cell
    newNode->next = nullptr;

    if (targetPosition != 1) {
        colonyNode* temp = colonyHead;
        int emptyCellCount = 0;

        // Find the node before which the new node should be inserted
        while (temp != nullptr && emptyCellCount < targetPosition - 1) {
            emptyCellCount += temp->emptyBlocks2TheLeft; // Count the empty cells indicated by currentEmptyCells
            temp = temp->next; // Move to the next node
        }

        if (temp != nullptr) {
            newNode->prev = temp;
            newNode->next = temp->next;
            if (temp->next != nullptr) {
                temp->next->prev = newNode;
                temp->next->emptyBlocks2TheLeft -= emptyCellCount - (targetPosition - 1); 
            }
            temp->next = newNode;
        }
        else {
            cout << "Position " << targetPosition << " not found." << endl;
            delete newNode;
        }
    }
    else {
        colonyNode* temp = colonyHead;

        // Find the first non-empty node to insert the new node before
        while (temp != nullptr && temp->emptyBlocks2TheLeft == 0) {
            temp = temp->next;
            temp->emptyBlocks2TheLeft--;
        }

        if (temp != nullptr) {
            newNode->next = temp;
            if (temp->prev != nullptr) {
                temp->prev->next = newNode;
            }
            else {
                colonyHead = newNode; // New node becomes the head of the colony
            }
            newNode->prev = temp->prev;
            temp->prev = newNode;
            
        }
        else {
            cout << "No suitable position found for building." << endl; 
            delete newNode;
        }

    }
    return colonyHead;
}
colonyNode* addBuildingNode(colonyNode* colonyHead, char buildType, int targetPosition) {
    colonyNode* newNode = new colonyNode;

    if (colonyHead == NULL) { // IF THE DLL IS EMPTY - NEW NODE WILL BE THE ONLY NODE
        colonyHead = addNodeToBeginning(colonyHead, buildType); 
    }
    else {
        colonyNode* temp = colonyHead;
        int totalEmptyBlocks = 0; // Initialize total empty blocks to 0

        // Calculate the total number of empty blocks in the colony linked list
        while (temp != nullptr) {
            totalEmptyBlocks += temp->emptyBlocks2TheLeft; //TOTAL EMPTY BLOCKS IS COUNTED, WILL BE USED TO FIND THE N'TH POSITION FOR CONSTRUCTION
            temp = temp->next;
        }

        // Check if targetPosition exceeds the total empty blocks - ADDING DASHES PROBLEM
        if (targetPosition > totalEmptyBlocks) { // (--HSS, position = 6)
            int requiredEmptyBlocks = targetPosition - totalEmptyBlocks - 1; // -1 COMES FROM THE BUILDING ITSELF
            // Insert newNode at the end
            temp = colonyHead;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            // Store the number of dash lines in the upcoming node
            newNode->buildType = buildType; // Set the actual buildType
            newNode->emptyBlocks2TheLeft = requiredEmptyBlocks;
            newNode->prev = temp;
            newNode->next = NULL;
            temp->next = newNode;
        }
        else { // IF THE TARGET POSITION IS WITHIN THE RANGE OF EXISTING EMPTY CELLS (--HSS---H , position = 4)
            colonyHead = addBuildingNodeAtPosition(colonyHead, buildType, targetPosition); 
        }
    }
    return colonyHead;
}
bool returnBackStock(stockNode* stockHead, consumpNode* consumpHead, char buildingType) { 
    // Find the corresponding consumpNode for the chosen building type 
    consumpNode* currentConsumpNode = consumpHead; 
    while (currentConsumpNode != nullptr && currentConsumpNode->buildType != buildingType) {  
        currentConsumpNode = currentConsumpNode->next; 
    }
    // Update the stock quantities based on the specific building type's consumption
    stockNode* currentStockNode = stockHead; 
    while (currentStockNode != nullptr) { 
        int consumpIndex = 0; // Index for accessing consumption quantities
        for (size_t i = 0; i < currentConsumpNode->consumpQtys.size(); i++) {  
            // Increment resourceQuantity by the corresponding consumption quantity
            currentStockNode->resourceQuantity += currentConsumpNode->consumpQtys[i];   
            currentStockNode = currentStockNode->next; // Move to the next stock node
            consumpIndex++; 
        }      
    }
    return true; // Resources have been returned to stock
}
bool consumeStock(stockNode* stockHead, consumpNode* consumpHead, char buildingType) {
    // Find the corresponding consumpNode for the chosen building type  
    consumpNode* currentConsumpNode = consumpHead;
    while (currentConsumpNode != nullptr && currentConsumpNode->buildType != buildingType) {
        currentConsumpNode = currentConsumpNode->next;
    }
    // Update the stock quantities based on consumption
    stockNode* currentStockNode = stockHead;
    bool isEnough = true;
    while (currentStockNode != nullptr) {
        int consumpIndex = 0; // Index for accessing consumption quantities
        for (size_t i = 0; i < currentConsumpNode->consumpQtys.size(); i++) {
            // Decrement resourceQuantity by the corresponding consumption quantity
            currentStockNode->resourceQuantity -= currentConsumpNode->consumpQtys[i];
            consumpIndex++;
            
            if (isEnough && currentStockNode->resourceQuantity < 0) {
                cout << "Insufficient resource " << currentStockNode->resourceName << endl;
                isEnough = false;
            }          
            currentStockNode = currentStockNode->next;
        }
    }
    return isEnough; // Resources have been returned to stock 
}
void constructBuilding(colonyNode*& colonyHead, stockNode* stockHead, consumpNode* consumpHead) {
    // Get user input for building type
    char buildingType;
    bool validBuildingType = false;
    cout << "Please enter the building type: " << endl ; 
    cin >> buildingType;

    while (!validBuildingType) {
        validBuildingType = isValidBuildingType(buildingType, consumpHead);

        if (!validBuildingType) { 
            cout << "Building type " << buildingType <<" is not found in the consumption DLL. Please enter a valid building type: " << endl;
            cin >> buildingType; 
        }
    }
    bool haveEnoughResource = consumeStock(stockHead, consumpHead, buildingType);   

    // Perform resource calculations
    if (haveEnoughResource) {  
        // Get user input for the position of the new building
        int targetPosition; 
        bool validPosition = false; 
        cout << "Please enter the index of the empty block where you want to construct a building of type " << buildingType << endl;
        cin >> targetPosition; 

        // Add the new building node to the colony DLL
        colonyHead = addBuildingNode(colonyHead, buildingType, targetPosition);
        
        // Update empty blocks to the left
        updateEmptyBlocks(colonyHead, targetPosition);
        
        cout << "Building of type "<< buildingType <<" has been added at the empty block number: " << targetPosition << endl;
    }
    else {
        cout << "Failed to add the building due to insufficient resources." << endl;
        returnBackStock(stockHead, consumpHead, buildingType);
    }
}

//FUNCTION FOR THE DESTRUCTIONS
bool isValid2Destruct(char buildingType, colonyNode* colonyHead) {
    colonyNode* currentColonyNode = colonyHead;
    while (currentColonyNode != nullptr) {
        if (currentColonyNode->buildType == buildingType) {
            return true; // Valid building type found
        }
        currentColonyNode = currentColonyNode->next;
    }
    return false; // No valid building type found
}
void destructBuilding(colonyNode*& colonyHead, stockNode* stockHead, consumpNode* consumpHead) {
    // Get user input for building type
    char buildingType; 
    bool validBuildingType = false; 
    cout << "Please enter the building type: " << endl; 
    cin >> buildingType; 

    while (!validBuildingType) {
        validBuildingType = isValid2Destruct(buildingType, colonyHead); 

        if (!validBuildingType) {
            cout << "The building of type " << buildingType << " is not found in the colony. " << endl;
            return;
        }
    }
    if (colonyHead == nullptr) { // Empty colony, nothing to destruct
        cout << "There is no building in the colony to destruct. " << endl;
        return;
    }
    // Find the node with the specified building type
    colonyNode* nodeToDelete = colonyHead; 
    while (nodeToDelete != nullptr && nodeToDelete->buildType != buildingType) { 
        nodeToDelete = nodeToDelete->next; 
    }
    if (nodeToDelete == nullptr) { 
        cout << "The building of type " << buildingType << " is not found in the colony. " << endl;
        return;
    }
    // Update the next node's emptyBlocks2TheLeft value
    if (nodeToDelete->next != nullptr) { 
        nodeToDelete->next->emptyBlocks2TheLeft += nodeToDelete->emptyBlocks2TheLeft+1; // Increment by one 
    }
    // Update colonyHead if needed
    if (colonyHead == nodeToDelete) {
        colonyHead = nodeToDelete->next;
    }
    // Update pointers in the adjacent nodes
    if (nodeToDelete->prev != nullptr) {
        nodeToDelete->prev->next = nodeToDelete->next;
    }
    if (nodeToDelete->next != nullptr) {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }
    // Delete the node
    cout << "The building of type " << buildingType << " has been deleted from the colony." << endl;
    returnBackStock(stockHead, consumpHead, buildingType); 
    delete nodeToDelete;
}

int main()
{
    stockNode* stockHead = readStockData();  // Read, store and print the resources from stockFile
    consumpNode* consumpHead = readConsumpData(); // Read, store and print the resources from consumptionFile
    colonyNode* colonyHead = readColonyData();  // Read, store and print the resources from colonyFile

    if (!performCalculations(stockHead, consumpHead, colonyHead)) {
        cout << "Failed to load the colony due to insufficient resources." << endl;
        cout << "Clearing the memory and terminating the program. " << endl;
        releaseAllMemory(stockHead, consumpHead, colonyHead); 
        return 0;
    } 
    else {         
        int menuchoice;

        cout << "Please enter your choice: " << endl;
        displayMenu();

        do {
            if (cin >> menuchoice) {
                if (menuchoice == 1) { // Construct a new building on the colony 
                    constructBuilding(colonyHead, stockHead, consumpHead); 
                }
                else if (menuchoice == 2) { // Destruct/Disassemble a building from the colony
                    destructBuilding(colonyHead, stockHead, consumpHead); 
                }
                else if (menuchoice == 3) { // Print the colony
                    cout << "Colony DLL:" << endl;
                    printColony(colonyHead);
                    printColonywithParanthesis(colonyHead);
                }
                else if (menuchoice == 4) { // Print the colony in reverse
                    cout << "(Reverse) Colony DLL:" << endl;
                    // Get the tail of the colony doubly linked list
                    colonyNode* colonyTail = colonyHead;
                    while (colonyTail->next != nullptr) {
                        colonyTail = colonyTail->next;
                    }
                    printColonyReverse(colonyTail);  // Call the function to print in reverse
                }
                else if (menuchoice == 5) { // Print the colony while showing inner empty blocks
                    cout << "Colony DLL: " << endl; 
                    printColonyWithEmpty(colonyHead); 
                }
                else if (menuchoice == 6) { // Print the colony while showing inner empty blocks in reverse 
                    cout << "(Reverse) Colony DLL:" << endl; 
                    colonyNode* colonyTail = colonyHead; 
                    while (colonyTail->next != nullptr) { 
                        colonyTail = colonyTail->next; 
                    }
                    printColonyReverseWithEmpty(colonyTail); 
                }
                else if (menuchoice == 7) { // Print the stock
                    cout << "Stock DLL:" << endl; 
                    printRemainingStock(stockHead); 
                }
                else if (menuchoice == 8) { // After couple of ending messages, the program terminates
                    cout << "Clearing the memory and terminating the program. " << endl; 
                    releaseAllMemory(stockHead, consumpHead, colonyHead); 
                    return 0;
                }
                else {
                    cout << "Invalid option number.";
                }
            }
            else {
                cout << "Invalid option number.";
                cin.clear(); // Clear the error state 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
            }
        } while (true);
    }

    return 0;
}
