#include <iostream>
#include <string>
using namespace std;

bool isPalindrome(int n){
    string s = to_string(n);
    for(int i = 0; i < (s.size() >> 1); i++){
        if(s[i] != s[s.size()-1 - i])
            return false;
    }
    return true;
}

int main(){

    auto start = std::chrono::high_resolution_clock::now();

    int bigger_palindrome = 0;
    for(int i = 100; i < 1000;i++)
        for(int j = i; j < 1000;j++)
            bigger_palindrome = isPalindrome(i*j) ? max(bigger_palindrome,i*j) : bigger_palindrome;

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    cout << bigger_palindrome << "\n";
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}