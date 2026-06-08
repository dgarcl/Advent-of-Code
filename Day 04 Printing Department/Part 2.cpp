#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ifstream file("input.txt");
    
    // Check that the file was found correctly
    if (!file.is_open()) {
        cout << "ERROR: Failed to open file." << endl;
        return 1;
    }
        
    string line;
    string padding = "";
    vector <string> input;

    // Compute the needed padding and append the contents in the file into a vector
    while (getline(file, line)) {
        // Check that the paddign isnt already computed as to only compute it once
        if (padding == "") {
            padding = string(line.length(), 'x');
        }

        input.push_back(line);
    }

    file.close();

    int totalAccessedRolls = 0;
    bool remove = true;

    // Repeat until we can no longer remove rolls of paper
    while (remove) {
        // Insert padding above and below
        input.insert(input.begin(), padding);
        input.insert(input.end(), padding);

        // Insert padding to the left and to the right
        for (int i = 0; i < input.size(); i++) {
            input[i] = "x" + input[i] + "x";
        }

        int accessedRolls = 0;

        // Loop through the rows
        for (int i = 0; i < input.size(); i++) {
            // Loop through the columns
            for (int j = 0; j < input[0].length(); j++) {
                // When a roll of paper is found, we check if it is accessible
                if (input[i][j] == '@') {
                    int adjacentRolls = 0;

                    // Loop through the neighbouring rows
                    for (int n = 0; n < 3; n++) {
                        // Loop through the neighbouring columns
                        for (int m = 0; m < 3; m++) {
                            // Take notes of all rolls of paper found
                            if (input[i + n - 1][j + m - 1] == '@') {
                                adjacentRolls += 1;
                            }
                        }
                    }

                    // If not enough are found, then that roll of paper is accessible
                    if (adjacentRolls < 5) {
                        input[i][j] = 'x';
                        accessedRolls += 1;
                    }
                }
            }
        }

        // If no roll of paper was removed, then can no longer remove rolls of paper
        if (accessedRolls == 0) {
            remove = false;
        }

        totalAccessedRolls += accessedRolls;

        // Remove padding to the left and to the right
        for (int i = 1; i < input.size(); i++) {
            input[i] = input[i].substr(1, input[0].length() - 2);
        }
        
        // Remove padding above and below
        input.erase(input.begin());
        input.erase(input.begin() + input.size());        
    }

    cout << "In total, " << totalAccessedRolls << " rolls of paper can be removed by the Elves and their forklifts" << endl;

    return 0;
}