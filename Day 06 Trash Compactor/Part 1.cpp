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

    vector <string> input;
    string line;

    // Copy the content of the file into our input vector
    while (getline(file, line)) {
        input.push_back(line);
    }

    file.close();

    vector <vector <string>> problems;

    // Find the numbers in our imput
    for (int i = 0; i < input.size(); i++) {
        vector <string> numbers;
        string num = "";

        // Loop through the characters of each line
        for (char c : input[i]) {
            // If a character is not a space, then we add it into our number
            if (c != ' ') {
                num += c;
            // If a character is a space, then we append the number to a vector
            } else if (num != "") {
                numbers.push_back(num);
                num = "";
            }
        }   

        // If there is any number left, we also append it
        if (num != "") {
            numbers.push_back(num);
        }

        problems.push_back(numbers);
    }

    long long grandtotal = 0;

    // Loop through the columns of our problems to find the grand total
    for (int i = 0; i < problems[0].size(); i++) {
        long long total;

        // Identify the operation we need to perform
        if (problems.back()[i] == "+") {
            total = 0;

            // Loop through the rows to find the result of the operation
            for (int j = 0; j < problems.size() - 1; j++) {
                total += stoll(problems[j][i]);
            }
        // Identify the operation we need to perform
        } else if (problems.back()[i] == "*") {
            total = 1;
            // Loop through the rows of to find the result of the operation
            for (int j = 0; j < problems.size() - 1; j++) {
                total *= stoll(problems[j][i]);
            }
        }

        grandtotal += total;
    }

    cout << "The grand total found by adding together all of the answers to the individual problems is " << grandtotal << endl;

    return 0;
}