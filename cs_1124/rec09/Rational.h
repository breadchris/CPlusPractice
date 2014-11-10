#ifndef RATIONAL_H
#define RATIONAL_H

#include <iostream>

namespace CS1124 {

    class Rational {
    protected:
        int numerator;
        int denominator;
    public:
        Rational(int _numerator, int _denominator);
        Rational(int _numerator);
        Rational();

        friend std::istream &operator>>(std::istream& is, Rational& rational);

        friend std::ostream &operator<<(std::ostream& os, Rational& rational);

        Rational& operator=(Rational& rational);
        Rational& operator=(int num);

        Rational& operator+=(Rational& rational);

        bool operator==(Rational& rational) const;
        bool operator==(int num) const;

        bool operator!=(Rational& rational) const;
        bool operator!=(int num) const;

        Rational& operator++();

        Rational operator++(int z);

        bool operator<(Rational& rational);

        bool operator<=(Rational& rational);

        bool operator>(Rational& rational);

        void setNumerator(int _numerator);
        void setDenominator(int _denominator);

        int getNumerator();
        int getDenominator();

        int gcd(int x, int y);
    };
    
}

#endif
