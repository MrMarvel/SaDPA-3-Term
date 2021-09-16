#include <memory>
#include <cassert>
#include <algorithm>

#include <vector>
#include <iostream>
#include <string>
#include <list>

using namespace std;



class SearchBinaryTree {
public:
    class TreeNode {
    public:
        int m_data;
        TreeNode* m_parent = NULL;
        TreeNode* m_left = NULL;
        TreeNode* m_right = NULL;
        TreeNode(int val) {
            m_parent = NULL;
            m_left = NULL; // В С++11 лучше использовать nullptr
            m_right = NULL;
            m_data = val;
        }
    };
private:
    TreeNode* m_root;
    int m_size;
    int m_height;

    void print_array_to_right(TreeNode*, int = 0, int = 0);
    void print_array_to_left(TreeNode*, int = 0, int = 0);
    void print_tree(TreeNode*, int = 0);
    void delete_tree(TreeNode*);
public:
    SearchBinaryTree(int);
    ~SearchBinaryTree();
    void printArrayLeftToRight();
    void printArrayRightToLeft();
    bool find(int);
    void insert(int);
    void erase(int);
    int size() { return m_size; };
    double getAverage() {
        list<TreeNode*> l;
        //Для начала поместим в очередь корень

        l.push_back(m_root);
        int sum = 0;
        int amount = 0;
        while (l.size() != 0) {
            TreeNode* tmp = l.front();
            l.pop_front();
            if (!tmp) continue;
            sum += tmp->m_data;
            amount++;
            //Если есть левый наследник, то помещаем его в очередь для дальнейшей обработки
            if (tmp->m_left) {
                l.push_back(tmp->m_left);
            }
            //Если есть правый наследник, то помещаем его в очередь для дальнейшей обработки
            if (tmp->m_right) {
                l.push_back(tmp->m_right);
            }
        }
        if (amount == 0) return 0;
        return sum*1. / amount;
    }
    bool removeTheLeftNode(TreeNode* tree = NULL) {
        if (!tree) tree = m_root;
        if (!tree) return false;
        bool o = false;
        if (tree->m_left) o = removeTheLeftNode(tree->m_left);
        if (o) return true;
        if (tree->m_right) o = removeTheLeftNode(tree->m_right);
        if (o) return true;
        erase(tree->m_data);
        return true;
    }
    int findHeight(int key) {
        TreeNode* curr = m_root;
        int height = 0;
        if (!curr) return -1;
        for (; curr && curr->m_data != key; height++) {
            if (curr->m_data > key)
                curr = curr->m_left;
            else
                curr = curr->m_right;
        }
        if (!curr) return -1;
        return height;
    }
};
SearchBinaryTree::SearchBinaryTree(int key) {
    m_root = new TreeNode(key);
    m_size = 1;
    m_height = 1;
}
SearchBinaryTree::~SearchBinaryTree() {
    delete_tree(m_root);
}

void SearchBinaryTree::delete_tree(TreeNode* curr) {
    if (curr) {
        delete_tree(curr->m_left);
        delete_tree(curr->m_right);
        delete curr;
        m_size--;
    }
}
void SearchBinaryTree::printArrayLeftToRight() {
    if (m_size < 1) {
        printf("Пусто!\n");
        return;
    }
    for (int i = 0; i <= m_height; i++) {
        print_array_to_right(m_root, i);
        cout << endl;
    }
}

void SearchBinaryTree::printArrayRightToLeft() {
    if (m_size < 1) {
        printf("Пусто!\n");
        return;
    }
    for (int i = 0; i <= m_height; i++) {
        print_array_to_left(m_root, i);
        cout << endl;
    }
}

void SearchBinaryTree::print_array_to_right(TreeNode* curr, int height, int step) {
    if (step >= height) {
        if (curr) printf("%2d  ", curr->m_data);
        else printf("%2s  ", "{}");
        return;
    }
    if (curr) {
        //cout << curr->m_data << "  ";
        print_array_to_left(curr->m_left, height, step + 1);
        print_array_to_left(curr->m_right, height, step + 1);
    }
}

void SearchBinaryTree::print_array_to_left(TreeNode* curr, int height, int step) {
    if (step >= height) {
        if (curr) printf("%2d  ", curr->m_data);
        else printf("%2s  ", "{}");
        return;
    }
    if (curr) {
        //cout << curr->m_data << "  ";
        print_array_to_left(curr->m_right, height, step + 1);
        print_array_to_left(curr->m_left, height, step + 1);
    }
}

void SearchBinaryTree::print_tree(TreeNode* curr, int height) {
    if (curr) // Проверка на ненулевой указатель
    {

        print_array_to_right(curr->m_left);
        cout << curr->m_data << " ";
        print_array_to_right(curr->m_right);
    }
    
}

bool SearchBinaryTree::find(int key) {
    TreeNode* curr = m_root;
    while (curr && curr->m_data != key) {
        if (curr->m_data > key)
            curr = curr->m_left;
        else
            curr = curr->m_right;
    }
    return curr != NULL;
}
void SearchBinaryTree::insert(int key) {
    TreeNode* curr = m_root;
    int height = 0;
    TreeNode* newNode = new TreeNode(key);
    if (!m_root) {
        m_root = newNode;
        m_size = 1;
        return;
    }
    while (curr && curr->m_data != key) {
        height++;
        if (curr->m_data > key && curr->m_left == NULL) {
            curr->m_left = newNode;
            newNode->m_parent = curr;
            ++m_size;
            if (m_height < height) m_height = height;
            return;
        }
        if (curr->m_data < key && curr->m_right == NULL) {
            curr->m_right = newNode;
            newNode->m_parent = curr;
            ++m_size;
            if (m_height < height) m_height = height;
            return;
        }
        if (curr->m_data > key)
            curr = curr->m_left;
        else
            curr = curr->m_right;
    }
}
void SearchBinaryTree::erase(int key) {
    TreeNode* curr = m_root;
    TreeNode* parent = NULL;
    while (curr && curr->m_data != key) {
        parent = curr;
        if (curr->m_data > key) {
            curr = curr->m_left;
        } else {
            curr = curr->m_right;
        }
    }
    if (!curr)
        return;
    if (curr->m_left == NULL) {
        // Вместо curr подвешивается его правое поддерево
        if (curr->m_right && curr->m_right->m_parent == curr)
            curr->m_right->m_parent = curr->m_parent;
        if (m_root == curr) m_root = curr->m_right;

        if (parent && parent->m_left == curr)
            parent->m_left = curr->m_right;
        if (parent && parent->m_right == curr)
            parent->m_right = curr->m_right;
        --m_size;
        delete curr;
        return;
    }
    if (curr->m_right == NULL) {
        // Вместо curr подвешивается его левое поддерево
        if (curr->m_left && curr->m_left->m_parent == curr)
            curr->m_left->m_parent = curr->m_parent;
        if (m_root == curr) m_root = curr->m_left;

        if (parent && parent->m_left == curr)
            parent->m_left = curr->m_left;
        if (parent && parent->m_right == curr)
            parent->m_right = curr->m_left;
        --m_size;
        delete curr;
        return;
    }
    // У элемента есть два потомка, тогда на место элемента поставим
    // наименьший элемент из его правого поддерева
    TreeNode* replace = curr->m_right;
    while (replace->m_left)
        replace = replace->m_left;
    int replace_value = replace->m_data;
    erase(replace_value);
    curr->m_data = replace_value;
}


int main() {
    setlocale(LC_ALL, "RUS");
    int a;
    cout << "Введите значение корня(0;99):\n";
    cin >> a;
    if (a < 0) a *= -1;
    SearchBinaryTree bt = SearchBinaryTree(a % 100);
    int choose = -1;
    int maxRepeats = 2;
    for (int repeat = maxRepeats; repeat > 0;) {
        printf("Меню:\n");
        printf("1. Добавить случайное число\n");
        printf("2. Добавить вводимое число\n");
        printf("3. Вывести дерево справа-налево\n");
        printf("4. Вывести среднее значение\n");
        printf("5. Вывести кол-во узлов в дереве\n");
        printf("6. Удалить самый левый лист дерева\n");
        printf("7. Найти уровень узла с вводимым значением\n");
        if (repeat < maxRepeats) printf("Вы хотите выйти? Напишите любой символ не из меню ещё %d раз, чтобы выйти!\n", repeat);
        cin >> choose;
        int x;
        switch (choose) {
        case 1:
            x = rand() % 100;
            printf("Добавляем случайное число %d\n", x);
            bt.insert(rand() % 100);
            break;
        case 2:
            printf("Введите добавляемое число:\n");
            cin >> x;
            bt.insert(x);
            break;
        case 3:
            printf("Дерево, изображённое справа-налево:\n");
            bt.printArrayRightToLeft();
            break;
        case 4:
            printf("Среднее значение: %f\n", bt.getAverage());
            break;
        case 5:
            printf("Кол-во узлов в дереве: %d\n", bt.size());
            break;
        case 6:
            printf("Удаление самого левого листа дерева\n");
            bt.removeTheLeftNode();
            break;
        case 7:
            printf("Введите значение узла с искомым уровнем:\n");
            cin >> x;
            printf("Узел со значением %d имеет высоту: %d\n", x, bt.findHeight(x));
            break;
        default:
            repeat--;
            continue;
        }
        repeat = maxRepeats;
    }
    system("pause");
    return 0;
}