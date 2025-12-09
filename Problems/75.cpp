#include <iostream>
#include <vector>
#include <chrono>

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

int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();


    //Vector that counts iteratively the number of solutions of the diophantine equations
    //p = a + b + c, c^2 = a^2 + b^2
    //It is well-known that the primitive pytagorean triplets can be parametrized as
    // a = k^2 - m^2, b = 2km, c = k^2 + m^2 with k > m > 0 coprime, one of them even.
    //Therefore, every solution is of the form
    //a = h (k^2 - m^2), b = h (2km), c = h(k^2 + m^2) with
    //p = h(2k(k+m)).
    //We first try to solve the case with h=1.
    //We will iterate through the parametric space while avoiding exceeding the perimeter
    //For each triplet we will increase the number associated with its perimeter by one.

    //To solve the general case, h will be a divisor of p so we must compute the number of solutions
    //of p/h = 2k(k+m) with every possible divisor h of p.
    //Since we already solved this case, we can propagate forward the result for p to all its multiples.
    std::vector<uint64_t> num_sol_prim(n, 0ULL); //solution for primitive triplets
    std::vector<uint64_t> num_sol(n, 0ULL); //solution for all triplets

    uint64_t sol = 0ULL;

    for(uint64_t m = 1ULL; (((m + 1ULL) * ((m << 1 ) + 1ULL)) << 1) <= n; m = m + 2ULL){ //m odd
        for(uint64_t k = m + 1ULL; ((k * (m + k)) << 1) <= n; k = k + 2ULL){ //k even
            if(binary_gcd(m, k) == 1ULL){
                num_sol_prim[((k * (m + k)) << 1) - 1ULL] += 1ULL; //if they are coprime then we add one for the perimeter
            }
        }
    }

    for(uint64_t m = 2ULL; (((m + 1ULL) * ((m << 1 ) + 1ULL)) << 1) <= n; m = m + 2ULL){ //m even
        for(uint64_t k = m + 1ULL; ((k * (m + k)) << 1) <= n; k = k + 2ULL){ //k odd
            if(binary_gcd(m, k) == 1ULL){
                num_sol_prim[((k * (m + k)) << 1) - 1ULL] += 1ULL; //if they are coprime then we add one for the perimeter
            }
        }
    }

    for(uint64_t i = 1ULL; i < num_sol_prim.size(); i++){
        if(num_sol_prim[i]){
            for(uint64_t j = i; j < num_sol_prim.size(); j = j + (i + 1)){ //We iterate over the multiples of the element
                num_sol[j] += num_sol_prim[i];
            }
        }

        if(num_sol[i] == 1){
            sol++;
        }
    }


    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";


    return 0;

}