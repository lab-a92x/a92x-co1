#include <iostream>

#define N 10000000000ULL
#define K 7830457UL
#define M 28433ULL
#define U 2ULL
#define V 1ULL

uint64_t quick_exp_mod(uint64_t u, uint32_t k, uint64_t n){

    unsigned __int128 pow = u;
    unsigned __int128 sol = 1ULL;

    while(0ULL < k){
        if(k & 1ULL){
            sol = (sol*pow) % n;
        }

        pow = pow*pow;
        pow = pow  % n;
        k >>= 1;
    }

    return sol;
}

int main(){
    uint64_t sol = quick_exp_mod(U, K, N);
    sol = (M*sol) % N;
    sol = (sol + V) % N;

    std::cout << sol << "\n";

}