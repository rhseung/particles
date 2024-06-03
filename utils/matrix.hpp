#pragma once

#include <vector>
#include <utility>

template <typename T>
class Matrix {
 private:
    std::vector<std::vector<T>> data;

 public:
    int row, col;

    Matrix(int row, int col): row{row}, col{col} {
        data.resize(row);
        for (int i = 0; i < row; ++i)
            data[i].resize(col);
    }
    Matrix(std::vector<std::vector<T>> matrix) {
        row = matrix.size();
        if (row > 0)
            col = matrix[0].size();
        else
            col = 0;

        data = matrix;
    }

    T operator[](int i) {
        return data[i];
    }

    bool is_square() {
        return row == col;
    }
};

template <typename T>
T det(Matrix<T> mat) {
    if (mat.is_square()) {
        if (mat.row == 2) {
            return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
        }
        else if (mat.row == 3) {
            // TODO:
        }
    }
}

int det2(int a, int b, int c, int d) {
    return a*d - b*c;
}

float det3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
    return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}

std::vector<std::vector<float>> adj3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
    float c11 = (e * i - f * h);
    float c12 = -(d * i - f * g);
    float c13 = (d * h - e * g);
    float c21 = -(b * i - c * h);
    float c22 = (a * i - c * g);
    float c23 = -(a * h - b * g);
    float c31 = (b * f - c * e);
    float c32 = -(a * f - c * d);
    float c33 = (a * e - b * d);

    return {
        {c11, c12, c13},
        {c21, c22, c23},
        {c31, c32, c33}
    };
}

std::vector<std::vector<float>> inverse3(std::vector<std::vector<float>>& matrix) {
    auto adj = adj3(matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1], matrix[2][2]);
    auto det = det3(matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], matrix[2][0], matrix[2][1], matrix[2][2]);

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            adj[i][j] /= det;

    return adj;
}

std::vector<float> solve_3linear(std::vector<std::vector<float>>& matrix, float a, float b, float c) {
    auto inv = inverse3(matrix);
    return {
        a*inv[0][0] + b*inv[0][1] + c*inv[0][2],
        a*inv[1][0] + b*inv[1][1] + c*inv[1][2],
        a*inv[2][0] + b*inv[2][1] + c*inv[2][2]
    };
}
