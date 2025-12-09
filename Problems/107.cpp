#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

//Union Find class to represents partitions with path compression when finding and union by rank
class UnionFind {
    std::vector<uint32_t> Parent;
    std::vector<uint32_t> Rank;

public:
    UnionFind(uint32_t n) {
        Parent.resize(n);
        Rank.resize(n, 0);

        for (uint32_t i = 0; i < n; i++) {
            Parent[i] = i;
        }
    }



    // Find the representative with path compression
    uint32_t find(uint32_t i) {
        if (Parent[i] != i)
            Parent[i] = find(Parent[i]);
        return Parent[i];
    }

    // Joins the partitions of i and j by rank
    void unionByRank(uint32_t i, uint32_t j) {
        uint32_t irep = find(i);
        uint32_t jrep = find(j);

        if (irep == jrep)
            return;

        if (Rank[irep] < Rank[jrep]) {
            Parent[irep] = jrep;
        } else if (Rank[irep] > Rank[jrep]) {
            Parent[jrep] = irep;
        } else {
            Parent[jrep] = irep;
            Rank[irep]++;
        }
    }
};


struct Edge {
    uint32_t from;
    uint32_t to;
    uint32_t weight;

    // Lexicographical order given by weight, from and to in that order
    bool operator<(const Edge& other) const noexcept {
        if (weight != other.weight)
            return weight < other.weight;
        if (from != other.from)
            return from < other.from;
        return to < other.to;
    }
};

//We apply Kruskal algorithm to a graph given by a list of its edges and its number of vertices
//in order to compute the weight of its minimum spanning tree
uint64_t Kruskal(std::list<Edge> edges, uint32_t n){
    uint64_t min_val = 0ULL;
    edges.sort();
    UnionFind p(n);
    for(auto it = edges.begin(); it != edges.end(); it++){
        if(p.find(it->from) != p.find(it->to)){
            p.unionByRank(it->from, it->to);
            min_val += it->weight;
        }
    }

    return min_val;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("../network.txt");
    std::list<Edge> edges;
    std::string line;
    uint32_t row = 0;
    uint64_t sol = 0ULL;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        uint32_t col = 0;

        while (std::getline(ss, token, ',')) {
            // erase spaces
            token.erase(0, token.find_first_not_of(" \t\r\n"));
            token.erase(token.find_last_not_of(" \t\r\n") + 1);

            if (token != "-") {
                try {
                    uint32_t weight = std::stoi(token);
                    //we only save if i < j
                    if (row < col) {
                        edges.push_back({row, col, weight});
                        sol += weight;
                    }
                } catch (...) {
                    std::cerr << "Error  when converting " << token << "\n";
                }
            }
            ++col;
        }
        ++row;
    }

    file.close();

    sol -= Kruskal(edges, row);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;


    std::cout << sol << "\n";

    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";



    return 0;
}