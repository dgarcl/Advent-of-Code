#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<vector<int>> getConfigs(vector<vector<int>>& A, vector<int>& b) { // Finds all valid solutions for a given system of linear equations
    vector<int> pivots;
    int rows = A.size();
    int cols = A[0].size();

    for (int i = 0; i < cols; i++) { // Transform our matrix into REF
        int p = pivots.size();

        for (int j = p; j < rows; j++) { // Finds pivot columns and swaps rows if necessary
            if (A[j][i] == 1) {
                swap(A[p], A[j]);
                swap(b[p], b[j]);

                pivots.push_back(i);
                break;
            }
        }

        for (int j = p + 1; j < rows; j++) { // Performs elimination on rows below the pivots
            if (A[j][i] == 1) {
                b[j] ^= b[p];

                for (int k = i; k < cols; k++) {
                    A[j][k] ^= A[p][k];
                }
            }
        }
    }

    for (int i = 0; i < pivots.size(); i++) { // Transforms our matrix into RREF
        int p = pivots[i];

        for (int j = i; j > 0; j--) { // Performs elimination on rows above the pivots
            if (A[j - 1][p] == 1) {
                b[j - 1] ^= b[i];

                for (int k = p; k < cols; k++) {
                    A[j - 1][k] ^= A[i][k];
                }
            }
        }
    }

    vector<vector<int>> configs;
    vector<vector<int>> nullspace;

    for (int i = 0; i < rows; i++) { // Scan the matrix for inconsistencies (0 = 1)
        int j;

        for (j = 0; j < cols; j++) {
            if (A[i][j] == 1) {
                break;
            }
        }

        if (j == cols && b[i] == 1) {
            return vector<vector<int>>();
        }
    }

    vector<int> particularsol(cols, 0);

    for (int i = 0; i < cols; i++) { // Build a particular solution and a basis for the nullspace
        int j;

        for (j = 0; j < pivots.size(); j++) { // Checks if the column has a pivot
            if (pivots[j] == i) {
                break;
            }
        }

        if (j < pivots.size()) { // We modify the particular solution for each non-free variable
            particularsol[i] = b[j];
        } else { // We add a vector to the nullspace for each free variable
            vector<int> temp(cols, 0);

            for (int k = 0; k < pivots.size(); k++) {
                if (A[k][i] == 1) {
                    temp[pivots[k]] = 1;
                }
            }

            temp[i] = 1;
            nullspace.push_back(temp);
        }
    }

    int solutions = 1;

    for (int i = 0; i < nullspace.size(); i++) { // Calculate the size of the solution set
        solutions *= 2;
    }

    for (int i = 0; i < solutions; i++) { // Build the different solutions of the system
        vector<int> sol = particularsol;
        int it = 0;
        int mod = 1;

        while (it < nullspace.size()) { // Add vectors of the nullspace to our solution to find new valid solutions
            if ((i / mod) % 2 == 1) {
                for (int j = 0; j < cols; j++) {
                    sol[j] ^= nullspace[it][j];
                }
            }

            it++;
            mod *= 2;
        }

        configs.push_back(sol);
    }

    return configs;
}

int main() {
    ifstream file("input.txt");

    if (!file.is_open()) { // Check that the file was found correctly
        cout << "ERROR: Failed to open file." << endl;

        return 1;
    }

    string line;
    int totalpresses = 0;

    while (getline(file, line)) { // Parse the content of the file
        int brackets = line.find("]");
        int braces = line.find("{");

        string lights = line.substr(1, brackets - 1);
        vector<int> toggles;

        for (int i = 0; i < lights.length(); i++) { // Convert the line into a binary vector of the necessary toggles
            if (lights[i] == '#') {
                toggles.push_back(1);
            } else {
                toggles.push_back(0);
            }
        }

        line = line.substr(brackets + 2, (braces - 1) - (brackets + 2));
        vector<vector<int>> buttons;
        int parsed = 0;

        while (parsed < line.length()) { // Build a vector of vectors with the lights that are affected for each button
            vector<int> temp;
            int parenthesis = line.find(")", parsed);

            for (int i = parsed + 1; i < parenthesis; i++) {
                if (isdigit(line[i])) {
                    temp.push_back(line[i] - '0');
                }
            }

            buttons.push_back(temp);
            parsed = parenthesis + 1;
        }

        vector<vector<int>> changes(toggles.size(), vector<int>(buttons.size(), 0));

        for (int i = 0; i < buttons.size(); i++) { // Build a binary matrix with the changes made by each button 
            for (int j = 0; j < buttons[i].size(); j++) {
                changes[buttons[i][j]][i] = 1;
            }
        }

        vector<vector<int>> configs = getConfigs(changes, toggles); // Returns a vector with all the valid configurations for the machine
        int minpresses = configs[0].size();

        for (int i = 0; i < configs.size(); i++) { // Find the minimum number of button presses among the valid configurations
            int presses = 0;

            for (int j = 0; j < configs[i].size(); j++) { // Count the number of button presses of a given configuration
                if (configs[i][j] == 1) {
                    presses++;
                }
            }

            if (presses < minpresses) { // Update the minimum number of button presses
                minpresses = presses;
            }
        }

        totalpresses += minpresses;
    }

    file.close();

    cout << "The fewest button presses required to correctly configure the indicator lights on all of the machines is " << totalpresses << endl;

    return 0;
}
