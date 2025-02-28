#include "Complex.h"
#include <cmath>

Complex::Complex(double r, double i) : real(r), imag(i) {}
Complex::Complex(const Complex& other) : real(other.real), imag(other.imag) {}

Complex& Complex::operator=(const Complex& other) {
    if (this != &other) {
        real = other.real;
        imag = other.imag;
    }
    return *this;
}

Complex::~Complex() noexcept {}

double Complex::get_real() const {return real;}
double Complex::get_imag() const {return imag;}

void Complex::set_real(double r) {real = r;}
void Complex::set_imag(double i) {imag = i;}

Complex& Complex::operator+=(const Complex& other) {
    real += other.real;
    imag += other.imag;
    return *this;
}

Complex Complex::operator+(Complex& other) const {
    return other += *this;
}

Complex& Complex::operator-=(const Complex& other) {
    real -= other.real;
    imag -= other.imag;
    return *this;
}

Complex Complex::operator-(const Complex& other) const {
    Complex result = *this;
    result -= other;
    return result;
}

Complex& Complex::operator*=(const Complex& other) {
    real = real * other.real - imag * other.imag;
    imag = real * other.imag + imag * other.real;
    return *this;
}

Complex Complex::operator*(Complex& other) const {
    return other *= *this;
}

Complex& Complex::operator/=(const Complex& other) {
    const double epsilon = 1e-9;

    double denom = other.real * other.real + other.imag * other.imag;
    if (std::abs(denom) < epsilon) {
        throw std::invalid_argument("Division by zero ?_?");
    }
    double new_real = (real * other.real + imag * other.imag) / denom;
    double new_imag = (imag * other.real - real * other.imag) / denom;

    real = new_real;
    imag = new_imag;
    return *this;
}

Complex Complex::operator/(const Complex& other) const {
    Complex result = *this;
    result /= other;
    return result;
}

double Complex::absolute() const {
    return std::sqrt(real * real + imag * imag);
}

double Complex::argument() const {
    return std::atan2(imag, real);
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << "(" << c.real << " + " << c.imag << "i)";
    return os;
}

std::istream& operator>>(std::istream& is, Complex& c) {
    is >> c.real >> c.imag;
    return is;
}
