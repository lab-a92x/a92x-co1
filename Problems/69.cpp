#include <iostream>
#include <cstdint>


// Fast modular exponentiation
uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod) {
    a %= mod;
    uint64_t res = 1;
    while (e) {
        if (e & 1) {
            res = (uint64_t)((__uint128_t)res * (__uint128_t)a % mod);
        }
        a = (uint64_t)((__uint128_t)a * (__uint128_t)a % mod);
        e >>= 1;
    }
    return res;
}

// Witness for Miller-Rabin
bool witness(uint64_t a, uint64_t n, uint64_t d, int s) {
    uint64_t x = pow_mod(a, d, n);
    if (x == 1 || x == n - 1) return false;
    for (int r = 1; r < s; r++) {
        x = (uint64_t)((__uint128_t)x * x % n);
        if (x == n - 1) return false;
    }
    return true;
}

// Deterministic Miller-Rabin for 64-bit unsigned integers
bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;

    // Small primes
    static const uint64_t small_primes[] = {
            2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (uint64_t p : small_primes) {
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    // Decomposition n-1 = d * 2^s
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    // Deterministic bases set for 64-bit unsigned integers
    static const uint64_t bases[] = {
            2ULL, 325ULL, 9375ULL, 28178ULL,
            450775ULL, 9780504ULL, 1795265022ULL
    };

    for (uint64_t a : bases) {
        if (a % n == 0) continue; // if a ≡ 0 (mod n),  then we skip that base
        if (witness(a, n, d, s)) return false;
    }
    return true;
}


int main() {
    uint64_t n;
    if (!(std::cin >> n)) return 0;

    auto start = std::chrono::high_resolution_clock::now();

    //The problem ask us to give the least number in {1,...,n} such that k/phi(k) attains the maximum value in {1,...,n}
    //This is equivalent to compute the minimum of phi(k)/k in the set {1,...,n}.
    // First of all, the ratio phi(k)/k depends only on the primes that divide k, not their power.
    //Therefore, the solution must be square-free. Moreover, taking the minimum prime not already taken minimices
    //phi(k)/k and k. This gives us a greedy approach. Computing iteratively the product of the first primes until
    //we can't add more primes without exceeding n.

    uint64_t prime; //Next prime

    uint64_t primes_prod = 2ULL;      // Product of the first primes. We start by with only 2


    uint64_t candidate = 3ULL;
    while (true) {


        // we look for the next prime
        while (true) {
            if (is_prime_u64(candidate)) {
                prime = candidate;
                candidate += 2;
                break;
            }
            candidate += 2;
        }

        uint64_t new_primes_prod = primes_prod*prime;
       if(n < new_primes_prod)
           break;
       else
           primes_prod = new_primes_prod;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << primes_prod << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

}