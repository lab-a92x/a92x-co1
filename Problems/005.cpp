#include <iostream>

//Stein algorithm for computing the gcd
uint64_t binary_gcd(uint64_t a, uint64_t b) {
    if (a == 0) return b;
    if (b == 0) return a;

    // we count shared factors of 2
    int shift = 0;
    while (((a | b) & 1) == 0) { // ambos pares
        a >>= 1;
        b >>= 1;
        ++shift;
    }

    // we get the odd part of a
    while ((a & 1) == 0)
        a >>= 1;

    do {
        //  we get the odd part of b
        while ((b & 1) == 0)
            b >>= 1;

        // we check a <= b
        if (a > b)
            std::swap(a, b);

        b -= a;
    } while (b != 0);

    return a << shift;
}

// Fast integer exponentiation
uint64_t pow_us(uint64_t a, uint64_t e) {
    uint64_t res = 1;
    while (e) {
        if (e & 1)
            res =  a * res;
        a =  a * a;
        e >>= 1;
    }
    return res;
}

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

//Function that computes  ⌊log_a (n)⌋ with binary search.
uint64_t floor_log(uint64_t a, uint64_t n){
    if(n < a)
        return 0;

    uint64_t beg = 1;
    uint64_t end = beg;
    uint64_t pow = a;
    uint64_t pow0 = pow;

    while(pow < n){
        pow0 = pow;
        beg = end;
        end = end << 1;
        pow = pow * pow;
    }


    while(beg != end){
        uint64_t mid = (beg + end) >> 1;
        pow = pow0 * pow_us(a, mid - beg);
        if(pow <= n && n < pow * a) {
            return mid;
        } else if(pow <= n){
            beg = mid + 1;
            pow0 = pow * a;
        } else {
            end = end - 1;
        }
    }

    return beg;

}


//Computes the m.c.m of the first n integers.
//It computes the primes in the range {1,...,n} using deterministic Rabin-Miller and then
//it computes the largest power of those primes in range
// before multiplying those powers to the final solution
int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t sol = 1ULL;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for(uint64_t p: small_primes) {
        if(p <= n) {
            sol *= pow_us(p, floor_log(p, n));
        }else{
            break;
        }
    }

    for(int p = 41; p <= n; p++){
        if(is_prime_u64(p)) {
            sol *= pow_us(p, floor_log(p, n));
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std:: cout << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}