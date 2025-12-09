#include <iostream>


//An easy recursive approach tell us that num_path(h,w) = num_path(h-1,w) + num_path(h,w-1).
//It is implemented here with dynamic programming in order to do it iteratively.
//num_path(h,w) also coincides with the number of permutations of h downs and w rights,
// which is a permutation with repetition, exactly (h+w)!/(h!*w!).
uint64_t num_path(uint32_t h, uint32_t w){

    //Only the straight path when there is only one direction posible. Also when no direction is posible.
    std::vector<std::vector<uint64_t >> matriz(h+1, std::vector<uint64_t>(w+1,1ULL));

    //We iterate the matrix by diagonals. We start with the first half of the matrix.
     for(uint32_t d = 1; d < h+1; d++){
         for(uint32_t i = d-1UL; 0UL < i; i--){
             uint32_t j = d-i;
             matriz[i][j] = matriz[i-1][j]+matriz[i][j-1];
         }
     }

    //We continue iterating over the diagonals of the second half of the matrix.
    for(uint32_t d = 1; d < w+1; d++){
        for(uint32_t j = d; j < w+1; j++){
            uint32_t i = h+d-j;
            matriz[i][j] = matriz[i-1][j]+matriz[i][j-1];
        }
    }

    return matriz[h][w];
}

int main(){
     uint32_t n;
     std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t np = num_path(n,n);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

     std::cout << np << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

     return 0;
}

