#include <iostream>
#include <vector>



int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    //Sieve that computes Euler totient function. The i-th entry corresponds to i+1
    std::vector<uint64_t> sieve(n, 0ULL);

    sieve[0] = 1ULL; //\phi(1)= 1.
    uint64_t sum = 0ULL; //Variable that carries the cumulative sum of \phi.

    for(uint64_t i = 1; i < sieve.size(); i++){ //We start iterating with 2

        if(sieve[i]){ //i+1is not prime
            uint64_t p = sieve[i]; //p is the largest prime divisor of i+1


            uint64_t k = (i+1)/p; //We decompose i+1 in k, which is coprime with p
            uint64_t l = p; //and l, which is the largest power of p that divides i+1.

            while(!(k % p)){
                k /= p;
                l *= p;
            }

            if(1 < k) //i+1 is not a power of p and l k is a proper decomposition of i+1.
                //Therefore, we can apply the multiplicativity of the Euler totient function
                // with coprime integers in order to compute the value of \phi(i+1)
                sieve[i] = sieve[k-1]*sieve[l-1];

            else  //i+1 is a power of p, specifically i+1 = l.
            // Euler totient function is easily determined for prime powers by
            //(p-1)*p^(exp-1)
                sieve[i] = (l/p)*(p-1);

        } else{ //i+1 is prime

            sieve[i] = i; //\phi(i+1) = (i+1)-1 = i

            for(uint64_t j = (i << 1)+1; j < sieve.size(); j = j+(i+1)){ //We iterate over the multiples of i+1
                sieve[j] = i+1; //We tag them in order to ascertain that i+1 divides them
            }
        }

        sum += sieve[i]; //We add the computed value to the cumulative sum
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sum << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;

}
