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
//If i = 1, then minimum_path(1,j) = value(1,j) + minimum_path(1,j-1).
//If j = 1, then minimum_path(i,1) = value(i,1) + minimum_path(i-1,1).
//Otherwise,
// minimum_path(i,j) = value(i,j)+ min{minimum_path(i,j-1), minimum_path(i-1,j)}.
//This recursive function defines the dynamic programming algorithm,
//O(n^2) in time complexity and O(1) in additional space.
int main(){

    std::ifstream file("../matrix.txt");
    std::vector<std::vector<uint64_t> > matrix;
    std::string line;
    uint64_t sol = 0ULL;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<uint64_t> vec;

        while (std::getline(ss, token, ',')) {
            // Erase spaces
            token.erase(0, token.find_first_not_of(" \t\r\n"));
            token.erase(token.find_last_not_of(" \t\r\n") + 1);
            vec.push_back(std::stoull(token));
        }
        matrix.push_back(vec);
    }

    file.close();

    auto start = std::chrono::steady_clock::now();


    for(size_t i = 1; i < matrix.size(); i++)
        matrix[0][i] += matrix[0][i-1];

    for(size_t i = 1; i < matrix.size(); i++){
        matrix[i][0] += matrix[i-1][0];
        for(size_t j = 1; j <  matrix.size(); j++){
            matrix[i][j] += std::min(matrix[i-1][j], matrix[i][j-1]);
        }
    }

    sol = matrix[matrix.size()-1][matrix.size()-1];

    auto end = std::chrono::steady_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);


    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << ms.count() << " ms\n";
}