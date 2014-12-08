#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    Node (int data = 0, Node* link = nullptr) : data(data), link(link) {}
    int data;
    Node* link;
};

Node* appendToHead(int val, Node*& headPtr) {
	Node* tmp = new Node(val, headPtr);
    headPtr = tmp;
    return tmp;
}

struct TNode {
  TNode(int data = 0, TNode *left = nullptr, TNode *right = nullptr)
    : data(data), left(left), right(right) {}
  int data;
  TNode *left, *right;
};

int max(TNode* root) {
    if ( root == nullptr )
        return -1;

    int left = max(root->left);
    int right = max( root->right);
    if( root->data > left && root->data > right )
       return root->data;
    else
        return left > right ? left : right;
}

void printBinary(int num) {
    if (num < 2) {
        cout << num;
    } else {
        printBinary(num/2);
        cout << num % 2;
    }
}

int sum(Node* l1, Node* l2) {
    if (l1 == nullptr || l2 == nullptr)
        return 0;
    return l1->data + l2->data + sum(l1->link, l2->link);
}

int sumString(char* str) {
    if (*str == '\0') return 0;
    return *str + sumString(++str);
}

int binarySearch(char str[], char c, int low, int high) {
    if( low > high )
        return -1;
        
    int mid = ( low + high ) / 2;
    
    if( str[mid] - c < 0 )
        return binarySearch( str, c, mid + 1, high );
    else if( str[mid] - c > 0 )
        return binarySearch( str, c, low, mid - 1 );
    else
        return mid;
}

void func(int n) { 
   if (n > 1) { 
      cout << 'a'; 
      func(n/2); 
      cout << 'b'; 
      func(n/2); 
   } 
   cout << 'c'; 
}

int main() {
    cout << "Print binary" << endl;
    printBinary(3);
    cout << endl;
    
    cout << "Sum of lists" << endl;
    Node* target = NULL;
    appendToHead(6, target);
    appendToHead(5, target);
    appendToHead(4, target);
    appendToHead(2, target);
    appendToHead(3, target);
    appendToHead(2, target);
    appendToHead(3, target);
    appendToHead(2, target);
    appendToHead(1, target);
    cout << sum(target, target) << endl;
    
    cout << "Max of tree" << endl;
    TNode a(1), b(2), c(4), d(8, &a, &b), e(16, &c), f(32, &d, &e);
    cout << max(&f) << endl;
    
    cout << "Sum of c string" << endl;
    char str[] = "LOL THIS SUCKSdasdfasdf";
    
    cout << sumString(str) << endl;
    
    cout << "Binary search" << endl;
    char str2[] = "Somestring that has characters";
    char car = 'e';
    sort(str2, str2 + sizeof(str2));
    for (int i = 0; i < sizeof(str2); i++)
        cout << str2[i];
    cout << endl;
    cout << binarySearch(str2, car, 0, sizeof(str2)) << endl;
    
}
