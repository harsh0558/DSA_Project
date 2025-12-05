#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

void generateFile(string filename, int buildingCount, int pathCount) {
    ofstream file(filename);
    if (!file) {
        cout << "Error creating " << filename << endl;
        return;
    }

    // 1. Generate Buildings and Rooms
    cout << "Generating " << buildingCount << " buildings for " << filename << "..." << endl;
    for (int i = 0; i < buildingCount; i++) {
        file << "Building_" << i << endl;
        
        // Add 2-5 random rooms per building
        int rooms = rand() % 4 + 2; 
        for (int j = 0; j < rooms; j++) {
            file << "ROOM Room_" << i << "_" << j << " " 
                 << (rand() % 5 + 1) << " " // Random Floor 1-5
                 << (rand() % 50 + 10) << endl; // Random Capacity 10-60
        }
    }
    file << "END_BUILDINGS" << endl;

    // 2. Generate Random Paths
    cout << "Generating " << pathCount << " paths..." << endl;
    for (int i = 0; i < pathCount; i++) {
        int id1 = rand() % buildingCount;
        int id2 = rand() % buildingCount;
        
        // Avoid self-loops
        while(id1 == id2) {
            id2 = rand() % buildingCount;
        }

        int dist = rand() % 200 + 10; // Random distance 10-210
        file << "Building_" << id1 << " " << "Building_" << id2 << " " << dist << endl;
    }
    file << "END_PATHS" << endl;

    file.close();
    cout << "Success: " << filename << " created." << endl;
}

int main() {
    srand(time(0)); // Seed random number generator

    // Generate 3 datasets of increasing size
    // Format: filename, number of buildings, number of paths
    generateFile("dataset_100.txt", 100, 200);
    generateFile("dataset_300.txt", 300, 900);
    generateFile("dataset_500.txt", 500, 2000);

    cout << "\nAll datasets generated successfully." << endl;
    return 0;
}