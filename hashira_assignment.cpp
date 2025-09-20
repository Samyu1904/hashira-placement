#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include "json.hpp"  // Download from https://github.com/nlohmann/json and include in project

using json = nlohmann::json;
using namespace std;

// Convert base-N string to long long
long long parseBase(const string& value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else throw invalid_argument("Invalid char in value");
        result = result * base + digit;
    }
    return result;
}

// Lagrange interpolation at x=0
long long lagrangeConstant(const vector<int>& x, const vector<long long>& y) {
    double result = 0.0;
    int n = x.size();
    for (int i = 0; i < n; ++i) {
        double term = static_cast<double>(y[i]);
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                term *= (0.0 - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    return llround(result);
}

int main() {
    ifstream file("input.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open input.json" << endl;
        return 1;
    }

    json j;
    file >> j;  // Parse JSON

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<int> xs;            // x-values (root keys)
    vector<long long> ys;      // decoded y-values (converted from base)

    // Collect first k roots to solve polynomial of degree k-1
    // roots keys might not be continuous, so we use iteration
    int count = 0;
    for (auto& element : j.items()) {
        string key = element.key();
        if (key == "keys") continue;  // skip keys object

        if (count >= k) break; // take minimum k roots

        int x_val = stoi(key); // root key as integer
        string base_str = element.value()["base"];
        string val_str = element.value()["value"];
        int base = stoi(base_str);
        long long val_dec = parseBase(val_str, base);

        xs.push_back(x_val);
        ys.push_back(val_dec);

        count++;
    }

    long long c = lagrangeConstant(xs, ys);

    cout << c << endl;  // print just the constant term as required

    return 0;
}
