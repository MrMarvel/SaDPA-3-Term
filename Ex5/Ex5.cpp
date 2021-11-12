#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include "Methods.h"
#include "Graph.h"

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

Graph<char, int> fillMatrixByEdges(vector<edge> edges) {
    vector<vector<int>> matrix;
    Graph<char, int> graph;
    map<char, int> dots;
    for (auto e : edges) {
        graph.addNode(e.a);
        graph.addNode(e.b);
        graph.addEdge(e.a, e.b, e.len);
        bool nfa = dots.find(e.a) == dots.end();
        bool nfb = dots.find(e.b) == dots.end();
        if (nfa) {
            matrix.resize(matrix.size() + 1);
            for (auto i = 0; i < matrix.size(); i++) {
                matrix[i].resize(matrix.size());
            }
            dots[e.a] = matrix.size() - 1;
        }
        if (nfb) {
            matrix.resize(matrix.size() + 1);
            for (auto i = 0; i < matrix.size(); i++) {
                matrix[i].resize(matrix.size());
            }
            dots[e.b] = matrix.size() - 1;
        }
        auto x = dots[e.b];
        auto y = dots[e.a];
        auto mass = e.len;
        matrix[y][x] = mass;
    }

    for (auto& v : matrix) {
        //replace(v.begin(), v.end(), 0, INT_MAX/2);
    }

    return graph;
};

int main() {
    setlocale(LC_ALL, "RUS");
    vector<edge> edges = {}; // Список рёбр
    //Ввод edges...
    new ifstream("");
    shared_ptr<ifstream> fin = shared_ptr<ifstream>(new ifstream("input.txt"));
    if (!fin->is_open()) {
        printf("Файл input.txt не найден!\n");
        system("pause");
        return -1;
    }
    istream &in = *fin;


    for (bool isEnd = false; !in.eof() && !isEnd;) {
        string line;
        getline(in, line);
        if (line.back() == ';') isEnd = true;
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

    int choose = 0;
    while (choose >= 0 && cin.good()) {
        printf("\nМеню:\n");
        printf("0. Продолжить вычисления\n");
        printf("1. Добавить точку\n");
        in >> choose;
        switch (choose) {
        case 1:
        {
            printf("FROM   TO    WEIGHT\n");
            printf("CHAR CHAR NUMERICAL\n");
            char a, b;
            int l;
            in >> a >> b >> l;
            edge e = { a, b, l };
            edges.push_back(e);
            break;
        }
        default:
            choose = -1;
        }
    }

    printf("\nДано:\n");
    for (auto e : edges) {
        cout << e.a << "-" << e.len << ">" << e.b << endl;
    }

    Graph<char, int> graph; //Граф
    graph = fillMatrixByEdges(edges);
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
    printf("\n");
    printf("Матрица расстояний:\n");
    Methods::print_matrix(graph.getPathMatrix());
    auto mat_fl = graph.getFloydMatrix();
    printf("\nМатрица Флойда:\n");
    Methods::print_matrix(mat_fl);
    printf("\nЦентр в вершине: ");
    cout << graph.findCenter();
    printf("\n");
    system("pause");
    return 0;
}