#include <iostream>
#include <vector>
#include <cstdint>

//This program computes the first positive integer whose number of partitions is divisible by n.
//In doing so, this program computes the number of partitions of n modulo n using Euler pentagonal formula.
//Its time complexity is in O(n*√n) and O(n) in space.
int main(){
    int64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    //We compute the number of partitions modulo n iteratively.
    //The i-th entry represents the number i+1.
    // It is important to work with signed integers because in the modular case,
    // the number of partitions is not monotone.
    std::vector<int64_t> p(n + 1);

    p[0] = 1LL; //1 only has one partition, itself.

    for (uint64_t i = 1ULL; i <= n; ++i) {
        int64_t total = 0LL;
        for (uint64_t k = 1ULL;; ++k) { //We get the pentagonal numbers in Euler formula until we exceed i
            uint64_t g1 = (k * (3ULL * k - 1ULL)) >> 1; //Next usual pentagonal number.
            uint64_t g2 = (k * (3ULL * k + 1ULL)) >> 1; //Next generalized pentagonal number.

            if (g1 > i) //We have exceeded i, so we stop. It is important to notice that i < g1 < g2.
                break;

            if(k & 1){
                total = (total + p[i - g1]) % n;
            } else{
                total = (total - p[i - g1]) % n;
            }

            if (g2 <= i) {
                if (k & 1) {
                    total = (total + p[i - g2]) % n;
                } else {
                    total = (total - p[i - g2]) % n;
                }
            }
        }

        p[i] = total;
        if(!p[i]){

            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed = end - start;

            std::cout << i << ": " <<  p[i] << "\n";
            std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
            return 0;
        }
    }

    return 0;
}
