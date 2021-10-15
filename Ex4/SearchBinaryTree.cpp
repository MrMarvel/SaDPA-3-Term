#include "SearchBinaryTree.h"

template<typename T>
SearchBinaryTree<T>::SearchBinaryTree(T * root) {
    m_root = new TreeNode(shared_ptr<T>(root));
    m_size = 1;
    m_height = 1;
};
template<typename T>
SearchBinaryTree<T>::SearchBinaryTree(T root) {
    m_root = new TreeNode(shared_ptr<T>(new T(root)));
    m_size = 1;
    m_height = 1;
};