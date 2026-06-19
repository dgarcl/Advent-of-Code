#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<int> getPresses(vector<vector<int>>& A, vector<int>& b) { // Finds the presses of every valid solution for a given system of linear equations
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

    for (int i = 1; i < pivots.size(); i++) { // Transforms our matrix into RREF
        int p = pivots[i];

        for (int j = i - 1; j >= 0; j--) { // Performs elimination on rows above the pivots
            if (A[j][p] == 1) {
                b[j] ^= b[i];

                for (int k = p; k < cols; k++) {
                    A[j][k] ^= A[i][k];
                }
            }
        }
    }

    for (int i = 0; i < rows; i++) { // Scan the matrix for inconsistencies (0 = 1)
        int j;

        for (j = 0; j < cols; j++) {
            if (A[i][j] == 1) {
                break;
            }
        }

        if (j == cols && b[i] == 1) {
            return vector<int>();
        }
    }

    vector<int> particularsol(cols, 0);
    vector<vector<int>> nullspace;

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

    vector<int> presses;
    int sols = 1;

    for (int i = 0; i < nullspace.size(); i++) { // Calculate the size of the solution set
        sols *= 2;
    }

    for (int i = 0; i < sols; i++) { // Build the different solutions of the system
        vector<int> config = particularsol;
        int it = 0;
        int mod = 1;

        while (it < nullspace.size()) { // Add vectors of the nullspace to our solution to find new valid solutions
            if ((i / mod) % 2 == 1) {
                for (int j = 0; j < cols; j++) {
                    config[j] ^= nullspace[it][j];
                }
            }

            it++;
            mod *= 2;
        }

        int sum = 0;

        for (int j = 0; j < config.size(); j++) {
            sum += config[j];
        }

        presses.push_back(sum);
    }

    return presses;
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

        for (int i = 0; i < lights.length(); i++) { // Convert the string of lights into a binary vector of the necessary toggles
            if (lights[i] == '#') {
                toggles.push_back(1);
            } else {
                toggles.push_back(0);
            }
        }

        vector<vector<int>> buttons;
        int parsed = brackets + 1;

        while (parsed < braces - 1) { // Build a vector of vectors with the lights that are affected for each button
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

        vector<vector<int>> wirings(toggles.size(), vector<int>(buttons.size(), 0));

        for (int i = 0; i < buttons.size(); i++) { // Build a binary matrix with the changes made by each button 
            for (int j = 0; j < buttons[i].size(); j++) {
                wirings[buttons[i][j]][i] = 1;
            }
        }

        vector<int> presses = getPresses(wirings, toggles); // Returns a vector with the number of presses of every valid configuration
        int minpresses = presses[0];

        for (int i = 1; i < presses.size(); i++) { // Find the minimum number of button presses for the machine
            if (presses[i] < minpresses) {
                minpresses = presses[i];
            }
        }

        totalpresses += minpresses;
    }

    file.close();

    cout << "The fewest button presses required to correctly configure the indicator lights on all of the machines is " << totalpresses << endl;

    return 0;
}
