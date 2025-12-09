#include <iostream>

#define N 100

//offset cycle of Julian calendar
const uint16_t Julian_offset[] = {0,6,5,4,2,1,0,6,4,3,2,1,6,5,4,3,1,0,6,5,3,2,1,0,5,4,3,2};

//Computes the number of Sundays that are the first of a month since 01/03/1901 until 31/12/(1900+N)
// according to the julian calendar (until 2100 the gregorian and julian calendar coincide).
int main(){
    uint16_t first_day_of_month[] = {2,1,2,1,2,2,2}; //The i-th entry represents
    // the number of months from march 1900 to february 1901
    // such that the first day of the month is the (i+1)-th day of the week.
    uint16_t first_day_of_month_small[] = {2, 1,1,1,2,1,2}; //The i-th entry represents
    // the number of months from march 1900 to december 1900
    // such that the first day of the month is the (i+1)-th day of the week.

    auto start = std::chrono::steady_clock::now();

    uint32_t sol = 0UL;

    for(uint32_t i = 0UL; i < std::min(28,N-1); i++){
        sol += first_day_of_month[ Julian_offset[i % 28UL]]; //we inverse the offset to note which day is sunday this year
    }

    if(28 <= N-1){
        sol *= (N-1)/28;
        for(uint32_t i = 0UL; i <= (N-1) % 28; i++){
            sol += first_day_of_month[ Julian_offset[i]];
        }
    }

    sol += first_day_of_month_small[ Julian_offset[100UL % 28UL]];

    sol -= first_day_of_month[0];

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << " sundays \n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}