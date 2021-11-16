#include    <stdio.h>
#include    <fstream>

using namespace std;

ofstream    fout;
ifstream    fin;

int     op;

void compress() {
    printf("Compressing...\n");
    fin.open("input.txt");
    fout.open("output.txt");
    char    ch;
    int     num(0);
    while (!fin.eof()) {
        ch = fin.get();
        num++;
        if (ch != fin.peek()) {
            num > 1 ? fout << (char)(num - 128) << ch : fout << ch;
            num = 0;
        } else if (num == 127) {
            fout << (char)(num - 128) << ch;
            num = 0;
        }
    }
    fin.close();
    fout.close();
    printf("Complete!\n");
}

void decompress() {
    printf("Decompressing...\n");
    fin.open("output.txt");
    fout.open("output2.txt");
    char    ch;
    int     num(0);
    while (!fin.eof()) {
        ch = fin.get();
        if ((int)ch < 0) { num = (int)ch + 128; fin >> ch; for (int i(0); i < num; i++) if (fin.peek() != '\0') fout << ch; } else if (fin.peek() != '\0') fout << ch;
    }
    fin.close();
    fout.close();
    printf("Complete!\n");
}

int main() {
    while (true) {
        printf("1 to compress\n2 to decompress\n");
        scanf("%d", &op);
        if (op == 1) compress();
        else decompress();
    }
    return 0;
}