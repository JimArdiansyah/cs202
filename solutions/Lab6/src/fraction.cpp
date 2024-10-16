#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include "fraction.hpp"

using namespace std;

// Function to simplify numerator and denominator by canceling common factors
void Simplify(multiset<int>& numerator, multiset<int>& denominator) {
    for (auto it = numerator.begin(); it != numerator.end(); ) {
        auto found = denominator.find(*it);
        if (found != denominator.end()) {
            it = numerator.erase(it);
            denominator.erase(found);
        } else {
            ++it;
        }
    }
}

void Fraction::Clear() {
    numerator.clear();
    denominator.clear();
}

bool Fraction::Multiply_Number(int n) {
    if (n <= 0) return false;
    numerator.insert(n);
    Simplify(numerator, denominator);
    return true;
}

bool Fraction::Divide_Number(int n) {
    if (n <= 0) return false;
    denominator.insert(n);
    Simplify(numerator, denominator);
    return true;
}

bool Fraction::Multiply_Factorial(int n) {
    if (n < 0) return false;
    for (int i = 2; i <= n; ++i) {
        numerator.insert(i);
    }
    Simplify(numerator, denominator);
    return true;
}

bool Fraction::Divide_Factorial(int n) {
    if (n < 0) return false;
    for (int i = 2; i <= n; ++i) {
        denominator.insert(i);
    }
    Simplify(numerator, denominator);
    return true;
}

bool Fraction::Multiply_Binom(int n, int k) {
    if (n < 0 || k < 0 || k > n) return false;
    for (int i = 1; i <= k; ++i) {
        numerator.insert(n - i + 1);
        denominator.insert(i);
    }
    Simplify(numerator, denominator);
    return true;
}

bool Fraction::Divide_Binom(int n, int k) {
    if (n < 0 || k < 0 || k > n) return false;
    for (int i = 1; i <= k; ++i) {
        denominator.insert(n - i + 1);
        numerator.insert(i);
    }
    Simplify(numerator, denominator);
    return true;
}

void Fraction::Invert() {
    swap(numerator, denominator);
}

void Fraction::Print() const {
    if (numerator.empty()) {
        cout << "1";
    } else {
        for (auto it = numerator.begin(); it != numerator.end(); ++it) {
            if (it != numerator.begin()) {
                cout << " * ";
            }
            cout << *it;
        }
    }

    if (!denominator.empty()) {
        cout << " / ";
        for (auto it = denominator.begin(); it != denominator.end(); ++it) {
            if (it != denominator.begin()) {
                cout << " * ";
            }
            cout << *it;
        }
    }
    cout << endl;
}

double Fraction::Calculate_Product() const {
    double num_product = 1.0;
    double denom_product = 1.0;

    for (const auto& num : numerator) {
        num_product *= num;
    }
    for (const auto& denom : denominator) {
        denom_product *= denom;
    }
    return num_product / denom_product;
}