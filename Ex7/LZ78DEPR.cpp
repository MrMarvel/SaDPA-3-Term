#include "LZ78DEPR.h"
namespace lz78 {
    namespace tree {
        Node::Node(std::string prefix, int code) {
            this->prefix = prefix;
            this->code = code;
            this->parent = nullptr;
        }

        Node::~Node() {
            this->clear();
        }

        int Node::get_code() {
            return this->code;
        }

        std::string Node::get_prefix() {
            return this->prefix;
        }

        std::string Node::get_full_prefix() {
            std::stringstream stream;
            stream << this->prefix;
            for (Node* p = this->parent; p != nullptr; p = p->parent) {
                stream << p->prefix;
            }
            std::string inverted_prefix = stream.str();
            return std::string(inverted_prefix.rbegin(), inverted_prefix.rend());
        }

        std::vector<Node*> Node::get_children() {
            return this->children;
        }

        Node* Node::get_child(int i) {
            return this->children.at(i);
        }

        Node* Node::get_child(std::string prefix) {
            std::vector<Node*>::iterator it;
            for (it = this->children.begin(); it != this->children.end(); ++it) {
                if ((*it)->prefix.compare(prefix) == 0) {
                    return *it;
                }
            }
            return nullptr;
        }

        void Node::clear() {
            std::vector<Node*>::iterator it;
            for (it = this->children.begin(); it != this->children.end(); ++it) {
                Node* child = *it;
                delete child;
            }
        }
    }
    namespace tree {
        PrefixTree::PrefixTree() : size(1) {
            this->root = new Node("", 0);
        }

        PrefixTree::~PrefixTree() {
            this->clear();
        }

        Node* PrefixTree::get_root() const {
            return this->root;
        }

        int PrefixTree::get_size() const {
            return this->size;
        }

        Node* PrefixTree::add(Node* node, std::string prefix) {
            Node* new_node = new Node(prefix, this->size);
            node->children.push_back(new_node);
            new_node->parent = node;
            this->size++;
            return new_node;
        }

        Node* PrefixTree::find(int code) {
            std::queue<Node*> to_visit;
            std::vector<Node*>::iterator it;
            if (code == 0) {
                return this->root;
            }
            for (it = this->root->children.begin(); it != this->root->children.end(); ++it) {
                to_visit.push(*it);
            }
            while (!to_visit.empty()) {
                Node* p = to_visit.front();
                to_visit.pop();
                if (p->code == code) {
                    return p;
                } else {
                    for (it = p->children.begin(); it != p->children.end(); ++it) {
                        to_visit.push(*it);
                    }
                }
            }
            return nullptr;
        }

        void PrefixTree::clear() {
            delete this->root;
        }
    }


    namespace comp {

        void Compressor::compress(std::string input_path, std::string output_path) {
            std::ifstream input_file(input_path);
            std::ofstream output_file(output_path, std::ios::binary);
            tree::PrefixTree* prefix_tree = new tree::PrefixTree();

            if (!input_file.is_open()) {
                throw "Unexistent input file";
            }
            if (!output_file.is_open()) {
                throw "Can't open output file\n";
            }

            char c, eof = (char)4;
            int code;
            tree::Node* prefix_node = prefix_tree->get_root();

            input_file.unsetf(std::ios_base::skipws);
            while (input_file >> c) {
                tree::Node* prefix_child = prefix_node->get_child(std::string(1, c));
                code = prefix_node->get_code();
                if (prefix_child == nullptr) {
                    output_file.write((char*)(&code), sizeof(int));
                    output_file.write(&c, sizeof(char));
                    prefix_tree->add(prefix_node, std::string(1, c));
                    prefix_node = prefix_tree->get_root();
                } else {
                    prefix_node = prefix_child;
                }
            }
            code = prefix_node->get_code();
            if (prefix_node != prefix_tree->get_root()) {
                output_file.write((char*)(&code), sizeof(int));
                output_file.write((char*)(&eof), sizeof(char));
            }

            input_file.close();
            output_file.close();
            delete prefix_tree;
        }

        void Compressor::decompress(std::string input_path, std::string output_path) {
            std::ifstream input_file(input_path);
            std::ofstream output_file(output_path);
            tree::PrefixTree* prefix_tree = new tree::PrefixTree();
            std::map<int, tree::Node*> dictionary;

            if (!input_file.is_open()) {
                throw "Unexistent input file";
            }
            if (!output_file.is_open()) {
                throw "Can't open output file\n";
            }

            int* code = (int*)malloc(sizeof(int));
            char* c = (char*)malloc(sizeof(char));
            char eof = (char)4;
            tree::Node* new_node = nullptr;

            input_file.unsetf(std::ios_base::skipws);
            while (input_file.read((char*)code, sizeof(int)) && input_file.read(c, sizeof(char))) {
                if (*code == 0) {
                    output_file << *c;
                    new_node = prefix_tree->add(prefix_tree->get_root(), std::string(1, *c));
                    dictionary[new_node->get_code()] = new_node;
                } else {
                    tree::Node* prefix_node = dictionary[*code];
                    if (prefix_node != nullptr) {
                        if (*c != eof) {
                            output_file << prefix_node->get_full_prefix() << *c;
                        } else {
                            output_file << prefix_node->get_full_prefix();
                        }
                        new_node = prefix_tree->add(prefix_node, std::string(1, *c));
                        dictionary[new_node->get_code()] = new_node;
                    }
                }
            }
            free(code);
            free(c);
            delete prefix_tree;
        }


    }
}