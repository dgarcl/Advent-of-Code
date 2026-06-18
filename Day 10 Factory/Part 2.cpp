#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

class Fraction {
private:
    long long num, denom;

public:
    Fraction(long long x = 1, long long y = 1) : num(x), denom(y) {
        if (denom == 0) {
            throw invalid_argument("Division by 0");
        }

        long long g = __gcd(abs(num), abs(denom));

        if (g != 0) {
            num /= g;
            denom /= g;
        }

        if (denom < 0) {
            num *= -1;
            denom *= -1;
        }
    }

    long long getNum() const {
        return num;
    }

    long long getDenom() const {
        return denom;
    }

    Fraction operator+(const Fraction& other) const {
        return Fraction(this->num * other.denom + other.num * this->denom, this->denom * other.denom);
    }

    Fraction operator-(const Fraction& other) const {
        return Fraction(this->num * other.denom - other.num * this->denom, this->denom * other.denom);
    }

    Fraction operator*(const Fraction& other) const {
        return Fraction(this->num * other.num, this->denom * other.denom);
    }

    Fraction operator/(const Fraction& other) const {
        return Fraction(this->num * other.denom, this->denom * other.num);
    }

    bool operator==(const Fraction& other) const {
        return(this->num == other.num && this->denom == other.denom);
    }

    bool operator!=(const Fraction& other) const {
        return(this->num != other.num || this->denom != other.denom);
    }

    bool operator<(const Fraction& other) const {
        return(this->num * other.denom < other.num * this->denom);
    }

    bool operator>(const Fraction& other) const {
        return(this->num * other.denom > other.num * this->denom);
    }

    void display() const {
        if (denom != 1) {
            cout << num << "/" << denom;
        }
        else {
            cout << num;
        }
    }
};

vector<vector<Fraction>> getConfigs(vector<vector<Fraction>>& A, vector<Fraction>& b) { // Finds all valid solutions for a given system of linear equations
    vector<int> pivots;
    int rows = A.size();
    int cols = A[0].size();

    for (int i = 0; i < cols; i++) { // Transform our matrix into REF
        int p = pivots.size();
        int j;

        for (j = p; j < rows; j++) { // Finds pivot columns and swaps rows if necessary
            if (A[j][i] != 0) {
                swap(A[p], A[j]);
                swap(b[p], b[j]);

                pivots.push_back(i);
                break;
            }
        }

        if (j < rows) {
            for (int j = p + 1; j < rows; j++) { // Performs elimination on rows below the pivots
                Fraction d = A[j][i] / A[p][i];
                b[j] = b[j] - (b[p] * d);

                for (int k = i; k < cols; k++) {
                    A[j][k] = A[j][k] - (A[p][k] * d);
                }
            }
        }
    }

    for (int i = 0; i < pivots.size(); i++) {
        int p = pivots[i];
        Fraction d = Fraction(1) / A[i][p];

        b[i] = b[i] * d;

        for (int j = p; j < cols; j++) {
            A[i][j] = A[i][j] * d;
        }
    }

    for (int i = 1; i < pivots.size(); i++) { // Transforms our matrix into RREF
        int p = pivots[i];

        for (int j = i - 1; j >= 0; j--) { // Performs elimination on rows above the pivots
            Fraction d = A[j][p];

            b[j] = b[j] - (b[i] * d);

            for (int k = p; k < cols; k++) {
                A[j][k] = A[j][k] - (A[i][k] * d);
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

        if (j == cols && b[i] != 0) {
            return vector<vector<Fraction>>();
        }
    }

    vector<Fraction> particularsol(cols, 0);
    vector<vector<Fraction>> nullspace;

    for (int i = 0; i < cols; i++) { // Build a particular solution and a basis for the nullspace
        int j;

        for (j = 0; j < pivots.size(); j++) { // Checks if the column has a pivot
            if (pivots[j] == i) {
                break;
            }
        }

        if (j < pivots.size()) { // We modify the particular solution for each non-free variable
            particularsol[i] = b[j];
        }
        else { // We add a vector to the nullspace for each free variable
            vector<Fraction> temp(cols, 0);

            for (int k = 0; k < pivots.size(); k++) {
                if (A[k][i] != 0) {
                    temp[pivots[k]] = A[k][i] * -1;
                }
            }

            temp[i] = 1;
            nullspace.push_back(temp);
        }
    }

    // -----------------------------------------------------------------------------------
    // From this point onwards we are trying to find the minimal positive integer solution
    // -----------------------------------------------------------------------------------

    vector<vector<Fraction>> configs;

    if (nullspace.size() == 0) { // NULL CASE
        configs.push_back(particularsol);

        return configs;
    }

    vector<vector<Fraction>> N(cols, vector<Fraction>(nullspace.size(), 0));
    vector<Fraction> c(cols, 0);

    rows = N.size();
    cols = N[0].size();
    pivots.clear();

    for (int i = 0; i < rows; i++) {
        c[i] = particularsol[i];

        for (int j = 0; j < cols; j++) {
            N[i][j] = nullspace[j][i];
        }
    }

    if (cols == 1) {
        for (int i = 0; i < 250; i++) {
            vector<Fraction> temp = particularsol;
            bool valid = true;

            for (int j = 0; j < rows; j++) {
                temp[j] = temp[j] + Fraction(i) * N[j][0];

                if (temp[j].getNum() < 0 || temp[j].getDenom() != 1) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                configs.push_back(temp);
            }
        }
    }

    if (cols == 2) {
        for (int i = 0; i < 250; i++) {
            for (int j = 0; j < 250; j++) {
                vector<Fraction> temp = particularsol;
                bool valid = true;

                for (int k = 0; k < rows; k++) {
                    temp[k] = temp[k] + Fraction(i) * N[k][0] + Fraction(j) * N[k][1];

                    if (temp[k].getNum() < 0 || temp[k].getDenom() != 1) {
                        valid = false;
                        break;
                    }
                }

                if (valid) {
                    configs.push_back(temp);
                }
            }
        }
    }

    if (cols > 2) {
        for (int i = 0; i < 250; i++) {
            for (int j = 0; j < 250; j++) {
                for (int k = 0; k < 250; k++) {
                    vector<Fraction> temp = particularsol;
                    bool valid = true;

                    for (int l = 0; l < rows; l++) {
                        temp[l] = temp[l] + Fraction(i) * N[l][0] + Fraction(j) * N[l][1] + Fraction(k) * N[l][2];

                        if (temp[l].getNum() < 0 || temp[l].getDenom() != 1) {
                            valid = false;
                            break;
                        }
                    }

                    if (valid) {
                        configs.push_back(temp);
                    }
                }
            }
        }
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
    Fraction totalpresses(0);

    while (getline(file, line)) { // Parse the content of the file
        int brackets = line.find("]");
        int braces = line.find("{");

        string lights = line.substr(1, brackets - 1);

        vector<vector<int>> buttons;
        int parsed = brackets + 1;

        while (parsed < (braces - 1)) { // Build a vector of vectors with the lights that are affected for each button
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

        vector<Fraction> joltages;
        parsed = braces + 1;

        while (parsed < line.length()) { // Build a vector with the required joltage levels
            int comma = line.find(",", parsed);

            if (comma == -1) {
                comma = line.length();
            }

            joltages.push_back(Fraction(stoi(line.substr(parsed, comma - parsed))));
            parsed = comma + 1;
        }

        vector<vector<Fraction>> changes(joltages.size(), vector<Fraction>(buttons.size(), 0));

        for (int i = 0; i < buttons.size(); i++) { // Build a binary matrix with the changes made by each button 
            for (int j = 0; j < buttons[i].size(); j++) {
                changes[buttons[i][j]][i] = 1;
            }
        }

        vector<vector<Fraction>> configs = getConfigs(changes, joltages); // Returns a vector with all the valid configurations for the machine
        Fraction minpresses(1000000000);

        for (int i = 0; i < configs.size(); i++) { // Find the minimum number of button presses among the valid configurations
            Fraction presses(0);

            for (int j = 0; j < configs[i].size(); j++) { // Count the number of button presses of a given configuration
                presses = presses + configs[i][j];
            }

            if (presses < minpresses) { // Update the minimum number of button presses
                minpresses = presses;
            }
        }

        if (minpresses == Fraction(1000000000)) {
            cout << "problem" << endl;
        } else {
            totalpresses = totalpresses + minpresses;
        }
    }

    file.close();

    cout << "The fewest button presses required to correctly configure the joltage level counter on all of the machines is ";
    totalpresses.display();
    cout << endl;

    return 0;
}
