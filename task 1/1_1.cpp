#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <climits>


class Encoder{
private:
    unsigned char* key;
    size_t keySize;
    unsigned char S[256];

    void initRC4(){
        for (int i = 0; i < 256; ++i){
            S[i] = static_cast <unsigned char>(i);
        }
        int j = 0;
        for (int i = 0; i < 256; ++i){
            j = (j + S[i] + key[i % keySize])%256;
            std::swap(S[i], S[j]);
        }
    }

    unsigned char generateKey(int& i, int& j, unsigned char* state){
        i = (i+j)%256;
        j = (j + state[i]) % 256;
        std::swap(state[i], state[j]);
        return state[(state[i] + state[j]) % 256];
    }

    unsigned char* readFile(const char* filePath, size_t& fileSize){
        //opening the file
        std::ifstream file(filePath, std::ios::binary);
        if (!file){
            throw std::runtime_error("Error, while trying to open file T_T");
        }

        // finding the size of file
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        unsigned char* buffer = (unsigned char*)malloc(fileSize* sizeof(unsigned char));
        if (buffer == nullptr){
            throw std::runtime_error("Error, while trying allocate memory &_&");
        }

        if (!file.read(reinterpret_cast<char*>(buffer), fileSize)){
            free(buffer);
            throw std::runtime_error("Error, while trying to read file !_!");
        }

        file.close();
        return buffer;
    }

    void writeFile(const char* filePath, const unsigned char* data, size_t dataSize){
        std::ofstream file(filePath, std::ios::binary);
        if (!file){
            throw std::runtime_error("Error opening file, while trying to write -_-,");
        }

        if (!file.write(reinterpret_cast<const char*>(data), dataSize)){
            throw std::runtime_error("Error, while trying to read file !_!");
        }
        file.close();
    }
public:
    // Constructer with RC4
    Encoder(unsigned char const* keyData, size_t keyDataSize) : key(nullptr), keySize(keyDataSize)
    {
        key = (unsigned char*)malloc(keySize* sizeof(unsigned char));
        if (key == nullptr){
            throw std::runtime_error("Error, while trying allocate memory &_&");
        }

        memcpy(key, keyData, keySize);
        initRC4();
    }

    Encoder(const Encoder& str) : key(nullptr), keySize(str.keySize){
        key = (unsigned char*)malloc(keySize* sizeof(unsigned char));
        if (key == nullptr){
            throw std::runtime_error("Error, while trying allocate memory ,_,");
        }
        memcpy(key, str.key, keySize);
        memcpy(S, str.S, sizeof(S));
    }

    Encoder& operator=(const Encoder& str){
        if (this != &str){
            free(key);
            keySize = str.keySize;
            key = (unsigned char*)malloc(keySize* sizeof(unsigned char));
            if (key == nullptr){
                throw std::runtime_error("Error, while trying allocate memory +_+");
            }
            memcpy(key, str.key, keySize);
            memcpy(S, str.S, sizeof(S));
        }
        return *this;
    }

    ~Encoder(){
        free(key);
    }

    void keyMutator(const char* input, const char* output, bool encrypt){
        size_t dataSize = 0;
        unsigned char* data = nullptr;
        unsigned char* result = nullptr;

        try
        {
            data = readFile(input, dataSize);
            result = (unsigned char*)malloc(dataSize* sizeof(unsigned char));
            if (key == nullptr){
                throw std::runtime_error("Error, while trying allocate memory Y_Y");
            }

            int i = 0, j = 0;
            unsigned char currentS[256];
            std::memcpy(currentS, S, sizeof(S));

            for(size_t k = 0; k < dataSize; ++k){
                unsigned char keyStrem = generateKey(i, j, currentS);
                result[k] = data[k]^keyStrem;
            }
            writeFile(output, result, dataSize);
            std::cout << (encrypt ? "File is encrypted" : "File if decrypted") << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            if (data) free(data);
            if (result) free(result);
            throw;
        }
        free(data);
        free(result);
    }
};

int main(){
    try{
        unsigned char keyData[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };
        size_t keySize = sizeof(keyData);

        Encoder encoder(keyData, keySize);

        encoder.keyMutator("test.txt", "output.enc", true);
        encoder.keyMutator("output.enc", "decrypted.txt", false);
    }
    catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}