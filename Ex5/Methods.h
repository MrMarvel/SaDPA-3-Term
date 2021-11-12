#pragma once

#include <iostream>
#include "SquareMatrix.h"

using namespace std;

namespace Methods {
    template<typename T>
    SquareMatrix<T> Floyd_Warshall(SquareMatrix<T> matrix) {
        auto size = matrix.size();
        for (int k = 0; k < size; k++) {	//����������� �� ���� �������� � ���� ����� �������� ���� ����� ������� k
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    int sum = matrix[i][k] + matrix[k][j];
                    if (sum == abs(matrix[i][k] - matrix[k][j])) sum = 0;
                    if (sum <= 0) sum = matrix[i][j];
                    if (matrix[i][j] <= 0) matrix[i][j] = sum;
                    matrix[i][j] = min(matrix[i][j], sum);	//����� �������� ����� ����� ������������ ����� ������ ������ � ������ ����� 
                }
            }
        }
        return matrix;
    }

    template<typename T>
    void print_matrix(Matrix<T> matrix) {	//matrix - ������� ���������
        auto size = matrix.size();
        for (int i = 0; i < size.first; i++) {
            for (int j = 0; j < size.second; j++) {
                if (matrix[i][j] == 0) {	//���� ������ ����� 101
                    std::cout << "INF" << " ";	//������� �������� ����������� �������������
                } else {	//�����
                    printf("%3d ", matrix[i][j]);	//������� �������� ������ �������
                }
            }
            std::cout << std::endl;	//������ ������� ������ ����� ���������� ����
        }
    }

}
