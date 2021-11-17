﻿
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include "RLE.cpp"
#include "LZ77.cpp"
#include "LZ78.h"

using namespace std;



void ex1() {
    constexpr int num = 1;
    int op;
    ofstream fout;
    ifstream fin;
    stringstream ss;
    printf("1 to compress\n2 to decompress\n");
    cin >> op;
    if (op == 1) {
        fin.open(to_string(num) + "/input.txt");
        fout.open(to_string(num) + "/output.txt");
        rle::compress(fin, ss);
        fout << ss.str();
        fin.seekg(0, std::ios::end);
        size_t uncompressed_size = fin.tellg();
        size_t compressed_size = ss.str().length();
        printf("Compression factor: %f\n", uncompressed_size * 1. / compressed_size);
    } else {
        fin.open(to_string(num) + "/output.txt");
        fout.open(to_string(num) + "/output2.txt");

        rle::decompress(fin, ss);
        fout << ss.str();
    }
    fin.close();
    fout.close();
    ss.str("");
}

void ex2() {
    constexpr int num = 2;
    int op;
    ofstream fout;
    ifstream fin;
    stringstream ss;
    int met = 0;
    printf("1 to LZ77\n2 to LZ78\n");
    cin >> met;
    printf("1 to compress\n2 to decompress\n");
    cin >> op;
    if (op == 1) {
        if (met == 1) {
            fin.open(to_string(num) + "/input.txt");
            fout.open(to_string(num) + "/output.txt");
            lz77::compress(fin, ss);
            fout << ss.str();
        } else {
            lz78::_tmain(to_string(num) + "/input2.txt", to_string(num) + "/output.txt", 1);
        }
        fin.close();
        fin.open(to_string(num) + "/input.txt");
        fin.seekg(0, std::ios::end);
        size_t uncompressed_size = fin.tellg();
        fin.close();
        fin.open(to_string(num) + "/output.txt");
        fin.seekg(0, std::ios::end);
        size_t compressed_size = fin.tellg();
        printf("Compression factor: %f\n", uncompressed_size * 1. / compressed_size);
    } else {

        if (met == 1) {
            fin.open(to_string(num) + "/output.txt");
            fout.open(to_string(num) + "/output2.txt");
            lz77::decompress(fin, ss);
            fout << ss.str();
        } else {
            lz78::_tmain(to_string(num) + "/output.txt", to_string(num) + "/output2.txt", 2);
        }
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    while (true) {
        printf("\nЗадания:\n");
        printf("1. Задание 1\n");
        printf("2. Задание 2\n");
        printf("3. Задание 3\n");
        int choose = 0;
        cin >> choose;
        switch(choose) {
        	case 1: {
                ex1();
                break;
        	}
            case 2: {
                ex2();
                break;
            }
            case 3: {
                break;
            }
        	default:
                return 0;
        }
    }
}
