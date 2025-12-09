#include <iostream>


//This problem asks us to give the sum of the diagonals of an n x n spiral.
//The elements of the diagonal are exactly the vertex of the squares.
//Let us fix an arbitrary square in the spiral. Let l be the side of said square.
//The paths from the 1,2,3 vertexes to the 2,3,4 vertexes respectively
//are sides of the square. Therefore, it is just an increase by l-1.
//The second one is l-1 times plus the position of the first,
//the third one is l-1 times plus the position of the second and
//the fourth is l-1 times plus the position of the third.
//The only task left to do is to relate this square to the previous one.
// The last vertex of the preceding square is just as far elementwise (not order-wise)
//from the first vertex of our fixed square as the last vertex.
//We have this situation: 4 # # # ... # 1
//                        # 4'# # ... # #
//
//Therefore, the first vertex position is also l-1 plus the position of the previous last vertex.
//This gives us an algorithm. We start at the first position.
// The side of the squares increases by two every time.
//The first square is of side 1 and only has one position. We take 1 = c, the preceding last vertex.
//The second one is of side 3, so the hop should be 3-1=2.
//Therefore, the new square vertexes are c + hop, (c+hop)+hop, ((c+hop)+hop)+hop, (((c+hop)+hop)+hop)+hop.
//The sum of the four is 4*c + 10*hop and the new last vertex is c + 4*hop.
//Lets derive a general formula.
//First of all we define a sequence hop_i where hop_i is the side minus one of the i+1-th square.
//Our base case is hop_0 = 0 for the first square.
//It is easy to deduce that hop_{k} = 2*k.
//We do the same for the sequence c_{i}, where c_{i} is the last vertex of the i+1-th square.
// c_{0} = 1 and c_{i+1} = c_{i} + 4*hop_{i}.
//With this definition it is again easy to deduce that
//c_{k} = 1 + \sum_{i = 1}^{k} 4*hop_{i} = 1 + 4*k*(k+1).
//Finally, let S_{i} be the sum of the vertex from the first square to the i+1-th one.
// S_{0} = 1 and S_{i+1} = S_{i} + 4*c_{i}+10*hop_{i+1}.
// The general formula is given by
// S_{k} = 1 + \sum_{i = 1}^{k} 4*(1 + 4*i*(i-1)) + 20*i =
// 1 + 4*\sum_{i = 1}^{k} 1 + 4* \sum_{i = 1}^{k} i + 16 \sum_{i = 1}^{k} i^2 =
// 1 + 4*k + 2*k*(k+1) + 8*(k*(k+1)*(2*k+1))/3 =
// 1 + 2*k*(8*k^2 + 15*k + 13)/3 .

// The formula works for the number of squares not their side length, which we are given.
// Given the spiral side length n, to compute the number of squares (moved one unit down to start at zero)
// we only need the formula  ⌊(n-1)/2⌋.
int main(){
    uint32_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t k = (n-1) >> 1;
    uint64_t sum = 1ULL + ( (( k * ( ((k*k) << 3) + 15ULL*k + 13ULL))/3ULL)  << 1);



    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sum << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}