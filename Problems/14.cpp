#include <iostream>
#include <unordered_map>

uint64_t Collatz_sequence_length(uint64_t x, std::unordered_map<uint64_t, uint64_t>& map_length){ //Computes the length of the Collatz sequence
    //and saves it in the map

    if(map_length.contains(x)){ //If it was already computed, nothing to compute
        return map_length[x];
    }

    if(x & 1ULL){
        map_length[x]= 1ULL + Collatz_sequence_length(3ULL * x + 1ULL, map_length);
    } else{
        map_length[x] = 1ULL + Collatz_sequence_length(x >> 1, map_length);
    }

    return map_length[x];
}

int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::steady_clock::now();

    //This vector saves the length of the Collatz sequence of each number.
    //The i-th entry correspond to the length of the Collatz sequence of i+1
    std::unordered_map<uint64_t, uint64_t> map_length;

    map_length[1ULL] = 1ULL; //Only length one Collatz sequence

    uint64_t max_length = 1ULL;
    uint64_t max_index = 1ULL;

    for(uint64_t i = (n+1ULL) >> 1; i < n; i++){ //We start at n/2 because for every previous element k <= n/2,
        // there is a chain that starts at 2k and has length increased by one.

        if(!map_length.contains(i+1ULL)) { //If it is not already visited

            uint64_t l = Collatz_sequence_length(i + 1ULL, map_length);
            if(max_length < l) {
                max_length =  l;
                max_index = i+1ULL;
            }
        }
    }

    auto end = std::chrono::steady_clock::now();

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);


    std::cout << max_index << ": " << max_length << " \n";
    std::cout << "Elapsed time: " << ms.count() << " ms\n";

    return 0;
}