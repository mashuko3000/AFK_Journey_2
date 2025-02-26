#include <fstream>
#include <stdexcept>
#include <iostream>

class encoder final{
public:
    encoder(unsigned char const *key, size_t key_size) {
        set_key(key, key_size);
    }

    void set_key(unsigned char const *key, size_t key_size) {
        KSA(key, key_size);
    }

    void encode(const char *input_file, const char *output_file) {
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

private:
    static const int buf_capacity = 256;
    unsigned char buffer[buf_capacity];
    int i, j;

    inline void swap(unsigned char &a, unsigned char &b) {
        unsigned char temp = a;
        a = b;
        b = temp;
    }

    void KSA(const unsigned char *key, size_t key_size) {
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

    unsigned char PRGA() {
        i = (i + 1) % buf_capacity;
        j = (j + buffer[i]) % buf_capacity;
        swap(buffer[i], buffer[j]);
        return buffer[(buffer[i] + buffer[j]) % buf_capacity];
    }
};

int main() {
    unsigned char key[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };

    const char* input_file = "test.txt";
    const char* encrypted_file = "encrypted.bin";
    const char* decrypted_file = "decrypted.txt";

    try {
        std::cout << "Encryption of file: " << input_file << std::endl;
        encoder enc(key, sizeof(key));
        enc.encode(input_file, encrypted_file);
        std::cout << "Encrypting was finished. The result was saved in " << encrypted_file << std::endl;

        std::cout << "Decryption of file: " << encrypted_file << std::endl;
        encoder dec(key, sizeof(key));
        dec.encode(encrypted_file, decrypted_file);

        encoder encc(key, sizeof(key));
        encc.encode("image.jpg", "image_encrypted.jpg");
        encoder decc(key, sizeof(key));
        decc.encode("image_encrypted.jpg", "image_decrypted.jpg");

        std::cout << "Decryption was finished. The result was saved in " << decrypted_file << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error ?_?: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
