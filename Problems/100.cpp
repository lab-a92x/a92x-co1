#include <iostream>
#include <cstdint>

class r2{
public:
    uint64_t x;
    uint64_t y;

    r2(uint64_t x_, uint64_t y_){
        this->x = x_;
        this->y = y_;
    }

    r2 operator+(const r2& other){
        return r2(other.x + x, other.y + y);
    }

    r2 operator*(const r2& other){
        return r2(other.x*this->x + ((other.y*this->y) << 1), other.x*this->y + other.y*this->x);
    }

    bool is_great_enough(uint64_t n){ //This function determines whether
        // when undoing the change of variables m is greater or equal to n.
        return n <= ((x + 1) >> 1);
    }
};


int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    //The problem asks about the least positive integer m greater or equal to n
    // such that there exists a positive integer k that satisfies (k*(k-1))/(m*(m-1)) = 1/2.
    //This last equation can be rewritten as the diophantine equation
    //(2*m-1)^2-2*(2k-1)^2 = -1.
    //With the integer change of variables x = 2*m-1, y = 2*k - 1, we get
    //a negative Pell equation, x^2 - 2*y^2 = -1.

    //The only positive integer solutions of that particular Pell equation are given by
    // the coordinates in base {1, √2} of the odd powers of (1+√2).

    r2 ini = r2(1ULL, 1ULL)*r2(1ULL, 1ULL); //we initialize this to power 2
    // in order to iterate over the odds powers.

    r2 pow = r2(1ULL, 1ULL); //Initial odd power.

    while(true){
        if(pow.is_great_enough(n)){ //This boolean value determines whether
            // when undoing the change of variables m is greater or equal to n.

            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed = end - start;

            std::cout << ((pow.x + 1) >> 1) << " disks: " << ((pow.y + 1) >> 1)
                      << " blue disks and " << ((pow.x + 1) >> 1) - ((pow.y + 1) >> 1)
                      << " red disks\n";
            std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

            break;
        }
        pow = pow*ini; //Next odd power.
    }

}



