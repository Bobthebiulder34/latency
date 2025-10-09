#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

// Positions of data(rows) in csv file/vector
int cputemppos = 0;
int cpuusepos = 1;
int gputemppos = 2;
int gpuusepos = 3;
int memtemppos = 4;
int memusepos = 5;
int cpuspeedpos = 6;
int gpuspeedpos = 7;
int memspeedpos = 8;
int cpupowerpos = 9;
int gpupowerpos = 10;
int mempowerpos = 11;
int ssdtemppos = 12;
int ssdusepos = 13;
int ssdspeedpos = 14;
int ssdpowerpos = 15;

std::vector<std::string> parseRow(const std::string& row) {
    std::vector<std::string> cells;
    std::stringstream ss(row);
    std::string cell;
    
    while (std::getline(ss, cell, ',')) {
        cells.push_back(cell);
    }
    
    return cells;
}

std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> allRows;
    std::ifstream file(filename);
    std::string row;
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return allRows;
    }
    
    while (std::getline(file, row)) {
        std::vector<std::string> parsedRow = parseRow(row);
        allRows.push_back(parsedRow);
    }
    
    file.close();
    return allRows;
}

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

std::string getCell(const std::vector<std::vector<std::string>>& csvData, int row, int col) {
    if (row >= 0 && row < csvData.size() && col >= 0 && col < csvData[row].size()) {
        return csvData[row][col];
    }
    return "";
}

int getRowCount(const std::vector<std::vector<std::string>>& csvData) {
    return csvData.size();
}

int getColumnCount(const std::vector<std::vector<std::string>>& csvData, int row) {
    if (row >= 0 && row < csvData.size()) {
        return csvData[row].size();
    }
    return 0;
}

// Parse incoming data format: "cpu temp 50"
struct SensorData {
    std::string component; // cpu, gpu, ram, ssd
    std::string metric;    // temp, usage, speed, power
    std::string value;
};

SensorData parseInput(const std::string& line) {
    SensorData data;
    std::stringstream ss(line);
    ss >> data.component >> data.metric >> data.value;
    return data;
}

int main() {
    // Read previous CSV data if it exists
    std::vector<std::vector<std::string>> previous = readCSV("sample.csv");
    
    // Read new data from file.txt
    std::ifstream infile("file.txt");
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open file.txt" << std::endl;
        return 1;
    }
    
    // Check if file.txt has new data (not just "Hello World!")
    std::string firstLine;
    std::getline(infile, firstLine);
    
    if (firstLine == "Hello World!") {
        std::cout << "No new data to process." << std::endl;
        infile.close();
        return 0;
    }
    
    // Open output file for processed data
    std::ofstream outfile("creation.txt", std::ios::app); // append mode
    if (!outfile.is_open()) {
        std::cerr << "Error: Cannot open creation.txt" << std::endl;
        infile.close();
        return 1;
    }
    
    // Process first line
    SensorData data = parseInput(firstLine);
    std::cout << "Processing: " << data.component << " " << data.metric << " = " << data.value << std::endl;
    
    // Write to creation.txt in CSV format
    outfile << data.component << "," << data.metric << "," << data.value << std::endl;
    
    // Process remaining lines in file.txt
    std::string line;
    while (std::getline(infile, line)) {
        if (!line.empty() && line != "Hello World!") {
            data = parseInput(line);
            std::cout << "Processing: " << data.component << " " << data.metric << " = " << data.value << std::endl;
            outfile << data.component << "," << data.metric << "," << data.value << std::endl;
        }
    }
    
    infile.close();
    outfile.close();
    
    // Reset file.txt back to "Hello World!"
    std::ofstream resetfile("file.txt");
    if (resetfile.is_open()) {
        resetfile << "Hello World!" << std::endl;
        resetfile.close();
        std::cout << "Reset file.txt to 'Hello World!'" << std::endl;
    } else {
        std::cerr << "Error: Cannot reset file.txt" << std::endl;
        return 1;
    }
    
    std::cout << "Processing complete!" << std::endl;
    return 0;
}
