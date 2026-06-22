#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Checks wether an id is valid or not
bool invalidID(string id) {
    // An id starting with 0 is not a valid id
    if (id[0] == '0') {
        return true;
    // Change this comment
    } else {
        int len = id.length();
        vector <int> divs;

        // Find all the possible lengths of a repeating sequence
        for (int i = 1; i < len; i++) {
            if (len % i == 0) {
                divs.push_back(i);
            }
        }

        // Loop through all possible lengths to check for a sequence
        for (int i = 0; i < divs.size(); i++) {
            int div = divs[i];
            int reps = len / div;

            bool invalid = true;

            // Check if our id is like the one formed from the sequence
            for (int j = 0; j < reps; j++) {
                // If they are different at any point, the sequence is discarted
                if (id.substr(0, div) != id.substr(div * j, div)) {
                    invalid = false;
                }
            }

            // If at no point were they different, the id is not valid
            if (invalid) {
                return true;
            }
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

    vector <string> ids;
    int start = 0;

    // Go through the input to find all the ids it contains
    for (int i = 0; i <= input.length(); i++) {
        // Find all id ranges by separating the imput by commas
        if (input[i] == ',' || i == input.length()) {
            string range = input.substr(start, i - start);

            int dash = range.find('-');

            long long firstID = stoll(range.substr(0, dash));
            long long lastID = stoll(range.substr(dash + 1));

            // Calculates all ids inside a range and appends them into a vector
            for (long long j = firstID; j <= lastID; j++) {
                ids.push_back(to_string(j));
            }

            start = i + 1;
        }      
    }

    vector <string> invalid_IDs;

    // Go through all ids and append the invalid ones into a different vector
    for (int i = 0; i < ids.size(); i++) {
        if (invalidID(ids[i])) {
            invalid_IDs.push_back(ids[i]);
        }
    }

    long long result = 0;

    // Sum all the invalid ids
    for (int i = 0; i < invalid_IDs.size(); i++) {
        long long num = stoll(invalid_IDs[i]);
        
        result += num;
    }

    cout << "Using this new rules, if you add up all of the invalid IDs you get " << result << endl;

    return 0;
}