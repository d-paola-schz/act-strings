#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <algorithm>
#include <cctype>
#include<chrono>


bool soloEspacios(const std::string& s) {
    // Si la cadena está vacía o solo tiene espacios, regresa true
    return s.empty() || all_of(s.begin(), s.end(), [](char c) { return isspace(c); });
}

bool tieneEspaciosConsecutivos(const std::string& s) {
    for (int i = 1; i < s.length(); i++) {
        if (s[i] == ' ' && s[i - 1] == ' ') {
            return true;
        }
    }

    return false;
}

std::string manacher(std::string S){
    std::string T = "^|";
    
    for(char c : S){
        T += c;
        T += '|';
    }
    
    T = T + "~"; 
    
    int n = T.size();
    std::vector<int>L(n, 0);
    
    int c = 0;
    int r = 0;
    int imax = 0;

    for(int i = 1;  i < n-1; i++){
        int m = 2*c - i;

        if(i < r){
            L[i] = std::min(r - i, L[m]);
        }

        while(T[i + L[i] + 1] == T[i - L[i] - 1]){
            L[i] += 1;

            if (L[imax] < L[i]) {
                int start = (i - L[i]) / 2;

                if (!soloEspacios(S.substr(start, L[i])) && !tieneEspaciosConsecutivos(S.substr(start, L[i]))) {
                    imax = i;
                }
            }
        }

        if(i + L[i] > r){
            c = i;
            r = i + L[i];
        }
    }

    int lmax = L[imax];
    int start = (imax - lmax)/2;
    
    return S.substr(start,lmax);
}


int main(){
    std::vector<std::string> libros = {"libro1.txt", "libro2.txt", "libro3.txt", "libro4.txt", "libro5.txt"};

    for(int i = 0; i < libros.size(); i++){
        std::ifstream archivo(libros[i]);
        
        if (!archivo.is_open()) {
            std::cerr << "Error: No se pudo abrir el archivo '"<< libros[i] <<"'. Asegúrate de que esté en la carpeta del proyecto." << std::endl;
            return 1;
        }
    
        std::string textoCompleto = "";
        std::string linea;
    
        while (getline(archivo, linea)) {
            if (!textoCompleto.empty()) textoCompleto +=" ";
    
            textoCompleto += linea;
        }
        archivo.close();
    
        std::cout << "Texto leído correctamente (" << textoCompleto.length() << " caracteres)." << std::endl;
    
        auto start = std::chrono::high_resolution_clock::now();
        std::string palindromoMasLargo = manacher(textoCompleto);
        auto end = std::chrono::high_resolution_clock::now();
        auto time =
            std::chrono::duration_cast<std::chrono::microseconds>(
                end - start
            ).count();
    
        std::cout << "El palindromo mas largo encontrado del "<< libros[i] <<": ["<< palindromoMasLargo << "]" << std::endl;
        std::cout << "Longitud: " << palindromoMasLargo.length() << " caracteres." << std::endl;
        std::cout << "Tiempo de busqueda: " << time  << " microsegundos "<< std::endl << std::endl;


    }

    return 0;
}