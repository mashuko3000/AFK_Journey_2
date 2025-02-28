#ifndef RC4_HPP
#define RC4_HPP

#include <fstream>
#include <stdexcept>

class encoder final{
public:
    encoder(unsigned char const *key, size_t key_size);
    void set_key(unsigned char const *key, size_t key_size);
    void encode(const char *input_file, const char *output_file);
private:
    static const int buf_capacity = 256;
    unsigned char buffer[buf_capacity];
    int i, j;

    void swap(unsigned char &a, unsigned char &b);
    void KSA(const unsigned char *key, size_t key_size);
    unsigned char PRGA();
};

#endif