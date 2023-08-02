#pragma once
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

//huffman

struct TreeNode {
    int freq;
    int byte;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, int y);
};

bool compareTreeNodePtrs(TreeNode* lhs, TreeNode* rhs);

TreeNode* create_tree(std::multiset<TreeNode*, decltype(&compareTreeNodePtrs)>& freq);

void generateHuffmanCodes(TreeNode*& root, std::map<int, std::string>& huffmanCodes, std::unordered_map<int, int>& byte_freq, std::string code);

std::string binaryCode(std::vector<int>& bytes);

std::string huffmanCode(std::vector<int>& bytes, std::map<int, std::string>& codes);

std::unordered_map<int, int> make_map(const std::vector<int> &nums);

std::multiset<TreeNode*, decltype(&compareTreeNodePtrs)> freq_list(std::unordered_map<int, int>& byte_freq);

int calcEfficiency(std::string& oldC, std::string& newC);

std::vector<int> decode(TreeNode* huffmanTree , std::string& huffmanCode);



//rsa

RSA* generateRSAKeyPair();

std::string rsaEncrypt(const std::string& plaintext, RSA* publicKey);

std::string rsaDecrypt(const std::string& ciphertext, RSA* privateKey);
