#include <iostream>
#include <vector>

using namespace std;

// Question 2
template <class Object>
Object summation(int n, Object x) {
    if (n-- == 0) return x;
    cout << ++x << endl;
    return summation(n, x);
}

void pattern(int n) {
    if (n==0) return;
    pattern(n-1); cout << n; pattern(n-1);
}

template<class Object>
Object addParts(const vector<Object> & a, int l, int r) {
    if (l == r) return a[l];
    int m = (l + r) / 2;
    return addParts(a, l, m) + addParts(a, m+1, r);
}

template<class Object>
Object sumVector( const vector<Object> & a) {
    return addParts(a, 0, a.size());
}

void swap(char& a, char& b) {
    char tmp;
    tmp = a;
    a = b;
    b = tmp;
}

void permute(string& str, int i, int n) 
{
   int j; 
   if (i == n)
       cout << str << endl;
   else
   {
        for (j = i; j <= n; j++)
       {
          swap(str[i], str[j]);
          permute(str, i + 1, n);
          swap(str[i], str[j]);
       }
   }
}

template<class Comparable>
void printVec(vector<Comparable> & a) {
    for (int i = 0; i < a.size(); i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

template<class Comparable>
void insertionSort( vector<Comparable> & a )
{
    int j;
    for( int p = 1; p < a.size( ); p++ )
    {
         Comparable tmp = a[ p ];
         for( j = p; j > 0 && tmp < a[ j - 1 ]; j-- )
                  a[ j ] = a[ j - 1 ];
         a[ j ] = tmp;
         printVec(a);  // prints the contents of the vector in order
    }
}

int main() {
    // Question 2
    cout << summation(5, 10) << endl;

    // Question 3
    pattern(5);
    cout << endl;

    // Question 4
    vector<int> a = vector<int>(2, 11);
    cout << sumVector(a) << endl;

    // Question 5
    string s = "cat";
    permute(s, 0, s.length() - 1);

    vector<int>* asdf = new vector<int>(8);
    asdf->push_back(9);
    asdf->push_back(8);
    asdf->push_back(-11);
    asdf->push_back(2);
    asdf->push_back(0);
    asdf->push_back(3);
    insertionSort(*asdf);
}

