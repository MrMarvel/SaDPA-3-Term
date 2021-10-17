// Ex4p2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "BalancedSearchBinaryTree.h"
#include <string>
#include <fstream>
#include <exception>
#include <typeinfo>
#include <stdexcept>

using namespace std;

string FILE_DB = "db.txt";

void fillFromFile(BalancedSearchBinaryTree<size_t, string>& bt) {
    ifstream ifs(FILE_DB);
    string line;
    int pos;
    while (!ifs.eof()) {
        pos = ifs.tellg();
        getline(ifs, line);
        bt.insert(pos, line);
    }
}

//Вариант АВЛ
int main() {
    setlocale(LC_ALL, "RUS");
    BalancedSearchBinaryTree<size_t, string> bt = BalancedSearchBinaryTree<size_t, string>();
    //Fill from file
    fillFromFile(bt);
    printf("Дерево восстановлено из файла!\n\n");
    //
    int choose = -1;
    int maxRepeats = 2;
    for (int repeat = maxRepeats; repeat > 0; printf("\n")) {
        try {
            printf("Меню:\n");
            printf("1. Добавить вводимую пару\n");
            printf("2. Удалить узел с вводимым ключём\n");
            printf("3. Вывести дерево\n");
            printf("4. Найти уровень узла с вводимым ключём\n");
            if (repeat < maxRepeats) printf("Вы хотите выйти? Напишите любой символ не из меню ещё %d раз, чтобы выйти!\n", repeat);
            if (cin.bad() || !cin.good()) {
                cin.clear();
                cin.ignore(32767, '\n');
            }
            cin >> choose;
            size_t x;
            string xs;
            string line;
            switch (choose) {
            case 1:
                printf("Введите добавляемое <число, строка>:\n");
                cin >> x;
                cin >> xs;
                getline(cin, line);
                bt.insert(x, xs+line);
                printf("\n");
                break;
            case 2:
                printf("Удаление узла с ключём:\n");
                cin >> x;
                bt.deleteNode(x);
                break;
            case 3:
                printf("Дерево:\n");
                bt.displayBinTree();
                break;
            case 4:
                printf("Введите значение узла с искомым ключём:\n");
                cin >> x;

                cout << "Узел с ключём " << x << ": " << bt.searchValueByKey(x) << "\n";
                break;
            default:
                repeat--;
                continue;
            }
            repeat = maxRepeats;
        } catch (exception e) {
            printf("Какая-то ошибка :/ \n");
            cout << e.what();
        } catch (...) {
            printf("Какая-то ошибка :/ \n");
        }
    }
    system("pause");
    return 0;
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
