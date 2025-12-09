#include <iostream>
#include <cstdint>
#include <thread>
#include <vector>
#include <mutex>

uint64_t sol = 0ULL;
std::mutex mtx; // mutex for protection access

//Function that computes ⌊√p⌋ using binary search.
uint64_t root2_integer(uint64_t p){
    uint64_t beg = 0ULL;
    uint64_t end = p;

    while(beg < end){
        uint64_t mid = (end + beg) >> 1;

        if(mid*mid <= p){
            if((mid+1)*(mid+1) > p){
                return mid;
            } else{
                beg = mid + 1ULL;
            }
        } else{
            end = mid - 1ULL;
        }
    }

    return beg;
}


//Function that computes ⌊∛p⌋ using binary search.
uint64_t root3_integer(uint64_t p){
    uint64_t beg = 0ULL;
    uint64_t end = p;

    while(beg < end){
        uint64_t mid = (end + beg) >> 1;

        if(mid*mid*mid <= p){
            if((mid+1)*(mid+1)*(mid+1) > p){
                return mid;
            }else{
                beg = mid + 1ULL;
            }
        } else{
            end = mid - 1ULL;
        }
    }

    return beg;
}

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

//This function computes the number of divisor of p in O(p^{1/3})
uint64_t count_num_div(uint64_t p, const std::vector<uint64_t> & primes){

    uint64_t i = 0ULL;
    uint64_t count = 1ULL;
    uint64_t k = root3_integer(p);

    while(i < primes.size() && primes[i] <= k){
        if(!(p % primes[i])){

            uint32_t exp = 2UL;
            p /= primes[i];

            while(!(p % primes[i])){
                exp = exp + 1UL;
                p /= primes[i];
            }

            count *= exp;
        }
        i++;
    }

    //In p it could remain nothing, a prime or the product of two primes because the primes remaining are bigger than n^{1/3}

    if(p == 1ULL){ //No prime remains
        return count;
    }

    if(is_prime_u64(p)) { //It remains only one prime
        return count << 1;
    }

    k = root2_integer(p);

    if(k*k == p) //It remains the same prime power two
        return 3ULL*count;

    return count << 2; //It remains two different primes
}

//Given n natural, this function counts the integral solutions of the equation
// 1/a + 1/b = p/10^n for positive a,b and p such that a <= b.
//It can be seen that it is equivalent to integer solutions for positive a,b and p such that a <= b
// of (pa-10^n)(pb-10^n) = 10^{2n}. Therefore, we only need to account every pair d1, d2 of divisors of 10^{2n} such that d2 = 10^{2n}/d1.
//For every pair of associated divisors, we end up with the equations pa = 10^{n}+d1 = c1, pb = 10^{n}+d2 = c2.
// Therefore, p can be every divisor of gcd(c1,c2) and a and b are determined when p is, a = c1/p and b = c2/p.
//Therefore, the number of solutions coincides with the sum of the number of divisor of gcd(c1,c2) for every posible pair of associated divisors.
//We only account every pair of divisor once, independently of the order due to the condition a <= b.
void count_solutions(int n, const std::vector<uint64_t> & primes,  const std::vector<uint64_t> & pot5){

    for(int i = 0; i < (n << 1)+1; i++){
        for(int j = i+1; j < (n << 1)+1; j++){
            uint64_t x1 = (pot5[n] << n) + (pot5[j] << i);
            uint64_t x2 = (pot5[n] << n) + (pot5[(n << 1)-j] << ((n << 1)-i));
            uint64_t p = binary_gcd(x1, x2);
            uint64_t k = count_num_div(p, primes);
            {
                std::lock_guard<std::mutex> lock(mtx);
                sol += k;
            }
        }
    }

    for(int j = 0; j < n+1; j++){
        uint64_t p = binary_gcd((pot5[n] << n) + (pot5[j] << j), (pot5[n] << n) + (pot5[j] << j));
        uint64_t k = count_num_div(p, primes);
        {
            std::lock_guard<std::mutex> lock(mtx);
            sol += k;
        }
    }



}

int main(){
    std::vector<std::thread> threads;

    int n;
    if (!(std::cin >> n)) return 0;

    auto start = std::chrono::high_resolution_clock::now();

    int m = ((n+2)/3) << 1;
    uint64_t pow10 = 1ULL;

    for(int i = 0; i < m;i++)
        pow10 *= 10ULL;

    std::vector<uint64_t> primes = {
            2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };

    for(uint64_t i = 41ULL; i < pow10;i++){
        if(is_prime_u64(i)) {
            primes.push_back(i);
        }
    }

    std::vector<uint64_t> pot5((n << 1) +1, 1ULL);
    for(int i = 1; i < pot5.size();i++)
        pot5[i] = 5ULL*pot5[i-1];

    for (int i = 0; i < n; ++i) {
        threads.emplace_back(count_solutions, i+1, primes, pot5);
    }

    for (auto &t : threads)
        t.join();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

}

//53490