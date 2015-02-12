#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Node {
public:
   Node( char ch, Node * ptr = nullptr):data(ch),next(ptr){}
   char data;
   Node * next;
};

void addNode(Node* n, char c) {
	n->next = new Node(c, n->next);
}

template <typename T>
bool containsDuplicate(vector<T> v) {
	vector<T> unique;
	for (int i = 0; i < v.size(); i++)
		for (int j = 0; j < unique.size(); j++)
			if (v[i] == unique[j])
				return true;
			else
				unique.push_back(v[i]);
	return false;
}

void pointerStuff() {
	Node* nodePtr;
	nodePtr = new Node('C');
	addNode(nodePtr, 'D');
	addNode(nodePtr, 'B');

	nodePtr = new Node('A', nodePtr);

	Node* tmp = nodePtr;
	while (tmp != nullptr) {	
		cout << "Address: " << tmp << " Value: " << tmp->data << endl;
		tmp = tmp->next;
	}

	tmp = nodePtr->next;
	free(nodePtr);
	nodePtr = tmp;
}

void arrayStuff() {
	int * intPtr = new int[5];
	int * tmpPtr = intPtr;
	*tmpPtr = 2; tmpPtr++;
	*tmpPtr = 3; tmpPtr++;
	*tmpPtr = 4; tmpPtr++;
	*tmpPtr = 5;

	for (tmpPtr = intPtr + 4; tmpPtr > intPtr; tmpPtr--) {
		*tmpPtr = *(tmpPtr - 1);
	}
	*intPtr = 1;

	for (tmpPtr = intPtr; tmpPtr < intPtr + 5; tmpPtr++) {
		cout << "Address: " << tmpPtr << " Value: " << *tmpPtr << endl;
	}

	delete[] intPtr;
}

// Included from given file

// timer class
class timer {
public:
    timer() : start(clock()) {}
    double elapsed() { return ( clock() - start ) / CLOCKS_PER_SEC; }
    void reset() { start = clock(); }
private:
    double start;
};


// Linear maximum contiguous subsequence sum algorithm.
// Comparable: must have constructor from int, must have
//     += and > defined, must have copy constructor
//     and operator=, and must have all properties
//     needed for vector.
// seqStart and seqEnd represent the actual best sequence.
template <class Comparable>
Comparable maxSubsequenceSum4( const vector<Comparable> & a, int & seqStart, int & seqEnd )
{
    int n = a.size( );
    Comparable thisSum = 0;
    Comparable maxSum = 0;
	
    for( int i = 0, j = 0; j < n; j++ )
    {
        thisSum += a[ j ];
		
        if( thisSum > maxSum )
        {
            maxSum = thisSum;
            seqStart = i;
            seqEnd = j;
        }
        else if( thisSum < 0 )
        {
            i = j + 1;
            thisSum = 0;
        }
    }
    return maxSum;
}



// Quadratic maximum contiguous subsequence sum algorithm.
// Comparable: must have constructor from int, must have
//     += and > defined, must have copy constructor
//     and operator=, and must have all properties
//     needed for vector.
// seqStart and seqEnd represent the actual best sequence.
template <class Comparable>
Comparable maxSubsequenceSum2( const vector<Comparable> & a,int & seqStart, int & seqEnd )
{
    int n = a.size( );
    Comparable maxSum = 0;
	
    for( int i = 0; i < n; i++ )
    {
        Comparable thisSum = 0;
        for( int j = i; j < n; j++ )
        {
            thisSum += a[ j ];
			
            if( thisSum > maxSum )
            {
                maxSum = thisSum;
                seqStart = i;
                seqEnd = j;
            }
        }
    }
	
    return maxSum;
}



// Cubic maximum contiguous subsequence sum algorithm.
// Comparable: must have constructor from int, must have
//     += and > defined, must have copy constructor
//     and operator=, and must have all properties
//     needed for vector.
// seqStart and seqEnd represent the actual best sequence.
template <class Comparable>
Comparable maxSubsequenceSum1( const vector<Comparable> & a, int & seqStart, int & seqEnd )
{
    int n = a.size( );
    Comparable maxSum = 0;
	
    for( int i = 0; i < n; i++ )
        for( int j = i; j < n; j++ )
        {
            Comparable thisSum = 0;
            for( int k = i; k <= j; k++ )
                thisSum += a[ k ];
			
            if( thisSum > maxSum )
            {
                maxSum = thisSum;
                seqStart = i;
                seqEnd = j;
            }
        }
	
    return maxSum;
}

int main() {
	cout << "==== Nodes ====" << endl;
	pointerStuff();
	cout << "==== Array ====" << endl;
	arrayStuff();

	cout.setf( ios::fixed, ios::floatfield );
    cout.precision( 6 );

	int x;
	vector<int> items;
	int n[] = {128, 256, 512, 1024, 2048, 4096};

	for (int size = 0; size < 6; size++) {
		for (int i = 0; i < n[size]; i++) {
			items.push_back((rand() % 2001) - 1000);
		}

		int ss, se;
		timer t;
		double nuClicks;

		t.reset();
		maxSubsequenceSum1( items, ss, se);
		nuClicks = t.elapsed();
		cout << "Sum1 - n: " << n[size] << " time elapsed: " << nuClicks << endl;

		t.reset();
		maxSubsequenceSum2( items, ss, se);
		nuClicks = t.elapsed();
		cout << "Sum2 - n: " << n[size] << " time elapsed: " << nuClicks << endl;

		t.reset();
		maxSubsequenceSum4( items, ss, se);
		nuClicks = t.elapsed();
		cout << "Sum4 - n: " << n[size] << " time elapsed: " << nuClicks << endl;
		items.clear();
	}
}
