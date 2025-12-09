#include <iostream>
#include <vector>
#include <cstdint>


//This program computes the number of partitions of n using Euler pentagonal formula.
//Its time complexity is in O(n*√n) and O(n) in space.
// It does not count n as a partition of n but will do so in intermediate results.
int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();
    
    //We compute the number of partitions iteratively.
    //The i-th entry represents the number i+1.
    std::vector<uint64_t> p(n + 1);

    p[0] = 1ULL; //1 only has one partition, itself.

    for (uint64_t i = 1ULL; i <= n; ++i) {
        uint64_t total = 0ULL;
        for (uint64_t k = 1ULL;; ++k) { //We get the pentagonal numbers in Euler formula until we exceed i
            uint64_t g1 = (k * (3ULL * k - 1ULL)) >> 1; //Next usual pentagonal number.
            uint64_t g2 = (k * (3ULL * k + 1ULL)) >> 1; //Next generalized pentagonal number.

            if (g1 > i) //We have exceeded i, so we stop. It is important to notice that i < g1 < g2.
                break;

            if(k & 1){
                total += p[i - g1];
            } else{
                total -= p[i - g1];
            }

            if (g2 <= i) {
                if (k & 1) {
                    total += p[i - g2];
                } else {
                    total -= p[i - g2];
                }
            }
        }
        p[i] = total;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << p[n]-1 << "\n"; //In the exercise it is said that
    // the whole number is not a partition, so we must subtract one.
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

}
