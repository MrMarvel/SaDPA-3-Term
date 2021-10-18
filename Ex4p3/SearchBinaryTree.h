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
class SearchBinaryTree {
public:
    class TreeNode {
    public:
        shared_ptr<K> key = NULL;
        shared_ptr<T> data = NULL;
        TreeNode* left = NULL;
        TreeNode* right = NULL;
        TreeNode(shared_ptr<K> key, shared_ptr<T> val) {
            if (!key) throw new exception();
            if (!val) throw new exception();
            left = NULL; // В С++11 лучше использовать nullptr
            right = NULL;
            this->key = key;
            data = val;
        }
        TreeNode(K key, T val) {
            left = NULL; // В С++11 лучше использовать nullptr
            right = NULL;
            this->key = shared_ptr<K>(new K(key));
            data = shared_ptr<T>(new T(val));
            if (!this->key) throw new exception();
            if (!data) throw new exception();
        }
    };
private:
    TreeNode* root = nullptr;
    int size = 0;
    int height = 0;
public:
    /*SearchBinaryTree(shared_ptr<K> key, shared_ptr<T> root) {
        this->root = new TreeNode(key, root);
        size = 1;
        height = 1;
    }*/
    SearchBinaryTree() {};
    SearchBinaryTree(K key, T root) {
        this->root = new TreeNode(key, root);
        size = 1;
        height = 1;
    }
    void displayBinTree(bool = true);
    void deleteNode(K key) {
        deleteNode(root, key);
    }
    void insert(K key, T data) {
        TreeNode* node = new TreeNode(key, data);
        if (!root) {
            root = node;
            return;
        }
        insert(root, node);
    }
    shared_ptr<T> searchValueByKey(K key) {
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
        if (!curr) return NULL;
        return shared_ptr<string>(new string(*curr->data));
    }

    

private:
    void printBinTree(TreeNode* node, bool = true, long n = 0);

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
    TreeNode* insert(TreeNode* pasteInto, TreeNode* paste) {
        //
        if (!paste) return nullptr;
        if (!pasteInto) return paste;

        shared_ptr<K> pasteKey, pasteIntoKey;
        TreeNode* pasteIntoLeft;
        TreeNode* pasteIntoRight = NULL;
        while (pasteInto) {
            pasteKey = paste->key;
            pasteIntoKey = pasteInto->key;
            if (*pasteKey < *pasteIntoKey) {
                pasteIntoLeft = pasteInto->left;
                if (pasteIntoLeft) {
                    pasteInto = pasteIntoLeft;
                    continue;
                }
                pasteInto->left = paste;
                break;
            }
            if (*pasteKey > *pasteIntoKey) {
                pasteIntoRight = pasteInto->right;
                if (pasteIntoRight) {
                    pasteInto = pasteIntoRight;
                    continue;
                }
                pasteInto->right = paste;
                break;
            }
            break;
        }
        return pasteInto;
        //
        /* If the tree is empty, return a new node */
        
        if (paste == NULL) return nullptr;
        if (!pasteInto) {
            return paste;
        }
        /* Otherwise, recur down the tree */
        if (*paste->key < *pasteInto->key)
            pasteInto->left = insert(pasteInto->left, paste);
        else if (*paste->key > *pasteInto->key)
            pasteInto->right = insert(pasteInto->right, paste);

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
        if (curr == nullptr)
            return;

        // Case 1: node to be deleted has no children i.e. it is a leaf node
        if (curr->left == nullptr && curr->right == nullptr) {
            // if node to be deleted is not a root node, then set its
            // parent left/right child to null
            if (curr != root) {
                if (parent->left == curr)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
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
void SearchBinaryTree<K, T>::displayBinTree(bool show) {
    printBinTree(root, show);
    if (show) cout << endl;
}

template<typename K, typename T>
void SearchBinaryTree<K, T>::printBinTree(TreeNode* node, bool show, long n) {
    if (!node) return;
    if(show)printBinTree(node->right, show, n + 5);
    for (long i = 0; i < n; i++)
        if (show) printf(" ");
    K k = *node->key;
    if (show) cout << k << " = " << *node->data << "\n";
    if(show)printBinTree(node->left, show, n + 5);
}