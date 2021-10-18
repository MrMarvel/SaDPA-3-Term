// Ex4p2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Ex4p3.h"

//Вариант АВЛ
int main() {
    setlocale(LC_ALL, "RUS");
    //Fill from file
    printf("Из какого файла восстановить(0 - db.txt, 1 - dbL.txt, 2 - dbXL.txt)?\n");
    int typeFile;
    cin >> typeFile;
    switch (typeFile) {
    case 1:
        fillFromFile(FILE_DB_L);
        break;
    case 2:
        fillFromFile(FILE_DB_XL);
        break;
    default:
        fillFromFile(FILE_DB);
        break;
    }
    printf("Восстановлено из файла ");
    cout << FILE_DB_L;
    printf("!\n\n");
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
            shared_ptr<string> res;
            string result;
            vector<Pair<int, string>> pasteList;
            switch (choose) {
            case 1:
                printf("Введите добавляемое <число, строка>:\n");
                cin >> x;
                cin >> xs;
                getline(cin, line);
                pasteList.clear();
                pasteList.push_back(Pair<int, string>(x, xs + line));
                printTimeInsert(pasteList);
                printf("\n");
                break;
            case 2:
                printf("Удаление узла с ключём:\n");
                cin >> x;
                printTimeRemove(x);
                break;
            case 3:
                printf("Дерево:\n");
                printTimePrint(typeFile >= 1 && typeFile <= 2);
                break;
            case 4:
                printf("Введите значение узла с искомым ключём:\n");
                cin >> x;
                res = printTimeFind(x);
                if (!res) {
                    printf("Узел с ключём %d НЕ НАЙДЕН!\n", x);
                    break;
                }
                result = *res;
                cout << "Узел с ключём " << x << ": " << result << "\n";
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
