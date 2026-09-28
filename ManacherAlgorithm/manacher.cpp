#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include <algorithm>
#include <cctype>

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
    std::ifstream archivo("libro1.txt");
    
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo 'texto.txt'. Asegúrate de que esté en la carpeta del proyecto." << std::endl;
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

    std::string palindromoMasLargo = manacher(textoCompleto);

    std::cout << "El palindromo mas largo encontrado es: ["<< palindromoMasLargo << "]" << std::endl;
    std::cout << "Longitud: " << palindromoMasLargo.length() << " caracteres." << std::endl;

    return 0;
}