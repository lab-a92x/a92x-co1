#include <iostream>
#include <vector>
#include <list>

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

// Safe positive integer cubic root floor
uint64_t floor_3root(uint64_t n) {
    if (n < 2) return n;
    uint64_t lo = 1;
    uint64_t hi = n;

    while (lo <= hi) {
        uint64_t mid =(hi + lo) >> 1;

        // compare mid*mid and n safely: mid*mid <= n  <=> mid <= n / mid
        if (mid <= (n / mid)/mid) {
            // mid*mid*mid <= n
            if ( (mid + 1) > (n / (mid + 1))/ (mid + 1) ) {
                return mid;
            }
            lo = mid + 1;
        } else {
            hi =  mid - 1;
        }
    }
    return lo;
}

// Safe positive integer pow 2/3 floor
uint64_t floor_pow_2_3(uint64_t n) {
    if (n < 2) return n;
    uint64_t lo = 1;
    uint64_t hi = n;

    while (lo <= hi) {
        uint64_t mid =(hi + lo) >> 1;

        // compare mid*mid and n safely: mid*mid <= n  <=> mid <= n / mid
        if (mid <= ((n*n) / mid)/mid) {
            // mid*mid*mid <= n
            if ( mid + 1 > ((n*n) / (mid + 1))/ (mid + 1) ) {
                return mid;
            }
            lo = mid + 1;
        } else {
            hi =  mid - 1;
        }
    }
    return lo;
}

int main(){
    uint64_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();

    uint64_t sol = 0ULL;

    uint64_t k2 = floor_2root(n);
    uint64_t k3 = floor_3root(n);
    uint64_t k23 = floor_pow_2_3(n);

    std::vector<uint32_t> euler_totient(k23 + 1, 0);
    HybridMap<uint64_t, uint64_t> s_phi(k2+1);
    std::list<uint32_t> primes;

    euler_totient[1] = 1ULL;
    s_phi[1] = 1;


    for (size_t i = 2; i < euler_totient.size(); ++i) {
        if(!euler_totient[i]){
            primes.push_back(i);

            euler_totient[i] = i;
        }

        for (uint32_t &p: primes) {
            if (euler_totient[i] < p || euler_totient.size() <= i * p)
                break;

            euler_totient[i * p] = p;
        }


        uint32_t p = euler_totient[i];
        if(i % (p*p)){
            euler_totient[i] = (p-1)*euler_totient[i/p];
        } else{
            euler_totient[i] = p*euler_totient[i/p];
        }

        s_phi[i] = s_phi[i-1] + euler_totient[i];
    }


    uint64_t index =  k3+1; //Index of the variable that we solve for, that is, the index such that we want to compute \Sigma_{s_f}(n/index)

    while(n/index <= k23){ //We check until we arrive to an uncomputed value for \Sigma_f and \Sigma_h
        index = n/((n/index)+1);
    }

    while(true){ //We solve by substitution the triangular system of equations iteratively

        uint64_t nr = n/index; //Current \Sigma_{phi} to compute

        uint64_t indexr_right = nr; //Right index to solve this equation iteratively for nr
        uint64_t indexr_left = nr >> 1;

        s_phi[nr] =  (nr*(nr+1)) >> 1; //We start by adding the independent term

        while(true){//we apply substitution

            s_phi[nr] -= (indexr_right - indexr_left)*s_phi[nr/indexr_right];

            indexr_right = indexr_left;
            indexr_left = nr/((nr/indexr_left)+1);
            if(indexr_right == 1)
                break;
        }

        index = n/((n/index)+1);
        if(index == 0)
            break;
    }

    sol = 3*(n * (n + 1) - (s_phi[n] << 1));



    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

