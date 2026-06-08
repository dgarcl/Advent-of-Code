#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

class JunctionBox { // We construct a class that handles the coordinates of each junction box
private:
    int X, Y, Z;

public:
    JunctionBox(int x = 0, int y = 0, int z = 0) : X(x), Y(y), Z(z) {} // Constructor of the class

    int getxcoord() { // Returns the x coordinate of a junction box
        return X;
    }

    double distance(const JunctionBox& other) const { // Computes the euclidean distance between two junction boxes
        double distance = 0;
        double temp = 0;

        // Calculates the distance on the x-axis
        temp = this->X - other.X;
        distance += temp * temp;

        // Calculates the distance on the y-axis
        temp = this->Y - other.Y;
        distance += temp * temp;

        // Calculates the distance on the z-axis
        temp = this->Z - other.Z;
        distance += temp * temp;

        return sqrt(distance);
    }

    bool operator == (const JunctionBox& other) const { // Compares if two junction boxes are equal
        return (this->X == other.X && this->Y == other.Y && this->Z == other.Z); 
    }

    void print() { // Prints the coordinates of a juntion box
        cout << "X = " << X << ", Y = " << Y << ", Z = " << Z  << "."; 
    }
};

class JunctionBoxPair { // We construct a class that handles each pair of junction boxes
private:
    JunctionBox box1, box2;
    double distance;

public:
    JunctionBoxPair(JunctionBox b1, JunctionBox b2, double d) : box1(b1), box2(b2), distance(d) {} // Constructor of the class

    JunctionBox getbox1() { // Returns one junction box of the pair
        return box1;
    }

    JunctionBox getbox2() { // Returns the other junction box of the pair
        return box2;
    }

    bool operator < (const JunctionBoxPair& other) const { // Compares if a pair of junction boxes is closer than another
        return (this->distance < other.distance);
    }

    void print() { // Prints the junction boxes of a pair and their distance
        box1.print();
        cout << "---- " << distance << "m ----";
        box2.print(); 
    }
};

int main() {
    ifstream file("input.txt");
    
    if (!file.is_open()) { // Check that the file was found correctly
        cout << "ERROR: Failed to open file." << endl;

        return 1;
    }

    vector <JunctionBox> junctionBoxes;
    string line;

    while (getline(file, line)) { // Copy the content of the file into a vector
        // Finds the x coordinate of the junction box
        int comma = line.find(',');
        int xcoord = stoi(line.substr(0, comma));
        line.erase(0, comma + 1); 
        
        // Finds the y coordinate of the junction box
        comma = line.find(',');
        int ycoord = stoi(line.substr(0, comma));
        line.erase(0, comma + 1); 
        
        // Finds the z coordinate of the junction box
        int zcoord = stoi(line);
        
        // We create a junction box object and append it to our vector
        JunctionBox box(xcoord, ycoord, zcoord);
        junctionBoxes.push_back(box);    
    }
    
    file.close();

    vector <JunctionBoxPair> pairs;

    for (int i = 0; i < junctionBoxes.size(); i++) { // Loop through the junction boxes in our vector
        for (int j = i + 1; j < junctionBoxes.size(); j++) { // Loop through the unchecked junction boxes in our vector
            double distance = junctionBoxes[i].distance(junctionBoxes[j]);

            // We create a junction box pair object and append it to our vector
            JunctionBoxPair pair(junctionBoxes[i], junctionBoxes[j], distance);
            pairs.push_back(pair);
        }
    }

    // Sort the pairs of junction boxes by their distance
    sort(pairs.begin(), pairs.end());

    // We create a vector to store the circuits of junction boxes we connect 
    vector <vector <JunctionBox>> circuits;
    int connections = 1000;

    // We create two junction box objects for the two closest jucntion boxes at each step
    JunctionBox box1;
    JunctionBox box2;

    for (int it = 0; it < connections; it++) { // We repeat the following process until we have tried for 1000 connections
        box1 = pairs[it].getbox1();
        box2 = pairs[it].getbox2();

        // Start as if the two closest junction boxes are not already in any circuit
        int circuit1 = -1;
        int circuit2 = -1;
        bool connected = false;

        for (int i = 0; i < circuits.size(); i++) { // Loop through the circuits in our vector
            for (int j = 0; j < circuits[i].size(); j++) { // Loop through the junction boxes in each circuit
                if (circuits[i][j] == box1) { // If the first junction box is already in a circuit, then it is connected and we take note of what circuit is in
                    connected = true;
                    circuit1 = i;
                }

                if (circuits[i][j] == box2) { // If the second junction box is already in a circuit, then it is connected and we take note of what circuit is in
                    connected = true;
                    circuit2 = i;
                }
            }
        }

        if (connected == true) { // If any junction box is connected, we have to modify the already existing circuits
            if (circuit1 == -1) { // If only the first junction box is in a circuit, then we append the second junction box to that circuit
                circuits[circuit2].push_back(box1);
            } else if (circuit2 == -1) { // If only the second junction box is in a circuit, then we append the first junction box to that circuit
                circuits[circuit1].push_back(box2);
            } else if (circuit1 != circuit2) { // If both junction boxes are in different circuits, we merge those circuits into a single one
                circuits[circuit1].insert(circuits[circuit1].end(), circuits[circuit2].begin(), circuits[circuit2].end());
                circuits.erase(circuits.begin() + circuit2);
            }
        }

        if (circuits.size() == 0 || connected == false) { // If the junction boxes are not connected to any circuit, then we create a new circuit
            vector <JunctionBox> newcircuit;

            newcircuit.push_back(box1);
            newcircuit.push_back(box2);

            circuits.push_back(newcircuit);
        } 
    }

    vector <int> sizes;

    for (int i = 0; i < circuits.size(); i++) { // We loop through our circuits and append their sizes to a vector
        sizes.push_back(circuits[i].size());
    }

    // Sort the sizes from largest to smallest
    sort(sizes.begin(), sizes.end());
    reverse(sizes.begin(), sizes.end());

    // Compute the product of the three largest sizes
    int result = sizes[0] * sizes[1] * sizes[2];

    cout << "If you multiply together the sizes of the three largest circuits, you get " << result << endl;

    return 0;
}