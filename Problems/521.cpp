#include <iostream>
#include <vector>
#include <list>

const uint64_t MOD = 1000000000;

// Safe positive integer sqrt floor
uint64_t floor_2root(uint64_t n) {
    if (n < 2) return n;
    uint64_t lo = 1;
    uint64_t hi = n;

    while (lo <= hi) {
        uint64_t mid =(hi + lo) >> 1;

        // compare mid*mid and n safely: mid*mid <= n  <=> mid <= n / mid
        if (mid <= n / mid) {
            // mid*mid <= n
            if ( (mid + 1) > n / (mid + 1) ) {
                return mid;
            }
            lo = mid + 1;
        } else {
            hi =  mid - 1;
        }
    }
    return lo;
}

int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t sol = 0ULL;

    uint64_t k2 = floor_2root(n);

    std::vector<uint32_t> spf(k2 + 1, 0);
    std::list<uint32_t> primes;


    for (size_t i = 2; i < spf.size(); ++i) {
        if(!spf[i]){
            primes.push_back(i);

            spf[i] = i;
        }

        for(uint32_t & p: primes){
            if(spf[i] < p || spf.size() <= i*p)
                break;

            spf[i*p] = p;
        }

        sol += spf[i];
    }



    std::vector<uint64_t> h_sieve(k2+1);
    uint64_t low = k2+1ULL;
    uint64_t high = std::min(low + h_sieve.size(), n+1);

    for(size_t i = low; i < high; i++){
        h_sieve[i - low] = i;
    }

    while(low <= n){

        for(auto it = primes.rbegin(); it != primes.rend(); it++ ){
            uint32_t p = *it;
            for(uint64_t i =  (low + p - 1)/p; i*p < high; i++){
                h_sieve[(i*p) - low] = p;
            }
        }

        uint64_t sol0 = 0ULL;
        for(size_t d = low; d < high; d++){
            sol0 += h_sieve[d - low];
        }

        sol = (sol + sol0) % MOD;


        low = high;
        high = std::min(low + h_sieve.size(), n+1);

        for(size_t i = low; i < high; i++){
            h_sieve[i - low] = i;
        }
    }




    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

//1000000000000