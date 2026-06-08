#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Checks wether an ID is valid or not
bool invalidID(string ID) {
    // An ID starting with 0 is not a valid ID
    if (ID[0] == '0') {
        return true;
    // An ID formed by repeating a substring cant have an odd length
    } else if (ID.length() % 2 == 1) {
        return false;
    // If it has an even length, we look at its first half
    } else {
        string substring = ID.substr(0, (ID.length()) / 2);

        // It it can be reconstructed from its first half it is not valid
        if (ID == substring + substring) {
            return true;
        }

        return false;
    }
}

int main() {
    ifstream file("input.txt");
    
    // Check that the file was found correctly
    if (!file.is_open()) {
        cout << "ERROR: Failed to open file." << endl;
        return 1;
    }

    string input;
    getline(file, input);

    file.close();

    vector <string> IDs;
    int start = 0;

    // Go through the input to find all the IDs it contains
    for (int i = 0; i <= input.length(); i++) {
        // Find all ID ranges by separating the imput by commas
        if (input[i] == ',' || i == input.length()) {
            string range = input.substr(start, i - start);

            int dash = range.find('-');

            long long firstID = stoll(range.substr(0, dash));
            long long lastID = stoll(range.substr(dash + 1));

            // Calculates all IDs inside a range and appends them into a vector
            for (long long j = firstID; j <= lastID; j++) {
                IDs.push_back(to_string(j));
            }

            start = i + 1;
        }      
    }

    vector <string> invalid_IDs;

    // Go through all IDs and append the invalid ones into a different vector
    for (int i = 0; i < IDs.size(); i++) {
        if (invalidID(IDs[i])) {
            invalid_IDs.push_back(IDs[i]);
        }
    }

    long long result = 0;

    // Sum all the invalid IDs
    for (int i = 0; i < invalid_IDs.size(); i++) {
        long long num = stoll(invalid_IDs[i]);
        
        result += num;
    }

    cout << "If you add up all of the invalid IDs you get " << result << endl;

    return 0;
}
