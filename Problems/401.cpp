#include <iostream>
#include <vector>

#define MOD  1000000000ULL
#define INV3  666666667

using uint128_t = unsigned __int128;

uint128_t integer_2root(uint64_t n){
    uint128_t begin = 0ULL;
    uint128_t end = n;

    uint128_t mid = n >> 20;

    if(n < mid * mid){
        end = mid;
    }

    while(begin < end){
        mid = (begin + end) >> 1;

        if(mid*mid < n && n < (mid+1ULL)*(mid+1ULL)){
            return mid;
        } else if(mid*mid < n){
            begin = mid + 1ULL;
        } else if(n < mid*mid){
            end = mid - 1ULL;
        } else{
            return mid;
        }
    }

    return begin;
}

//This function computes the sum of the first n consecutives terms of σ_{2} in O(√n).
//This program uses the decomposition σ_{2} = f_2 * s, where * is the Dirichlet convolution,
// f_{2}(k) = k^{2} and s(k) = 1 for every positive integer. Applying Dirichlet hyperbola method,
//we deduce that
// \sum_{k = 1}^{n} σ_{2}(k) = \sum_{x = 1}^{⌊√n⌋} f_{2}(x) . ( sum_{y = 1}^{⌊n/x⌋} s(y) )
//                           + \sum_{y = 1}^{⌊√n⌋} s(y) . ( \sum_{x = 1}^{⌊n/y⌋} f_{2}(x) )
//                           - (\sum_{x = 1}^{⌊√n⌋} f_{2}(x)) . (\sum_{y = 1}^{⌊√n⌋} s(y)) =
//               \sum_{x = 1}^{⌊√n⌋}  (x^{2}.⌊n/x⌋ + (⌊n/x⌋ . (⌊n/x⌋+1) . (2⌊n/x⌋+1))/6)
//               - (⌊√n⌋^{2} . (⌊√n⌋+1) . (2⌊√n⌋+1))/6
int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t sol = 0ULL;
    uint128_t prod = 0ULL;

    uint128_t m = integer_2root(n);

    for(uint128_t t = 1ULL; t <= m;t++){
        uint128_t x = (n/t);

        prod = (t*t) % MOD;
        prod = (prod * x) %MOD;
        sol = (sol + prod) % MOD;


        prod = (x* (x+1ULL)) % MOD;
        prod = (prod* ((x << 1) + 1ULL)) % MOD;
        prod = (prod* INV3) % MOD;
        prod >>= 1;
        sol = (sol +  prod) % MOD;

    }

    prod = (m*m) % MOD;
    prod = (prod* (m+1ULL)) % MOD;
    prod = (prod* ((m << 1) + 1ULL)) % MOD;
    prod = (prod* INV3) % MOD;
    prod >>= 1;

    sol =  (sol + (MOD - prod)) % MOD;

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

//1000000000000000