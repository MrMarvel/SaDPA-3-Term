// Ex1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>

using namespace std;

int set1InPos(int a, int p) {
    int mask = 0x1 << p;
    return a | mask;
}
int op1(int a) {
    int mask = 0x78;
    return a | mask;
}

void ex1() {
    printf("\nEx1\n");
    int a = 0x1;
    int b = op1(a);
    printf("%d -> Операция №1 -> %d\n", a, b);
}

int op2(int a) {
    int mask = 4096 + 2048 + 2;
    mask = ~mask;
    return a & mask;
}

void ex2() {
    int exNum = 2;
    printf("\nEx%d\n", exNum);
    int a;
    printf("Введите изменяемое число:\n");
    cin >> a;
    int b = op2(a);
    printf("0x%x -> Операция №%d -> 0x%x\n", a, exNum, b);
}

int op3(int a) {
    int multiplicator = 32;
    int pow = log2(multiplicator);
    return a << pow;
}

void ex3() {
    int exNum = 3;
    printf("\nEx%d\n", exNum);
    int a;
    printf("Введите изменяемое число:\n");
    cin >> a;
    int b = op3(a);
    printf("%d -> Операция №%d -> %d\n", a, exNum, b);
}

int op4(int a) {
    int devisor = 32;
    int pow = log2(devisor);
    return a >> pow;
}

void ex4() {
    int exNum = 4;
    printf("\nEx%d\n", exNum);
    int a;
    printf("Введите изменяемое число:\n");
    cin >> a;
    int b = op4(a);
    printf("%d -> Операция №%d -> %d\n", a, exNum, b);
}

int op5(int a, int p) {
    int mask = 0x1 << sizeof(mask) * 8 - 1; //старшая единица
    int mask1 = mask >> 1;
    mask1 &= ~mask;
    mask1 >>= sizeof(mask1) * 8 - 2 - p;
    return a | mask1;
}

void ex5() {
    int exNum = 5;
    printf("\nEx%d\n", exNum);
    int a;
    printf("Введите изменяемое число:\n");
    cin >> a;
    int p;
    printf("Введите позицию для установки 1:\n");
    cin >> p;
    int b = op5(a, p);
    printf("%d -> Операция №%d -> %d\n", a, exNum, b);
}

void test() {
    ex1();
    ex2();
    ex3();
    ex4();
    ex5();
}
int main() {
    setlocale(LC_ALL, "RUS");
    test();

    system("pause");
}