#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <thread>

static bool abs_compare(int a, int b);

class Matrix {

private:

    size_t rows;
    size_t cols;
    std::vector<std::vector<double>> m;

public:

    Matrix();

    Matrix(size_t rows, size_t cols);

    Matrix(const Matrix &M);

    ~Matrix();

    Matrix &operator=(const Matrix &M);

    Matrix &operator=(Matrix &&M) noexcept;

    Matrix &operator+=(const Matrix &M);

    Matrix &operator-=(const Matrix &M);

    Matrix &operator*=(const Matrix &M);

    std::vector<double> &operator[](int i);

    const std::vector<double> &operator[](int i) const;

    virtual bool is_empty();

    std::pair<size_t, size_t> shape();

    Matrix &colchange(Matrix &col, size_t index);

    Matrix getcols(size_t from, size_t upto);

    Matrix slice(size_t from, size_t upto, size_t index);

    double maxval();

    void read_csv(const char *filename, const char delim);

    void append(const std::initializer_list<double> &);

    friend const Matrix operator+(Matrix A, const Matrix &B);

    friend const Matrix operator-(Matrix A, const Matrix &B);

    friend const Matrix operator*(Matrix A, const Matrix &B);

    friend std::ostream &operator<<(std::ostream &os, const Matrix &M);

    friend Matrix eye(size_t s);

    friend Matrix hconcat(Matrix &A, Matrix &B);

    friend Matrix vconcat(Matrix &A, Matrix &B);

    ///////////////////////////////////////////////////

    friend Matrix mul(Matrix &, Matrix &);

    friend void test_mul(Matrix &A, Matrix &B);
};
