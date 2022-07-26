#include "matrix.hpp"

static bool abs_compare(int a, int b) {
    return (std::abs(a) < std::abs(b));
}

Matrix::Matrix() : rows(0), cols(0) {}

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    std::vector<double> tmp(cols, 0);
    m.assign(rows, tmp);
}

Matrix::Matrix(const Matrix &M) {
    rows = M.rows;
    cols = M.cols;
    m = M.m;
}

Matrix::~Matrix() = default;

Matrix &Matrix::operator=(const Matrix &M) {

    if (this == &M) {
        return *this;
    }
    rows = M.rows;
    cols = M.cols;
    m = M.m;

    return *this;
}

Matrix &Matrix::operator=(Matrix &&M) noexcept {

    if (this == &M) {
        return *this;
    }
    std::swap(rows, M.rows);
    std::swap(cols, M.cols);
    std::swap(m, M.m);

    return *this;

}

Matrix &Matrix::operator+=(const Matrix &M) {

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
            m[i][j] += M[i][j];
        }
    }

    return *this;
}

const Matrix operator+(Matrix A, const Matrix &B) { // NOLINT(readability-const-return-type)
    A += B;
    return A;
}

Matrix &Matrix::operator-=(const Matrix &M) {

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
            m[i][j] -= M[i][j];
        }
    }

    return *this;
}

const Matrix operator-(Matrix A, const Matrix &B) { // NOLINT(readability-const-return-type)
    A -= B;
    return A;
}

Matrix &Matrix::operator*=(const Matrix &M) {

    if (cols != M.rows) {
        std::cerr << "multiply incorrect shapes" << std::endl;
        std::exit(-1);
    }

    Matrix tmp(rows, M.cols);

    for (auto i = 0; i < rows; ++i) {
        for (auto j = 0; j < M.cols; ++j) {
            for (auto r = 0; r < M.rows; ++r) {
                tmp[i][j] += m[i][r] * M[r][j];
            }
        }
    }
    *this = tmp;

    return *this;
}

const Matrix operator*(Matrix A, const Matrix &B) { // NOLINT(readability-const-return-type)

    if (A.cols != B.rows) {
        std::cerr << "multiply incorrect shapes" << std::endl;
        std::exit(-1);
    }

    A *= B;

    return A;
}

std::vector<double> &Matrix::operator[](int i) {
    return m[i];
}

const std::vector<double> &Matrix::operator[](int i) const {
    return m[i];
}

std::ostream &operator<<(std::ostream &os, const Matrix &M) {

    if (M.rows == 0 && M.cols == 0) {
        std::cout << "empty matrix!" << std::endl;
        return os;
    }

    for (auto i = 0; i < M.rows; ++i) {
        for (auto j = 0; j < M.cols; ++j) {
            std::cout << M.m[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return os;
}

bool Matrix::is_empty() {
    if (rows == 0 && cols == 0)
        return true;
    else
        return false;
}

std::pair<size_t, size_t> Matrix::shape() {
    return std::make_pair(rows, cols);
}

Matrix &Matrix::colchange(Matrix &col, size_t index) {
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
        m[i][index] = col[i][0];
    }

    return *this;
}

Matrix Matrix::getcols(size_t from, size_t upto) {

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
            M[i][0] = m[i][index];
        }

        return M;
    }

    Matrix M(rows, upto - from);

    for (auto i = 0; i < M.rows; ++i) {
        for (auto j = from; j < upto; ++j) {
            M[i][j - from] = m[i][j];
        }
    }

    return M;
}

Matrix Matrix::slice(size_t from, size_t upto, size_t index) {

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
        mid[i - from][0] = m[i][index];
    }

    return mid;
}

double Matrix::maxval() {

    if (this->is_empty()) {
        std::cerr << "maxval(): empty matrix!" << std::endl;
        std::exit(-1);
    }

    std::vector<double> max_values;
    std::vector<double>::iterator result;
    size_t index;

    for (auto i = 0; i < rows; ++i) {
        result = std::max_element(m[i].begin(), m[i].end(), abs_compare);
        index = std::distance(m[i].begin(), result);
        max_values.push_back(m[i][index]);
    }

    result = std::max_element(max_values.begin(), max_values.end(), abs_compare);
    index = std::distance(max_values.begin(), result);

    return max_values[index];
}

void Matrix::read_csv(const char *filename, const char delim) {

    std::ifstream file;
    file.open(filename, std::ifstream::in);

    if (!file.is_open()) {
        std::cerr << "error while opening the file " << filename << std::endl;
        std::exit(-1);
    }

    m.clear();

    std::string line, word;
    std::vector<double> row;

    while (file >> line) {

        std::stringstream s(line);
        row.clear();

        while (std::getline(s, word, delim)) {

            if (!word.empty())
                row.push_back(std::stod(word));
        }
        m.push_back(row);
    }

    rows = m.size();
    cols = row.size();

    file.close();

}

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
        C.m.push_back(A[i]);
    }

    for (auto i = 0; i < B.rows; ++i) {
        C.m.push_back(B[i]);
    }

    C.rows = A.rows + B.rows;
    C.cols = A.cols;

    return C;
}


int main() {

    Matrix A;
    Matrix B = eye(4);
    Matrix C, D;
    D = C = B;

    B += B;

    std::cout << B << C << D << std::endl;

    A.read_csv("table_3_1.csv", ',');
    std::cout << A.shape().first << " " << A.shape().second << std::endl;

}

// TODO: написать оператор << для удобства записи матрицы
// TODO: исключения на отрицательные индексы матрицы и from-upto construction
// TODO: добавить исключение в read_csv на неравное количество столбцов в csv
// TODO: подумать над деструктором (очистка вектора), а то падает Хаф
// TODO: использовать reserve для push_back, чтобы не было лишнего копирования
