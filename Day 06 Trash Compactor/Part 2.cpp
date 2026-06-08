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
    vector <string> numbers;

    // Loop through the columns of our input to find the problems 
    for (int i = 0; i < input[0].size() - 1; i++) {
        string num = "";

        // If the next character of the last row is a space, we read the number of our column
        if (input.back()[i + 1] == ' ') {
            // Loop through the numbers in the column except the last column
            for (int j = 0; j < input.size() - 1; j++) {
                // We only want to append numbers so we ignore the spaces in the column
                if (input[j][i] != ' ') {
                    num += input[j][i];
                }
            }
            
            numbers.push_back(num);
        // If the next character of the last row is an operator, we will start a new problem
        } else {
            // We also need to append our previous problem and clear its contents
            if (numbers.size() != 0) {
                problems.push_back(numbers);
                numbers.clear();
            }

            numbers.push_back(string(1, input.back()[i + 1]));

        }

    }

    problems.push_back(numbers);

    long long grandtotal = 0;

    // Loop through each of the problems to find the grand total
    for (int i = 0; i < problems.size(); i++) {
        long long total;

        // Identify the operation we need to perform
        if (problems[i][0] == "+") {
            total = 0;

            // Loop through the numbers to find the result of problem
            for (int j = 1; j < problems[i].size(); j++) {
                total += stoll(problems[i][j]);
            }
        // Identify the operation we need to perform
        } else if (problems[i][0] == "*") {
            total = 1;

            // Loop through the numbers to find the result of problem
            for (int j = 1; j < problems[i].size(); j++) {
                total *= stoll(problems[i][j]);
            }            
        }

        grandtotal += total;
    }

    cout << "The grand total found by adding together all of the answers to the individual problems is " << grandtotal << endl;

    return 0;
}