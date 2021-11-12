#pragma once
#include "Pair.h"
#include <vector>

using namespace std;

template<typename T>
class Matrix {
protected:
    vector<vector<T>> matrix;
    size_t a = 0, b = 0;
public:
    Matrix() {}
    Matrix(int a, int b) : a(a), b(b) {}
    //Возвращает строку
    vector<T>& operator[](size_t const& index) {
        return matrix[index];
    }
    const vector<T>& operator[](size_t const& index) const {
        return matrix[index];
    }
    //Возвращает элемент
    T& get(size_t const& i, size_t const& i2) {
        return matrix[i][i2];
    }

    Pair<size_t, size_t> size() {
        return { a,b };
    }

    size_t getA() {
        return a;
    }

    size_t getB() {
        return b;
    }
    virtual Matrix& resize(size_t na, size_t nb) {
        a = na;
        b = nb;
        matrix.resize(a);
        for (auto& v : matrix) {
            v.resize(b);
        }
        return *this;
    }
};