#include <iostream>
#include <vector>

double prob = 0.0;


void push_envelop(std::vector<uint64_t>& envelop, uint64_t i){
    switch (i) {
        case 0:
            envelop[1] = envelop[1]+1;
        case 1:
            envelop[2] = envelop[2]+1;
        case 2:
            envelop[3] = envelop[3]+1;
        case 3:
            return;
    }
}



void count(std::vector<uint64_t>& envelop,  double multiplier){
    if(!envelop[0] && !envelop[1] && !envelop[2] && envelop[3] == 1ULL){
        prob = prob + multiplier*((double)envelop[4]);
        return;
    }

    uint64_t k = envelop[0]  + envelop[1] + envelop[2]  + envelop[3];
    if(k == 1){
        envelop[4] = envelop[4]+1;
    }

    for(uint64_t i = 0; i < 3; i++){
        if(envelop[i]){
            std::vector<uint64_t> new_envelop = envelop;
            new_envelop[i] = new_envelop[i] - 1;
            push_envelop(new_envelop, i);
            count(new_envelop, ((double )envelop[i] * multiplier) / k);
        }
    }

    if(envelop[3]){
        std::vector<uint64_t> new_envelop = envelop;
        new_envelop[3] = new_envelop[3] - 1;
        count(new_envelop, ((double )envelop[3] * multiplier) / k);
    }

    return;
}



int main(){

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<uint64_t> envelop(5, 1ULL);
    envelop[4] = 0ULL;

    count(envelop, 1.0);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << prob << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
    
    return 0;
}
