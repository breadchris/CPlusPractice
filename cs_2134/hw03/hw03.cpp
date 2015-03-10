// Question 1
class Vector
{
public:
    // ...
    iterator erase( iterator vItr) {
        if (!theSize) return;

        delete vItr;
        iterator endItr = end();
        iterator startItr = vItr;
        while (startItr != endItr) {
            *startItr = *(startItr + 1)
            startItr++;
        }
        return vItr;
    }
    // ...
};

// Question 2
#include <iostream>
#include <set>
#include <vector>

using namespace std;

template<class T>
struct dup_compare {
    bool operator() (const T& a, const T& b) const{ return a != b; }
};

template<class Object>
bool containsDuplicates(vector<Object>& v) {
    set< Object, dup_compare<Object> > noDup;

    for (Object& obj : v) { if (!noDup.insert(obj).second) return true; }

    return false;
}

int main() {
    vector<int>* vec = new vector<int>();
    for (int i=1; i<=5; ++i) vec->push_back(i*10);

    cout << containsDuplicates(*vec) << endl;

    vec->push_back(20);

    cout << containsDuplicates(*vec) << endl;
}

// Question 3
class trainStopData {
public:

	trainStopData(string& id, string& name, double lat, double lon) : stop_id(id), stop_name(name), stop_lat(lat), stop_lon(lon) {}


	const string& get_id() const { return stop_id; }
	const string& get_stop_name() const { return stop_name; }
	double get_latitude() const { return stop_lat; }
	double get_longitude() const { return stop_lon; }
private:
	string stop_id;   // id of train stop
	string stop_name; // name of train station
	double stop_lat;  // latitude of train stop location
	double stop_lon;  // longitude of train stop location
};

template <class itr, class pred_func, class op_func>
int perform_if(itr itrStart, itr itrEnd, pred_func pred, op_func op) {
	int count = 0;
	while(itrStart != itrEnd) {
		if(pred(*itrStart)) {
			op(*itrStart);
			count++;
		}
		itrStart++;
	}
	return count;
}

class isStopOnRoute {
public:
	isStopOnRoute(char r) : route(r) {}

	bool operator()(const trainStopData& tsd) const {
		return (tsd.get_id()).at(0) == route;
	}

private:
	char route;
};

class isSubwayStop {
public:
	isSubwayStop(const string& id) : stopId(id) {}

	bool operator()(const trainStopData& tsd) {
		return stopId == tsd.get_id();
	}

private:
	string stopId;
};

class isSubwayStopNearX {
public:
	isSubwayStopNearX(double lat, double lon, double d) : latitude(lat), longitude(lon), d(d) {}

	bool operator()(const trainStopData& tsd) {

		return haverdist(latitude, longitude, tsd.get_latitude(), tsd.get_longitude()) <= d;
	}

private:
	double latitude;
	double longitude;
	double d;
};

class printTrainStopInfo {
public:
	void operator()(const trainStopData& tsd) {
		cout << "----------------------" << endl;
		cout << "Stop ID: " << tsd.get_id() << endl;
		cout << "Stop Name: " << tsd.get_stop_name() << endl;
		cout << "Stop Coordinates (latitude, longitude): " << setprecision(10) << tsd.get_latitude() << ", " << setprecision(10) << tsd.get_longitude() << endl; // set precision to high number to avoid rounding from atof() function.
		cout << "----------------------" << endl;
	}
};

void tokenizer(char s[], vector<string>& tokens, const char delim[]) {
	tokens.clear();
	char *pch;
	pch = strtok(s, delim);
	while(pch != NULL) {
		string temp = pch;
		tokens.push_back(pch);
		pch = strtok(NULL, delim);
	}
}

void batchInput(vector<trainStopData>& train_stops) {
	ifstream ifs("MTA_train_stop_data.txt");
	if(!ifs) {
		cout << "Error opening file!" << endl;
		return;
	}
	const char *delim = ",";
	string line;
	getline(ifs, line);
	vector<string> tokens;
	while(getline(ifs, line)) {
		char temp[line.size() + 1];
		strcpy(temp, line.c_str());
		tokenizer(temp, tokens, delim);
		double templat =  atof(tokens[2].c_str());
		double templon =  atof(tokens[3].c_str());

		trainStopData temptsd(tokens[0], tokens[1], templat, templon);
		train_stops.push_back(temptsd);
	}

}

void menu(vector<trainStopData>& train_stops) {
	int choice, counter;
	string search_key;
	double lat, lon, d;
    vector<trainStopData>::iterator itrStart;
    vector<trainStopData>::iterator itrEnd;
	printTrainStopInfo p;

	while(true)
	{
		cout << "Choose an option or enter 0 to exit" << endl;
		cout << "1: Provide a subway stop id, and have the location returned to you." << endl;
		cout << "2: Provide a route, and have all of its stops listed for you." << endl;
		cout << "3: Provide a set of coordinates and a distance, and have the stops within this distance listed for you." << endl;

		itrStart = train_stops.begin();
		itrEnd = train_stops.end();
		counter = 0;

		cin >> choice;
		cin.ignore();

		switch(choice)
		{
			case 0:
				return;

			case 1:
			{
				cout << "Enter the stop id: ";
			    cin >> search_key;
			    isSubwayStop iss(search_key);
				counter = perform_if(itrStart, itrEnd, iss, p);
				if(counter == 0) {
					cout << "There are no stops that match your search.";
				}
				break;
			}
			case 2:
			{
				cout << "Enter the name of the subway route (Just a letter or number): ";
                cin >> search_key;
                isStopOnRoute isor(search_key.at(0));
                counter = perform_if(itrStart, itrEnd, isor, p);
                if(counter == 0) {
					cout << "There are no routes that match your search.";
				}
				break;
            }
			case 3:
			{
				cout << "Enter the lattitude of the coordinate: ";
				cin >> lat;
				cout << "Enter the longitude of the coordinate: ";
				cin >> lon;
				cout << "Enter the distance: ";
				cin >> d;
				isSubwayStopNearX issnx(lat, lon, d);
				counter = perform_if(itrStart, itrEnd, issnx, p);
				if(counter == 0) {
					cout << "There are no stops within that distance.";
				}
				break;
			}
			default:
			{
				cout << "Menu option not found. Try again." << endl;
				break;
			}
		}
		cout << endl;
	}
}
