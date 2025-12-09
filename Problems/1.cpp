#include <iostream>

//Computes the sum of the multiples of 3 or 5 less than n.
//Using the inclusion-exclusion principle,
//that is equivalent as the sum of the multiples of 3 plus the multiples of 5 minus the multiples of 15
int main(){
    uint32_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    n = n-1;
    uint32_t m3 = n/3;
    uint32_t m5 = n/5;
    uint32_t m15 = n/15;

    uint32_t sol = 3*((m3*(m3+1)) >> 1)+ 5*((m5*(m5+1)) >> 1)-15*((m15*(m15+1)) >> 1);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}