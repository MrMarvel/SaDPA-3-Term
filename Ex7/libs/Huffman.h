#pragma once

#include <map>
#include <string>
#include "BitIO.h"
#include "Node.h"

using namespace std;

class Huffman {

    struct compare {
        bool operator()(Node* l, Node* r) {
            return l->freq > r->freq;
        }
    };

public:
    Node* root;
    int countOfOperations = 0;

    void startHuffman(map<char32_t, int>& letters, BitOutput*& writer, map<char32_t, string>& huffmanCodes);
    void makeCodes(struct Node* root, string str, map<char32_t, string>& huffmanCodes);
    void encodeNode(Node* node, BitOutput*& writer);
    Node* readNode(BitInput*& reader);
    void read(BitInput*& reader, basic_ofstream<char32_t>& outputStream, int length);
};