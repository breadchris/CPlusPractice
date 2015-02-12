#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class TrainStopData {
private:
    string stop_id;    // id of train stop (1st token)
    string stop_name;  // name of station (4th token)
    double stop_lat;   // latitude of train stop location
    double stop_lon;   // longitude of train stop location
public:
    TrainStopData() {}

    void set_stop_id(string& _id) {
        stop_id = _id;
    }
    void set_stop_name(string& _name) {
        stop_name = _name;
    }
    void set_latitude(double _lat) {
        stop_lat = _lat;
    }
    void set_longitude(double _lon) {
        stop_lon = _lon;
    }

    string get_stop_id() const {
        return stop_id;
    }
    string get_stop_name() const {
        return stop_name;
    }
    double get_latitude() const {
        return stop_lat;
    }
    double get_longitude() const {
        return stop_lon;
    }

    friend ostream& operator<<(ostream& os, const TrainStopData& tsd);
};

ostream& operator<<(ostream& os, const TrainStopData& tsd)
{
    os << "ID: " << tsd.get_stop_id() << " Name: " << tsd.get_stop_name()
       << " Lat: " << tsd.get_latitude() << " Lon: " << tsd.get_longitude();
    return os;
}

void readFile(const string fileName, vector<TrainStopData> v) {
    ifstream dataFile(fileName);
    string line; int lc = 0;
    while (getline(dataFile, line)) {
        if (lc++ <= 1) continue;
        istringstream s(line);
        string field;
        int i = 0;
        TrainStopData tsd;
        while (getline(s, field, ',')) {
            switch (i++) {
                case 0: tsd.set_stop_id(field);         break;
                case 2: tsd.set_stop_name(field);       break;
                case 4: tsd.set_latitude(stod(field));  break;
                case 5: tsd.set_longitude(stod(field)); break;
                default: break;
            }
        }
        cout << tsd << endl;
    }
}

int main() {
    vector<TrainStopData> v;
    readFile("MTA_train_stop_data.webarchive", v);
}
