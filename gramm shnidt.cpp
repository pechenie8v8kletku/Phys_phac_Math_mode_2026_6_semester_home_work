#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <functional>
using Matrix = std::vector<std::vector<double>>;


//—Ћќжность алгоритма √аусса жардана O(n^3) так что тут или скрыто или €вно будет видно 3 вложенных цикла, часть здесь замаскирована под STL но все еще остаетс€ циклом
void swap_rows(Matrix& a, size_t i, size_t j) {
    std::swap(a[i], a[j]);
}
/*
void divide_row(std::vector<double>& row, double value) {
    std::transform(
        row.begin(),
        row.end(),
        row.begin(),
        [value](double x) {
            return x / value;
        }
    );
}
*/
void divide_row(
    std::vector<double>& row,
    double value
) {

    using namespace std::placeholders;

    auto divide_by =
        std::bind(
            std::divides<double>{},
            _1, 
            value
        );

    std::transform(
        row.begin(),
        row.end(),
        row.begin(),
        divide_by
    );
}
// 
void subtract_rows(
    std::vector<double>& target,
    const std::vector<double>& source,
    double factor
) {
    std::transform(
        target.begin(),
        target.end(),
        source.begin(),
        target.begin(),
        [factor](double a, double b) {
            return a - factor * b;
        }
    );
}

std::vector<double> gauss_jordan(Matrix matrix) {
    const size_t n = matrix.size();

    std::for_each(
        matrix.begin(),
        matrix.end(),
        [n](const auto& row) {
            if (row.size() != n + 1) {
                throw std::runtime_error("Error in matrix shape");
            }
        }
    );

    std::vector<size_t> cols(n);
    std::iota(cols.begin(), cols.end(), 0);

    std::for_each(
        cols.begin(),
        cols.end(),
        [&](size_t col) {

            auto pivot_it = std::find_if(
                matrix.begin() + col,
                matrix.end(),
                [col](const auto& row) {
                    return std::abs(row[col]) > 0;
                }
            );

            if (pivot_it == matrix.end()) {
                throw std::runtime_error("No unique solution  ");
            }

            size_t pivot_row =
                std::distance(matrix.begin(), pivot_it);

            swap_rows(matrix, col, pivot_row);

            divide_row(
                matrix[col],
                matrix[col][col]
            );

            std::for_each(
                cols.begin(),
                cols.end(),
                [&](size_t row) {

                    if (row == col) {
                        return;
                    }

                    double factor =
                        matrix[row][col];

                    subtract_rows(
                        matrix[row],
                        matrix[col],
                        factor
                    );
                }
            );
        }
    );

    std::vector<double> solution(n);

    std::transform(
        matrix.begin(),
        matrix.end(),
        solution.begin(),
        [n](const auto& row) {
            return row[n];
        }
    );

    return solution;
}



int main() {
    {Matrix system = {
        {1, 1, 3,5},
        {2, -1, 0,1},
        {1, 1, 8,2}

    };
    std::cout << "___________initial MATRIX___________" << "\n\n";
    std::for_each(
        system.begin(),
        system.end(),
        [](auto x) {
            std::for_each(
                x.begin(),
                x.end(),
                [](double z) {
                    std::cout << z << "  ";
                }
            );
            std::cout << "\n";
        }
    );
    std::cout << "\n";

    auto solution = gauss_jordan(system);
    std::cout << "_______SOLUTION_______" << "\n\n";

    std::for_each(
        solution.begin(),
        solution.end(),
        [](double x) {
            std::cout << x << "  ";
        }
    );
    }
    std::cout << "\n";
    {
        Matrix system = {
        {1, 0, 0,5},
        {0, 4, 0,1},
        {0, 0, 8,256}

        };
        std::cout << "___________initial MATRIX___________" << "\n\n";
        std::for_each(
            system.begin(),
            system.end(),
            [](auto x) {
                std::for_each(
                    x.begin(),
                    x.end(),
                    [](double z) {
                        std::cout << z << "  ";
                    }
                );
                std::cout << "\n";
            }
        );
        std::cout << "\n";

        auto solution = gauss_jordan(system);
        std::cout << "_______SOLUTION_______" << "\n\n";

        std::for_each(
            solution.begin(),
            solution.end(),
            [](double x) {
                std::cout << x << "  ";
            }
        );


    }
}