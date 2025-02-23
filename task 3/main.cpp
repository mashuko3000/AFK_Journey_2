#include <iostream>
#include <cmath>
#include <stdexcept>

class Complex {
private:
    double real;
    double imag;
public:
    // Constructor
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // Constructor copying
    Complex(const Complex& other) : real(other.real), imag(other.imag) {}

    // The assignment copy operator
    Complex& operator=(const Complex& other){
        if (this != &other){
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

    //Destructor
    ~Complex() = default;

    double get_real() const {return real;}
    double get_imag() const {return imag;}

    void set_real(double r) {real = r;}
    void set_imag(double i) {imag = i;}

    Complex operator+(const Complex& other) const{
        return Complex(real + other.real, imag + other.imag);
    }

    Complex& operator+=(const Complex& other) {
        real += other.real;
        imag += other.imag;
        return *this;
    }

    Complex operator-(const Complex& other) const{
        return Complex(real - other.real, imag - other.imag);
    }

    Complex& operator-=(const Complex& other){
        real -= other.real;
        imag -= other.imag;
        return *this;
    }

    Complex operator*(const Complex& other) const{
        return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
    }

    Complex& operator*=(const Complex& other){
        real = real * other.real - imag * other.imag;
        imag = real * other.imag + imag * other.real;
        return *this;
    }

    Complex operator/(const Complex& other) const{
        double denom = other.real * other.real + other.imag * other.imag;
        if (denom == 0){
            throw std::invalid_argument("Division by zero ?_?");
        }
        return Complex((real * other.real + imag * other.imag)/denom, (imag * other.real - real * other.imag)/denom);
    }

    Complex& operator/=(const Complex& other){
        double denom = other.real * other.real + other.imag * other.imag;
        if (denom == 0){
            throw std::invalid_argument("Division by zero ?_?");
        }
        double new_real = (real * other.real + imag * other.imag)/denom;
        double new_imag = (imag * other.real - real * other.imag)/denom;

        real = new_real;
        imag = new_imag;

        return *this;
    }

    double absolute() const{
        return std::sqrt(real * real + imag * imag);
    }

    double argument() const{
        return std::atan2(imag, real);
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c){
        os << "(" << c.real << " + " << c.imag << "i)";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Complex& c){
        std::cout << "Enter real part: ";
        is >> c.real;
        std::cout << "Enter imaginary part: ";
        is >> c.imag;
        return is;
    }
};

int main(){
    Complex a(4.0, 2.0);
    Complex b(5.0, -1.0);

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    Complex c = a + b;
    std::cout << "a + b = " << c << std::endl;

    c = a - b;
    std::cout << "a - b = " << c << std::endl;

    c = a * b;
    std::cout << "a * b = " << c << std::endl;

    c = a/b;
    std::cout << "a / b = " << c << std::endl;

    std::cout << "Absolute of a: " << a.absolute() << std::endl;
    std::cout << "Argument of a: " << a.argument() << std::endl;

    Complex d;
    std::cin >> d;
    std::cout << "You entered: " << d << std::endl;
}