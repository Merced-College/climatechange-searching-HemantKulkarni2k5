#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "StateClimate.h"

// Binary search function to find a state by FIPS code
int binarySearch(const std::vector<StateClimate>& data, int fips) {
    int left = 0, right = data.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (data[mid].getFips() == fips)
            return mid; // Found
        else if (data[mid].getFips() < fips)
            left = mid + 1;
        else
            right = mid - 1;
    }
    return -1; // Not found
}

int main() {
    std::vector<StateClimate> climateData;
    std::ifstream file("climdiv_state_year.csv");
    
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    std::getline(file, line); // Skip header line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        int fips, year;
        double temp, tempc;
        char comma;

        ss >> fips >> comma >> year >> comma >> temp >> comma >> tempc;
        climateData.emplace_back(fips, year, temp, tempc);
    }
    file.close();

    // Sort the data by FIPS code (required for binary search)
    std::sort(climateData.begin(), climateData.end(), [](const StateClimate& a, const StateClimate& b) {
        return a.getFips() < b.getFips();
    });

    // User search loop
    while (true) {
        std::cout << "Enter a FIPS code to search (or 'exit' to quit): ";
        std::string input;
        std::cin >> input;
        
        if (input == "exit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }

        int fips;
        try {
            fips = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid input. Please enter a numeric FIPS code." << std::endl;
            continue;
        }

        int index = binarySearch(climateData, fips);
        if (index != -1) {
            climateData[index].display();
        } else {
            std::cout << "State not found. Please try again." << std::endl;
        }
    }

    return 0;
}
