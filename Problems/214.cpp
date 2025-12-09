#include <iostream>
#include <unordered_map>
#include <vector>
#include <list>

const size_t M = 25;

//This program computes the sum of all primes strictly less than n,
// whose totient chain has length M.
//The program makes a linear sieve, computes euler totient function dynamically and
// computes the length of the totient sequence in a vector using dynamic programming.
//Nevertheless, the length of the totient sequence can be put as an additive function plus two for n > 1,
//where the additive function is the length of the chain starting from the next number that is greater than 2.

int main(){
    uint32_t n;
    std::cin >> n;

    auto start = std::chrono::high_resolution_clock::now();
    uint64_t  sol = 0ULL;
    std::vector<size_t>  totient_sequence_length(n, 0UL);
    std::vector<uint32_t> spf_sieve(n, 0UL);
    std::vector<uint32_t> totient(n, 0UL);
    std::list<uint32_t> primes;
    spf_sieve[1] = 1;
    totient[1] = 1;
    totient_sequence_length[1] = 1;

    for(size_t i = 2; i < n;i++){
        if(!spf_sieve[i]){
            spf_sieve[i] = i;
            primes.push_back(i);
            totient[i] = i-1;

            totient_sequence_length[i] = totient_sequence_length[i-1]+1;
            if(totient_sequence_length[i] == M){
                sol += i;
            }
        } else{
            if((i/spf_sieve[i]) % spf_sieve[i]){
                totient[i] = (spf_sieve[i]-1UL)*totient[i/spf_sieve[i]];
            }else{
                totient[i] = spf_sieve[i]*totient[i/spf_sieve[i]];
            }
            totient_sequence_length[i] = totient_sequence_length[totient[i]]+1;
        }

        for(uint32_t & p: primes){
            if(spf_sieve[i] < p || n <= i*p){
                break;
            }

            spf_sieve[p*i] = p;
        }


    }




    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << sol << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}