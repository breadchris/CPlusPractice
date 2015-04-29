#include <iostream>
#include <vector>

using namespace std;

// Question 1
class closest_stop {
public:
    closest_stop(double lat, double lon) : latitude(lat), longitude(lon), shortest_dist(numeric_limits<double>::max()) {}
    bool operator()(const trainStopData& tsd) {
        double new_dist = haverdist(latitude, longitude, tsd.get_latitude(), tsd.get_longitude());
        if (new_dist < shortest_dist) {
            shortest_dist = new_dist;
            return true;
        }
        return false;
    }
private:
    double latitude;
    double longitude;
    double shortest_dist;
};

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

template <class Itr>
void mergeSort(Itr tmp, Itr first, Itr last ) {
    if (first + 1 != last) {
        Itr mid = first + ((last-first)/2);
        mergeSort(tmp, first, mid);
        mergeSort(tmp, mid, last);
        Itr tmpEnd = merge(first, mid, mid, last, tmp);
        while(tmp != tmpEnd) {
            *first = *tmp;
            first++;
            tmp++;
        }
    }
}

class student {
private:
    string name;
public:
    student(const string& _name):name(_name) {}
    string& getName() {
        return name;
    }
};

struct meFirst {
  meFirst(string& _me) : me(_me) {}
  bool operator()(student& a, student& b) {
    if (a.getName() == me) return true;
    return a.getName().compare(b.getName()) < 0 ? true : false;
  }
private:
  string me;
};

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

    // Question 6
    vector<int> ints ={10, 80, 14, 77, 35, 21, 99, 12};
    mergeSort(ints.begin(), ints.end());
    
    for (int num: ints) {
        cout << num << ", ";
    }

    cout << endl;

    // Question 7
    student* s1 = new student("Student 1");
    student* s2 = new student("Student 2");
    student* s3 = new student("Student 3");
    student* s4 = new student("Student 4");
    student* s5 = new student("Student 5");

    vector<student> lol = {*s1, *s2, *s3, *s4, *s5};
    sort(lol.begin(), lol.end(), meFirst("Student 3"));

    for (student haha: lol) {
        cout << haha.get_name()  << ", ";
    }

    cout << endl;
}

