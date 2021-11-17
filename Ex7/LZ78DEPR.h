#pragma once
#include<string>
#include<sstream>
#include<iostream>
#include<queue>
#include<sstream>
#include<fstream>
#include<string>
#include<iostream>
#include<cstdlib>
#include<map>
namespace lz78 {


    namespace tree {
        class Node {
        private:
            int code;
            std::string prefix;
            std::vector<Node*> children;
            Node* parent;
            void clear();

            friend class PrefixTree;
        public:
            Node(std::string prefix, int code);
            ~Node();
            int get_code();
            std::string get_prefix();
            std::string get_full_prefix();
            std::vector<Node*> get_children();
            Node* get_child(int i);
            Node* get_child(std::string prefix);
        };
        class PrefixTree {

        private:
            int size = 0;
            Node* root = nullptr;

        public:
            PrefixTree();
            ~PrefixTree();
            int get_size() const; // O(1)
            Node* get_root() const; // O(1)
            Node* find(int code);
            Node* add(Node* node, std::string object);
            void clear();
        };
    }

    namespace comp {

        class Compressor {

        private:


        public:
            static void compress(std::string input_path, std::string output_path);
            static void decompress(std::string input_path, std::string output_path);
        };
    }
}