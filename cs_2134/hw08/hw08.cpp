#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <algorithm>
#include <set>

using namespace std;

// Question 1
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

void inputTrainData(map<string, int>& stop_id_to_int ,vector<string>& int_to_stop_id) {
	
	ifstream ifs("MTA_train_stop_data.txt");
	if(!ifs) {
		cerr << "Error opening file!" << endl;
		return;
	}
	string line;
	getline(ifs, line);
	int tracker = 0;
	const char *delim = ",";
	vector<string> tokens;
	while(getline(ifs, line)) {
		char temp[line.size() + 1];
		strcpy(temp, line.c_str());
		tokenizer(temp, tokens, delim);
		stop_id_to_int.insert(pair<string, int>(tokens[0], tracker++));
		int_to_stop_id.push_back(tokens[0]);
	}
}

void inputTransfers(vector< list<int> >& adjacent, map<string, int>& stop_id_to_int, vector<string>& int_to_stop_id) {
	ifstream ifs("transfers.txt");
	if(!ifs) {
		cerr << "Error opening file!" << endl;
		return;
	}
	string line;
	getline(ifs, line);

	adjacent[0].push_front(1);
	adjacent[adjacent.size()-1].push_front(int_to_stop_id.size()-2);
	for(int i = 1; i < int_to_stop_id.size()-1; i++) {

		char tl = int_to_stop_id[i][0];

		if(int_to_stop_id[i-1][0] == tl) {
			adjacent[i].push_front(i-1);
		}
		if(int_to_stop_id[i+1][0] == tl) {
			adjacent[i].push_front(i+1);
		}
	}

	const char *delim = ",";
	
	vector<string> tokens;
	while(getline(ifs, line)) {
		char temp[line.size() + 1];
		strcpy(temp, line.c_str());
		tokenizer(temp, tokens, delim);
		int s = stop_id_to_int[tokens[0]];
		int t = stop_id_to_int[tokens[1]];
		adjacent[s].push_front(t);
	}
}




//Code from Prof Lisa Hellerstein (very slightly modified)
// CS2134
//
// This is a bare-bones implementation of the single-source unweighted
// shortest paths algorithm 
// It does not have nice classes.

// Assumption: The vertices of the graph are labeled from 0 to n - 1,
// where n is the number of vertices.


const int DEFAULT_VAL =  -1;      // must be less than 0

// typedef vector< train_stop_data > Graph;        // You need to change the code to use this data type
typedef vector<list<int> > Graph;        // The graph is given in an adjacency list.
                                         // Vertices are indexed from 0 to V-1
                                         // The indices of the vertices adjacent to vertex i
                                         // are in the list Graph[i].
                                         // Graph can be directed or undirected.

struct vertexInf                    // Stores information for a vertex
{                                   
    int dist;  // distance to vertex from the source
    int prev;  // previous node in BFS tree
};

void printpath(int j,  vector<vertexInf> & vinfo, vector<string>& int_to_stop_id)
{
    stack<int> t;

    int current = j;
    while (current != DEFAULT_VAL)
    {
       t.push(current);
       current = vinfo[current].prev;
    }
    while (!t.empty())
    {
       cout << int_to_stop_id[t.top()] << " ";
       t.pop();
    }
}
     

// Breadth First Search
// The unweighted shortest path algorithm on the graph g, with vertex i as the source
// Prints the length (number of edges) of the shortest path from the source to every vertex
// in the graph

void shortestpaths( Graph& g, const string& stop_from, const string& stop_to, map<string, int>& stop_id_to_int, vector<string>& int_to_stop_id )
{
    queue<int> q;             // q is the queue of vertex numbers
    int s = stop_id_to_int[stop_from];
    if (s < 0 || s > g.size()-1) { 
    	cout << "Bad index" << endl;
        return;
    }

    vector<vertexInf> vertices(g.size());               // stores BFS info for the vertices
                                                        // info for vertex j is in position j

    for (int j=0; j < vertices.size(); ++j) {                // Initialize distances and prev values 
    	vertices[j].dist = DEFAULT_VAL; 
    	vertices[j].prev = DEFAULT_VAL; 
    }   


    vertices[s].dist = 0;

    q.push(s);
    while  (!q.empty() ) {
      int v = q.front();
      q.pop();
      for (list<int>::const_iterator w = g[v].begin(); w != g[v].end(); w++) { 

            if (vertices[*w].dist == DEFAULT_VAL) {        // distance of *w from source not determined yet
            
                vertices[*w].dist = vertices[v].dist+1;
                vertices[*w].prev = v;
                q.push(*w);
            }
      }
    }

	cout << "from: " << stop_from << endl;
	cout << "to: " << stop_to << endl;
	cout << "distance: " << vertices[stop_id_to_int[stop_to]].dist << endl;
	cout << "shortest path: "; 
	printpath(stop_id_to_int[stop_to],vertices, int_to_stop_id); 
	cout << endl << endl;
}

/*
int main()
{
	vector<string> int_to_stop_id;
	map<string, int> stop_id_to_int;
	inputTrainData(stop_id_to_int, int_to_stop_id);

	vector< list<int> > adjacent(int_to_stop_id.size());
	inputTransfers(adjacent, stop_id_to_int, int_to_stop_id);

    shortestpaths(adjacent, "F26","J21", stop_id_to_int, int_to_stop_id);

}
*/

// Question 2
int hash1(string str) {
	int hash = 0;

	for (int i = 0; i < str.length(); i++) {
		hash += str[i];
	}

	return hash >= 180000 ? hash - 180000 : hash;
}

// Based on the djb2 hashing algorithm
int hash2(string str) {
	unsigned long hash = 5381;

	for (int i = 0; i < str.length(); i++) {
		hash = ((hash << 5) + hash) + str[i];
	}

	return hash >= 180000 ? hash - 180000 : hash;
}

void hashWordsFromFile(string filename) {
	ifstream ifs(filename);
	if(!ifs) {
		cerr << "Error opening file!" << endl;
		return;
	}

	set<int> algo1hashes;
	set<int> algo2hashes;
	string line;
	while(getline(ifs, line)) {
		algo1hashes.insert(hash1(line));
		algo2hashes.insert(hash2(line));
	}

	cout << "Hash 1 unqiue hashes: " << algo1hashes.size() << endl;
	cout << "Hash 2 unqiue hashes: " << algo2hashes.size() << endl;
}

int main() {
	hashWordsFromFile("ENABLE.txt");
}
