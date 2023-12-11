// cs204_the_1.cpp 
// Ipek Akkus 30800

#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <map> 
#include <sstream>
#include <string>
#include <vector>  
#include <unordered_map> 

using namespace std;

struct Resource {
    string name;
    int quantity;
    int index;
};
struct Consumption {
    string buildingType;
    vector<int> requiredResources;
};
// Function to check validity of the file name
bool isValidFilename(const string& filename, const string& first, const string& last) {
    if (filename.size() <= first.size() + last.size()) {
        return false;
    }
    // Check if the filename starts with the given start (it will be specify as the file name changes i.e. stock, consumption, colony)
    for (size_t i = 0; i < first.size(); i++) {
        if (filename[i] != first[i]) {
            return false;
        }
    }
    // Check if the filename ends with the given last.
    size_t lastStart = filename.size() - last.size();
    for (size_t i = lastStart; i < filename.size(); i++) {
        if (filename[i] != last[i - lastStart]) {
            return false;
        }
    }
    // Check if the characters between the first and end are all digits.
    for (size_t i = first.size(); i < lastStart; i++) {
        if (!isdigit(filename[i])) {
            return false;
        }
    }
    return true;
}
// Function to read the resources from a text file.
vector<Resource> readResources(const string& filename) {
    // Open the file.
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file " << filename << " for reading." << endl;
        return {};
    }
    // Create a vector to store the resource information.
    vector<Resource> resources;
    // Read the file line by line.
    string resourceName;
    int resourceQuantity;
    while (file >> resourceName >> resourceQuantity) {
        // Create a resource object.
        Resource resource;
        resource.name = resourceName;
        resource.quantity = resourceQuantity;

        // Add the resource object to the vector.
        resources.push_back(resource);
    }
    return resources;
}
// Function to read the consumption from a text file.
vector<Consumption> readConsumption(const string& filename) {
    // Open the file.
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file " << filename << " for reading." << endl;
        return {};
    }
    // Create a vector to store the consumption information.
    vector<Consumption> consumptions;
    // Read the file line by line.
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        Consumption consumption;
        iss >> consumption.buildingType;

        int resourceQuantity;
        while (iss >> resourceQuantity) {
            consumption.requiredResources.push_back(resourceQuantity);
        }
        // Add the consumption object to the vector.
        consumptions.push_back(consumption);
    }
    return consumptions;
}
// Function to read the colony matrix from a text file.
vector<vector<string>> readColony(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file " << filename << " for reading." << endl;
        return {};
    }
    vector<vector<string>> colonyMatrix;
    string line;
    while (getline(file, line)) {
        vector<string> row;
        for (char c : line) {
            if (c == '-') {
                row.push_back("-");
            }
            else {
                string buildingType(1, c);
                row.push_back(buildingType);
            }
        }
        colonyMatrix.push_back(row);
    }
    return colonyMatrix;
}
// Function to read the colony matrix from a text file and classify building types with their quantities.
pair<vector<vector<string>>, map<string, int>> readColonyAndClassify(const string& filename, const vector<Consumption>& consumptions) { 
    vector<vector<string>> colonyMatrix = readColony(filename);  

    map<string, int> buildingQuantities; 

    for (const auto& consumption : consumptions) { 
        string buildingType = consumption.buildingType; 
        int quantity = 0; 
        for (const auto& row : colonyMatrix) { 
            for (const auto& cell : row) { 
                if (cell == buildingType) { 
                    quantity++; 
                }
            }
        }
        buildingQuantities[buildingType] = quantity; 
    }

    return { colonyMatrix, buildingQuantities }; 
}

void printColonyMatrix(const vector<vector<string>>& colonyMatrix) {
    cout << "Colony Matrix:" << endl;
    for (const auto& row : colonyMatrix) {
        for (const auto& cell : row) {
            cout << setw(5) << left << cell;
        }
        cout << endl;
    }
}
void printBuildingQuantities(const map<string, int>& buildingQuantities) {
    cout << "Building Quantities:" << endl;
    for (const auto& entry : buildingQuantities) {
        cout << setw(10) << left << entry.first << " " << entry.second << endl;
    }
}
void printResources(const vector<Resource>& resources) {
    // Print the resource information.
    cout << "Resource stock:" << endl;
    for (auto it = resources.begin(); it != resources.end(); it++) {
        cout << it->name << " " << it->quantity << endl;
    }
}
void printConsumptions(const vector<Consumption>& consumptions) {
    // Print the consumption information.
    for (const auto& consumption : consumptions) {
        // Print the building type.
        cout << consumption.buildingType << " ";

        // Print the resources.
        for (int quantity : consumption.requiredResources) {
            cout << quantity << " ";
        }
        cout << endl;
    }
}
void printColony(const vector<vector<string>>& colonyMatrix) {
    cout << "Colony:" << endl;
    for (const auto& row : colonyMatrix) { 
        for (const auto& cell : row) {
            cout << setw(5) << left << cell;
        }
        cout << endl;
    }
}

// Function to print the remaining resources after loading the colony.
void printRemainingResources(vector<Resource>& resources) { 
    cout << "Resource stock: " << endl;
    for (const auto& resource : resources) { 
        cout << resource.name << " " << resource.quantity << endl; 
    }
}
// Function to calculate remaining resources after loading the colony and check for negative quantities.
vector<Resource> calculateRemainingResources( vector<Resource>& resources, const map<string, int>& buildingQuantities, const vector<Consumption>& consumptions) {
    vector<Resource> remainingResources = resources;  // Create a copy of the original resources
    // Create a map to store the required materials for each building type in the colony
    unordered_map<string, vector<int>> requiredMaterials;  
    for (const auto& consumption : consumptions) {
        string buildingType = consumption.buildingType;
        requiredMaterials[buildingType] = consumption.requiredResources;
    }
    // Calculate the total required materials for each building type in the colony
    for (const auto& entry : buildingQuantities) {
        string buildingType = entry.first;
        int quantityRequired = entry.second;
        for (size_t i = 0; i < requiredMaterials[buildingType].size(); i++) {
            int materialQuantity = requiredMaterials[buildingType][i];
            remainingResources[i].quantity -= (quantityRequired * materialQuantity);          
            if (remainingResources[i].quantity < 0) {
                cout << "Not enough " << remainingResources[i].name << " to construct the colony." << endl;
                remainingResources.clear(); // Clear the vector to indicate insufficient resources
                return remainingResources;  // Return the updated vector
            }
        }
    }
    return remainingResources; // Return the updated vector
}
// Function to calculate required materials for constructing buildings in the colony for a specific resource.
vector<vector<int>> getRequiredMaterial(const vector<vector<string>>& colonyMatrix, const vector<Consumption>& consumptions, const vector<Resource>& resources, const Resource& resource) {
    vector<vector<int>> requiredMaterialsMatrix(colonyMatrix.size(), vector<int>(colonyMatrix[0].size(), 0));
    // Find the resource index in the 'resources' vector
    int resourceIndex = -1;
    for (size_t i = 0; i < resources.size(); ++i) {
        if (resources[i].name == resource.name) {
            resourceIndex = static_cast<int>(i);
            break;
        }
    }
    // Return an empty matrix if the resource is not found
    if (resourceIndex == -1) {
        return requiredMaterialsMatrix;
    }
    for (size_t row = 0; row < colonyMatrix.size(); ++row) {
        for (size_t col = 0; col < colonyMatrix[row].size(); ++col) {
            if (colonyMatrix[row][col] != "-") { // Check if the cell is not empty
                const string& buildingType = colonyMatrix[row][col];
                // Find the consumption data for the building type
                const Consumption* consumption = nullptr;
                for (const auto& c : consumptions) {
                    if (c.buildingType == buildingType) {
                        consumption = &c;
                        break;
                    }
                }
                if (consumption != nullptr) { // Building type found in consumptions
                    int requiredQuantity = consumption->requiredResources[resourceIndex];
                    int totalRequired = requiredQuantity * resource.quantity;
                    requiredMaterialsMatrix[row][col] = totalRequired;
                }
            }
        }
    }
    return requiredMaterialsMatrix;
}
vector<vector<vector<int>>> getRequiredMaterialsMatrices(const vector<Consumption>& consumptions, vector<Resource>& resources, vector<vector<string>>& colonyMatrix) {
    vector<vector<vector<int>>> allRequiredMaterialsMatrices;

    for (const auto& resource : resources) {
        vector<vector<int>> requiredMaterialsMatrix = getRequiredMaterial(colonyMatrix, consumptions, resources, resource);
        for (auto& row : requiredMaterialsMatrix) {
            for (int& requiredQuantity : row) {
                if (resource.quantity != 0) {
                    requiredQuantity /= resource.quantity;
                }
            }
        }
        allRequiredMaterialsMatrices.push_back(requiredMaterialsMatrix);
    }

    return allRequiredMaterialsMatrices;
}


// Function to print the consumption of a resources one by one by each building in the colony.
void printResourceConsumption(const vector<Consumption>& consumptions, vector<Resource>& resources, const vector<vector<string>>& colonyMatrix) {
    for (const auto& resource : resources) {
        cout << "Consumption of resource " << resource.name << " by each building in the colony:" << endl;
        vector<vector<int>> requiredMaterialsMatrix = getRequiredMaterial(colonyMatrix, consumptions, resources, resource); 
        for (const auto& row : requiredMaterialsMatrix) {
            for (int requiredQuantity : row) {
                if (resource.quantity == 0){
                    int consumptionPerBuilding = requiredQuantity;
                    // Each value is already divided by numBuildingsOfType in getRequiredMaterials function
                    cout << setw(5) << left << consumptionPerBuilding << "";
                }
                else {
                    int consumptionPerBuilding = requiredQuantity / resource.quantity;
                    // Each value is already divided by numBuildingsOfType in getRequiredMaterials function
                    cout << setw(5) << left << consumptionPerBuilding << "";
                }
            }
            cout << endl;
        }
    }
}
// Function to calculate specific resource consumption for each building at each cell in the colony.
void printSpecificCellConsumption(const vector<Consumption>& consumptions, const vector<Resource>& resources, const vector<vector<string>>& colonyMatrix, const string& resourceName) {
    // Find the specific resource in the resources vector.
    const Resource* specificResource = nullptr;
    for (const auto& resource : resources) {
        if (resource.name == resourceName) {
            specificResource = &resource;
            break;
        }
    }

    if (specificResource != nullptr) {
        cout << "Consumption of resource " << specificResource->name << " by each building in the colony:" << endl;
        vector<vector<int>> requiredMaterialsMatrix = getRequiredMaterial(colonyMatrix, consumptions, resources, *specificResource);
        for (const auto& row : requiredMaterialsMatrix) {
            for (int requiredQuantity : row) {
                if (specificResource->quantity == 0) {
                    int consumptionPerBuilding = requiredQuantity;
                    // Each value is already divided by numBuildingsOfType in getRequiredMaterials function
                    cout << setw(5) << left << consumptionPerBuilding << "";
                }
                else {
                    int consumptionPerBuilding = requiredQuantity / specificResource->quantity;
                    // Each value is already divided by numBuildingsOfType in getRequiredMaterials function
                    cout << setw(5) << left << consumptionPerBuilding << "";
                }
            }
            cout << endl;
        }
    }
    else {
        cout << "The resource " << resourceName << " was not found in the resources stock." << endl;
    }
}

// FUNCTIONS FOR MENUCHOICE == 1

// Function to check for an empty cell on the colony file
bool hasEmptyCell(const vector<vector<string>>& colonyMatrix) {

    for (const auto& row : colonyMatrix) {
        for (const auto& cell : row) {
            if (cell == "-") {
                return true;
            }
        }
    }
    return false;
}
// Function to prompt the user to choose the type of the building.
string chooseBuildingType(const vector<Consumption>& consumptions) {
    string buildingType;
    bool validBuildingType = false;
    cout << "Please enter the type of the building that you want to construct: " << endl;
    cin >> buildingType;

    // Loop until a valid building type is entered
    while (!validBuildingType) {
        for (const auto& consumption : consumptions) {
            if (consumption.buildingType == buildingType) {
                validBuildingType = true;
                break;
            }
        }
        if (!validBuildingType) {
            cout << "Invalid building type, please enter a valid building type: " << endl;
            cin >> buildingType;
        }
    }
    return buildingType;
}
// Function to prompt the user to enter the number of blocks (cells) that the building will occupy.
int enterNumBlocks(const vector<vector<string>>& colonyMatrix) {
    int numBlocks;
    int emptyCells = 0;
    for (const auto& row : colonyMatrix) {
        for (const auto& cell : row) {
            if (cell == "-") {
                emptyCells++;
            }
        }
    }
    cout << "Please enter the number of cells that the building will occupy: " << endl;
    cin >> numBlocks;
    do {
        if ((numBlocks <= 0) || (numBlocks > emptyCells)) {
            cout << "Invalid number of cells, please enter a valid number of cells: " << endl;
            cin >> numBlocks;
        }
        else {
            break; // Valid input, exit the loop
        }
    } while (true);
    return numBlocks;
}
// Function to ask the user for the row and column index until valid ones are entered.
vector<pair<int, int>> enterBuildingIndices(const vector<vector<string>>& colonyMatrix, int& numBlocks) {
    vector<pair<int, int>> buildingIndices;
    bool displayPrompt = true;

    for (int i = 0; i < numBlocks;) {
        int row, col;

        if (displayPrompt) {
            cout << "Please enter the row and the column index of the cell number " << i + 1 << " : " << endl;
        }
        cin >> row >> col;

        if (row >= 0 && row < colonyMatrix.size() && col >= 0 && col < colonyMatrix[0].size() && colonyMatrix[row][col] == "-") {
            buildingIndices.emplace_back(row, col);
            ++i;
            displayPrompt = true; // Reset the flag for the next iteration
        }
        else {
            cout << "Invalid row or column index, please enter a valid row and column index:" << endl;
            displayPrompt = false; // Don't display the initial prompt again
        }
    }
    return buildingIndices;
}
// Function to check if a cell is empty
bool isCellEmpty(const vector<vector<string>>& colonyMatrix, int row, int col) {
    return colonyMatrix[row][col] == "-";
}
// Function to ask the user for new indices until all blocks of the building are placed in empty cells
void askForNewIndices(vector<pair<int, int>>& buildingIndices, const vector<vector<string>>& colonyMatrix, int numBlocks) {
    while (buildingIndices.size() < numBlocks) {
        cout << "Please enter the row and the column index of the cell number " << buildingIndices.size() + 1 << ": " << endl;
        int row, col;

        // Check if the user input is valid
        if (cin >> row >> col) {
            // Check if the selected indices are within the colonyMatrix boundaries
            if (row >= 0 && row < colonyMatrix.size() && col >= 0 && col < colonyMatrix[0].size()) {
                // Check if the chosen cell is empty
                if (isCellEmpty(colonyMatrix, row, col)) {
                    buildingIndices.push_back(make_pair(row, col));
                }
                else {
                    cout << "The cell is not empty, please enter the row and the column index of another cell: " << endl;
                    //cout << "Invalid row or column index, please enter a valid row and column index:" << endl;
                }
            }
            else {
                cout << "Invalid row or column index, please enter a valid row and column index:" << endl;
            }
        }
        else {
            cout << "Invalid row or column index, please enter a valid row and column index:" << endl;
        }
        // Clear the input stream by discarding characters until a newline is encountered
        cin.clear();
        while (cin.get() != '\n')
            continue;
    }
}
//Function to construct new building in the colony
void constructBuilding(vector<vector<string>>& colonyMatrix, const vector<Consumption>& consumptions, vector<Resource>& resources) {
    if (!hasEmptyCell(colonyMatrix)) {
        cout << "There are no empty cells in the colony. Can not add a new building." << endl;
        return;
    }

    string buildingType = chooseBuildingType(consumptions);
    int numBlocks = enterNumBlocks(colonyMatrix);
    vector<pair<int, int>> buildingIndices;

    // Ask the user for new indices until all blocks of the building are placed in empty cells
    askForNewIndices(buildingIndices, colonyMatrix, numBlocks);

    // Temporary colony matrix to store the changes in case of insufficient resources
    vector<vector<string>> tempColonyMatrix = colonyMatrix;

    // Update the temporary colonyMatrix with the new building
    for (const auto& index : buildingIndices) {
        int row = index.first;
        int col = index.second;
        tempColonyMatrix[row][col] = buildingType;
    }

    // Deduct the required resources for constructing the building from the resources vector
    int buildingIndex = -1;
    for (size_t i = 0; i < consumptions.size(); i++) {
        if (consumptions[i].buildingType == buildingType) {
            buildingIndex = i;
            break;
        }
    }

    if (buildingIndex != -1) {
        int numBuildingsOfType = buildingIndices.size();
        for (size_t i = 0; i < consumptions[buildingIndex].requiredResources.size(); i++) {
            resources[i].quantity -= consumptions[buildingIndex].requiredResources[i] * numBuildingsOfType;
            if (resources[i].quantity < 0) {
                cout << "Not enough " << resources[i].name << " to construct the colony." << endl;
                return; // Abort the construction process
            }
        }
        // If there are enough resources, update the actual colonyMatrix
        colonyMatrix = tempColonyMatrix;
        cout << "The building is added to the colony." << endl;
    }
    else {
        cout << "Building type not found in consumption data." << endl;
    }
}

// FUNCTIONS FOR MENUCHOICE == 2

// Function to check if there is at least one cell with a building in the colony.
bool hasBuilding(const vector<vector<string>>& colonyMatrix) {
    for (const auto& row : colonyMatrix) {
        for (const auto& cell : row) {
            if (cell != "-") {
                return true;
            }
        }
    }
    return false;
}
// Function to prompt the user to choose a cell with a building to destruct.
void chooseCellToDestruct(const vector<vector<string>>& colonyMatrix, int& row, int& col) {
    cout << "Please enter the row and the column index of the cell that you want to remove: " << endl;
    cin >> row >> col;
    while (true) {
        if (row >= 0 && row < colonyMatrix.size() && col >= 0 && col < colonyMatrix[0].size()) {
            if (colonyMatrix[row][col] != "-") {
                break; // Valid input, exit the loop
            }
            else {
                cout << "The cell is already empty, please enter the row and the column index of another cell:" << endl;
                cin >> row >> col;
            }
        }
        else {
            cout << "Invalid row or column index, please enter a valid row and column index: " << endl;
            cin >> row >> col;
        }
    }
}
// Function to destruct the building in the chosen cell and update the stock accordingly.
void destructBuilding(vector<vector<string>>& colonyMatrix, vector<Resource>& resources, const vector<Consumption>& consumptions, int row, int col) {
    string buildingType = colonyMatrix[row][col];
    // Update the colonyMatrix to mark the cell as empty ("-").
    colonyMatrix[row][col] = "-";    
    // Find the corresponding building in the consumption data.
    int buildingIndex = -1;
    for (size_t i = 0; i < consumptions.size(); i++) {
        if (consumptions[i].buildingType == buildingType) {
            buildingIndex = i;
            break;
        }
    }   
    if (buildingIndex != -1) {
        // Get the number of buildings of the specific type that is being destructed.
        map<string, int> buildingCounts;
        for (const auto& row : colonyMatrix) {
            for (const auto& cell : row) {
                if (cell != "-") {
                    buildingCounts[cell]++;
                }
            }
        }
        cout << "The building is removed, and the corresponding resources are added back to the stock." << endl;
    } else {
        cout << "Building type not found in consumption data." << endl;
    }
}
// Function to calculate and print the consumption of a specific resource by each building in the colony.
void printSpecificResourceConsumption(const vector<Consumption>& consumptions, const vector<Resource>& resources, const vector<vector<string>>& colonyMatrix, const string& resourceName) {
    // Find the specific resource in the resources vector.
    const Resource* specificResource = nullptr;
    for (const auto& resource : resources) {
        if (resource.name == resourceName) {
            specificResource = &resource;
            break;
        }
    }

    if (specificResource != nullptr) {
        vector<vector<int>> requiredMaterialsMatrix = getRequiredMaterial(colonyMatrix, consumptions, resources, *specificResource); 
        cout << "Consumption of resource " << specificResource->name << " by each building in the colony:" << endl;
        for (size_t i = 0; i < colonyMatrix.size(); i++) {
            for (size_t j = 0; j < colonyMatrix[i].size(); j++) {
                if (colonyMatrix[i][j] != "-") { 
                    int buildingIndex = -1; 
                    for (size_t k = 0; k < consumptions.size(); k++) { 
                        if (consumptions[k].buildingType == colonyMatrix[i][j]) { 
                            buildingIndex = k; 
                            break;
                        }
                    }
                    if (buildingIndex != -1) { 
                        cout << setw(5) << left << requiredMaterialsMatrix[i][j] << ""; 
                    }
                    else {
                        cout << setw(5) << left << 0 << ""; 
                    }
                }
                else {
                    cout << setw(5) << left << 0 << "";
                }
            }
            cout << endl;
        }
    }
    else {
        cout << "The resource " << resourceName << " was not found in the resources stock." << endl;
    }
}

// Function to calculate the total consumption of each resource and update the resource stock.
void calculateAndPrintResourceConsumption(const vector<Consumption>& consumptions, const vector<Resource>& resources, const vector<vector<string>>& colonyMatrix) {
    vector<Resource> resourcesCopy = resources; // Create a copy of the resources vector to modify.

    // Iterate through each resource and calculate its total consumption.
    for (const auto& resource : resourcesCopy) {
        int totalConsumption = 0;
        // Call the existing function to get the requiredMaterialsMatrix for the specific resource.
        vector<vector<int>> requiredMaterialsMatrix = getRequiredMaterial(colonyMatrix, consumptions, resourcesCopy, resource);
        // Calculate the total consumption for the current resource.
        for (size_t i = 0; i < colonyMatrix.size(); i++) {
            for (size_t j = 0; j < colonyMatrix[i].size(); j++) {
                if (colonyMatrix[i][j] != "-") {
                    totalConsumption += requiredMaterialsMatrix[i][j];
                }
            }
        }
        // Subtract the total consumption from the resource stock.
        for (auto& res : resourcesCopy) {
            if (res.name == resource.name) {
                res.quantity = totalConsumption;
                break;
            }
        }
    }
    // Call the function to print the updated resource stock.
    printResources(resourcesCopy);
}

int main() {

    //Welcoming message
    cout << "Welcome to the colony management system " << endl;

    string stockFile, consumptionFile, colonyFile;

    // FIRST FILE AND ITS READING OPERATIONS
    cout << "Please enter file name for resources stock: " << endl;
    cin >> stockFile;
    while (!isValidFilename(stockFile, "stock", ".txt")) {
        cout << "Unable to open the file " << stockFile << " for reading." << endl;
        cout << "Please enter the correct file name: " << endl;
        cin >> stockFile;
    }
    // Read, store and print the resources from stockFile
    vector<Resource> resources = readResources(stockFile);
    cout << "Available resources loaded from " << stockFile << endl;
    printResources(resources);

    // SECOND FILE AND ITS READING OPERATIONS
    cout << "Please enter file name for resource consumption per building type: " << endl;
    cin >> consumptionFile;
    while (!isValidFilename(consumptionFile, "consumption", ".txt")) {
        cout << "Unable to open the file " << consumptionFile << " for reading." << endl;
        cout << "Please enter the correct file name: " << endl;
        cin >> consumptionFile;
    }
    // Read, store and print the resources from consumptionFile
    vector<Consumption> consumptions = readConsumption(consumptionFile);
    cout << "Resources consumption per building type loaded from " << consumptionFile << endl;
    cout << "Resources consumption per building type:" << endl;
    printConsumptions(consumptions);


    // THIRD FILE AND ITS READING OPERATIONS
    cout << "Please enter file name for colony: " << endl;
    cin >> colonyFile;
    while (!isValidFilename(colonyFile, "colony", ".txt")) {
        cout << "Unable to open the file " << colonyFile << " for reading." << endl;
        cout << "Please enter the correct file name: " << endl;
        cin >> colonyFile;
    }
    // Read, store and print the resources from colonyFile
    pair<vector<vector<string>>, map<string, int>> colonyData = readColonyAndClassify(colonyFile, consumptions);
    vector<vector<string>> colonyMatrix = colonyData.first;
    map<string, int> buildingQuantities = colonyData.second;

    // Calculate remaining resources after loading the colony and check for negative quantities
    vector<Resource> remainingResources = calculateRemainingResources(resources, buildingQuantities, consumptions);
    if (remainingResources.empty()) {
        //Terminating the program due to insufficient resources. 
        cout << "Not enough resources to build this colony. " << endl;
        cout << "Thank you for using the colony management system. The program will terminate." << endl;
        cout << "Goodbye!" << endl;
        return 1; // Return a non-zero value to indicate an error.
    }
    else {
        cout << "Colony loaded from " << colonyFile << endl;
        printColony(colonyMatrix);
    }
    // Print the remaining resources after loading the colony
    cout << "Resources stock after loading the colony: " << endl;
    printRemainingResources(remainingResources);
    printResourceConsumption(consumptions, resources, colonyMatrix);

    int menuchoice;

    cout << "Please enter an option number: " << endl;
    cout << "1. Construct a new building on the colony. " << endl
        << "2. Destruct/Disassemble a building from the colony. " << endl
        << "3. Print the colony. " << endl
        << "4. Print the consumption of all resources by each building in the colony. " << endl
        << "5. Print the consumption of a specific resource by each building in the colony. " << endl
        << "6. Print the resources stock." << endl
        << "7. Exit the program." << endl;
    
    do {
        //cout << "Please enter an option number: " << endl;
        if (cin >> menuchoice) {
            if (menuchoice == 1) { // Construct buildings in empty cells, if any
                constructBuilding(colonyMatrix, consumptions, resources); 
             }
            else if (menuchoice == 2) { // Destruct/Disassemble a building
                // If there is at least one occupied cell, user can destruct
                if (!hasBuilding(colonyMatrix)) {
                    cout << "There are no buildings in the colony. Can not remove a building." << endl;
                }
                else {
                    int row, col;
                    chooseCellToDestruct(colonyMatrix, row, col);
                    destructBuilding(colonyMatrix, resources, consumptions, row, col);
                }
            }
            else if (menuchoice == 3) { //displaying the colony
                printColony(colonyMatrix);
            }
            else if (menuchoice == 4) { //Consumption of all resources by each building in the colony
                printResourceConsumption(consumptions, resources, colonyMatrix);
            }
            else if (menuchoice == 5) { // Calculate and print the consumption for the specific resource chosen by the user
                string resourceName;
                cout << "Please enter the type of the resource: " << endl;
                cin >> resourceName;
                printSpecificCellConsumption(consumptions, resources, colonyMatrix, resourceName); 
            }
            else if (menuchoice == 6) {   //printing the remaining (updated) resource stock
                vector<Resource> remainingResources1 = calculateRemainingResources(resources, buildingQuantities, consumptions);
                printRemainingResources(remainingResources1);
            }
            else if (menuchoice == 7) {   // After couple of ending messages, the program terminates
                cout << "Thank you for using the colony management system. The program will terminate." << endl;
                cout << "Goodbye!" << endl;
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
        cout << "Please enter an option number: " << endl;
    } while (true);

    return 0;
}
