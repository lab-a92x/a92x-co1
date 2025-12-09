#include <iostream>
#include <vector>
#include <cstdint>



//this function computes the number of ways that n can be payed
int main(){
    u_int64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    //array that stores coins values
    uint64_t coin_list[] = {1ULL, 2ULL, 5ULL, 10ULL, 20ULL, 50ULL, 100ULL, 200ULL};




    //vector that counts the ways of adding coins to form a certain quantity
    //The i-th entry correspond to the number of ways  of adding coins to form  i
    std::vector<uint64_t> coin_count(n+1, 0ULL);

    coin_count[0ULL] = 1ULL; //One way for zero (empty sum)

    for(uint64_t c: coin_list){
        for(uint64_t j = c; j < coin_count.size(); j++){
            coin_count[j] += coin_count[j - c];
        }
    }


    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << coin_count[coin_count.size()-1ULL] << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;

}
