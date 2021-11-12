#pragma once
#include "Matrix.h"


template<typename T>
class SquareMatrix : public Matrix<T> {
private:
    using Matrix<T>::resize;
public:
    SquareMatrix() {}
    SquareMatrix(int a) : Matrix<T>(a, a) {}
    size_t size() {
        return this->a;
    }
    virtual SquareMatrix& resize(size_t a) {
        resize(a, a);
        return *this;
    }
};