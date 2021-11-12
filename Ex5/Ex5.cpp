#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <map>

using namespace std;

int p[100000];
int rk[100000];

vector<vector<int>> Floyd_Warshall(vector<vector<int>> matrix) {
    auto size = matrix.size();
    for (int k = 0; k < size; k++) {	//Пробегаемся по всем вершинам и ищем более короткий путь через вершину k
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int sum = matrix[i][k] + matrix[k][j];
                matrix[i][j] = min(matrix[i][j], sum);	//Новое значение ребра равно минимальному между старым ребром и суммой ребер 
                if (matrix[i][j] <= 0) matrix[i][j] = sum;
            }
        }
    }
    return matrix;
}

void print_matrix(vector<vector<int>> matrix) {	//matrix - матрица смежности
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == 0) {	//если ячейка равна 101
                std::cout << "INF" << " ";	//вывести условное обозначение бесконечности
            }
            else {	//иначе
                printf("%3d ", matrix[i][j]);	//вывести значение ячейки матрицы
            }
        }
        std::cout << std::endl;	//делаем перенос строки после заполнения ряда
    }
}

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

vector<vector<int>> fillMatrixByEdges(vector<edge> edges) {
    vector<vector<int>> matrix;
    
    map<char, int> dots;
    for (auto e : edges) {
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

    return matrix;
}

int main() {
    setlocale(LC_ALL, "RUS");
    vector<edge> edges = {}; // Список рёбр
    //Ввод edges...
    new ifstream("");
    shared_ptr<ifstream> fin = shared_ptr<ifstream>(new ifstream("input2.txt"));
    if (!fin->is_open()) {
        printf("Файл input.txt не найден!\n");
        system("pause");
        return -1;
    }
    istream &in = *fin;

    vector<vector<int>> mat_sm; //Матрица смежности

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
    printf("\n");
    mat_sm = fillMatrixByEdges(edges);
    print_matrix(mat_sm);
    printf("\n");
    print_matrix(Floyd_Warshall(mat_sm));
    system("pause");
    return 0;
}