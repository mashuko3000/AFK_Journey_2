#include "RC4.hpp"
#include <iostream>

encoder::encoder(unsigned char const *key, size_t key_size) {
    set_key(key, key_size);
}

void encoder::set_key(unsigned char const *key, size_t key_size) {
    KSA(key, key_size);
}

void encoder::encode(const char *input_file, const char *output_file) {
    std::ifstream inputFile(input_file, std::ios::binary);
    std::ofstream outputFile(output_file, std::ios::binary);

    if (!inputFile || !outputFile) {
        throw std::runtime_error("Error, while trying to open file T_T");
    }

    unsigned char buffer[4096];
    while (inputFile.peek() != EOF) {
        inputFile.read(reinterpret_cast<char *>(buffer), sizeof(buffer));
        size_t bytebufferread = inputFile.gcount();
        for (size_t i = 0; i < bytebufferread; ++i) {
            buffer[i] ^= PRGA();
        }
        outputFile.write(reinterpret_cast<char *>(buffer), bytebufferread);
    }

    inputFile.close();
    outputFile.close();
}

void encoder::swap(unsigned char &a, unsigned char &b) {
    unsigned char temp = a;
    a = b;
    b = temp;
}

void encoder::KSA(const unsigned char *key, size_t key_size) {
    for (int i = 0; i < buf_capacity; i++) {
        buffer[i] = i;
    }

    j = 0;
    for (int i = 0; i < buf_capacity; i++) {
        j = (j + buffer[i] + key[i % key_size]) % buf_capacity;
        swap(buffer[i], buffer[j]);
    }

    i = j = 0;
}

unsigned char encoder::PRGA() {
    i = (i + 1) % buf_capacity;
    j = (j + buffer[i]) % buf_capacity;
    swap(buffer[i], buffer[j]);
    return buffer[(buffer[i] + buffer[j]) % buf_capacity];
}