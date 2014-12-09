#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iostream>

namespace PolyMath {
    
    struct Node {
        Node (int, Node*);
        int data;
        Node* link;
    };

    class Polynomial {
    private:
        Node* coefficients;
        int degree;
    public:
        Polynomial(Node*, int);
        
        Polynomial(const Polynomial&);
        
        ~Polynomial();
        
        Polynomial& operator+=(const Polynomial&);
        
        Polynomial& operator+(const Polynomial&);
        
        bool operator==(const Polynomial&);
        
        bool operator!=(const Polynomial&);
        
        int evaluate(int);
        
        int getDegree() const;
        
        Node* getCoefficients() const;
        
    };
    
    Node* appendToHead(int, Node*&);
    
    std::ostream& operator<<(std::ostream&, const Polynomial&);
    
}

#endif