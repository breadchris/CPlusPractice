#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

void readStopData(const string& fileName, map<string, int>& stopIDToInt, vector<string>& intToStopID) {
    ifstream dataFile(fileName);
    string line; string field; int lc = 0; 
    int index = 0;
    while (getline(dataFile, line)) {
        if (lc++ <= 1) continue;
        istringstream s(line);
        getline(s, field, ',');
        stopIDToInt[field] = index++;
        intToStopID.push_back(field);
    }
}

vector< list<int>* > readTransferData(const string& fileName, map<string, int>& stopIDToInt, vector<string>& intToStopID) {
    vector< list<int>* >* adjacent = new vector< list<int>* >(intToStopID.size());
    for (int i = 0; i < intToStopID.size(); i++) {
        (*adjacent)[i] = new list<int>();
    }

    ifstream dataFile(fileName);
    string line; string field; int lc = 0; 
    string idFrom; string idTo; 
    int fromStopL; int toStopL;

    while (getline(dataFile, line)) {
        if (lc++ < 1) continue;
        istringstream s(line);
        int i = 0;
        while (getline(s, field, ',')) {
            switch (i++) {
                case 0: idFrom = field; break;
                case 1:
                    idTo = field;
                    fromStopL = stopIDToInt[idFrom];
                    toStopL = stopIDToInt[idTo];
                    if (fromStopL == toStopL) continue;
                    if (find((*adjacent)[fromStopL]->begin(), (*adjacent)[fromStopL]->end(), toStopL)
                        == (*adjacent)[fromStopL]->end())
                        (*adjacent)[fromStopL]->push_front(toStopL);
                    if (find((*adjacent)[toStopL]->begin(), (*adjacent)[toStopL]->end(), fromStopL)
                            == (*adjacent)[toStopL]->end())
                        (*adjacent)[toStopL]->push_front(fromStopL);
                    break;
                default: break;
            }
        }
    }

    return *adjacent;
}

int main() {
    map<string, int> stopIDToInt;
    vector<string> intToStopID;
    readStopData("MTA_train_stop_data.txt", stopIDToInt, intToStopID);
    vector< list<int>* > adjacent = readTransferData("transfers.txt", stopIDToInt, intToStopID);

    list<int>::iterator itr;
    for (int i = 0; i < adjacent.size(); i++) {
        if (adjacent[i]->size() == 0) continue;
        cout << "From stop: " << intToStopID[i] << " you can trasfer to:";
        for (itr = adjacent[i]->begin(); itr !=  adjacent[i]->end(); itr++) {
            cout << " " << intToStopID[*itr];
        }
        cout << endl;
    }
}
