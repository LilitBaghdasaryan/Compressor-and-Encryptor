#include <iostream>
#include <string>
#include <vector>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
//#include "header.h"

// Generate RSA key pair and return the public key
RSA* generateRSAKeyPair() {
    RSA* rsaKey = RSA_new();
    BIGNUM* bn = BN_new();
    unsigned long exponent = RSA_F4; // Commonly used value for RSA public exponent

    if (BN_set_word(bn, exponent) != 1) {
        // Error occurred while setting the exponent
        RSA_free(rsaKey);
        BN_free(bn);
        return nullptr;
    }

    if (RSA_generate_key_ex(rsaKey, 2048, bn, nullptr) != 1) {
        // Error occurred while generating the key pair
        RSA_free(rsaKey);
        BN_free(bn);
        return nullptr;
    }

    BN_free(bn);
    return rsaKey;
}

// Encrypt the given plaintext using RSA public key
std::string rsaEncrypt(const std::string& plaintext, RSA* publicKey) {
    int rsaSize = RSA_size(publicKey);
    unsigned char* encryptedData = new unsigned char[rsaSize];

    int encryptedLength = RSA_public_encrypt(plaintext.length(), reinterpret_cast<const unsigned char*>(plaintext.c_str()), encryptedData, publicKey, RSA_PKCS1_PADDING);
    if (encryptedLength == -1) {
        delete[] encryptedData;
        return "";
    }
    std::string encryptedString(reinterpret_cast<char*>(encryptedData), encryptedLength);
    delete[] encryptedData;
    return encryptedString;
}

// Decrypt the given ciphertext using RSA private key
std::string rsaDecrypt(const std::string& ciphertext, RSA* privateKey) {
    int rsaSize = RSA_size(privateKey);
    unsigned char* decryptedData = new unsigned char[rsaSize];

    int decryptedLength = RSA_private_decrypt(ciphertext.length(), reinterpret_cast<const unsigned char*>(ciphertext.c_str()), decryptedData, privateKey, RSA_PKCS1_PADDING);
    if (decryptedLength == -1) {
        // Error occurred during decryption
        delete[] decryptedData;
        return "";
    }

    std::string decryptedString(reinterpret_cast<char*>(decryptedData), decryptedLength);
    delete[] decryptedData;
    return decryptedString;
}

// int main()
// {
//      // Generate RSA key pair
//     RSA* rsaKeyPair = generateRSAKeyPair();
//     if (rsaKeyPair == nullptr) {
//         // Error occurred while generating the key pair
//         return 1;
//     }

//     // Encrypt and decrypt a sample message
//     std::string plaintext = "010101111111110010010001010101110111110001111";
//     std::string ciphertext = rsaEncrypt(plaintext, rsaKeyPair);
//     //uxarkel inch vor mekin
//     std::string decryptedText = rsaDecrypt(ciphertext, rsaKeyPair);
//     std::cout << decryptedText;
//     if (decryptedText != plaintext) {
//         // Error occurred during encryption or decryption
//         return 1;
//     }
//     // Clean up
//     RSA_free(rsaKeyPair);
// }