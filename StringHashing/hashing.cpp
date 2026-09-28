#include<iostream>
#include<string>
#include<random>
#include<unordered_set>
#include<map>
#include<vector>
#include<chrono>

std::string randomString(const std::string& chars){
    int len = 4 + (rand() % 5);
    std::string s;
    for (int i = 0; i < len; i++) {
        s += chars[rand() % chars.length()];
    }
    return s;
}

long long hashing(std::string s, const std::string& chars){
    int p = 67; 
    int m = 1000000;
    long long hash = 0;
    
    for(char c : s){
        int valor = chars.find(c) + 1;
        hash = (hash * p + valor) % m;
    }

    return hash;
}

void printVector(std::vector<std::string> v){
    for(const std::string& s : v){
        std::cout << s << " , ";
    }
    std::cout << std::endl;
}

void printMap(std::map<long long, std::vector<std::string>> m){
    std::cout << "Hash \t | \t Strings" << std::endl;
    std::cout << "==========================" << std::endl;
    for(const auto& element : m){
        std::cout << element.first << "\t | \t"; printVector(element.second);
    }
}

int main(){
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    srand(time(0));
    std::unordered_set<std::string> stringsGen;
    std::map<long long, std::vector<std::string>> hashed;
    int counter;
    
    while(stringsGen.size() < 200000){
        stringsGen.insert(randomString(chars));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    for(const auto& str : stringsGen){
        long long temp = hashing(str, chars);
        if(!hashed[temp].empty()){
            counter++;
        }

        hashed[temp].push_back(str);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto time =
            std::chrono::duration_cast<std::chrono::microseconds>(
                end - start
            ).count();  
    printMap(hashed);
    std::cout << "Colisiones encontradas: " << counter << std::endl;
    std::cout << "Tiempo de hashing: " << time  << " microsegundos "<< std::endl;

}