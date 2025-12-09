#include <iostream>
#include <vector>

//Computes the sum of the even elements of the Fibonacci sequence less or equal to n.
//The even numbers of the Fibonacci sequences appear in the positions that are multiples of 3.
int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t sol = 0ULL;
    if(2 <= n) {
        uint32_t fib1 = 1UL;
        uint32_t fib2 = 2UL;
        sol = 2ULL;
        while(true){
            fib2 = fib1+fib2;
            fib1 = fib2 - fib1;

            fib2 = fib1+fib2;
            fib1 = fib2 - fib1;

            fib2 = fib1+fib2;
            fib1 = fib2 - fib1;

            if(n < fib2){
                break;
            } else {
                sol = sol + fib2;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}