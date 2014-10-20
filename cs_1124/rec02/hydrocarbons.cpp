/*
 * Program Name: Recitation 2
 * Name: Christopher Thompson
 * Description:
 * Storing and sorting hydrocarbons
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

struct Hydrocarbon {
    int carbonAtoms;
    int hydrogenAtoms;
    vector<string> names;
};

void printHydrocarbons(const vector<Hydrocarbon>& hydrocarbons);
bool getSmallestHydrocarbon(const vector<Hydrocarbon>& hydrocarbons);

int main() {
    ifstream input("hydrocarbons.txt");

    vector<Hydrocarbon> hydrocarbons;
    string name;
    char junk;
    int carbons;
    int hydrogens;
    
    while(input >> name >> junk >> carbons >> junk >> hydrogens) {
        Hydrocarbon hydro;
        bool newName = true;
        int index = 0;
        for (size_t i = 0; i < hydrocarbons.size(); i++) {
            for (size_t j = 0; j < hydrocarbons[i].names.size(); j++) {
                if (hydrocarbons[i].names[j].compare(name) == 0) {
                    newName = false;
                    index = i;
                    break;
                }
            }
        }

        if (newName) {
            hydro.carbonAtoms = carbons;
            hydro.hydrogenAtoms = hydrogens;
            hydro.names.push_back(name);
            hydrocarbons.push_back(hydro);
        } else {
            hydrocarbons[index].names.push_back(name);
        }
    }

    printHydrocarbons(hydrocarbons);
    
    return 0;
}

int findMaxC(const vector<Hydrocarbon>& hydrocarbons) {
    int maxC = 1;
    for (size_t i = 0; i < hydrocarbons.size(); i++) {
        if (hydrocarbons[i].carbonAtoms > maxC) {
            maxC = hydrocarbons[i].carbonAtoms;
        }
    }
    return maxC;
}

void printHydrocarbons(const vector<Hydrocarbon>& hydrocarbons) {
    for (size_t i = 1; i <= findMaxC(hydrocarbons); i++) {
        for (size_t k = 0; k < hydrocarbons.size(); k++) {
            if (hydrocarbons[k].carbonAtoms == i) {
                cout << "C" << hydrocarbons[k].carbonAtoms << "H" << hydrocarbons[k].hydrogenAtoms << " ";
                for (size_t j = 0; j < hydrocarbons[k].names.size(); j++) {
                    cout << hydrocarbons[k].names[j] << " ";
                }
                cout << endl;
            }
        }
    }
}

