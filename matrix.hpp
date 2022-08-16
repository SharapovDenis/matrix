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

    explicit Matrix(std::vector<double> &);

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

    Matrix transpose();

    Matrix &colchange(Matrix &col, size_t index);

    Matrix getcols(size_t from, size_t upto);

    Matrix slice(size_t from, size_t upto, size_t index);

    double maxval();

    void read_csv(const char *filename, char delim);

    void append(const std::initializer_list<double> &);

    friend const Matrix operator+(Matrix A, const Matrix &B);

    friend const Matrix operator-(Matrix A, const Matrix &B);

    friend const Matrix operator*(Matrix A, const Matrix &B);

    friend std::ostream &operator<<(std::ostream &os, const Matrix &M);

    friend Matrix multiply(const Matrix &, const Matrix &);

    friend Matrix eye(size_t s);

    friend Matrix hconcat(Matrix &A, Matrix &B);

    friend Matrix vconcat(Matrix &A, Matrix &B);

    friend void test_mul(Matrix &, Matrix &);

};

double multiply(std::vector<double> &, std::vector<double> &);

struct ThrRow {
    size_t idx;
    std::vector<double> *row;
};
