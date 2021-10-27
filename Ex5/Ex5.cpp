#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int p[100000];
int rk[100000];

void init_dsu() {
    for (int i = 0; i < 100000; i++) {
        p[i] = i;
        rk[i] = 1;
    }
}

int get_root(int v) {
    if (p[v] == v) {
        return v;
    } else {
        return p[v] = get_root(p[v]);   //На выходе из рекурсии переподвешиваем v
    }
}

bool merge(int a, int b) {
    int ra = get_root(a), rb = get_root(b);

    if (ra == rb) {
        return false;
    } else {
        if (rk[ra] < rk[rb]) {
            p[ra] = rb;
        } else if (rk[rb] < rk[ra]) {
            p[rb] = ra;
        } else {
            p[ra] = rb;
            rk[rb]++;
        }

        return true;
    }
}

//Тип для представления рёбер.
struct edge {
    char a, b;
    int len;

    bool operator<(const edge& other) {
        return len < other.len;
    }
};

int main() {
    setlocale(LC_ALL, "RUS");
    vector<edge> edges = {};
    //Ввод edges...
    new ifstream("");
    shared_ptr<ifstream> fin = shared_ptr<ifstream>(new ifstream("input2.txt"));
    if (!fin->is_open()) {
        printf("Файл input.txt не найден!\n");
        system("pause");
        return -1;
    }
    istream &in = *fin;

    while (!in.eof()) {
        string line;
        getline(in, line);
        istringstream iss = istringstream(line);
        char a, b;
        int x;
        iss >> a >> b >> x;
        edge e = { a, b, x };
        edges.push_back(e);
    }
    printf("Дано:\n");
    for (auto e : edges) {
        cout << e.a << "-" << e.len << ">" << e.b << endl;
    }

    sort(edges.begin(), edges.end());

    int mst_weight = 0;

    init_dsu();
    vector<edge> finEdges;
    for (edge e : edges) {
        if (merge(e.a, e.b)) {      //Если a и b находятся в разных компонентах,
            finEdges.push_back(e);
            mst_weight += e.len;    //Добавить ребро в минимальный остов.
        }
    }

    cout << "Минимальный путь: " << mst_weight << endl;
    printf("Остов:\n");
    for (auto e : finEdges) {
        cout << e.a << "-" << e.len << ">" << e.b << endl;
    }
    system("pause");
    return 0;
}