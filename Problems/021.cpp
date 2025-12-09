#include <iostream>


int main() {
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    //Sieve that counts iteratively the sum of divisors of positive integers until n.
    //The i-th entry represents the number i+1
    std::vector<uint64_t> lpf(n, 0ULL);
    lpf[0] = 1ULL; //1 has only one divisor

    for (uint64_t i = 1ULL; i < lpf.size(); i++) { //We start with 2
        if (lpf[i]) { //The number is not prime

            uint64_t p = lpf[i]; //This gives us the largest prime divisor of i+1
            uint64_t k = (i + 1) / p; //We decompose i+1 in k, which is not divisible by p
            uint64_t l = p; //and l, which is the largest power of p that divides i+1

            int exp = 1;
            while (!(k % p)) {
                k /= p;
                l *= p;
                exp++;
            }

            if (1 < k) //(i+1) is not l, therefore k and l is a proper decomposition
                // and we can apply the multiplicativity of the sum of divisors for coprime integers
                lpf[i] = lpf[k - 1] * lpf[l - 1];

            else //In this case, i+1 is a power of p, specifically l, therefore the sum of divisors is
            // 1+p+...+p^exp = (p^(exp+1)-1)/(p-1)
                lpf[i] = (l*p-1)/(p-1);

        } else { //The number is prime
            lpf[i] = i+2ULL; //i+1 only two divisors, 1 and itself so the sum is i+2

            for (uint64_t j = (i << 1) + 1;
                 j < lpf.size(); j = j + (i + 1)) { //We iterate over the multiples of the prime
                lpf[j] = i + 1; //we tag that the prime i+1 divides j+1
            }
        }
    }

    //Now we check for amicable numbers. lpf saves the whole sum,
    // therefore we must subtract i+1 for the i-th entry since amicable numbers only count proper divisors

    uint64_t sol = 0ULL;

    for (uint64_t i = 0ULL; i < lpf.size(); i++){
        uint64_t b = lpf[i]-(i+1ULL);

        if(b != i+1ULL && 0ULL < b && b <= n && lpf[b-1ULL] - b == i+1){
            sol += i+1ULL;
        }

    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

}

