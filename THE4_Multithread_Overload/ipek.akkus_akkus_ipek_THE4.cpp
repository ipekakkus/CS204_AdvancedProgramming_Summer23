// CS 204 THE-4
// OVERLOADING OPERATIONS AND MULTITHREADS
// IPEK AKKUS 30800

#include <iostream>
#include <sstream>
#include <thread>
#include <queue>
#include <chrono> // for chrono::system_clock
#include <ctime> // for localtime, time_t, tm
#include <iomanip>

#include <random>
#include <time.h>

#include "PrintQueue.h"

using namespace std;

PrintQueue printQueue; // Shared queue for printing
mutex queueMutex, outputMutex; // Mutex to protect the printQueue and output
int maxPrintJobs;
int printID = 0;

int random_range(const int& min, const int& max) { // Given by the THE-4 file, used for page range and time range
    static mt19937 generator(time(0));
    uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

// Function to get the current time in HH:MM:SS format
string getCurrentTimeFormatted() {
    auto currentTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
    struct tm timeinfo;
    localtime_s(&timeinfo, &currentTime);

    stringstream ss;
    ss << setfill('0') << setw(2) << timeinfo.tm_hour << ":" << setw(2) << timeinfo.tm_min << ":" << setw(2) << timeinfo.tm_sec;

    return ss.str(); 
}

void userThread(int userID, int minTime, int maxTime, int minPages, int maxPages, int& printID) {
    // Page number and Time is randomly chosen by the user
    int randomPageNumber = random_range(minPages, maxPages);
    int randomTime = random_range(minTime, maxTime);

    while (true) {
        queueMutex.lock(); // Mutex is locked to check whether maxPrintJob number is reached or not
        if (printID == maxPrintJobs) {
            queueMutex.unlock();
            break;
        }
        printID = printID + 1; // printID should be incremented by 1, as the ID's will be consecutive
        PrintJobNode job(printID, randomPageNumber); // Create new job node
        printQueue = printQueue + job; // Dequeue the job using the += operator , enqueue to the rear of queue
        queueMutex.unlock();

        outputMutex.lock(); // Display current information about queue
        string currentTime = getCurrentTimeFormatted();
        cout << "User " << userID << " sent new print job with ID " << printID
            << " sent to the printer queue, number of pages: " << randomPageNumber
            << " (print queue size: " << printQueue.queueSize() << ") " << currentTime << endl;
        outputMutex.unlock();
        this_thread::sleep_for(chrono::seconds(randomTime)); // Each user thread should be wait some time after sending the Job
    }
}

void PrinterThread() {
    int counter = 0; // The counter is to check maxPrintJobs
    while (counter < maxPrintJobs) {
        queueMutex.lock();
        if (!printQueue.isEmpty()) {
            counter += 1;
            PrintJobNode job;
            printQueue = printQueue - job; // Dequeue the job using the -= operator

            // Display information about the current job
            outputMutex.lock();
            string startTime = getCurrentTimeFormatted();
            cout << "The printer started to print the job with ID: " << job.printJobID
                << ", number of pages: " << job.numberOfPages
                << " (queue size is: " << printQueue.queueSize() << ") " << startTime << endl;
            outputMutex.unlock();

            if (printQueue.queueSize() == 0) {
                cout << "The print queue is empty." << endl;
            }
            
            else {
                cout << "The print queue contains: " << endl;
                for (int i = 0; i < job.printJobID; i++) {
                    cout << "Print Job ID: " << job.printJobID << ", Pages: " << job.numberOfPages << endl;
                }
            }

            this_thread::sleep_for(chrono::seconds(job.numberOfPages)); // Simulation for print, 1 second for each page

            // Display information about the current job
            outputMutex.lock();
            string finishTime = getCurrentTimeFormatted();
            cout << endl << "The printer finished to print the job with ID: " << job.printJobID
                << ", number of pages: " << job.numberOfPages
                << " (queue size is: " << printQueue.queueSize() << ") " << finishTime << endl;
            outputMutex.unlock();
        }

        queueMutex.unlock();
    }
}

int main() {

    cout << "Please enter the max number of print jobs: ";
    cin >> maxPrintJobs;

    int minTime, maxTime;
    cout << "Please enter the min and max values for the time range between two print jobs:" << endl;
    cout << "Min: ";
    cin >> minTime;
    cout << "Max: ";
    cin >> maxTime;
    int randomTime = random_range(minTime, maxTime);

    int minPages, maxPages;
    cout << "Please enter the min and max values for the number of pages in a print job:" << endl;
    cout << "Min number of pages: ";
    cin >> minPages;
    cout << "Max number of pages: ";
    cin >> maxPages;
    cout << "Simulation starts " << getCurrentTimeFormatted() << endl;
    this_thread::sleep_for(chrono::seconds(randomTime)); // Simulation for print, 1 second for each page


    thread user_1(userThread, 1, minTime, maxTime, minPages, maxPages, ref(printID));
    thread user_2(userThread, 2, minTime, maxTime, minPages, maxPages, ref(printID));
    thread user_3(userThread, 3, minTime, maxTime, minPages, maxPages, ref(printID));
    thread printerThread(PrinterThread);

    user_1.join();
    user_2.join();
    user_3.join();
    printerThread.join();

    cout << "Simulation ended at: " << getCurrentTimeFormatted() << endl;
    return 0;

}

