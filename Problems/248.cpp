#include <iostream>
#include <list>
#include <array>
#include <fstream>

#define NUM_P 6
#define F13 6227020800ULL

//Computes the n-th element whose Euler totient function is 13!.= 2^{10}*3^{5}*5^{2}*7*11*13.
//Every element that satisfies that condition must has prime factors in  {2,3,5,7,11,13}
// or have a prime factor p such that p-1 is 13-smooth and p^{2} is not a factor because
// if a prime factor p greater than 13  satisfies that p-1 is not 13-smooth or p^{2} is a factor,
//the Euler totient function value would not be 13-smooth.
// We will call those prime factors extra.
//An extra prime factor must be of the form 1 + 2^{k_2} 3^{k_3} 5^{k_5} 7^{k_7} 11^{k_11} 13^{k_13}
//with 0 ≤ k_2 ≤ 10, 0 ≤ k_3 ≤ 5, 0 ≤ k_5 ≤ 2, 0 ≤ k_7 , k_11 , k_13 ≤ 1.
//There are 1584 = 11*6*3*2*2*2 candidates and we can check quickly with deterministic Miller-Rabin
//for unsigned 64-bit integers if any of those is a prime bigger than 13.
//What is left is applying the classical pointer algorithm to produce the numbers
// with prime factors in a set of PRIMES in increasing order,
//but with exponent restriction given by the Euler totient function value.
//Let p_1,...,p_r be the extra PRIMES,
// we want to generate in increasing order the numbers of the form
// 2^{β_2}*3^{β_3}*5^{β_5}*7^{β_7}*11^{β_11}*13^{β_13}*p_{1}^{γ_1}*...*p_{r}^{γ_r}
//with 0 ≤ β_2 ≤ 10, 0 ≤ β_3 ≤ 5, 0 ≤ β_5 ≤ 2, 0 ≤ β_7 , β_11 , β_13, γ_1,..., γ_r ≤ 1.
//It only remains to check the value of the Euler totient function and count when we reach the quota.

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

const std::array<uint32_t, NUM_P> PRIMES = {2UL, 3UL, 5UL, 7UL, 11UL, 13UL};
const std::array<uint16_t, NUM_P> PRIMES_EXP_MAX = {10, 5, 2, 1, 1, 1};
const std::array<std::array<uint16_t, NUM_P>, NUM_P> PRIMES_PHI {{
                                                                     {{0, 0, 0, 0, 0, 0}},
                                                                     {{1, 0, 0, 0, 0, 0}},
                                                                     {{2, 0, 0, 0, 0, 0}},
                                                                     {{1, 1, 0, 0, 0, 0}},
                                                                     {{1, 0, 1, 0, 0, 0}},
                                                                     {{2, 1, 0, 0, 0, 0}}
                                                                 }};

struct smooth_13{
    uint64_t value;
    std::array<uint16_t, NUM_P> exp_phi;
    uint64_t phi;
    uint16_t count;
    static uint16_t r;

    smooth_13(): value(1ULL), exp_phi(), phi(1ULL), count(r){}

    smooth_13(const smooth_13 & b, uint16_t i){
        exp_phi = b.exp_phi;
        if(b.value % PRIMES[i]){
            phi = b.phi*(PRIMES[i]-1);
        } else {
            phi = b.phi*PRIMES[i];
        }
        value = PRIMES[i]*b.value;
        exp_phi[i]++;
        count = r;
    }

    smooth_13( const smooth_13 & b, uint64_t p, const std::array<uint16_t, NUM_P> & ex){
        value = p*b.value;
        count = r;
        phi = b.phi*(p-1);
        exp_phi =b.exp_phi;

        for(uint16_t i = 0; i < NUM_P; i++)
            exp_phi[i] += ex[i];
    }

    bool decrease(){
         return --count;
    }


    bool valid_exp(const std::array<uint16_t, NUM_P> & ex){
        bool cond = true;

        for(size_t i = 0; i < NUM_P; i++)
            cond = cond && exp_phi[i] + ex[i] <= PRIMES_EXP_MAX[i];

        return cond;
    }

    bool valid_exp(uint16_t i){
        bool cond = true;

        if(value % PRIMES[i]) {
            for (uint16_t j = 0; j < NUM_P; j++)
                cond = cond && exp_phi[j] + PRIMES_PHI[i][j] <= PRIMES_EXP_MAX[j];

            return cond;
        } else{
            return exp_phi[i]+1 <= PRIMES_EXP_MAX[i];
        }
    }
};

uint16_t smooth_13::r = 0;
int main(){

    uint32_t n, counter = 0UL;
    uint64_t sol = 1ULL;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();


    std::list<uint64_t> extra_primes; //We build the list of extra primes

    std::unordered_map<uint64_t, std::array<uint16_t, NUM_P>> extra_phi;

    uint64_t prod = 2ULL;
    for(uint16_t i2 = 1; i2 <= PRIMES_EXP_MAX[0];i2++){  //no factor of 2 implies prod is odd and odd+1 even, no prime
        for(uint16_t i3 = 0; i3 <= PRIMES_EXP_MAX[1];i3++){
            for(uint16_t i5 = 0; i5 <= PRIMES_EXP_MAX[2];i5++){
                for(uint16_t i7 = 0; i7 <= PRIMES_EXP_MAX[3];i7++){
                    for(uint16_t i11 = 0; i11 <= PRIMES_EXP_MAX[4];i11++){
                        for(uint16_t i13 = 0; i13 <= PRIMES_EXP_MAX[5];i13++){
                            if(13 < prod && is_prime_u64(1ULL+prod)){
                                extra_primes.push_back(prod+1);
                                extra_phi.insert({prod+1,{{i2,i3,i5,i7,i11,i13}}});
                            }
                            prod *= PRIMES[5];
                        }
                        prod /= pow_(PRIMES[5], PRIMES_EXP_MAX[5]+1);
                        prod *= PRIMES[4];
                    }
                    prod /= pow_(PRIMES[4], PRIMES_EXP_MAX[4]+1);
                    prod *= PRIMES[3];
                }
                prod /= pow_(PRIMES[3], PRIMES_EXP_MAX[3]+1);
                prod *= PRIMES[2];
            }
            prod /= pow_(PRIMES[2], PRIMES_EXP_MAX[2]+1);
            prod *= PRIMES[1];
        }

        prod = 1 << (i2+1);
    }

    smooth_13::r = extra_primes.size() + NUM_P;
    std::list<smooth_13> inc_gen;
    inc_gen.push_back(smooth_13());

    std::array<std::list<smooth_13>::iterator, NUM_P> pointer_primes; //array of pointers for the primes in {2,3,5,7,11,13}
    std::unordered_map<uint64_t , std::list<smooth_13>::iterator> pointer_extra_primes; //map of pointers for the extra primes

    pointer_primes.fill(inc_gen.begin());

    std::list<uint16_t> waiting_PRIMES; //primes in {2,3,5,7,11,13} whose iterator reached
    // the end of the list and are waiting for a new event.

    std::list<std::list<uint64_t>::iterator> waiting_EXTRA;  //extra primes whose iterator reached
    // the end of the list and are waiting for a new event.

    std::list<uint16_t> ready_PRIMES; //primes in {2,3,5,7,11,13} whose pointer has not reached the end for now.
    std::list<std::list<uint64_t>::iterator> ready_EXTRA; //extra primes whose pointer has not reached the end.



    for(auto it = extra_primes.begin(); it != extra_primes.end(); it++){
        pointer_extra_primes.insert({*it, inc_gen.begin()});
        ready_EXTRA.push_back(it);
    }


    for(size_t i = 0; i < NUM_P;i++)
        ready_PRIMES.push_back(i);

    while(waiting_PRIMES.size() < NUM_P || waiting_EXTRA.size() < extra_primes.size()){

        auto ir = ready_PRIMES.begin();
        uint16_t min = *ir;
        uint64_t min_prod = PRIMES[min] * (pointer_primes[min]->value);
        std::list<decltype(ready_PRIMES.begin())> optimum_PRIMES;
        optimum_PRIMES.push_back(ir); //saves the primes that produce the same minimum
        ir++;

        while( ir != ready_PRIMES.end()){
            if(PRIMES[*ir] * (pointer_primes[*ir]->value) < min_prod){
                min = *ir;
                min_prod = PRIMES[min] * (pointer_primes[min]->value);

                optimum_PRIMES.clear();
                optimum_PRIMES.push_back(ir);
            } else if(PRIMES[*ir] * (pointer_primes[*ir]->value) == min_prod){
                optimum_PRIMES.push_back(ir);
            }
            ir++;
        }

        std::list<decltype(ready_EXTRA.begin())> optimum_EXTRA;
        auto it_extra = ready_EXTRA.end();
        for(auto it = ready_EXTRA.begin(); it != ready_EXTRA.end(); it++){
            if(**it * (pointer_extra_primes[**it]->value) < min_prod){
                it_extra = it;

                optimum_EXTRA.clear();
                optimum_EXTRA.push_back(it);
                min_prod = **it * (pointer_extra_primes[**it]->value);
            }else if(**it * (pointer_extra_primes[**it]->value) == min_prod){
                optimum_EXTRA.push_back(it);
            }
        }



        if(it_extra == ready_EXTRA.end()) {
            inc_gen.push_back(smooth_13(*pointer_primes[min], min));

            for (auto il = optimum_PRIMES.begin(); il != optimum_PRIMES.end();il++) {
                if(pointer_primes[**il]->decrease()) {
                    pointer_primes[**il]++;
                } else{
                    pointer_primes[**il] = inc_gen.erase(pointer_primes[**il]);
                }

                while (pointer_primes[**il] != inc_gen.end() && !pointer_primes[**il]->valid_exp(**il)) {
                    if(pointer_primes[**il]->decrease()) {
                        pointer_primes[**il]++;
                    } else{
                        pointer_primes[**il] = inc_gen.erase(pointer_primes[**il]);
                    }
                }

                if (pointer_primes[**il] == inc_gen.end()) {
                    waiting_PRIMES.push_back(**il);
                    ready_PRIMES.erase(*il);
                }
            }

        } else{
            inc_gen.push_back(smooth_13(*pointer_extra_primes[**it_extra], **it_extra, extra_phi[**it_extra]));
        }

        for(auto il: optimum_EXTRA){
            if(pointer_extra_primes[**il]->decrease()) {
                pointer_extra_primes[**il]++;
            } else{
                pointer_extra_primes[**il] = inc_gen.erase(pointer_extra_primes[**il]);
            }

            while(pointer_extra_primes[**il]!= inc_gen.end() && !(pointer_extra_primes[**il]->value % **il) && !pointer_extra_primes[**il]->valid_exp(extra_phi[**il])){
                if(pointer_extra_primes[**il]->decrease()) {
                    pointer_extra_primes[**il]++;
                } else{
                    pointer_extra_primes[**il] = inc_gen.erase(pointer_extra_primes[**il]);
                }
            }

            if(pointer_extra_primes[**il] == inc_gen.end()){
                waiting_EXTRA.push_back(*il);
                ready_EXTRA.erase(il);
            }
        }

        //Check whether the Euler totient function coincides
        if(inc_gen.back().phi == F13){
            counter++;
            if(counter == n){
                sol = inc_gen.back().value;
                break;
            }
        }


        //We check whether the sleeping primes can wake up
        auto it = waiting_PRIMES.begin();
        while( it != waiting_PRIMES.end()){
            if(inc_gen.back().valid_exp(*it)){
                ready_PRIMES.push_back(*it);
                pointer_primes[*it] = std::prev(inc_gen.end());
                it = waiting_PRIMES.erase(it);
            } else{
                it++;
            }
        }

        auto ite = waiting_EXTRA.begin();
        while( ite != waiting_EXTRA.end()){
            if((inc_gen.back().value % **ite) && inc_gen.back().valid_exp(extra_phi[**ite]) ){ //The extra prime does not divide the new value, so it should wake up
                ready_EXTRA.push_back(*ite);
                pointer_extra_primes[**ite] = std::prev(inc_gen.end());
                ite = waiting_EXTRA.erase(ite);
            }else{ //The extra prime  divides the new value, it should continue sleeping
                ite++;
            }
        }

    }


    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Solution: " << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

}