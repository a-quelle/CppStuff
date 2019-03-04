#pragma once
#include <iostream>

class LargeNumber{
    uint8_t* data = nullptr;
    size_t size = 0;
    LargeNumber();    
    static void modHelper(LargeNumber& out, const LargeNumber& mod);
    bool halve();
    void twice();
public:
    size_t getSize() const;    
    friend bool operator<(const LargeNumber& a, const LargeNumber& b);
    friend bool operator>(const LargeNumber& a, const LargeNumber& b);
    friend bool operator==(const LargeNumber& a, const LargeNumber& b);
    friend LargeNumber operator+(const LargeNumber& lhs, const LargeNumber& rhs);
    friend LargeNumber operator-(const LargeNumber& lhs, const LargeNumber& rhs);
    friend LargeNumber operator*(const LargeNumber& lhs, LargeNumber rhs);
    friend LargeNumber operator%(const LargeNumber& lhs, const LargeNumber& rhs);
    LargeNumber& operator++();
    LargeNumber& operator--();
    friend std::ostream& operator<<(std::ostream& os, const LargeNumber& n);
    const uint8_t* getData() const;    
    template<int n>
    LargeNumber(const char (&inputData)[n]);
    LargeNumber(const LargeNumber& orig);
    LargeNumber(unsigned char* inputData, size_t size);
    LargeNumber(LargeNumber&& orig);
    LargeNumber& operator=(LargeNumber& orig);
    LargeNumber& operator=(LargeNumber&& orig);
    static LargeNumber modularPower(const LargeNumber& base, LargeNumber exp, const LargeNumber& modulus);
    ~LargeNumber();
};

template<int n>
LargeNumber::LargeNumber(const char (&inputData)[n]): size(n){
    data = new unsigned char[n];
    std::copy(inputData, inputData + size, data);
}