// Ex4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "SearchBinaryTree.h"
#include <string>
#include <fstream>

using namespace std;
SearchBinaryTree<string, int>* sbt = new SearchBinaryTree<string, int>(string("s"), 5);
class File {
private:
    ifstream *ifs = nullptr;
    void insertAll() {
        ifstream& in = *ifs;
        string xs;
        int x;
        string line = "";
        for (int pos = 1; !in.eof() && in.good(); pos++) {
            in >> xs;
            if (xs == "") continue;
            if (!sbt) {
                sbt = new SearchBinaryTree<string, int>(xs, pos);
                continue;
            }
            sbt->insert(xs, pos);
        }
    }
public:
    File(string file) {
        ifs = new ifstream(file);
        insertAll();
    }
};

int main() {
    setlocale(LC_ALL, "RUS");
    File f("input.txt");
    printf("Восстановлено из файла input.txt:\n");
    sbt->displayBinTree();
    printf("Вставка {\"hi\" 4}:\n");
    sbt->insert("hi", 4);
    sbt->displayBinTree();
    printf("Поиск по ключу \"s\":\n");
    cout << sbt->searchValueByKey("s") << endl;
    printf("Удаление по ключу \"s\":\n");
    sbt->deleteNode("s");
    sbt->displayBinTree();
    //shared_ptr<SearchBinaryTree<int>> sbt = shared_ptr<SearchBinaryTree<int>>(new SearchBinaryTree<int>(1));
    system("pause");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
