#include "matrix.hpp"
#include <cmath>


Matrix mergeHT(Matrix &h0, Matrix &h1, short int sign) {

    int t0, s;
    const size_t m  = h0.shape().first;
    const size_t n0 = h0.shape().second;
    const size_t n = n0 * 2;
    const double r0 = (double) (n0 - 1) / (double) (n - 1);

    Matrix h(m, n);
    Matrix mid(m, 1);

    for (auto t = 0; t < n; ++t) {
        t0 = (int) round(t * r0);
        s  = (int) sign * (t - t0);

        Matrix h1_upper_slice = h1.slice(s, m, t0);
        Matrix h1_lower_slice = h1.slice(0, s, t0);
        Matrix C = vconcat(h1_upper_slice, h1_lower_slice);
        Matrix H = h0.getcols(t0, t0);

        mid = H + C;
        h.colchange(mid, t);
    }

    return h;
}

Matrix fht2(Matrix &image, short int sign) {
    const size_t m = image.shape().first;
    const size_t n = image.shape().second;
    const int n0 = (int) round((double) n / 2);
    Matrix h;

    if (n < 2) {
        h = image;
    }
    else {
        Matrix left_image  = image.getcols(0, n0);
        Matrix right_image = image.getcols(n0, n);
        Matrix first_arg  = fht2(left_image, sign);
        Matrix second_arg = fht2(right_image, sign);
        h = mergeHT(first_arg, second_arg, sign);
    }

    return h;
}

int main() {

    Matrix A;
    A.read_csv("table_3_1.csv", ',');
    fht2(A, 1);

//    std::cout << fht2(A, 1) << std::endl;

}

// TODO: добавить const для констант в mergeHT
// TODO: вся проблема скорости в постоянном копировании!!!
