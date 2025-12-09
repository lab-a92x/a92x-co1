#include <iostream>
#include <list>
#include <array>
#include <fstream>

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

// Fast  exponentiation
uint64_t pow_(uint64_t a, uint64_t e) {
    uint64_t res = 1;
    while (e) {
        if (e & 1) {
            res = (uint64_t)((__uint128_t)res * (__uint128_t)a);
        }
        a = (uint64_t)((__uint128_t)a * (__uint128_t)a);
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

// Determinist Miller–Rabin for 64-bits unsigned integers
bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;

    //Small list of primes
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

    // Determinist bases set for 64-bit integers
    static const uint64_t bases[] = {
            2ULL, 325ULL, 9375ULL, 28178ULL,
            450775ULL, 9780504ULL, 1795265022ULL
    };

    for (uint64_t a : bases) {
        if (a % n == 0) continue; // If a ≡ 0 (mod n), then we skip this base
        if (witness(a, n, d, s)) return false;
    }
    return true;
}

int main(){
    std::ifstream infile("../Files/Output.txt"); if (!infile) { std::cerr << "Error when opening reading file.\n"; return 1; } //TODO

    uint32_t n;
    uint64_t count = 0;
    while(infile >> n){
        count++;
        if(7 < n && n % 7 == 0 && is_prime_u64(n/7) && (n/7 - 1) % 7 == 0){
            std::cout << n  <<  "\n";
            infile.close();
            return 0;
        }
    }
    infile.close();
    std::cout <<  "Count: " << count << "\n";
    return 0;

}