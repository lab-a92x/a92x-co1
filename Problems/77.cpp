#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <cstdint>

#define LIMIT 1000


// Fast modular power
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

// Witness for Miller–Rabin
bool witness(uint64_t a, uint64_t n, uint64_t d, int s) {
    uint64_t x = pow_mod(a, d, n);
    if (x == 1 || x == n - 1) return false;
    for (int r = 1; r < s; r++) {
        x = (uint64_t)((__uint128_t)x * x % n);
        if (x == n - 1) return false;
    }
    return true; // 'a' it proves that n is composite
}

// Determinist Miller–Rabin for 64-bit unsigned integers
bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;

    // We check for small primes
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

    // Determinist base set for 64bit unsigned integers
    static const uint64_t bases[] = {
            2ULL, 325ULL, 9375ULL, 28178ULL,
            450775ULL, 9780504ULL, 1795265022ULL
    };

    for (uint64_t a : bases) {
        if (a % n == 0) continue; // if a ≡ 0 (mod n) we skip this base
        if (witness(a, n, d, s)) return false;
    }
    return true;
}


//this function computes the first number that can be written as a sum of primes in more than
int main(){
    u_int64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    //list that stores primes
    std::list<uint64_t> prime_list;
    prime_list.push_back(2ULL); //we start with two in the list

    uint64_t i = 3ULL; //index for finding primes

    while(i <= LIMIT){
        if(is_prime_u64(i)){
            prime_list.push_back(i);
        }
        i = i  + 2ULL;
    }



    //vector that counts the ways of adding primes to form a number
    //The i-th entry correspond to the number of ways  of adding primes to form  i
    std::vector<uint64_t> prim_count(LIMIT, 0ULL);

    prim_count[0ULL] = 1ULL; //One way for zero (empty sum)

    for(auto it = prime_list.begin(); std::next(it) != prime_list.end(); it++){
        for(uint64_t j = *it; j < prim_count.size(); j++){
            prim_count[j] += prim_count[j-*it];
        }
    }

    for(uint64_t j = 0ULL; j < prim_count.size() && j < prime_list.back(); j++){
        if(n <= prim_count[j]){
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed = end - start;

            std::cout << j << "\n";
            std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
            return 0;
        }
    }

    for(uint64_t j = prime_list.back(); j < prim_count.size(); j++){

        prim_count[j] += prim_count[j-prime_list.back()];

        if(n <= prim_count[j]){
            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed = end - start;

            std::cout << j << "\n";
            std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
            return 0;
        }

    }

    return 0;

}
