#include "RC4.hpp"
#include <iostream>

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
