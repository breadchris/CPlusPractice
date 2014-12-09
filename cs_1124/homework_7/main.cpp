#include <iostream>
#include "Polynomial.h"

using namespace std;
using namespace PolyMath;

int main() {
    
    Node* head = new Node(5, nullptr);
    appendToHead(4, head);
    appendToHead(3, head);
    appendToHead(2, head);
    
    Polynomial poly1 = Polynomial(head, 4);
    
    Node* head2 = new Node(1, nullptr);
    appendToHead(1, head2);
    appendToHead(0, head2);
    appendToHead(3, head2);
    appendToHead(8, head2);
    appendToHead(9, head2);
    
    Polynomial poly2 = Polynomial(head2, 6);
    
    Node* head3 = new Node(5, nullptr);
    appendToHead(4, head3);
    appendToHead(3, head3);
    appendToHead(2, head3);
    
    Polynomial poly3 = Polynomial(head3, 4);
    
    cout << "Poly 1:\t\t" << poly1 << endl;
    cout << "Poly 2:\t\t" << poly2 << endl;
    cout << "Poly 3:\t\t" << poly3 << endl;
    
    bool equal = poly1 == poly2;
    cout << "Poly 1 == Poly 2: \t" << equal << endl;
    
    bool notequal = poly1 != poly3;
    cout << "Poly 1 != Poly 3: \t" << notequal << endl;
    
    cout << "Poly 1 + 2:\t" << poly1 + poly2 << endl;
    
    poly1 += poly2;
    
    cout << "Poly 1 += 2:\t" << poly1 << endl;
    cout << "Poly 2:\t\t" << poly2 << endl;
    
    int value = poly1.evaluate(5);
    
    cout << "Poly 1 evalute w 5:\t" << value << endl;
}