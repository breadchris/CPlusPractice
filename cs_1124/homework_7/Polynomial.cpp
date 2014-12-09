#include <iostream>
#include <math.h>
#include "Polynomial.h"

using namespace std;

namespace PolyMath {
    Node::Node (int data = 0, Node* link = nullptr) : data(data), link(link) {};
    
    Node* appendToHead(int val, Node*& headPtr) {
    	Node* tmp = new Node(val, headPtr);
        headPtr = tmp;
        return tmp;
    }
    
    Polynomial::Polynomial(Node* _coefficients, int _degree) : coefficients(_coefficients), degree(_degree) {}
    
    Polynomial::Polynomial(const Polynomial& polynomial) {
        this->degree = polynomial.degree;
        this->coefficients = nullptr;
       
        Node* head = polynomial.coefficients;
        if (head != nullptr) {
            Node* oHead = new Node(head->data, nullptr);
            this->coefficients = oHead;
            
            while (head->link != nullptr) {
                head = head->link;
                oHead->link = new Node(head->data, nullptr);
                oHead = oHead->link;
            }
        }
    }
    
    Polynomial::~Polynomial() {
        Node* head = coefficients;
        while (head != nullptr) {
            Node* tmp = head->link;
            delete head;
            head = tmp;
        }
        
        coefficients = nullptr;
    }       
    
    Polynomial& Polynomial::operator+=(const Polynomial& polynomial) {
        if (this->degree < polynomial.degree)
            this->degree = polynomial.degree;
        Node* headPtr1 = this->coefficients;
        Node* headPtr2 = polynomial.coefficients;
        Node* prev = nullptr;
        
        while (headPtr1 != nullptr || headPtr2 != nullptr) {
            if (headPtr1 == nullptr) {
                prev->link = new Node(headPtr2->data, nullptr);
            } else if (headPtr2 != nullptr) {
                headPtr1->data += headPtr2->data;
            }
            
           
            if (headPtr1 != nullptr) {
                prev = headPtr1;
                headPtr1 = headPtr1->link;
            } else 
                prev = prev->link;
            if (headPtr2 != nullptr) 
                headPtr2 = headPtr2->link;
            
        }
        
        return *this;
    }
    
    Polynomial& Polynomial::operator+(const Polynomial& polynomial) {
        Polynomial* newPoly = new Polynomial(*this);
        *newPoly += polynomial;
        return *newPoly;
    }
    
    bool Polynomial::operator==(const Polynomial& polynomial) {
        if (this->degree == polynomial.degree) {
            Node* coeffPtr1 = this->coefficients;
            Node* coeffPtr2 = polynomial.coefficients;
            
            while (coeffPtr1 != nullptr && coeffPtr2 != nullptr ) {
                if (coeffPtr1->data != coeffPtr2->data) return false;
                coeffPtr1 = coeffPtr1->link;
                coeffPtr2 = coeffPtr2->link;
            }
            
            return true;
        } else {
            return false;
        }
    }
    
    bool Polynomial::operator!=(const Polynomial& polynomial) {
        return !(*this == polynomial);
    }
    
    int Polynomial::evaluate(int value) {
        int returnValue = 0;
        int power = 0;
        Node* coeffPtr = this->coefficients;
        while (coeffPtr != nullptr) {
            returnValue += coeffPtr->data * pow(value, power);
            power++;
            coeffPtr = coeffPtr->link;
        }
        
        return returnValue;
    }
    
    int Polynomial::getDegree() const {
        return this->degree;
    }
    
    Node* Polynomial::getCoefficients() const {
        return this->coefficients;
    }
    
    std::ostream& operator<<(ostream& os, const Polynomial& polynomial) {
        Node* head = polynomial.getCoefficients();
        int power = 0;
        bool coeffIsZero = false;
        while (head != nullptr) {
            if (head->data != 0)
                if (power == 0)
                    os << head->data;
                else
                    os << head->data << "x^" << power;
            else
                coeffIsZero = true;
            
            power++;
            if (power != polynomial.getDegree() && !coeffIsZero) {
                cout << " + ";
            }
            
            coeffIsZero = false;
            head = head->link;
        }
        
        return os;
    }
}