#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

static bool abs_compare(int a, int b);

class Matrix {

private:

    size_t rows;
    size_t cols;
    std::vector<std::vector<double>> *m;

public:

    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(const Matrix &M);
    explicit Matrix(const std::vector<std::vector<double>> &ptr);
    ~Matrix();
    Matrix &operator=(const Matrix &M);
    Matrix &operator=(Matrix &&M) noexcept;
    Matrix &operator+=(const Matrix &M);
    friend const Matrix operator+(Matrix A, const Matrix &B);
    Matrix &operator-=(const Matrix &M);
    friend const Matrix operator-(Matrix A, const Matrix &B);
    Matrix &operator*=(const Matrix &M);
    friend const Matrix operator*(Matrix A, const Matrix &B);
    std::vector<double> &operator[](int i);
    const std::vector<double> &operator[](int i) const;
    friend std::ostream &operator<<(std::ostream &os, const Matrix &M);
    virtual bool is_empty();
    void print();
    std::pair<size_t, size_t> shape();
    Matrix &colchange(Matrix &col, size_t index);
    friend Matrix eye(size_t s);
    friend Matrix hconcat(Matrix &A, Matrix &B);
    friend Matrix vconcat(Matrix &A, Matrix &B);
    Matrix getcols(size_t from, size_t upto);
    Matrix slice(size_t from, size_t upto, size_t index);
    double maxval();
    void read_csv(const char *filename, const char delim);

};
