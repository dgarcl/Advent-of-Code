#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Fraction { // Class to handle arithmetic with fractions
private:
    long long num, denom;

public:
    Fraction(long long x = 1, long long y = 1) : num(x), denom(y) {
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

    Fraction& operator+=(const Fraction& other) {
        *this = *this + other;
        return *this;
    }

    Fraction& operator-=(const Fraction& other) {
        *this = *this - other;
        return *this;
    }

    Fraction& operator*=(const Fraction& other) {
        *this = *this * other;
        return *this;
    }

    Fraction& operator/=(const Fraction& other) {
        *this = *this / other;
        return *this;
    }

    bool operator==(const Fraction& other) const {
        return(this->num == other.num && this->denom == other.denom);
    }

    bool operator<(const Fraction& other) const {
        return(this->num * other.denom < other.num * this->denom);
    }

    bool operator>(const Fraction& other) const {
        return(this->num * other.denom > other.num * this->denom);
    }

    bool operator!=(const Fraction& other) const {
        return!(*this == other);
    }

    explicit operator int() const {
        return num / denom;
    }

    friend ostream& operator<<(ostream& os, const Fraction& f);
};

ostream& operator<<(ostream& os, const Fraction& f) {
    if (f.denom == 1) {
        os << f.num;
    } else {
        os << f.num << "/" << f.denom;
    }

    return os;
}

vector<Fraction> getPresses(vector<vector<Fraction>>& A, vector<Fraction>& b) { // Finds the presses of every valid solution for a given system of linear equations
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

        if (j < rows) { // Check wether a pivot was found
            for (int j = p + 1; j < rows; j++) { // Performs elimination on rows below the pivots
                Fraction d = A[j][i] / A[p][i];
                b[j] -= b[p] * d;

                for (int k = i; k < cols; k++) {
                    A[j][k] -= A[p][k] * d;
                }
            }
        }
    }

    for (int i = 0; i < pivots.size(); i++) { // Scales rows so that pivots positions are 1s
        int p = pivots[i];
        b[i] /= A[i][p];

        for (int j = p + 1; j < cols; j++) {
            A[i][j] /= A[i][p];
        }

        A[i][p] = 1;
    }

    for (int i = 1; i < pivots.size(); i++) { // Transforms our matrix into RREF
        int p = pivots[i];

        for (int j = i - 1; j >= 0; j--) { // Performs elimination on rows above the pivots
            b[j] -= b[i] * A[j][p];

            for (int k = p + 1; k < cols; k++) {
                A[j][k] -= A[i][k] * A[j][p];
            }

            A[j][p] = 0;
        }
    }

    for (int i = 0; i < rows; i++) { // Scan the matrix for inconsistencies (0 = sth)
        int j;

        for (j = 0; j < cols; j++) {
            if (A[i][j] != 0) {
                break;
            }
        }

        if (j == cols && b[i] != 0) {
            return vector<Fraction>();
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
        } else { // We add a vector to the nullspace for each free variable
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

    vector<Fraction> presses;

    if (nullspace.size() == 0) { // If there were no free variables, the solution is the particular solution
        Fraction sum = 0;

        for (int i = 0; i < cols; i++) {
            sum += particularsol[i];
        }

        presses.push_back(sum);

        return presses;
    }

    int nulls = nullspace.size();
    vector<vector<Fraction>> N(cols, vector<Fraction>(nulls, 0));
    vector<Fraction> c(cols, 0);

    for (int i = 0; i < cols; i++) { // Build another matrix equation to find the weights of the free variables
        c[i] = particularsol[i] * -1;

        for (int j = 0; j < nulls; j++) { // The new matrix columns' are the vectors of the nullspace
            N[i][j] = nullspace[j][i];
        }
    }

    vector<Fraction> bounds(nulls, Fraction(250));

    for (int i = 0; i < cols; i++) { // Find bounds for the weights by ensuring positivity on each row
        int j;

        for (j = 0; j < nulls; j++) { // Scan the structure of each row searching for a suitable inequality
            if (N[i][j] > 0) {
                break;
            }
        }

        if (j == nulls) { // If all entries on a row are negative we have found an upper bound for the weight
            for (j = 0; j < nulls; j++) {
                if (N[i][j] < 0 && (c[i] / N[i][j]) < bounds[j]) { // Update the weight's upper bound
                    bounds[j] = c[i] / N[i][j];
                }
            }
        }
    }

    vector<int> coeffs(nulls, 0);

    while (true) { // Find all possible configurations within the computed bounds
        vector<Fraction> config = particularsol;
        int i;

        for (i = 0; i < cols; i++) { // Build a specific configuration from a set of coefficients for the free variables
            for (int j = 0; j < nulls; j++) { 
                config[i] += N[i][j] * coeffs[j];
            }

            if (config[i] < 0 || config[i] != int(config[i])) { // Check that the configuration is valid
                break;
            }
        }

        if (i == cols) { // For valid configurations, calculate their number of presses
            Fraction sum = 0;

            for (int j = 0; j < cols; j++) {
                sum += config[j];
            }

            presses.push_back(sum);
        }

        int pos = 0;

        while (pos < coeffs.size()) { // Find the next set of coefficients for the free variables
            coeffs[pos]++;

            if (bounds[pos] + 1 > coeffs[pos]) {
                break;
            }

            coeffs[pos] = 0;
            pos++;
        }

        if (pos == coeffs.size()) { // When all possible combinations of weights have been checked we exit the loop
            break;
        }
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
    Fraction totalpresses = 0;

    while (getline(file, line)) { // Parse the content of the file
        int brackets = line.find("]");
        int braces = line.find("{");

        string lights = line.substr(1, brackets - 1);

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

        vector<vector<Fraction>> wirings(joltages.size(), vector<Fraction>(buttons.size(), 0));

        for (int i = 0; i < buttons.size(); i++) { // Build a matrix with the changes made by each button 
            for (int j = 0; j < buttons[i].size(); j++) {
                wirings[buttons[i][j]][i] = 1;
            }
        }

        vector<Fraction> presses = getPresses(wirings, joltages); // Returns a vector with the number of presses of every valid configuration
        Fraction minpresses = presses[0];

        for (int i = 1; i < presses.size(); i++) { // Find the minimum number of button presses for the machine
            if (presses[i] < minpresses) { 
                minpresses = presses[i];
            }
        }

        totalpresses += minpresses;
    }

    file.close();

    cout << "The fewest button presses required to correctly configure the joltage level counter on all of the machines is " << totalpresses << endl;

    return 0;
}
