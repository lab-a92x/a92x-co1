#include <iostream>

int main() {
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t greatest_prime = 0ULL;

    //In order to compute the biggest prime factor,
    //A naive approach is to start by treating the case of 2 in particular.
    //Then, we search iteratively the odd numbers. When we encounter a new number that divides n,
    //it must be prime because it is minimal between the non-unity divisors of the current n.
    //In order to ascertain that the number is minimal, we must divide n for the maximum prime power
    //that divides it when encountering a prime, making sure that no smaller prime divides the new n.

    if(!(n & 1)){
        greatest_prime = 2ULL;
        n = n >> 1;
    }
    while (!(n & 1)) {
        n = n >> 1;
    }

    for (uint64_t i = 3ULL; i*i <= n; i = i + 2ULL) {
        if(!(n % i)){
            greatest_prime = i;
            n /= i;
        }
        while (!(n % i)) {
            n /= i;
        }

        if(n == 1ULL){
            break;
        }
    }

    if (n > 1)
        greatest_prime = n;

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << greatest_prime << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}

//600851475143