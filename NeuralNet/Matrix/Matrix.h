#pragma once
#include <vector>

template <typename T>
struct Matrix : private std::vector<T> {
    Matrix(): std::vector<T>() {}
    Matrix(int rows, int columns) : std::vector<T>(rows*columns), numberOfRows(rows), numberOfColumns(columns) {}
    ~Matrix() {}
    T& operator() (const int x, const int y) {
        return (*this)[x*numberOfColumns + y];
    }

    T& operator[] (const int x) {
        return (*(std::vector<T>*)this)[x];
    }

    void resize(const int x, const int y){
        numberOfRows = x;
        numberOfColumns = y;
        std::vector<T>::resize(x*y);
    }
    void clear() {
        std::vector<T>::clear();
    }
    int size() {
        return std::vector<T>::size();
    }

private:
    int numberOfRows = 0;
    int numberOfColumns = 0;
};

template <typename T>
struct Tensor : private std::vector<T> {
    Tensor(): std::vector<T>() {}
    Tensor(int upper, int rows, int columns) : std::vector<T>(upper*rows*columns), 
    upper(upper), lowerRows(rows), lowerColumns(columns) {}
    ~Tensor() {}
    T& operator() (const int x, const int y, const int z) {
        return (*this)[x*lowerRows*lowerColumns + y*lowerColumns+z];
    }

    T& operator[] (const int x) {
        return (*(std::vector<T>*)this)[x];
    }

    void resize(const int x, const int y, const int z){
        upper = x;
        lowerRows = y;
        lowerColumns = z;
        std::vector<T>::resize(x*y*z);
    }
    void clear() {
        std::vector<T>::clear();
    }
    int size() {
        return std::vector<T>::size();
    }

private:
    int upper = 0;
    int lowerRows = 0;
    int lowerColumns = 0;
};