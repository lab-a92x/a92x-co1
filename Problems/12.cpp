#include <iostream>
#include <vector>

//limit until we search
#define MAX_LIMIT 20000

int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    //Sieve that counts iteratively the number of divisors of positive integers.
    //The i-th entry represents the number i+1
    std::vector<uint64_t> spf(MAX_LIMIT, 0ULL);
    spf[0] = 1ULL; //1 has only one divisor

    for(uint64_t i = 1; i < spf.size(); i++){ //We start with 2
        if(spf[i]){ //The number is not prime

            uint64_t p = spf[i]; //This gives us the largest prime divisor of i+1
            uint64_t k = (i+1)/p; //We decompose i+1 in k, which is not divisible by p
            uint64_t l = spf[i]; //and l, which is the largest power of p that divides i+1

            int exp = 1;
            while(!(k % spf[i])){
                k /= spf[i];
                l *= spf[i];
                exp++;
            }

            if(1 < k) //(i+1) is not l, therefore k and l is a proper decomposition
                // and we can apply the multiplicativity of the count of divisors for coprime integers
                spf[i] = spf[k-1]*spf[l-1];

            else //In this case, i+1 is a power of p, specifically l, therefore it has exp +1 divisors, where l = p^exp
                spf[i] = exp + 1;

        } else{ //The number is prime
            spf[i] = 2ULL; //Only two divisors, 1 and itself

            for(uint64_t j = (i << 1)+1; j < MAX_LIMIT; j = j+(i+1)){ //We iterate over the multiples of the prime
                spf[j] = i+1; //we tag that the prime i+1 divides j+1
            }
        }
    }


    //The triangular numbers are of the form (k*(k+1))/2 for positive k.
    //k and k+1 are coprime always and therefore, we can apply
    // the multiplicativity of the count of divisors for coprime integers
    // k/2 and k+1 or k and (k+1)/2, depending on the parity of k.
    for(uint64_t i = 0; i < spf.size(); i++){
        if(i & 1){
            uint64_t d = spf[((i+1) >> 1)-1]*spf[i+1];
            if(n < d){
                std::cout << ((i+1) >> 1)*(i+2) << "\n";
                break;
            }

        }else{
            uint64_t d = spf[i]*spf[((i+2) >> 1)-1];
            if(n < d){

                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> elapsed = end - start;

                std::cout << ((i+2) >> 1)*(i+1) << "\n";
                std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
                break;
            }

        }
    }

    return 0;

}
