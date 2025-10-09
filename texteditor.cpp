#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> parseRow(const std::string& row) {
    std::vector<std::string> cells;
    std::stringstream ss(row);
    std::string cell;
    while (std::getline(ss, cell, ',')) {
        cells.push_back(cell);
    }
    return cells;
}

int main() {
    std::ifstream infile("file.txt");
    if (!infile.is_open()) {
        std::cerr << "Error: Cannot open file.txt" << std::endl;
        return 1;
    }

    std::ofstream outfile("creation.txt", std::ios::app); // append mode
    if (!outfile.is_open()) {
        std::cerr << "Error: Cannot open creation.txt" << std::endl;
        infile.close();
        return 1;
    }

    std::string line;
    bool wrote = false;
    while (std::getline(infile, line)) {
        if (!line.empty()) {
            std::vector<std::string> cells = parseRow(line);
            for (size_t i = 0; i < cells.size(); ++i) {
                outfile << cells[i];
                if (i < cells.size() - 1) outfile << ",";
            }
            outfile << std::endl;
            wrote = true;
        }
    }
    infile.close();
    outfile.close();

    // Clear file.txt (reset to Hello World!)
    std::ofstream resetfile("file.txt");
    if (resetfile.is_open()) {
        resetfile << "Hello World!" << std::endl;
        resetfile.close();
        std::cout << "Reset file.txt to 'Hello World!'" << std::endl;
    } else {
        std::cerr << "Error: Cannot reset file.txt" << std::endl;
        return 1;
    }

    if (wrote)
        std::cout << "Appended CSV lines from file.txt to creation.txt and cleared file.txt." << std::endl;
    else
        std::cout << "No lines to append. file.txt cleared." << std::endl;
    return 0;
}
