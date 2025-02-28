#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <stdexcept>

class Complex final {
private:
    double real;
    double imag;
public:
    Complex(double r = 0.0, double i = 0.0);
    Complex(const Complex& other);

    Complex& operator=(const Complex& other);

    virtual ~Complex() noexcept;

    double get_real() const;
    double get_imag() const;
    void set_real(double r);
    void set_imag(double i);

    Complex& operator+=(const Complex& other);
    Complex operator+(Complex& other) const;
    Complex& operator-=(const Complex& other);
    Complex operator-(const Complex& other) const;
    Complex& operator*=(const Complex& other);
    Complex operator*(Complex& other) const;
    Complex& operator/=(const Complex& other);
    Complex operator/(const Complex& other) const;

    double absolute() const;
    double argument() const;

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    friend std::istream& operator>>(std::istream& is, Complex& c);
};

#endif