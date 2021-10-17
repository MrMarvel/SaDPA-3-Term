#pragma once
#include <memory>
#include <cassert>
#include <algorithm>

#include <vector>
#include <iostream>
#include <string>
#include <list>

using namespace std;
template<typename K, typename T>
class BalancedSearchBinaryTree {
public:
    class TreeNode {
    public:
        shared_ptr<K> key = NULL;
        shared_ptr<T> data = NULL;
        size_t height = 0;
        TreeNode* left = NULL;
        TreeNode* right = NULL;
    public:
        TreeNode(shared_ptr<K> key, shared_ptr<T> val) {
            left = NULL; // � �++11 ����� ������������ nullptr
            right = NULL;
            this->key = key;
            data = val;
            if (!key) throw new exception();
            if (!val) throw new exception();
        }
        TreeNode(K key, T val) {
            left = NULL; // � �++11 ����� ������������ nullptr
            right = NULL;
            this->key = shared_ptr<K>(new K(key));
            data = shared_ptr<T>(new T(val));
            if (!this->key) throw new exception();
            if (!data) throw new exception();
        }
    };
private:
    TreeNode* root = nullptr;
    size_t size = 0;
    size_t height = 0;
public:
    BalancedSearchBinaryTree() {}
    BalancedSearchBinaryTree(K key, T root) {
        TreeNode* firstNode = new TreeNode(key, root);
        insert(firstNode);
    }
    void displayBinTree();
    inline size_t getHeight(K key) {
        TreeNode* node = searchElemByKey(key, root);
        return getHeight(node);
    }
    inline void deleteNode(K key) {
        deleteNode(root, key);
        rebalance();
    }
    void insert(K key, T data) {
        TreeNode* node = new TreeNode(key, data);
        if (!root) {
            root = node;
            return;
        }
        root = insert(root, node);
        rebalance();
    }
    inline T searchValueByKey(K key) {
        TreeNode* parent = root;
        if (!parent) return NULL;
        TreeNode* curr = parent;
        while (curr != nullptr && *curr->key != key) {
            // update parent node as current node
            parent = curr;

            // if given key is less than the current node, go to left subtree
            // else go to right subtree
            if (key < *curr->key)
                curr = curr->left;
            else
                curr = curr->right;
        }
        return *(curr->data);
    }

    

    void rebalance();
private:
    void printBinTree(TreeNode* node, long n = 0);
    void refreshHeight(TreeNode* node);
    TreeNode* rotateRight(TreeNode* node) {
        TreeNode* nodeLeft = node->left;
        node->left = nodeLeft->right;
        nodeLeft->right = node;
        refreshHeight(node);
        refreshHeight(nodeLeft);
        return nodeLeft;
    }
    TreeNode* rotateLeft(TreeNode* node) {
        TreeNode* nodeRight = node->right;
        node->right = nodeRight->left;
        nodeRight->left = node;
        refreshHeight(node);
        refreshHeight(nodeRight);
        return nodeRight;
    }
    TreeNode* balance(TreeNode* node = nullptr) {
        if (!node) node = root;
        refreshHeight(node);
        if (bfactor(node) > 0) {
            TreeNode* nodeRight = node->right;
            if (bfactor(nodeRight) < 0) node->right = rotateRight(nodeRight);
            return rotateLeft(node);
        }
        if (bfactor(node) < 0) {
            TreeNode* nodeLeft = node->left;
            if (bfactor(nodeLeft) > 0) node->left = rotateLeft(nodeLeft);
            return rotateRight(node);
        }
        return node; //������������ �� �����
    }
    int getHeight(TreeNode* node = root) {
        if (!node) return -1;
        return node->height;
    }

    int bfactor(TreeNode* node) {
        if (!node) return 0;
        return getHeight(node->right) - getHeight(node->left);
    }

    TreeNode* searchElemByKey(K key, TreeNode*& parent) {
        parent = NULL;
        TreeNode* curr = root;
        while (curr != nullptr && *curr->key != key) {
            // update parent node as current node
            parent = curr;

            // if given key is less than the current node, go to left subtree
            // else go to right subtree
            if (key < *curr->key)
                curr = curr->left;
            else
                curr = curr->right;
        }
        return curr;
    }
    TreeNode* insert(TreeNode* paste, TreeNode* pasteInto = nullptr) {
        /* If the tree is empty, return a new node */
        if (paste == NULL) return pasteInto;
        if (!pasteInto) {
            return paste;
        }
        /* Otherwise, recur down the tree */
        if (*paste->key < *pasteInto->key)
            pasteInto->left = insert(paste, pasteInto->left);
        else if (*paste->key > *pasteInto->key)
            pasteInto->right = insert(paste, pasteInto->right);

        /* return the (unchanged) node pointer */
        return pasteInto;
    }

    // A utility function to delete node from a BST
    void deleteNode(TreeNode* toDelete, K key) {
        // pointer to store parent node of current node
        TreeNode* parent = nullptr;
        // start with root node
        TreeNode* curr = toDelete;
        // search key in BST and set its parent pointer
        curr = searchElemByKey(key, parent);
        // return if key is not found in the tree
        if (!curr) return;

        // Case 1: node to be deleted has no children i.e. it is a leaf node
        if (!curr->left && !curr->right) {
            // if node to be deleted is not a root node, then set its
            // parent left/right child to null
            if (curr != root) {
                if (parent) {
                    if (parent->left == curr)
                        parent->left = nullptr;
                    else
                        parent->right = nullptr;
                }
            }
            // if tree has only root node, delete it and set root to null
            else
                root = nullptr;
            // deallocate the memory
            free(curr);	 // or delete curr;
        }

        // Case 2: node to be deleted has two children
        else if (curr->left && curr->right) {
            // find its in-order successor node
            TreeNode* successor = minimumKey(curr->right);
            // store successor value
            K sKey = *successor->key;
            T val = *successor->data;
            // recursively delete the successor. Note that the successor
            // will have at-most one child (right child)
            deleteNode(root, sKey);
            // Copy the value of successor to current node
            *curr->key = sKey;
            *curr->data = val;
        }
        // Case 3: node to be deleted has only one child
        else {
            TreeNode* child = (curr->left) ? curr->left : curr->right;
            // if node to be deleted is not a root node, then set its parent to its child
            if (curr != root) {
                if (curr == parent->left)
                    parent->left = child;
                else
                    parent->right = child;
            }
            // if node to be deleted is root node, then set the root to child
            else
                root = child;
            // deallocate the memory
            free(curr);
        }
    }
    TreeNode* minimumKey(TreeNode* node) {
        if (node == NULL)
            return NULL;
        else if (node->left == NULL)
            return node;
        else
            return minimumKey(node->left);
    }
};

template<typename K, typename T>
void BalancedSearchBinaryTree<K, T>::displayBinTree() {
    printBinTree(root);
}

template<typename K, typename T>
void BalancedSearchBinaryTree<K, T>::printBinTree(TreeNode* node, long n) {
    if (!node) return;
    printBinTree(node->right, n + 5);
    for (long i = 0; i < n; i++)
        printf(" ");
    K k = *node->key;
    cout << k << " = " << *node->data << "\n";
    printBinTree(node->left, n + 5);
}

template<typename K, typename T>
void BalancedSearchBinaryTree<K, T>::refreshHeight(TreeNode* node) {
    if (!node) return;
    int hl = getHeight(node->left);
    int hr = getHeight(node->right);
    node->height = (hl > hr ? hl : hr)+1;
}

template<typename K, typename T>
void BalancedSearchBinaryTree<K, T>::rebalance() {
    root = balance();
}
