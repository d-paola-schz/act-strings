#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
using namespace std;
using namespace chrono;

vector<int> zFunction(const string& s) {
    int n = s.size();
    vector<int> z(n, 0);
    int l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (i < r) {
            z[i] = min(r - i, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}

vector<int> searchZ(const string& text, const string& pattern) {
    vector<int> result;
    string s = pattern + "$" + text;
    vector<int> z = zFunction(s);
    int m = pattern.size();
    for (int i = m + 1; i < (int)s.size(); i++) {
        if (z[i] == m) {
            result.push_back(i - m - 1);
        }
    }
    return result;
}

vector<int> prefixFunction(const string& p) {
    int m = p.size();
    vector<int> pi(m, 0);
    int k = 0;
    for (int i = 1; i < m; i++) {
        while (k > 0 && p[i] != p[k]) {
            k = pi[k - 1];
        }
        if (p[i] == p[k]) {
            k++;
        }
        pi[i] = k;
    }
    return pi;
}


vector<int> searchKMP(const string& text, const string& pattern) {
    vector<int> result;
    vector<int> pi = prefixFunction(pattern);
    int m = pattern.size();
    int k = 0; 
    for (int i = 0; i < (int)text.size(); i++) {
        while (k > 0 && text[i] != pattern[k]) {
            k = pi[k - 1];
        }
        if (text[i] == pattern[k]) {
            k++;
        }
        if (k == m) {
            result.push_back(i - m + 1);
            k = pi[k - 1];
        }
    }
    return result;
}

string toLower(string s) {
    for (char& c : s) {
        c = tolower((unsigned char)c);
    }
    return s;
}


void showPhrase(const string& original, int pos) {
    string phrase = original.substr(pos, 50);
    for (char& c : phrase) {
        if (c == '\n' || c == '\r') c = ' ';
    }
    cout << "   pos " << pos << ": \"" << phrase << "\"" << endl;
}

void runAll(const string& original, const string& lowerText, const vector<string>& words, bool useZ) {
    cout << "========== " << (useZ ? "Z FUNCTION" : "KMP") << " ==========" << endl;
    for (const string& w : words) {
        auto start = high_resolution_clock::now();
        vector<int> found = useZ ? searchZ(lowerText, w) : searchKMP(lowerText, w);
        auto end = high_resolution_clock::now();
        double ms = duration<double, milli>(end - start).count();

        cout << "\nWord: \"" << w << "\" -> " << found.size() << " occurrences" << endl;
        cout << "Time: " << ms << " ms" << endl;
        
        for (int i = 0; i < (int)found.size() && i < 5; i++) {
            showPhrase(original, found[i]);
        }
        if (found.size() > 5) {
            cout << "   ... and " << found.size() - 5 << " more" << endl;
        }
    }
    cout << endl;
}

int main() {
    ifstream file("libro1.txt");
    if (!file) {
        cout << "Could not open libro1.txt (run the program from this folder)" << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string original = buffer.str();
    string lowerText = toLower(original);

    vector<string> words = {"arthur", "knight", "sword", "castle", "queen",
                            "battle", "lancelot", "merlin", "excalibur", "round table"};

    runAll(original, lowerText, words, true);  
    runAll(original, lowerText, words, false); 
    return 0;
}
