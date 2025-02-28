#include <iostream>

class logical_values_array final{
private:
    unsigned int _value;
public:
    //Constructor
    logical_values_array(unsigned int value = 0) : _value(value) {}

    //Accessor for _value
    unsigned int getValue() const{
        return _value;
    }

    //Iversion
    logical_values_array operator~() const{
        return logical_values_array(~_value);
    }

    // Conjuction
    logical_values_array operator&(const logical_values_array& other) const{
        return logical_values_array(_value & other._value);
    }

    // Dicjunction
    logical_values_array operator|(const logical_values_array& other) const {
        return logical_values_array(_value | other._value);
    }

    // Implication
    logical_values_array implication(const logical_values_array& other){
        return logical_values_array(~_value | other._value);
    }

    // Coinplication
    logical_values_array coimplication(const logical_values_array& other){
        return logical_values_array(_value | ~other._value);
    }

    // Addition modulo 2 (XOR)
    logical_values_array operator^(const logical_values_array& other){
        return logical_values_array(_value ^ other._value);
    }

    // Equivalence (XNOR)
    logical_values_array equivalance(const logical_values_array& other){
        return logical_values_array(~(_value ^ other._value));
    }

    // Pierce arrow (NOR)
    logical_values_array pierce_arrow(const logical_values_array& other){
        return logical_values_array(~(_value | other._value));
    }

    // Sheffer stroke (NAND)
    logical_values_array sheffer_stroke(const logical_values_array& other){
        return logical_values_array(~(_value & other._value));
    }


    static bool equals(const logical_values_array& lhs, const logical_values_array& rhs){
        return lhs._value == rhs._value;
    }

    bool get_bit(size_t position) const{
        if (position >= sizeof(_value) * 8){
            throw std::out_of_range("Bit position is out of range -_-");
        }
        return (*this)[position];
    }

    bool operator[](size_t position) const{
        return ((_value >> position) & 1) != 0U;
    }

    void binary_string(char* buf) const{
        if (buf == nullptr){
            throw std::invalid_argument("Buf is empty +_=");
        }
        for (int i = sizeof(unsigned int) * 8 - 1; i>=0; --i){
            *buf++ = (((_value >> i) & 1) ? '1' : '0');
        }
        *buf = '\0';
    }
};

int main(){
    logical_values_array a(0b0101);
    logical_values_array b(0b0110);

    char buffer [50];

    try{
        std::cout << "a & b : ";
        
        (a & b).binary_string(buffer);
        std::cout << buffer << std::endl;

        std::cout << "a | b : ";
        (a | b).binary_string(buffer);
        std::cout << buffer << std::endl;

        std::cout << "a ^ b : ";
        (a ^ b).binary_string(buffer);
        std::cout << buffer << std::endl;

        std::cout << "~a : ";
        (~a).binary_string(buffer);
        std::cout << buffer << std::endl;

        std::cout << "a -> b : ";
        a.implication(b).binary_string(buffer);
        std::cout << buffer << std::endl;

        std::cout << "a <-> b : ";
        a.equivalance(b).binary_string(buffer);
        std::cout << buffer << std::endl;

        std::cout << "a NOR b : ";
        a.pierce_arrow(b).binary_string(buffer);
        std::cout << buffer << std::endl;

        std::cout << "a NAND b : ";
        a.sheffer_stroke(b).binary_string(buffer);
        std::cout << buffer << std::endl;

        std::cout << "a[2]: " << a[2] << std::endl;
    }

    catch (const std::out_of_range& e){
        std::cerr<<"Out of range error: " << e.what() << std::endl;
    }

    catch (const std::invalid_argument& e){
        std::cerr<<"Invalid argument error: " << e.what() << std::endl;
    }

    catch (...){
        std::cerr << "Unknown error X_X" << std::endl;
    }
    
    return 0;
}