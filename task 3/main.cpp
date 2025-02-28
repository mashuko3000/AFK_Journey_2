#include <iostream>
#include "complex.h"

int main(){
    Complex a(4.0, 2.0);
    Complex b(5.0, -1.0);
    Complex c;
    Complex d;

    try{
        std::cout << "a = " << a << std::endl;
        std::cout << "b = " << b << std::endl;
        c = a + b;
        std::cout << "a + b = " << c << std::endl;

        c = a - b;
        std::cout << "a - b = " << c << std::endl;

        c = a * b;
        std::cout << "a * b = " << c << std::endl;

        c = a/b;
        std::cout << "a / b = " << c << std::endl;

        std::cout << "Absolute of a: " << a.absolute() << std::endl;
        std::cout << "Argument of a: " << a.argument() << std::endl;

        
        std::cin >> d;
        std::cout << "You entered: " << d << std::endl;
    }
    catch (const std::invalid_argument& e){
        std::cerr << "Invalid argument error Y_Y: " << e.what() << std::endl;
    }
    catch (...){
        std::cerr << "Unknown error X_X" << std::endl;
    }
    return 0;
}