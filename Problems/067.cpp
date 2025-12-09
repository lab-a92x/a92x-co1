#include <iostream>
#include <fstream>
#include <vector>

//This problem asks us to give the largest valued path when traversing a triangle with n rows.
//There are n*(n+1)/2 elements in the triangle so an efficient algorith is in Ω(n^2),
//because if we skip one element, then that element could be arbitrarily large and
// consequently be part of the solution.
//An efficient way of solving this is applying Bellman optimality principle.
//For each element, we are only interested in the largest valued path possible
// that starts in the origin and finishes in that element.
//Let (i,j) represent the j-th element of the i-th row.
//largest_path(1,1) = value(1,1) since it is the origin. Now, lets suppose 1 < i.
//If j = 1, then largest_path(i,1) = value(i,1)+largest_path(i-1,1).
//If j = i, then largest_path(i,i) = value(i,i)+largest_path(i-1,i-1).
//Every other case satisfies
// largest_path(i,j) = value(i,j)+ max{largest_path(i-1,j-1), largest_path(i-1,j)}.
//This recursive function defines the dynamic programming algorithm,
//O(n^2) in time complexity and O(1) in additional space.
int main(){
    std::ifstream data("../data.txt");

    uint32_t num;
    uint32_t ite_bound = 1UL;
    std::vector<uint64_t> triangle;

    while(data >> num){
        triangle.resize(triangle.size()+ite_bound);
        triangle[triangle.size() -ite_bound] = num;
        for(uint32_t i = ite_bound-1UL; 1UL <= i; i = i - 1UL){
            data >> triangle[triangle.size() - i ];
        }
        ite_bound = ite_bound+1UL;

    }

    ite_bound--;

    auto start = std::chrono::steady_clock::now();

    uint32_t base = 1UL;
    uint32_t prev_base = 0UL;
    for(uint32_t row = 2UL; row < ite_bound; row++){
        triangle[base] += triangle[prev_base];

        for(uint32_t j = 1UL; j < row - 1UL; j++){
            triangle[base + j] += std::max(triangle[prev_base + j-1UL], triangle[prev_base + j]);
        }

        triangle[base + row -1UL] += triangle[prev_base + row - 2UL];

        prev_base = base;
        base += row;
    }

    triangle[base] += triangle[prev_base];
    uint64_t max_value = triangle[base];

    for(uint32_t j = 1UL; j < ite_bound - 1UL; j++){
        triangle[base + j] += std::max(triangle[prev_base + j-1UL], triangle[prev_base + j]);

        if(max_value < triangle[base+ j])
            max_value = triangle[base + j];
    }

    triangle[base + ite_bound -1UL] += triangle[prev_base + ite_bound - 2UL];
    if(max_value < triangle[base+ ite_bound -1UL])
        max_value = triangle[base + ite_bound -1UL];



    auto end = std::chrono::steady_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);


    std::cout << max_value << "\n";
    std::cout << "Elapsed time: " << ms.count() << " ms\n";
}