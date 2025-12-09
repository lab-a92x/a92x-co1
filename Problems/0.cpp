#include <iostream>


//Computes the sum of the first n odd squares
int main() {
    uint32_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();
    uint32_t  m = n - (n >> 1);
    uint32_t sol = (m*(((m*m) << 2) - 1))/3;

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}

