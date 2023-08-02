//#include "header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <set>
#include <queue>

struct TreeNode {
    int freq;
    int byte;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, int y);
};

TreeNode::TreeNode(int x, int y): freq(x), byte(y), left(nullptr), right(nullptr) {}

bool compareTreeNodePtrs(TreeNode* lhs, TreeNode* rhs)
{
    return lhs->freq < rhs->freq;
}

TreeNode* create_tree(std::multiset<TreeNode*, decltype(&compareTreeNodePtrs)>& freq)
{
    int sum = 0;
    TreeNode* l = nullptr;
    TreeNode* r = nullptr;
    TreeNode* root = nullptr;
    while (freq.size() >= 2)
    {
        auto it = freq.begin();
        l = (*it);
        std::advance(it, 1);
        r = (*it);
        sum = l->freq + r->freq;
        root = new TreeNode(sum, -1);
        root->left = l;
        root->right = r;
        freq.erase(freq.begin(), std::next(freq.begin(), 2));
        freq.insert(root);
    }
    return root;
}

void generateHuffmanCodes(TreeNode*& root, std::map<int, std::string>& huffmanCodes, std::unordered_map<int, int>& byte_freq, std::string code = "") 
{
    if(byte_freq.size() == 0)
    {
        return;
    }
    if (root->left == nullptr && root->right == nullptr) {
        for(auto pos : byte_freq)
        {
            if(pos.second == root->freq && pos.second != -1)
            {
                huffmanCodes[pos.first] = code;
                root->byte = pos.first;
                byte_freq[pos.first] = -1;
                break;
            }
        }
        return;
    }
    generateHuffmanCodes(root->left, huffmanCodes, byte_freq, code + '0');
    generateHuffmanCodes(root->right, huffmanCodes, byte_freq, code + '1');
}

std::string binaryCode(std::vector<int>& bytes)
{
    std::string str = "";
    for(int byte : bytes) 
    {
        for (int i = 7; i >= 0; --i) 
        {
            str += (byte & (1 << i)) ? '1' : '0';
        }
    }
    return str;
}

std::string huffmanCode(std::vector<int>& bytes, std::map<int, std::string>& codes)
{
    std::string str = "";
    for(int i = 0; i < bytes.size(); ++i)
    {
        str += codes[bytes[i]];
    }
    return str;
}

std::unordered_map<int, int> make_map(const std::vector<int> &nums)
{
    std::unordered_map<int, int> byte_freq;
    for(int i = 0; i < nums.size(); ++i)
    {
        byte_freq[nums[i]] += 1;
    }
    return byte_freq;
}

std::multiset<TreeNode*, decltype(&compareTreeNodePtrs)> freq_list(std::unordered_map<int, int>& byte_freq)
{
    std::multiset<TreeNode*, decltype(&compareTreeNodePtrs)> freqs(&compareTreeNodePtrs);
    for(auto pos : byte_freq)
    {
        TreeNode* freq = new TreeNode(pos.second, -1);
        freqs.insert(freq);
    }
    return freqs;
}
 
int calcEfficiency(std::string& oldC, std::string& newC)
{
    int sizeO = oldC.size();
    int sizeN = newC.size();
    return 100 - ((sizeN * 100) / sizeO);  
}

std::vector<int> decode(TreeNode* huffmanTree , std::string& huffmanCode)
{
    std::vector<int> decoded;
    TreeNode* here = huffmanTree;
   
    for(int i = 0; i < huffmanCode.size(); ++i)
    {
        if(huffmanCode[i] == '0')
        {
            here = here->left;
        }
        else if(huffmanCode[i] == '1')
        {
            here = here->right;
        }
        
        if (here->left == NULL and here->right == NULL)
        {
            decoded.push_back(here->byte);
            here = huffmanTree;
        }
    }
    return decoded;
}


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
    std::cout << binary_code << '\n' << '\n';
    std::map<int, std::string> huffmanCodes;
    std::string s;
    generateHuffmanCodes(huffmanTree, huffmanCodes, byte_freq, s);
    //for(auto pos: huffmanCodes)
        //std::cout << pos.first << "  " << pos.second << '\n';
    std::string huffman_code = huffmanCode(nums, huffmanCodes);
    std::cout << huffman_code << '\n';
    //std::cout << "Compressed.\n";
    std::cout << "Efficiency of compression:" << calcEfficiency(binary_code, huffman_code) << "%.";
    std::cout << '\n';
    return 0;
}