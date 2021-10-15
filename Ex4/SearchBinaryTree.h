#pragma once
#include <memory>
#include <cassert>
#include <algorithm>

#include <vector>
#include <iostream>
#include <string>
#include <list>

using namespace std;
template<typename T>
class SearchBinaryTree {
public:
    class TreeNode {
    public:
        shared_ptr<T> data;
        TreeNode* left = NULL;
        TreeNode* right = NULL;
        TreeNode(shared_ptr<T> val) {
            left = NULL; // В С++11 лучше использовать nullptr
            right = NULL;
            data = val;
        }
    };
private:
    shared_ptr<TreeNode> m_root;
    int m_size;
    int m_height;
public:
    SearchBinaryTree(T* root);
    SearchBinaryTree(T root);
};
