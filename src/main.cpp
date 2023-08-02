#include "header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <set>
#include <queue>
#include <openssl/bn.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream input_file(filename, std::ios::binary);
    if (!input_file) {
        std::cerr << "Error: unable to open file " << filename << std::endl;
        return 1;
    }

    char byte;
    std::vector<int> nums;
    while (input_file.get(byte)) {
        nums.push_back(static_cast<int>(static_cast<unsigned char>(byte)));
    }
  
    std::unordered_map<int, int> byte_freq = make_map(nums);
    std::multiset<TreeNode*, decltype(&compareTreeNodePtrs)> frequences = freq_list(byte_freq);
    TreeNode* huffmanTree = create_tree(frequences);
    std::string binary_code = binaryCode(nums);

    std::map<int, std::string> huffmanCodes;
    std::string s;
    generateHuffmanCodes(huffmanTree, huffmanCodes, byte_freq, s);
    std::string huffman_code = huffmanCode(nums, huffmanCodes);
    std::cout << "Compressed.\n";
    std::cout << "Efficiency of compression:" << calcEfficiency(binary_code, huffman_code) << "%.";
    std::cout << '\n';

    
 // Generate RSA key pair
    RSA* rsaKeyPair = generateRSAKeyPair();
    if (rsaKeyPair == nullptr) {
        // Error occurred while generating the key pair
        return 1;
    }

    const int PART_SIZE = 200;
    std::string plaintext = huffman_code;
    std::string ciphertext = "";
    std::string decryptedText = "";
for (size_t i = 0; i < plaintext.length(); i += PART_SIZE) {
    std::string part = plaintext.substr(i, PART_SIZE);

    // Encrypt each part separately
    std::string encryptedPart = rsaEncrypt(part, rsaKeyPair);
    if (encryptedPart.empty()) {
        // Error occurred during encryption
        return 1;
    }

    // Append the encrypted part to the ciphertext
    ciphertext += encryptedPart;

    // Append the encrypted part to the encryptedText
    encryptedText += encryptedPart;

    // Decrypt each part separately
    std::string decryptedPart = rsaDecrypt(encryptedPart, rsaKeyPair);
    if (decryptedPart.empty()) {
        // Error occurred during decryption
        return 1;
    }

    // Append the decrypted part to the decryptedText
    decryptedText += decryptedPart;
}
    // Clean up
    RSA_free(rsaKeyPair);

    std::vector<int> decoded = decode(huffmanTree, huffman_code);
    std::string decodedBin = binaryCode(decoded);
    if(decodedBin == binary_code)
    {
        std::cout << "Decompressed perfectly!\n";
    }

    return 0;
}