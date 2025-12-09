#include <iostream>

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

//This function computes the sum of the primes that are less or equal to n.
//We use deterministic Rabin-Miller test for 64-bits unsigned integers for efficiently computing primes.
int main(){
    
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();
    
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    uint64_t sum = 0ULL;
    for(uint64_t p: small_primes){
        if(p <= n)
            sum += p;
    }


    uint64_t i = 41ULL;

    while(i <= n){
        if(is_prime_u64(i)){
            sum += i;
        }
        i = i + 2ULL;
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sum << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;

}