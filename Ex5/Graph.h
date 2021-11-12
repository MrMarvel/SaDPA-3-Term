#pragma once
#include "Methods.h"
#include "SquareMatrix.h"
#include <type_traits>
#include <set>
#include <algorithm>
#include <vector>

template<typename T>
struct Node {
public:
    T name;
    bool operator==(const Node& n) const {
        return name == n.name;
    }
};

template<typename T, typename V>
struct Edge {
public:
    Node<T> a, b;
    int len;
public:
    bool operator<(const Edge& other) const{
        return len < other.len;
    }
    bool operator==(const Edge& e) const {
        return a == e.a &&
            b == e.b &&
            len == e.len;
    }
};


template<typename T, typename L>
class Graph {
public:
    vector<Node<T>> nodes;
    vector<Edge<T, L>> edges;
    SquareMatrix<L> pathMatrix;
    SquareMatrix<L> floydMatrix;
    bool isFloydOutdated = false;
public:
    Graph() {}
    Graph& addNode(const T& name) {
        Node<T> node = { name };
        if (find(nodes.begin(), nodes.end(), node) != nodes.end()) return *this;
        nodes.push_back(node);
        pathMatrix.resize(pathMatrix.size() + 1);
        isFloydOutdated = true;
        return *this;
    }
    Graph& addEdge(const T& n1, const T& n2, const L& l) {
        Edge<T, L> edge = { n1,n2,l };
        if (find(edges.begin(), edges.end(), edge) != edges.end()) return *this;
        edges.push_back(edge);
        int y = find(nodes.begin(), nodes.end(), Node<T>({ n1 })) - nodes.begin();
        int x = find(nodes.begin(), nodes.end(), Node<T>({ n2 })) - nodes.begin();
        auto &v = pathMatrix[y];
        v[x] = l;
        isFloydOutdated = true;
        return *this;
    }
    SquareMatrix<L>& getFloydMatrix() {
        if (!isFloydOutdated) return floydMatrix;
        floydMatrix = Methods::Floyd_Warshall(pathMatrix);
        isFloydOutdated = false;
        return floydMatrix;
    }
    SquareMatrix<L>& getPathMatrix() {
        return pathMatrix;
    }
    T& findCenter() {
        int c = 0;
        int minDistance = INT_MAX;
        for (auto i = 0; i < floydMatrix.size(); i++) {
            vector<L> &v = floydMatrix[i];
            auto it = max(v.begin(), v.end());
            if (it == v.end()) continue;
            int maxLocalDistance = *it;
            if (minDistance <= maxLocalDistance) continue;
            minDistance = maxLocalDistance;
            c = i;
        }
        return nodes[c].name;
    }
};

template<typename T, typename L>
class GraphOriented : Graph<T,L> {
public:
};