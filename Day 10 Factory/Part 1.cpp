#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Machine {
private:
    string IndicatorLights;
    vector <vector <int>> ButtonWirings;
    vector <int> JoltageReqs;

public:
    Machine(string l, vector <vector <int>> b, vector <int> j) : IndicatorLights(l), ButtonWirings(b), JoltageReqs(j) {}

    string getIndicatorLights() {
        return IndicatorLights;
    }

    vector <vector <int>> getButtonWirings() {
        return ButtonWirings;
    }

    void print() {
        cout << "[" << IndicatorLights << "] (";

        for (int i = 0; i < ButtonWirings.size(); i++) {
            for (int j = 0; j < ButtonWirings[i].size(); j++) {
                if (j != ButtonWirings[i].size() - 1) {
                    cout << ButtonWirings[i][j] << ",";
                } else {
                    cout << ButtonWirings[i][j];
                }
            }

            if (i != ButtonWirings.size() - 1) {
                cout << ") (";
            } else {
                cout << ") {";
            }
        }

        for (int i = 0; i < JoltageReqs.size(); i++) {
            if (i != JoltageReqs.size() - 1) {
                cout << JoltageReqs[i] << ",";
            } else {
                cout << JoltageReqs[i];
            }
        }
           
        cout << "}" << endl;
    }
};

string toggle(string lights, vector <int> &buttons) {
    for (int i = 0; i < buttons.size(); i++) {
        if (lights[buttons[i]] == '#') {
            lights[buttons[i]] = '.';
        } else {
            lights[buttons[i]] = '#';
        }
    }

    return lights;
}

int main() {
    ifstream file("test.txt");
   
    if (!file.is_open()) { // Check that the file was found correctly
        cout << "ERROR: Failed to open file." << endl;

        return 1;
    }

    string line;
    vector <Machine> Machines;

    while (getline(file, line)) { // Copy the content of the file into a vector
        int brackets = line.find("]");
        int braces = line.find("{");
       
        string lights = line.substr(1, brackets - 1);

        vector <vector <int>> ButtonWirings;
        string buttons = line.substr(brackets + 2, (braces - 1) - (brackets + 2));
        int parsed = 0;
       
        while (parsed < buttons.length()) {
            vector <int> temp;
            int parenthesis = buttons.find(")", parsed);

            for (int i = parsed + 1; i < parenthesis; i++) {
                if (isdigit(buttons[i])) {
                    temp.push_back(buttons[i] - '0');
                }
            }

            ButtonWirings.push_back(temp);
            parsed = parenthesis + 1;
        }

        vector <int> JoltageReqs;
        string joltages = line.substr(braces + 1, line.length() - braces - 2);
        parsed = 0;

        while (parsed < joltages.length()) {
            int comma = joltages.find(",", parsed);

            if (comma == -1) {
                comma = joltages.length();
            }

            JoltageReqs.push_back(stoi(joltages.substr(parsed, comma - parsed)));
            parsed = comma + 1;
        }

        Machine machineinfo(lights, ButtonWirings, JoltageReqs);
        Machines.push_back(machineinfo);
    }
   
    file.close();

    // This part below was just a test / proof of concept
    int toggles = 0;

    for (int i = 0; i < Machines.size(); i++) {
        cout << "--- Machine " << i + 1 << "---" << endl;

        string start(Machines[i].getIndicatorLights().length(), '.');
        vector <vector <int>> buttons = Machines[i].getButtonWirings();

        for (int j = 0; j < buttons.size(); j++) {
            start = toggle(start, buttons[j]);

            cout << start << endl;

            if (start == Machines[i].getIndicatorLights()) {
                toggles += j + 1;

                cout << "SOLVED" << endl;;
            }
        }
    }

    cout << "Toggles: " << toggles << endl;

    return 0;
}

// To-Do: Figure out how to approach the problem of finding the least toggles for each set of lights