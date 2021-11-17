#pragma once

#include <map>
#include <string>
#include <vector>
#include "BitIO.h"
#include "Node.h"

using namespace std;

class ShannonFano {
public:

public:
    struct Letter {
        char32_t letter;
        int freq;
        string code = "";

        Letter(char32_t letter, int freq) {
            this->letter = letter;
            this->freq = freq;
        }
    };

    Node* root;
    int countOfOperations = 0;

    void buildTree(vector<Letter>& letters);
    void startShannonFano(vector<Letter>& letters, int b, int e);
    int median(vector<Letter>& letters, int b, int e);
    void encodeNode(Node* node, BitOutput*& writer);
    Node* readNode(BitInput*& reader);
    void read(BitInput*& reader, basic_ofstream<char32_t>& outputStream, int length);

};