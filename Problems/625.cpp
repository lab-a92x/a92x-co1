#include <iostream>
#include <list>

const uint64_t MOD = 998244353;

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

// Fast  exponentiation
uint64_t pow_int(uint64_t a, uint16_t e) {
    uint64_t res = 1;
    while (e) {
        if (e & 1) {
            res = (uint64_t)((__uint128_t)res * (__uint128_t)a);
        }
        a = (uint64_t)((__uint128_t)a * (__uint128_t)a);
        e >>= 1;
    }
    return res;
}

uint64_t floor_pow_2_3(uint64_t n){
    uint64_t begin = 0ULL;
    uint64_t end = n;

    uint64_t mid = n >> 18;

    if(n*n < mid * mid * mid){
        end = mid;
    }

    while(begin < end){
        mid = (begin + end) >> 1;

        if(mid*mid*mid < n*n ){
            if(n*n < (mid+1ULL)*(mid+1ULL)*(mid+1ULL)){
                return mid;
            } else {
                begin = mid + 1ULL;
            }
        } else if(n*n < mid * mid * mid){
            end = mid - 1ULL;
        } else{
            return mid;
        }
    }

    return begin;
}

uint64_t floor_3root(uint64_t n){
    uint64_t begin = 0ULL;
    uint64_t end = n;

    uint64_t mid = n >> 18;

    if(n < mid * mid * mid){
        end = mid;
    }

    while(begin < end){
        mid = (begin + end) >> 1;

        if(mid*mid*mid < n ){
            if(n < (mid+1ULL)*(mid+1ULL)*(mid+1ULL)){
                return mid;
            } else {
                begin = mid + 1ULL;
            }
        } else if(n < mid * mid * mid){
            end = mid - 1ULL;
        } else{
            return mid;
        }
    }

    return begin;
}

uint64_t floor_2root(uint64_t n){
    uint64_t begin = 0ULL;
    uint64_t end = n;

    uint64_t mid = n >> 18;

    if(n < mid * mid){
        end = mid;
    }

    while(begin < end){
        mid = (begin + end) >> 1;

        if(mid*mid < n){
            if( n < (mid+1ULL)*(mid+1ULL)){
                return mid;
            } else {
                begin = mid + 1ULL;
            }
        } else if(n < mid*mid){
            end = mid - 1ULL;
        } else{
            return mid;
        }
    }

    return begin;
}

// It can be proved without much hardship that
// \sum_{i = 1}^{n} \sum_{j = 1}^{n} gcd(i,j) = \sum_{i = 1}{n} (\id*\varphi)(i)
// where * is the Dirichlet convolution, \id is the identity function and \varphi is Euler totient function.
//Let f = \id*\varphi, since \varphi = \mu * \id, \id * \id = I * f, where \mu is the Möbius function and I is the function identically 1.
// In order to compute  \Sigma_{f}(n) = \sum_{i = 1}^{n} f(i), we derive the equation
//
//   \Sigma_{f}(n) = \Sigma_{\id*\id}(n) - \sum_{d = 2}^{n} \Sigma_{f} (⌊n/d⌋)

//At first it may seem that the formula is recursive but we only need to compute the values already in the first equation
//because ⌊⌊n/d⌋/k⌋ = ⌊n/(kd)⌋.
//Moreover, there 
// Using Dirichlet hyperbola method, we can compute \Sigma_{\id*\id}(k) = \sum_{i = 1}^{k} \id*\id(i) in O(√k).
int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();
    uint64_t  sol = 0ULL;

    uint64_t m = floor_pow_2_3(n);
    std::vector<uint32_t> spf_sieve(m, 0UL);
    std::vector<uint16_t> sieve_exp(m, 1UL);
    std::list<uint32_t> primes;
    spf_sieve[1] = 1;

    std::vector<uint32_t> f(m, 0UL);
    HybridMap<uint64_t, uint32_t> s_f(m);

    f[1] = 1UL;
    s_f[1] = 1UL;

    for(size_t i = 2; i < m; i++){
        if(!spf_sieve[i]){
            spf_sieve[i] = i;
            primes.push_back(i);
        }

        for(uint32_t & p: primes){
            if(spf_sieve[i] <= p || spf_sieve.size() <= i*p){
                break;
            }

            spf_sieve[p*i] = p;
        }

        if(spf_sieve[i] * i < spf_sieve.size() ) {
            spf_sieve[spf_sieve[i] * i] = spf_sieve[i];
            sieve_exp[spf_sieve[i] * i] = sieve_exp[i] + 1;
        }

        uint32_t p = spf_sieve[i];
        uint16_t k =  sieve_exp[i];
        uint32_t p_power = pow_int(p, k-1);
        f[i] = p_power % MOD;

        f[i] = ( (uint64_t ) f[i] * f[i / (p_power * p)]) % MOD;
        f[i] = ( (uint64_t ) f[i] * (((k + 1ULL) * p - k) % MOD)) % MOD;

        s_f[i] = (s_f[i-1] + f[i]) % MOD;
    }

    f.clear();
    spf_sieve.clear();
    sieve_exp.clear();

    uint64_t index =  floor_3root(n); //Index of the variable that we solve for, that is, the index such that we want to compute \Sigma_{s_f}(n/index)

    while(s_f.contains(n / index)){ //We check until we arrive to an uncomputed value for \Sigma_f and \Sigma_h
        index = n/((n/index)+1);
    }

    while(true){ //We solve by substitution the triangular system of equations iteratively

        uint64_t nr = n/index; //Current \Sigma_{s_f} to compute

        uint64_t indexr = nr; //Index to solve this equation iteratively for nr

        uint32_t s_h = 0ULL;

        uint64_t nrr = floor_2root(nr); //We compute \Sigma_{s_h}(nr) using Dirichlet hyperbola trick

        for(size_t j = 1; j <= nrr; j++){
            uint64_t fac = (j*(nr/j)) % MOD;
            fac = (fac*(1 + nr/j)) %MOD;
            s_h = ( (uint64_t ) s_h + fac ) % MOD;
        }

        {
            uint64_t fac = ((nrr*(nrr + 1))>> 1) % MOD;
            fac = (fac*fac) % MOD;
            fac = (MOD - fac);
            s_h =  ( (uint64_t) s_h + fac ) % MOD;
        }

        s_f[nr] = s_h; //We start by adding the independent term
        while(true){

            uint64_t fac = (MOD - s_f[nr / indexr]) % MOD; //fac is the addition of the term \Sigma_{s_f}(nr/indexr)
            fac = (fac*(((nr/(nr/indexr)) - (nr/(1+ (nr/indexr)))) % MOD)) %MOD;

            s_f[nr] = ( (uint64_t) s_f[nr] + fac ) % MOD;

            indexr = nr/((nr/indexr)+1);
            if(indexr == 1)
                break;
        }



        index = n/((n/index)+1);
        if(index == 0)
            break;
    }



    sol = s_f[n];

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}


//100000000000