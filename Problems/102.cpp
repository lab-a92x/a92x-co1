#include <iostream>
#include <fstream>
#include <sstream>

class Point{
    int32_t x;
    int32_t y;

public:
    Point(int32_t x_, int32_t y_): x(x_), y(y_){}

    int32_t det(const Point & p, const Point & q){
        return (p.x - this->x)*(q.y - this->y) - (p.y - this->y)*(q.x - this->x);
    }

    int32_t lambda1(const Point & p){
        return this->y * p.x - this->x * p.y;
    }

    int32_t lambda2(const Point & p){
        return this->x * p.y - this->y * p.x ;
    }

};

//The interior points of the triangle A,B,C are an affine combination of said points.
//The problem ask us how many triangles contains the origin.
//In order to determine if the triangle A,B,C  contains the origin, we see if the origin is an affine combination of those points.
// Having an affine combination is equivalent to having a solution for this system
// (0,0) = λ1 (a1, a2) + λ2 (b1, b2) + (1- λ1 - λ2) (c1, c2), such that 0 ≤ λ1, λ2 and λ1 + λ2 ≤ 1.
// The first equation is equivalent to
// (-c1,-c2) = λ1 (a1 - c1, a2 - c2) + λ2 (b1 - c1, b2 - c2), which can be rewritten as
//      |-c1|     | a1 - c1  ,  b1 - c1 | | λ1 |
//      |   | =   |                     | |    |
//      |-c2|     | a2 - c2  ,  b2 - c2 | | λ2 |

//We denote that 2x2 matrix as M. M has zero determinant if and only if the three points are collinear.
//That is, the matrix is invertible (or equivalently has non-zero determinant) if and only if the three points form a proper triangle, which can be supposed.
//The only solution to the previous system is given by
//      | λ1 |                 | c2 b1 - c1 b2 |          | -c1 |
//      |    | =  (1/det(M))   |               | = M^{-1} |     |
//      | λ2 |                 | c1 a2 - c2 a1 |          | -c2 |

// It only remains to check if  0 ≤ λ1, λ2 and λ1 + λ2 ≤ 1, which can be done easily with the values of det(M), c2 b1 - c1 b2 and  c1 a2 - c2 a1.
// It is important to check the inequalities without dividing by det(M). We multiply both ends of the inequality in order to not do so.
//Therefore, we must take care of the sign of det(M) in order for the new inequalities to remain equivalent.
int main(){
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("../triangles.txt");

    int32_t x1, y1, x2,y2, x3,y3;

    uint64_t sol = 0ULL;

    while(file >> x1 >> y1 >> x2 >> y2 >> x3 >> y3){ //I changed the original commas of the txt for white spaces for easier parsing
        Point A(x1, y1);
        Point B(x2, y2);
        Point C(x3, y3);

        int32_t m = C.det(A,B);
        int32_t l1 = C.lambda1(B);
        int32_t l2 = C.lambda2(A);

        sol += ((0L < m && 0L <= l1 && 0L <= l2 && l1+l2 <= m) || (m < 0L && l1 <= 0L && l2 <= 0L && m <= l1+l2));
    }

    file.close();



    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;


    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    return 0;
}