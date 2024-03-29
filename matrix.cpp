#include "matrix.hpp"
#include "safe_structures.cpp"

static bool abs_compare(int a, int b) {
    return (std::abs(a) < std::abs(b));
}

Matrix::Matrix() : rows(0), cols(0) {}

Matrix::Matrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    std::vector<double> tmp(cols, 0);
    m.assign(rows, tmp);
}

Matrix::Matrix(std::vector<double> &row) {
    rows = 1;
    cols = row.size();
    m.assign(1, row);
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

    for (const auto &ptr : M.m) {
        for (const auto &p : ptr) {
            os << p << " ";
        }
        os << std::endl;
    }

    return os;
}

bool Matrix::is_empty() {
    return m.empty();
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

void Matrix::append(const std::initializer_list<double> &_list) {

    if (_list.size() == 0) {
        return;
    }

    if (m.empty()) {
        cols = _list.size();
    }

    if (cols != _list.size()) {
        std::cerr << "append(): bad shapes!" << std::endl;
        std::exit(-1);
    }

    m.emplace_back(std::vector<double>(_list));
    rows += 1;
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

Matrix Matrix::transpose() {

    Matrix M(cols, rows);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            M[j][i] = m[i][j];
        }
    }

    return M;
}

Matrix multiply(const Matrix &A, const Matrix &B) {

    if (A.cols != B.rows) {
        std::cerr << "multiply() incorrect shapes" << std::endl;
        std::exit(-1);
    }

    Matrix tmp(A.rows, B.cols);

    for (auto i = 0; i < A.rows; ++i) {
        for (auto j = 0; j < B.cols; ++j) {
            for (auto r = 0; r < B.rows; ++r) {
                tmp[i][j] += A[i][r] * B[r][j];
            }
        }
    }

    return tmp;
}

double multiply(const std::vector<double> &row, const std::vector<double> &col) {

    if (row.size() != col.size()) {
        std::cerr << "multiply() incorrect vector sizes" << std::endl;
        std::exit(-1);
    }

    double tmp = 0;

    for (size_t i = 0; i < row.size(); ++i) {
        tmp += row[i] * col[i];
    }

    return tmp;
}

void row_col_multiply(threadsafe_stack<ThrRow> *rows, Matrix *B, Matrix *C) {

    while (!rows->empty()) {
        std::shared_ptr<ThrRow> row = rows->pop();
        std::cout << row->idx << std::endl;
    }


}

void test_mul(Matrix &A, Matrix &B) {

    Matrix C(A.rows, B.cols);
    const int num_threads = 4;
    threadsafe_stack<ThrRow> stack_rows;
    ThrRow mid_row{};
    std::vector<std::thread> threads;

    for (auto ptr = A.m.rbegin(); ptr != A.m.rend(); ++ptr) {
        mid_row.idx = ptr - A.m.rbegin();
        mid_row.row = &(*ptr);
        stack_rows.push(mid_row);
    }

    for (size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back(std::thread(row_col_multiply, &stack_rows, &B, &C));
    }

    for (size_t i = 0; i < num_threads; ++i) {
        threads[i].join();
    }

}

int main() {

    Matrix A, B;
    A.append({1, 2, 3, 4});
    A.append({1, 2, 3, 4});
    A.append({1, 2, 3, 4});
    A.append({1, 2, 3, 4});
    A.append({1, 2, 3, 4});
    A.append({1, 2, 3, 4});
    B.append({5, 6, 7, 8});
    B.append({5, 6, 7, 8});
    B.append({5, 6, 7, 8});
    B.append({5, 6, 7, 8});
    B.append({5, 6, 7, 8});
    B.append({5, 6, 7, 8});

    test_mul(A, B);

//    A.read_csv("table_3_1.csv", ',');
//    std::cout << A.shape().first << " " << A.shape().second << std::endl;

}

// TODO: исключения на отрицательные индексы матрицы и from-upto construction
// TODO: добавить исключение в read_csv на неравное количество столбцов в csv
// TODO: подумать над деструктором (очистка вектора), а то падает Хаф
// TODO: использовать reserve для push_back, чтобы не было лишнего копирования
// TODO: попробовать использовать std::move для векторов и строк
