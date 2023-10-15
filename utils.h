#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>

constexpr size_t nmax {100};

size_t number_of_digits(double n) {
    std::ostringstream strs;

    strs << n;
    return strs.str().size();
}

void print_matrix(double **M, size_t n, size_t m) {
    size_t max_len_per_column[nmax];

    for (size_t j = 0; j < m; ++j) {
        size_t max_len {};

        for (size_t i = 0; i < n; ++i)
            if (const auto num_length {number_of_digits(M[i][j])}; num_length > max_len)
                max_len = num_length;

        max_len_per_column[j] = max_len;
    }

    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < m; ++j)
            std::cout << (j == 0 ? "\n| " : "") << std::setw(max_len_per_column[j]) << M[i][j] << (j == m - 1 ? " |" : " ");

    std::cout << '\n';
}