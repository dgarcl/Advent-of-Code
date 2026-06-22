#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Shape {
private:
    int rows;
    int cols;
    vector<vector<char>> shape;

public:
    Shape(vector<vector<char>> s) : shape(s), rows(s.size()), cols(s[0].size()) {}

    friend ostream& operator<<(ostream& os, const Shape& s);
};

ostream& operator<<(ostream& os, const Shape& s) {
    for (int i = 0; i < s.rows ; i++) {
        for (int j = 0; j < s.cols; j++) {
            cout << s.shape[i][j] << " ";
        }

        os << endl;
    }

    return os;
}

class Region {
private:
    int width;
    int length;
    vector<int> quantities;

public:
    Region(int w, int l, vector<int>& q) : width(w), length(l), quantities(q) {}

    friend ostream& operator<<(ostream& os, const Region& r);
};

ostream& operator<<(ostream& os, const Region& r) {
    os << r.width << "x" << r.length << ": ";

    for (int i = 0; i < r.quantities.size(); i++) {
        os << r.quantities[i] << " ";
    }

    return os;
}

int main() {
    ifstream file("test.txt");
    
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

    string section = "shapes";
    vector<Shape> shapes;
    int it = 0;

    while (section == "shapes") {
        if (input[it].back() == ':') {
            it++;
            vector<vector<char>> shape;

            while (input[it].length() > 0) {
                vector<char> temp;

                for (int i = 0; i < input[it].length(); i++) {
                    temp.push_back(input[it][i]);
                }

                shape.push_back(temp);
                it++;
            }

            Shape s(shape);
            shapes.push_back(s);
            it++;
        } else {
            section = "regions";
        }
    }

    for (int i = 0; i < shapes.size(); i++) {
        cout << i << ":" << endl << shapes[i] << endl;
    }

    vector<Region> regions;

    while (section == "regions") {
        int x = input[it].find('x');
        int colon = input[it].find(':');

        int width = stoi(input[it].substr(0, x));
        int length = stoi(input[it].substr(x + 1, colon));

        vector<int> quantities;
        string quantity = "";

        for (int i = colon + 2; i < input[it].size(); i++) {
            if (input[it][i] != ' ') {
                quantity += input[it][i];
            }
            else {
                quantities.push_back(stoi(quantity));
                quantity = "";
            }
        }

        quantities.push_back(stoi(quantity));

        Region region(width, length, quantities);
        regions.push_back(region);

        if (it < input.size() - 1) {
            it++;
        } else {
            section = "break";
        }
    }

    for (int i = 0; i < regions.size(); i++) {
        cout << regions[i] << endl;
    }

    return 0;
}
