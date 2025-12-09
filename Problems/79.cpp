#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define LIMIT 1000000000000ULL

bool satisfies(const std::vector<bool>& orden, uint64_t n){
    std::string s = std::to_string(n);
    std::vector<int> vec_max(10,-1);
    std::vector<int> vec_min(10,-1);
    for(int i = 0; i < s.size(); i++){
        vec_max[s[i]-'0'] = i;
        if(vec_min[s[i]-'0'] < 0){
            vec_min[s[i]-'0'] = i;
        }
    }

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(orden[i*10+j]){
                if(-1 == vec_min[i] || -1 == vec_min[j] || vec_min[j] >= vec_max[i]){ //trata bien el tema de caracteres repetidos
                    return false;
                }
            }
        }
    }

    return true;
}

//Outputs the smaller positive integer that satisfies all the condition imposed by the login file
int main(){
    std::ifstream archivo("../login.txt");

    if (!archivo) {
        std::cerr << "Error opening login.txt\n";
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<bool> order(100, false);

    std::string linea;
    while (std::getline(archivo, linea)) {
        int a1 = linea[0]-'0';
        int a2 = linea[1]-'0';
        int a3 = linea[2]-'0';
        order[10 * a2 + a1] = true;
        order[10 * a3 + a1] = true;
        order[10 * a3 + a2] = true;
    }

    for(uint64_t i = 1000000ULL; i < LIMIT; i++){
        if(satisfies(order, i)){

            auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> elapsed = end - start;

            std::cout << i << "\n";
            std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
            break;
        }
    }

    return 0;
}