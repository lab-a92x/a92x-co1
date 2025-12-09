#include <iostream>
#include <vector>
#include <list>

// Fast exponentiation (safe with 128-bit intermediate)
uint64_t pow_int(uint64_t a, uint16_t e) {
    uint64_t res = 1;
    __uint128_t base = a;
    while (e) {
        if (e & 1) {
            res = (uint64_t)((__uint128_t)res * base);
        }
        base = (base * base);
        e >>= 1;
    }
    return res;
}

// floor(sqrt(n)) safe
uint64_t floor_2root(uint64_t n) {
    uint64_t lo = 0, hi = std::min<uint64_t>(n, (1ULL<<32)); // hi = n is safe but we can keep it simple
    // safer: set hi = n, binary search will handle quickly; keep hi = n for correctness
    hi = n;
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

// floor(cuberoot(n)) safe
uint64_t floor_3root(uint64_t n) {
    uint64_t lo = 0, hi = std::min<uint64_t>(n, (1ULL<<32));
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t cube = (__uint128_t)mid * mid * mid;
        if (cube <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

// floor(n^{2/3}) i.e. floor( (n^2)^{1/3} ) -- safe implementation
uint64_t floor_pow_2_3(uint64_t n) {
    // find x such that x^3 <= n^2 < (x+1)^3
    __uint128_t target = (__uint128_t)n * (__uint128_t)n;
    uint64_t lo = 0, hi = std::min<uint64_t>(n, (1ULL<<32)); // x cannot exceed n
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t cube = (__uint128_t)mid * mid * mid;
        if (cube <= target) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}





uint64_t compute_sh(const uint64_t & n, const std::vector<int16_t> & mobius){
    uint64_t sol = 0ULL;
    uint64_t k = floor_2root(n);

    for(size_t m = 1; m <= k; m++){
        uint64_t sum = 0ULL;
        for (size_t d = 1; d <= floor_2root(n/(m*m)); ++d) {
            if(mobius[d] > 0){
                sum += n/(m*m*d*d);
            } else if(mobius[d] < 0){
                sum -= n/(m*m*d*d);
            }
        }
        sol += m*sum;
    }

    return sol;

}


//This function computes the sum of the first n consecutives terms of σ_{2} in O(√n).
//This program uses the decomposition σ_{2} = f_2 * s, where * is the Dirichlet convolution,
// f_{2}(k) = k^{2} and s(k) = 1 for every positive integer. Applying Dirichlet hyperbola method,
//we deduce that
// \sum_{k = 1}^{n} σ_{2} = \sum_{x = 1}^{⌊√n⌋} f_{2}(x) . ( sum_{y = 1}^{⌊n/x⌋} s(y) )
//                           + \sum_{y = 1}^{⌊√n⌋} s(y) . ( \sum_{x = 1}^{⌊n/y⌋} f_{2}(x) )
//                           - (\sum_{x = 1}^{⌊√n⌋} f_{2}(x)) . (\sum_{y = 1}^{⌊√n⌋} s(y)) =
//               \sum_{x = 1}^{⌊√n⌋}  (x^{2}.⌊n/x⌋ + (⌊n/x⌋ . (⌊n/x⌋+1) . (2⌊n/x⌋+1))/6)
//               - (⌊√n⌋^{2} . (⌊√n⌋+1) . (2⌊√n⌋+1))/6
int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t sol = 0ULL;

    uint64_t k2 = floor_2root(n);
    uint64_t k3 = floor_3root(n);
    uint64_t k23 = floor_pow_2_3(n);

    std::vector<int16_t> mobius(k2 + 1, 0);
    std::vector<uint32_t> h(k2 + 1, 0);
    std::list<uint32_t> primes;

    mobius[1] = 1;
    h[1] = 1UL;

    for (size_t i = 2; i < h.size(); ++i) {
        if(!h[i]){
            primes.push_back(i);

            h[i] = i;
            mobius[i] = -1;
        }

        if(!mobius[i]) { // \mu(i) = 0 so we are not needed to actualise \mu for its multiples
            for (uint32_t &p: primes) {
                if (h[i] < p || h.size() <= i * p)
                    break;

                h[i * p] = p;
            }
        }else{
            for (uint32_t &p: primes) {
                if (h[i] <= p || h.size() <= i * p)
                    break;

                h[i * p] = p;
                mobius[i*p] = -mobius[i];
            }

            if(i*h[i] < h.size()){
                h[i*h[i]] = h[i];
            }
        }


        uint16_t exp = 0;
        uint32_t rem = i;
        while( !(rem % h[i]) ){
            exp++;
            rem /= h[i];
        }

        h[i] = h[rem]* pow_int(h[i], exp >> 1);
    }


    for(size_t d = 1; d <= k3; d++){
        sol += compute_sh(n/d, mobius);
    }

    for(size_t d = 1; d <= k2; d++){
        sol += h[d]*(n/d);
    }

    sol -= k3*compute_sh(k23, mobius);


    std::vector<uint64_t> h_sieve(k2, 1);
    uint64_t low = k2+1ULL;
    uint64_t high = std::min(low + h_sieve.size(), k23+1);

    while(low <= k23){

        for(uint32_t & p : primes ){
            for(uint64_t i =  (low + p - 1)/p; i*p < high; i++){
                uint16_t exp = 1;
                uint64_t rem = i;
                while( !(rem % p) ){
                    exp++;
                    rem /= p;
                }

                h_sieve[(i*p) - low] *= pow_int(p, exp >> 1);

            }
        }

        for(size_t d = low; d < high; d++){
            sol += h_sieve[d - low]*(n/d);
        }

        std::fill(h_sieve.begin(), h_sieve.end(), 1);

        low = high;
        high = std::min(low + h_sieve.size(), k23+1);
    }




    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

//1000000000000000