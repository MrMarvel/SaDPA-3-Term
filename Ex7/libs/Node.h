#pragma once

struct Node {
    char32_t data;
    int freq;
    Node* left, * right;

    Node(char32_t data, int freq) {
        left = right = nullptr;
        this->data = data;
        this->freq = freq;
    }

    Node(char32_t data, Node* left, Node* right) {
        this->data = data;
        this->left = left;
        this->right = right;
    }

    bool isLeafNode() {
        return left == nullptr;
    };
};