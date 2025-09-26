#include <string> //for string
#include <vector> //for vector
#include <iostream> //for cout
#include "texteditor.h" //for TextEditor class
#include <fstream> //for ifstream
#include <sstream> //for stringstream
//Positions of data(rows) in csv file/vectr
int cputemppos=0;
int cpuusepos=1;
int gputemppos=2;
int gpuusepos=3;
int memtemppos=4;
int memusepos=5;
int cpuspeedpos=6;
int gpuspeedpos=7;
int memspeedpos=8;
int cpupowerpos=9;
int gpupowerpos=10;
int mempowerpos=11;
int ssdtemppos=12;
int ssdusepos=13;
int ssdspeedpos=14;
int ssdpowerpos=15;
/* ADD if you have extra time
std::vector<std::string> applications;
std::vector<std::vector<int>> percentages;
*/

std::vector<std::string> parseRow(const std::string& row) {
    std::vector<std::string> cells;
    std::stringstream ss(row);
    std::string cell;
    
    while (std::getline(ss, cell, ',')) {
        cells.push_back(cell);
    }
    
    return cells;
}

// Function to read CSV file and return vector of rows (each row is a vector)
std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> allRows;
    std::ifstream file(filename);
    std::string row;
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return allRows; // Return empty vector
    }
    
    while (std::getline(file, row)) {
        std::vector<std::string> parsedRow = parseRow(row);
        allRows.push_back(parsedRow);
    }
    
    file.close();
    return allRows;
}

// Function to print all CSV data
void printCSVData(const std::vector<std::vector<std::string>>& csvData) {
    for (size_t i = 0; i < csvData.size(); ++i) {
        std::cout << "Row " << i << ": ";
        for (size_t j = 0; j < csvData[i].size(); ++j) {
            std::cout << "[" << csvData[i][j] << "]";
            if (j < csvData[i].size() - 1) std::cout << " | ";
        }
        std::cout << std::endl;
    }
}

// Function to get a specific cell value
std::string getCell(const std::vector<std::vector<std::string>>& csvData, int row, int col) {
    if (row >= 0 && row < csvData.size() && col >= 0 && col < csvData[row].size()) {
        return csvData[row][col];
    }
    return ""; // Return empty string if out of bounds
}

// Function to get number of rows
int getRowCount(const std::vector<std::vector<std::string>>& csvData) {
    return csvData.size();
}

// Function to get number of columns in a specific row
int getColumnCount(const std::vector<std::vector<std::string>>& csvData, int row) {
    if (row >= 0 && row < csvData.size()) {
        return csvData[row].size();
    }
    return 0;
}



std::vector<std::vector<std::string>> previous=readCSV("sample.csv");
const char stop=',';
int main()
{
    int i=0;
    int k=0;
    int j=previous.size();
    std::ifstream file("file.txt");
    std::ofstream ofile("creation.txt");
    std::string line;
    while (std::getline(file, line))
    {
        if(line[i]!=stop)
        {
            switch(k){
                case 
            }
        }
    }
    file.close();
    ofile.close();
    std::ofstream
}
