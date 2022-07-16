#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>

static bool abs_compare(int a, int b) {
    return (std::abs(a) < std::abs(b));
}

class Matrix {

private:

    size_t rows;
    size_t cols;
    std::vector<std::vector<double>> *m;

public:

    Matrix() : rows(0), cols(0), m(nullptr) {}

    Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
        m = new std::vector<std::vector<double>>(rows, std::vector<double>(cols));
        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                m->at(i)[j] = 0;
            }
        }
    }

    Matrix(const Matrix &M) {
        rows = M.rows;
        cols = M.cols;

        m = new std::vector<std::vector<double>>(rows, std::vector<double>(cols));

        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                m->at(i)[j] = M[i][j];
            }
        }
    }

    explicit Matrix(const std::vector<std::vector<double>> &ptr) {
        rows = ptr.size();

        if (rows != 0) {
            cols = ptr[0].size();
        } else {
            rows = 0;
        }

        m = new std::vector<std::vector<double>>(rows, std::vector<double>(cols));

        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                m->at(i)[j] = ptr[i][j];
            }
        }
    }

    ~Matrix() {
        delete m;
    }

    Matrix &operator=(const Matrix &M) {

        if (this == &M) {
            return *this;
        }

        delete m;

        rows = M.rows;
        cols = M.cols;
        m = new std::vector<std::vector<double>>(rows, std::vector<double>(cols));

        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                m->at(i)[j] = M[i][j];
            }
        }

        return *this;
    }

    Matrix &operator=(Matrix &&M) noexcept {

        if (this == &M) {
            return *this;
        }

        delete m;

        m = std::exchange(M.m, nullptr);
        rows = std::exchange(M.rows, 0);
        cols = std::exchange(M.cols, 0);

        return *this;

    }

    Matrix &operator+=(const Matrix &M) {

        if (rows != M.rows) {
            std::cerr << "summarize incorrect shapes" << std::endl;
            std::exit(-1);
        }

        if (cols != M.cols) {
            std::cerr << "summarize incorrect shapes" << std::endl;
            std::exit(-1);
        }

        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                m->at(i)[j] += M[i][j];
            }
        }

        return *this;
    }

    friend const Matrix operator+(Matrix A, const Matrix &B) { // NOLINT(readability-const-return-type)
        A += B;
        return A;
    }

    Matrix &operator-=(const Matrix &M) {

        if (rows != M.rows) {
            std::cerr << "summarize incorrect shapes" << std::endl;
            std::exit(-1);
        }

        if (cols != M.cols) {
            std::cerr << "summarize incorrect shapes" << std::endl;
            std::exit(-1);
        }

        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                m->at(i)[j] -= M[i][j];
            }
        }

        return *this;
    }

    friend const Matrix operator-(Matrix A, const Matrix &B) { // NOLINT(readability-const-return-type)
        A -= B;
        return A;
    }

    Matrix &operator*=(const Matrix &M) {

        if (cols != M.rows) {
            std::cerr << "multiply incorrect shapes" << std::endl;
            std::exit(-1);
        }

        Matrix tmp(rows, M.cols);

        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < M.cols; ++j) {
                for (auto r = 0; r < M.rows; ++r) {
                    tmp[i][j] += m->at(i)[r] * M[r][j];
                }
            }
        }
        *this = tmp;

        return *this;
    }

    friend const Matrix operator*(Matrix A, const Matrix &B) { // NOLINT(readability-const-return-type)

        if (A.cols != B.rows) {
            std::cerr << "multiply incorrect shapes" << std::endl;
            std::exit(-1);
        }

        A *= B;

        return A;
    }

    std::vector<double> &operator[](int i) {
        return m->at(i);
    }

    const std::vector<double> &operator[](int i) const {
        return m->at(i);
    }

    friend std::ostream &operator<<(std::ostream &os, const Matrix &M) {

        if (M.rows == 0 && M.cols == 0) {
            std::cout << "empty matrix!" << std::endl;
            return os;
        }

        for (auto i = 0; i < M.rows; ++i) {
            for (auto j = 0; j < M.cols; ++j) {
                std::cout << M.m->at(i)[j] << " ";
            }
            std::cout << std::endl;
        }

        return os;
    }

    virtual bool is_empty() {
        if (rows == 0 && cols == 0)
            return true;
        else
            return false;
    }

    void print() {

        if (rows == 0 && cols == 0) {
            std::cout << "empty matrix!" << std::endl;
            return;
        }

        for (auto i = 0; i < rows; ++i) {
            for (auto j = 0; j < cols; ++j) {
                std::cout << m->at(i)[j] << " ";
            }
            std::cout << std::endl;
        }
    }

    std::pair<size_t, size_t> shape() {
        return std::make_pair(rows, cols);
    }

    Matrix &colchange(Matrix &col, size_t index) {
        // Can change col of any size except the size that is bigger than M.rows

        if (col.cols != 1) {
            std::cerr << "colchange(): incorrect col shape" << std::endl;
            std::exit(-1);
        }

        if (col.rows > this->rows) {
            std::cerr << "colchange(): col.rows is bigger than this->rows" << std::endl;
            std::exit(-1);
        }

        if (index > this->cols) {
            std::cerr << "colchange(): index is bigger than this->rows" << std::endl;
            std::exit(-1);
        }

        for (auto i = 0; i < col.rows; ++i) {
            m->at(i)[index] = col[i][0];
        }

        return *this;
    }

    friend Matrix eye(size_t s);

    friend Matrix hconcat(Matrix &A, Matrix &B);

    friend Matrix vconcat(Matrix &A, Matrix &B);

    Matrix getcols(size_t from, size_t upto) {

        if (upto > cols) {
            std::cerr << "getcols(): upto > this->rows" << std::endl;
            std::exit(-1);
        }

        if (upto < from) {
            std::cerr << "getcols(): upto < from" << std::endl;
            std::exit(-1);
        }

        if (from == upto) {
            size_t index = from;
            Matrix M(rows, 1);

            for (auto i = 0; i < rows; ++i) {
                M[i][0] = m->at(i)[index];
            }

            return M;
        }

        Matrix M(rows, upto - from);

        for (auto i = 0; i < M.rows; ++i) {
            for (auto j = from; j < upto; ++j) {
                M[i][j-from] = m->at(i)[j];
            }
        }

        return M;
    }

    Matrix slice(size_t from, size_t upto, size_t index) {

        if (index > cols) {
            std::cerr << "slice(): index > this->rows" << std::endl;
            std::exit(-1);
        }

        if (upto > rows) {
            std::cerr << "slice(): upto > rows" << std::endl;
            std::exit(-1);
        }

        if (upto < from) {
            std::cerr << "slice(): upto < from" << std::endl;
            std::exit(-1);
        }

        if (upto == from) {
            Matrix ret;
            return ret;
        }

        Matrix mid(upto - from, 1);

        for (size_t i = from; i < upto; ++i) {
            mid[i-from][0] = m->at(i)[index];
        }

        return mid;
    }

    double maxval() {

        if (this->is_empty()) {
            std::cerr << "maxval(): empty matrix!" << std::endl;
            std::exit(-1);
        }

        std::vector<double> max_values;
        std::vector<double>::iterator result;
        size_t index;

        for (auto i = 0; i < rows; ++i) {
            result = std::max_element(m->at(i).begin(), m->at(i).end(), abs_compare);
            index = std::distance(m->at(i).begin(), result);
            max_values.push_back(m->at(i)[index]);
        }

        result = std::max_element(max_values.begin(), max_values.end(), abs_compare);
        index = std::distance(max_values.begin(), result);

        return max_values[index];
    }

};

Matrix eye(size_t s) {

    Matrix M(s, s);

    for (auto i = 0; i < M.rows; ++i) {
        M[i][i] = 1;
    }

    return M;
}

Matrix hconcat(Matrix &A, Matrix &B) {

    if (B.is_empty()) {
        return A;
    }

    if (A.is_empty()) {
        return B;
    }

    if (A.rows != B.rows) {
        std::cerr << "hconcat(): A.rows != B.rows" << std::endl;
        std::exit(-1);
    }

    Matrix C(A.rows, A.cols + B.cols);

    for (auto i = 0; i < C.rows; ++i) {
        for (auto j = 0; j < C.cols; ++j) {
            if (j < A.cols)
                C[i][j] = A[i][j];
            else
                C[i][j] = B[i][j - A.cols];
        }
    }
    return C;
}

Matrix vconcat(Matrix &A, Matrix &B) {

    if (B.is_empty()) {
        return A;
    }

    if (A.is_empty()) {
        return B;
    }

    if (A.cols != B.cols) {
        std::cerr << "vconcat(): A.cols != B.cols" << std::endl;
        std::exit(-1);
    }

    Matrix C(0, 0);

    for (auto i = 0; i < A.rows; ++i) {
        C.m->push_back(A[i]);
    }

    for (auto i = 0; i < B.rows; ++i) {
        C.m->push_back(B[i]);
    }

    C.rows = A.rows + B.rows;
    C.cols = A.cols;

    return C;
}


int main() {

    std::vector<double> a = {1,2,3};
    std::vector<double> b = {4,5,6};
    std::vector<double> c = {7,8,9};

    std::vector<std::vector<double>> v;
    v.push_back(a);
    v.push_back(b);
    v.push_back(c);

    Matrix A(v);
    Matrix B = A;

    std::cout << A.maxval() << std::endl;

}

// TODO: написать оператор << для удобства записи матрицы
// TODO: исключения на отрицательные индексы матрицы и from-upto construction
// TODO: вынести friends за класс
