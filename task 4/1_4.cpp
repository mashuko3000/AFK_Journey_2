#include <iostream>
#include <stdexcept>
#include <cmath>
#include <iomanip>

class Matrix {
private:
    size_t rows;
    size_t cols;
    double** data;

    void allocate_memory() {
        data = new double*[rows];
        try {
            for (size_t i = 0; i < rows; ++i) {
                data[i] = new double[cols]();
            }
        } catch (std::bad_alloc &e) {
            for (size_t j = 0; j < rows; ++j) {
                delete[] data[j];
            }
            delete[] data;
            throw;
        }
    }

    void deallocate_memory() {
        for (size_t i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }

    void copy_data(const Matrix& other) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }

public:
    Matrix(size_t r, size_t c) : rows(r), cols(c) {
        if (rows == 0 || cols == 0) {
            throw std::invalid_argument("Matrix dimensions must be more than zero.");
        }
        allocate_memory();
    }

    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        allocate_memory();
        copy_data(other);
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            deallocate_memory();
            rows = other.rows;
            cols = other.cols;
            allocate_memory();
            copy_data(other);
        }
        return *this;
    }

    ~Matrix() {
        deallocate_memory();
    }

    double* operator[](size_t index) const { return data[index]; }

    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match for addition.");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions must match for multiplication.");
        }
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                result[i][j] = 0;
                for (size_t k = 0; k < cols; ++k) {
                    result[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    Matrix operator*(double scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = data[i][j] * scalar;
            }
        }
        return result;
    }

    friend Matrix operator*(double scalar, const Matrix& matrix) {
        return matrix * scalar;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("Matrix dimensions must match for subtraction.");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    Matrix transpose() const {
        Matrix result(cols, rows);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[j][i] = data[i][j];
            }
        }
        return result;
    }

    double determinant() const {
        if (rows != cols) {
            throw std::invalid_argument("Matrix must be square to compute determinant.");
        }
        Matrix temp(*this);
        double det = 1.0;

        for (size_t i = 0; i < rows; ++i) {
            size_t max_row = i;
            for (size_t k = i + 1; k < rows; ++k) {
                if (std::abs(temp[k][i]) > std::abs(temp[max_row][i])) {
                    max_row = k;
                }
            }

            if (temp[max_row][i] == 0) {
                return 0;
            }

            if (max_row != i) {
                for (size_t j = 0; j < cols; ++j) {
                    std::swap(temp[i][j], temp[max_row][j]);
                }
                det = -det;
            }

            det *= temp[i][i];
            for (size_t k = i + 1; k < rows; ++k) {
                double factor = temp[k][i] / temp[i][i];
                for (size_t j = i; j < cols; ++j) {
                    temp[k][j] -= factor * temp[i][j];
                }
            }
        }
        return det;
    }

    Matrix inverse() const {
        if (rows != cols) {
            throw std::invalid_argument("Matrix must be square to compute inverse.");
        }
        double det = determinant();
        if (det == 0) {
            throw std::invalid_argument("Matrix is singular and cannot be inverted.");
        }

        Matrix temp(rows, cols * 2);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                temp[i][j] = data[i][j];
            }
            temp[i][cols + i] = 1;
        }

        for (size_t i = 0; i < rows; ++i) {
            size_t max_row = i;
            for (size_t k = i + 1; k < rows; ++k) {
                if (std::abs(temp[k][i]) > std::abs(temp[max_row][i])) {
                    max_row = k;
                }
            }

            if (max_row != i) {
                for (size_t j = 0; j < cols * 2; ++j) {
                    std::swap(temp[i][j], temp[max_row][j]);
                }
            }

            if (temp[i][i] == 0) {
                throw std::invalid_argument("Matrix is singular and cannot be inverted.");
            }

            for (size_t k = i + 1; k < rows; ++k) {
                double factor = temp[k][i] / temp[i][i];
                for (size_t j = i; j < cols * 2; ++j) {
                    temp[k][j] -= factor * temp[i][j];
                }
            }
        }

        for (int i = rows - 1; i >= 0; --i) {
            for (int k = i - 1; k >= 0; --k) {
                double factor = temp[k][i] / temp[i][i];
                for (size_t j = 0; j < cols * 2; ++j) {
                    temp[k][j] -= factor * temp[i][j];
                }
            }
        }

        for (size_t i = 0; i < rows; ++i) {
            double factor = temp[i][i];
            if (factor == 0) {
                throw std::invalid_argument("Matrix is singular and cannot be inverted.");
            }
            for (size_t j = 0; j < cols * 2; ++j) {
                temp[i][j] /= factor;
            }
        }

        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result[i][j] = temp[i][cols + j];
            }
        }

        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (size_t i = 0; i < matrix.rows; ++i) {
            for (size_t j = 0; j < matrix.cols; ++j) {
                os << std::setw(5) << matrix[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }
};

int main() {
    Matrix A(2, 2);
    A[0][0] = 4; A[0][1] = 7;
    A[1][0] = 2; A[1][1] = 6;

    Matrix B(2, 2);
    B[0][0] = 1; B[0][1] = 3;
    B[1][0] = 5; B[1][1] = 2;

    try {
        std::cout << "Matrix A:\n" << A << std::endl;
        std::cout << "Matrix B:\n" << B << std::endl;

        Matrix C = A + B;
        std::cout << "A + B:\n" << C << std::endl;

        C = A - B;
        std::cout << "A - B:\n" << C << std::endl;

        C = A * B;
        std::cout << "A * B:\n" << C << std::endl;

        C = A * 2.0;
        std::cout << "A * 2:\n" << C << std::endl;

        C = 3.0 * B;
        std::cout << "3 * B:\n" << C << std::endl;

        Matrix D = A.transpose();
        std::cout << "Transpose of A:\n" << D << std::endl;

        std::cout << "Determinant of A: " << A.determinant() << std::endl;
        Matrix E = A.inverse();
        std::cout << "Inverse of A:\n" << E << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
