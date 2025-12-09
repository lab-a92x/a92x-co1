#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

//This problem asks us to give the minimum valued path when traversing a matrix with n rows and columns.
//An efficient way of solving this is applying Bellman optimality principle.
//For each element, we are only interested in the largest valued path possible
// that starts in the origin and finishes in that element.
//Let (i,j) represent the element in the i-th row and j-th column.
//minimum_path(1,1) = value(1,1) since it is the origin. Now, lets suppose 1 < i or 1  < j.
//If j = 1, then minimum_path(i,1) = value(i,1) + minimum_path(i-1,1).
// For j > 1, lets suppose we know the minimum_path(i, j-1) for every i.
// For every element of the j-th column, every path that finishes in them must pass the j-1-th column
//at some moment since it started in the first column.
//We can start by computing the cost of arriving in the j-th column from (j-1)-th colum.
//Then we propagate the results upwards and downwards.
//This recursive function defines the dynamic programming algorithm,
//O(n^2) in time complexity and O(n) in additional space.
int main(){

    std::ifstream file("../matrix.txt");
    std::vector<std::vector<uint64_t> > matrix;
    std::string line;
    uint32_t row = 0UL;
    uint64_t sol = 0ULL;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<uint64_t> vec;

        while (std::getline(ss, token, ',')) {
            // erase spaces
            token.erase(0, token.find_first_not_of(" \t\r\n"));
            token.erase(token.find_last_not_of(" \t\r\n") + 1);
            vec.push_back(std::stoull(token));
        }
        matrix.push_back(vec);
        ++row;
    }

    file.close();

    auto start = std::chrono::steady_clock::now();

    std::vector<uint64_t> dp_prev_column(row, 0ULL);
    std::vector<uint64_t> dp_curr_column(row, 0ULL);

    for(size_t i = 0; i < row; i++)
        dp_prev_column[i] = matrix[i][0];


    for(size_t j = 1; j < row; j++){
        for(size_t i = 0; i < row; i++)
            dp_curr_column[i] = matrix[i][j] + dp_prev_column[i];

        for(size_t i = 1; i < row; i++)
            dp_curr_column[i] = std::min(matrix[i][j] + dp_curr_column[i - 1], dp_curr_column[i]);

        for(size_t i = row-1; i-- > 0;)
            dp_curr_column[i] = std::min(matrix[i][j] + dp_curr_column[i + 1], dp_curr_column[i]);

        std::swap(dp_prev_column, dp_curr_column);
    }

    sol = dp_prev_column[0];

    for(size_t i = 1; i < row; i++)
        sol = std::min(sol, dp_prev_column[i]);





    auto end = std::chrono::steady_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);


    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << ms.count() << " ms\n";
}