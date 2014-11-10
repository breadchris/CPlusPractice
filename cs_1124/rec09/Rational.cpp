
#include <iostream>
#include "Rational.h"

using namespace std;

namespace CS1124 {

    Rational::Rational(int _numerator, int _denominator)
        : numerator(_numerator), denominator(_denominator) {}
    Rational::Rational(int _numerator)
        : numerator(_numerator), denominator(1) {}
    Rational::Rational() {}


    std::istream &operator>>(istream& is, Rational& rational) {
        int numerator, denominator;
        char slash;
        is >> numerator >> slash >> denominator;
        return is;
    }

    std::ostream &operator<<(ostream& os, Rational& rational) {
        if (rational.denominator == 0) {
            os << "[-] Error: Denominator is 0" << endl;
        } else {
            int gcd = rational.gcd(rational.numerator, rational.denominator);
            os << rational.numerator / gcd << "/" << rational.denominator / gcd << endl;
        }
        return os;
    }

    // Operator overrides for member functions
    
    Rational& Rational::operator=(int num) {
        this->numerator = num;
        this->denominator = 1;
        return *this;
    }

    Rational& Rational::operator=(Rational& rational) {
        this->numerator = rational.numerator;
        this->denominator = rational.denominator;
        return *this;
    }

    Rational& Rational::operator+=(Rational& rational) {
        this->numerator *= rational.denominator;
        this->numerator += rational.numerator;
        this->denominator *= rational.numerator;
        return *this;
    }

    bool Rational::operator==(Rational& rational) const {
        return (this->numerator / this->denominator)
            == (rational.numerator / rational.denominator);
    }

    bool Rational::operator==(int num) const {
        return (this->numerator / this->denominator)
            == (num);
    }

    bool Rational::operator!=(Rational& rational) const {
        return (this->numerator / this->denominator)
            != (rational.numerator / rational.denominator);
    }

    bool Rational::operator!=(int num) const {
        return (this->numerator / this->denominator)
            != (num);
    }

    Rational& Rational::operator++() {
        this->numerator += this->denominator;
        return *this;
    }

    Rational Rational::operator++(int z) {
        this->numerator += this->denominator;
        return Rational(this->numerator, this->denominator);
    }

    bool Rational::operator<(Rational& rational) {
        return (this->numerator / this->denominator)
            < (rational.numerator / rational.denominator);
    }

    bool Rational::operator<=(Rational& rational) {
        return (this->numerator / this->denominator)
            <= (rational.numerator / rational.denominator);
    }

    bool Rational::operator>(Rational& rational) {
        return (this->numerator / this->denominator)
            > (rational.numerator / rational.denominator);
    }

    // Operator overrides for non-member functions
    
    Rational operator+(Rational& r1, Rational& r2) {
        return r1 += r2;
    }

    Rational& operator--(Rational& r1) {
        r1.setNumerator(r1.getNumerator() - r1.getDenominator());
        return r1;
    }

    Rational& operator--(Rational& r1, int z) {
        r1.setNumerator(r1.getNumerator() - r1.getDenominator());
        return r1;
    }

    bool operator>=(Rational& r1, Rational& r2) {
        return (r1.getNumerator() / r1.getDenominator())
            >= (r2.getNumerator() / r2.getDenominator());
    }

    void Rational::setNumerator(int _numerator) {
        this->numerator = _numerator;
    }

    void Rational::setDenominator(int _denominator) {
        this->denominator = _denominator;
    }

    int Rational::getNumerator() {
        return this->numerator;
    }

    int Rational::getDenominator() {
        return this->denominator;
    }

    int Rational::gcd(int x, int y) {
        while (y != 0) {
            int temp = x % y;
            x = y;
            y = temp;
        }
        return x;
    }
}

