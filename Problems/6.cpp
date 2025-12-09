#include <iostream>

//The program computes the difference between the square of the sum of the first n positive integers
// and the sum of the squares of the first n integers.
//That difference is given by the formula (n*(n-1)*(n+1)*(3*n+2))/12.
int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t sol = (n*(n-1ULL)*(n+1ULL)*(3ULL*n+2ULL))/12ULL;

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}