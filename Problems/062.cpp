#include <iostream>
#include <unordered_map>
#include <numeric>

class cube_per {
private:
    uint32_t counter;   //counter that reflects the number of permutations of this cube encountered
    uint64_t first_value;    //first cube that yields this permutation

public:
    static uint64_t n;

    // Constructor
    explicit cube_per(uint64_t v) :  counter(1UL), first_value(v) {}
    cube_per() : counter(1UL), first_value(0ULL) {}

    //Increasing the counter
    bool counter_increase() {
        counter++;
        return n <= counter;
    }

    //Get first value
    [[nodiscard]] uint64_t get_fv() const{
        return first_value;
    }

};

uint64_t cube_per::n;

std::array<uint16_t, 10> digit_count(uint64_t n) {
    std::array<uint16_t, 10> counts{}; // all elements initialized to 0

    if (n == 0) {
        counts[0] = 1;
        return counts;
    }

    while (n > 0) {
        uint16_t digit = n % 10;
        counts[digit]++;
        n /= 10;
    }

    return counts;
}

struct ArrayHash {
    std::size_t operator()(const std::array<uint16_t ,10>& a) const noexcept {
        std::size_t h = 0;
        for (uint16_t x : a)
            h ^= std::hash<int>{}(x) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        return h;
    }
};

//Computes the least positive integer that at least n permutations of the digits of this number are cubes.
//We represent each permutation as the array representing the number of each of its digits.
//We must ignore the left zeroes because they are not considered in the problem and
// when considering left zeroes we loose the invariant of number of digits for permutations of a number,
// which is critical for finding a solution.
int main(){
    std::cin >>  cube_per::n;

    auto start = std::chrono::high_resolution_clock::now();

    if(cube_per::n <= 1){
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;


        std::cout << 1 << "\n"; //The first positive number which is also a cube

        std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
    }

    // unordered_map whose key is array with 10 elements
    std::unordered_map<std::array<uint16_t ,10>, cube_per, ArrayHash> map;

    uint64_t min_cube; //variable that will store the best candidate for solution
    for(uint32_t i = 0UL; ; i++){
        uint64_t prod3 = i;
        prod3 *= prod3 * prod3;
        std::array<uint16_t ,10> ci = digit_count(prod3);

        if(!map.contains(ci)) { //We adjoin the new element

            map[ci] = cube_per(prod3);

        } else{ //We increase the counter and check if it is a solution

            if(map[ci].counter_increase()){ //we found a candidate. The solution has its number of digits.
                //It may happen that the real minimum is discovered later

                min_cube = map[ci].get_fv(); //Actual best candidate
                uint32_t i_digits = ci[0]+ci[1]+ci[2]+ci[3]+ci[4]+ci[5]+ci[6]+ci[7]+ci[8]+ci[9];

                for(uint32_t j = i+1; ; j++){ //we iterate over the cubes that has the same number of digits
                    uint64_t j_prod3 = j;
                    j_prod3 *= j_prod3 * j_prod3;
                    std::array<uint16_t ,10> cj = digit_count(j_prod3);
                    uint32_t j_digits = cj[0]+cj[1]+cj[2]+cj[3]+cj[4]+cj[5]+cj[6]+cj[7]+cj[8]+cj[9];

                    if(i_digits < j_digits) { //we already have more digits so we already can send the solution
                        auto end = std::chrono::high_resolution_clock::now();

                        std::chrono::duration<double> elapsed = end - start;


                        std::cout << min_cube << "\n";

                        std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

                        return 0;
                    }else{ //it may provide a better solution.
                        // We do not adjoin more elements to the map because they will be worse that the current solution.
                        if(map.contains(cj) && map[cj].counter_increase() && map[cj].get_fv() < min_cube ) {
                            min_cube = map[cj].get_fv();
                        }
                    }

                }
            }

        }
    }

}