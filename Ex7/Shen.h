#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

#include "libs/Huffman.h"
#include "libs/ShannonFano.h"
#include "libs/Utils.h"
#include "libs/getopt.h"
#include "libs/Node.h"

using namespace std;

namespace shen {

	static class Shen {
	private:
		static void printBT(const std::string& prefix, const Node* node, bool isLeft) {
			if (node != nullptr) {
				std::cout << prefix;
				if (prefix.empty()) cout << "+---";
				else std::cout << (isLeft ? "+-0-" : "+-1-");

				// print the value of the node
				std::wcout << static_cast<wchar_t>(node->data) << std::endl;

				// enter the next tree level - left and right branch
				printBT(prefix + (isLeft ? "|   " : "    "), node->left, true);
				printBT(prefix + (isLeft ? "|   " : "    "), node->right, false);
			}
		}

		static void printBT(const Node* node) {
			printBT("", node, false);
		}


	public:


		static void HuffmanZip(string path) {
			map<char32_t, int> letters;
			map<char32_t, string> huffmanCodes;
			basic_ifstream<char32_t> ifs(path, ios::in);

			int length = 0;

			char32_t c;
			while (!ifs.eof()) {
				ifs.get(c);
				letters[c]++;
				length++;
			}
			cout << "Таблица символов:\n";
			printf("+------+-------+-----------+\n");
			printf("|%6s|%7s|%11s|\n", "символ", "частота", "вероятность");
			printf("+------+-------+-----------+\n");
			for (auto p : letters) {
				wcout << "|     " << static_cast<wchar_t>(p.first) << "|";
				printf("%7d|%11.9f|\n", p.second, p.second*1./length);
			}
			printf("+------+-------+-----------+\n");

			Huffman* h = new Huffman();

			ofstream out(getFileName(path).append(".coded"), ios::binary | ios::out);
			out.write((const char*)&length, sizeof(length));

			BitOutput* bitOutput = new BitOutput(out);

			h->startHuffman(letters, bitOutput, huffmanCodes);
			printBT(h->root);


			ifs.clear();
			ifs.seekg(0, ifs.beg);

			while (!ifs.eof()) {
				ifs.get(c);
				bitOutput->writeBits(huffmanCodes[c]);

			}

			bitOutput->forceWrite();


			ifs.close();
			out.close();

			delete h;
			delete bitOutput;

		}

		static void HuffmanUnzip(string path) {
			Huffman* h = new Huffman();

			// Unziped text file
			basic_ofstream<char32_t> out(getFileName(path).append("_decoded.txt"), ios::out);

			ifstream in(path, ios::binary | ios::in);

			int length;
			in.read((char*)&length, sizeof(length));

			BitInput* bitInput = new BitInput(in);

			h->readNode(bitInput);
			h->read(bitInput, out, length);


			out.close();
			in.close();

			delete h;
			delete bitInput;

		}

		static void ShannonFanoZip(string path) {
			map<char32_t, int> letters;
			basic_ifstream<char32_t> ifs(path, ios::in);
			int length = 0;

			char32_t c;
			while (!ifs.eof()) {
				ifs.get(c);
				letters[c]++;
				length++;

			}

			ShannonFano* f = new ShannonFano();
			vector<ShannonFano::Letter> lettersVector;

			for (auto it = letters.begin(); it != letters.end(); ++it) {
				lettersVector.push_back({ it->first, it->second });
			}

			sort(lettersVector.begin(), lettersVector.end(), [](const ShannonFano::Letter& p1, const ShannonFano::Letter& p2) {
				return (p1.freq > p2.freq);
				 });

			f->startShannonFano(lettersVector, 0, letters.size() - 1);

			ofstream out(getFileName(path).append(".coded"), ios::binary | ios::out);
			out.write((const char*)&length, sizeof(length));

			BitOutput* bitOutput = new BitOutput(out);

			ifs.clear();
			ifs.seekg(0, ifs.beg);
			f->buildTree(lettersVector);
			printBT(f->root);
			f->encodeNode(f->root, bitOutput);

			while (!ifs.eof()) {
				ifs.get(c);
				auto letterCode = find_if(lettersVector.begin(), lettersVector.end(),
										  [c](const ShannonFano::Letter& p) {
											  return p.letter == c;
										  });

				for (char u : letterCode->code) {
					bitOutput->writeBit(to_bool(u));

				}

			}

			bitOutput->forceWrite();


			ifs.close();
			out.close();

			delete f;
			delete bitOutput;

		}

		static void ShannonFanoUnzip(string path) {
			ShannonFano* f = new ShannonFano();

			basic_ofstream<char32_t> out(getFileName(path).append("_decoded.txt"), ios::out);

			ifstream in(path, ios::binary | ios::in);

			int length;
			in.read((char*)&length, sizeof(length));

			BitInput* bitInput = new BitInput(in);

			f->readNode(bitInput);
			f->read(bitInput, out, length);


			out.close();
			in.close();

			delete f;
			delete bitInput;

		}
	public:
		static int main() {
			setlocale(LC_ALL, "");

			int arg = 'h';

			switch (arg) {
				case 'h':
					HuffmanZip(optarg);
					break;
				case 'a':
					HuffmanUnzip(optarg);
					break;
				case 's':
					ShannonFanoZip(optarg);
					break;
				case 'u':
					ShannonFanoUnzip(optarg);
					break;
				case '?':
					cout << "This argument is invalid." << endl;
					cout << "Please, use:" << endl;
					cout << "-h [--path] \t Huffman Zip" << endl;
					cout << "-a [--path] \t Huffman Unzip" << endl;
					cout << "-s [--path] \t Shannon-Fano Zip" << endl;
					cout << "-u [--path] \t Shannon-Fano Unzip" << endl;
					break;
			};


			return 0;
		}
	};
}