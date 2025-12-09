#include <iostream>
#include <list>

const uint64_t MOD = 1000000000;

template <typename Key, typename Value>
class HybridMap {
private:
    size_t m_;  // threshold for vector
    std::vector<Value> vec_;
    std::unordered_map<Key, Value> map_;

public:
    // Constructor: m is the vector size
    explicit HybridMap(size_t m) : m_(m), vec_(m) {}

    // Access / insert element
    Value& operator[](Key key) {
        if constexpr (std::is_integral<Key>::value) {
            if (key < m_) {
                return vec_[key];
            }
        }
        return map_[key];
    }

    // Const access
    const Value& operator[](Key key) const {
        if constexpr (std::is_integral<Key>::value) {
            if (key < m_) {
                return vec_[key];
            }
        }
        auto it = map_.find(key);
        if (it == map_.end()) {
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }

    // Check if key exists
    bool contains(const Key& key) const {
        if constexpr (std::is_integral<Key>::value) {
            if (key < m_) return true;  // always in vector
        }
        return map_.contains(key);
    }

    // Erase key
    void erase(const Key& key) {
        if constexpr (std::is_integral<Key>::value) {
            if (key < m_) {
                throw std::runtime_error("Cannot erase vector element");
            }
        }
        map_.erase(key);
    }

    // Reserve space for the hash map
    void reserve(size_t n) { map_.reserve(n); }

    // Size (vector + map)
    size_t size() const { return m_ + map_.size(); }

    // Iteration over all elements
    template <typename Func>
    void for_each(Func&& f) const {
        for (size_t i = 0; i < vec_.size(); ++i)
            f(i, vec_[i]);
        for (const auto& p : map_)
            f(p.first, p.second);
    }
};

// Safe positive integer sqrt floor
uint64_t floor_2root(uint64_t n) {
    if (n < 2) return n;
    uint64_t lo = 1;
    uint64_t hi = n;

    while (lo <= hi) {
        uint64_t mid =(hi + lo) >> 1;

        // compare mid*mid and n safely: mid*mid <= n  <=> mid <= n / mid
        if (mid <= n / mid) {
            // mid*mid <= n
            if ( (mid + 1) > n / (mid + 1) ) {
                return mid;
            }
            lo = mid + 1;
        } else {
            hi =  mid - 1;
        }
    }
    return lo;
}


uint64_t compute_sum_sigma(const uint64_t & n){

    uint64_t k = floor_2root(n);
    uint64_t sol = 0ULL;



    for(size_t m = 1; m <= k; m++){
        uint64_t sum = ((m % MOD)*((n/m) % MOD)) % MOD;
        if(n/m & 1) {
            sum = (sum + ((((n / m) % MOD) * (((1 + n/m) >> 1) % MOD)) % MOD)) % MOD;
        } else{
            sum = (sum + (((((n / m) >> 1) % MOD) * ((1 + n/m) % MOD)) % MOD)) % MOD;
        }
        sol = (sol + sum) % MOD;
    }

    {
        uint64_t sum = ( k * (((k*(k+1)) >> 1) % MOD)) % MOD;
        sum = MOD-sum;
        sol = (sol + sum) % MOD;
    }

    return sol;
}

//We are asked to compute S(n) = \sum_{i = 1}^{n} \sum_{j = 1}^{n} σ(i j).
// Using Dirichlet hyperbola method, we can compute efficiently in O(√n) \sum_{i = 1}^{n} \sum_{j = 1}^{n} σ(i) σ(j) = (\sum_{i = 1}^{n} σ(i) )^{2}.
// This is very similar to S(n). To relate both formulas we only need the following identity
// σ(i) σ(j)  = \sum{d | gcd(i,j)} d σ(ij/d^{2}).

//Therefore,
// (\sum_{i = 1}^{n} σ(i) )^{2} = \sum_{i = 1}^{n} \sum_{j = 1}^{n} σ(i) σ(j) =
// \sum_{i = 1}^{n} \sum_{j = 1}^{n} \sum{d | gcd(i,j)} d σ(ij/d^{2}) =
// \sum_{d = 1}{n} d \sum_{i ∈ {1,...,n}, d | i} \sum_{j ∈ {1,...,n}, d | j}  σ(ij/d^{2}) =
//  \sum_{d = 1}{n} d \sum_{i' = 1}^{⌊n/d⌋} \sum_{j' = 1}^{⌊n/d⌋}  σ(i' j')  =
//  \sum_{d = 1}{n} d S(⌊n/d⌋).

//This gives us the recursive formula S(n) = (\sum_{i = 1}^{n} σ(i) )^{2} -  \sum_{d = 2}{n} d S(⌊n/d⌋).
//At first it may seem that the formula induces a lot of recursion but we only need to compute the values already in the first equation
//because ⌊⌊n/d⌋/k⌋ = ⌊n/(kd)⌋.

//Moreover, there are at most 2√n values of ⌊n/d⌋, and we can iterate those values efficiently,
//using the fact that for given k, ⌊n/x⌋ = k if and only if ⌊n/(k+1)⌋+1 <= x <= ⌊n/k⌋.
//We can form quickly an interval partition of {2,...,n}, iterate over it and for each interval
// (a,b) of said partition where ⌊n/a⌋ = ⌊n/b⌋ = k ,
// compute \sum_{i = a}^{b} i S(k) = (b*(b+1)/2 - (a-1)*a/2) S(K).

//Therefore, we have a triangular linear system of at most 2√n equations and variables
// for solving S, namely n/n = 1, n/(n/2) = 2, ...., n/1 = n,
//which can be solved by computing the prefix sum of σ and applying substitution.
//The cost of solving the ⌊n/d⌋ equation is in O(√⌊n/d⌋) since
// computing the prefix sum of σ  using Dirichlet hyperbola belongs to that cost and
// there are at most 2√⌊n/d⌋ variables already computed to substitute.
//In conclusion, the total cost lies in O(√(n/n) + √(n/n-1) + ... + √(n/1)) \subseteq O(n^{3/4}).

//We store the solutions of the system addressing by ⌊n/d⌋.
//Since the values of ⌊n/d⌋ are clustered in the interval {1,...,√n} for √n <= d, and
// for d < √n are isolated, we use a vector for the first case and a hash map for the second case.
int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();
    uint64_t  sol = 0ULL;

    uint64_t k = floor_2root(n);

    HybridMap<uint64_t, uint32_t> S(k);

    S[1] = 1UL;

    uint64_t index = n >> 1;
    while(1 <= index){
        uint64_t nr = n/index; //Solve S(nr) iteratively
        uint64_t indexr_left = (nr >> 1) + 1ULL;
        uint64_t indexr_right = nr;
        S[nr] = 0ULL;

        while(true){

            uint64_t sum = 0ULL;
            if(indexr_left & 1){
                sum = ((indexr_left % MOD)*(((indexr_left-1) >> 1) % MOD)) %  MOD;
            } else{
                sum = (((indexr_left >> 1) % MOD)*((indexr_left-1) % MOD)) %  MOD;
            }

            sum = MOD - sum;
            if(indexr_right & 1){
                sum = (sum +  (((indexr_right % MOD)*(((indexr_right+1) >> 1) % MOD)) %  MOD)) % MOD;
            } else{
                sum = (sum +  ((((indexr_right >> 1) % MOD)*((indexr_right+1) % MOD)) %  MOD)) % MOD;
            }

            sum = (sum * S[nr/indexr_right]) % MOD;

            S[nr] = (S[nr] + sum) % MOD;

            indexr_right = indexr_left - 1;
            indexr_left = nr/((nr/indexr_right)+1) + 1;
            if(indexr_right == 1)
                break;
        }

        uint64_t ss = compute_sum_sigma(nr);
        ss = (ss*ss) % MOD;

        S[nr] = (ss + (MOD - S[nr])) % MOD;

        index = n/(nr+1);
    }


    sol = S[n];

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

//100000000000

