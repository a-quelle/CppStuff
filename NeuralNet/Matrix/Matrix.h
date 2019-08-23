#pragma once

template <typename T>
struct Matrix{
    Matrix(){}
    Matrix(int rows, int columns): rows(rows), cols(columns) {
        data = new T[rows*columns]();
    }
    Matrix(Matrix& other){
        rows = other.rows;
        cols = other.cols;
        data = new T[rows*cols];
        memcpy(data, other.data, rows*cols*sizeof(T));
    }
    Matrix(Matrix&& other){
        rows = other.rows;
        cols = other.cols;
        data = other.data;
        other.data = nullptr;
    }
    ~Matrix() {
        delete[] data;
    }

    T& operator() (const int x, const int y) {
        return data[x*cols + y];
    }

    T& operator[] (const int x) {
        return data[x];
    }

    const T* getData()
    {
        return data;
    }

    void resize(const int x, const int y){
        rows = x;
        cols = y;
        delete[] data;
        data = new T[x*y]();
    }

    int size() {
        return rows*cols;
    }

private:
    T* data = nullptr;
    int rows = 0;
    int cols = 0;
};

template <typename T>
struct Tensor{
    Tensor(){}
    Tensor(int upper, int rows, int columns): upper(upper), lowerRows(rows), lowerCols(columns) {
        data = new T[upper*rows*columns]();
    }    
    Tensor(Tensor& other){
        upper = other.upper;
        lowerRows = other.lowerRows;
        lowerCols = other.lowerCols;
        data = new T[upper*lowerRows*lowerCols];
        memcpy(data, other.data, upper*lowerRows*lowerCols*sizeof(T));
    }
    Tensor(Tensor&& other){
        upper = other.upper;
        lowerRows = other.lowerRows;
        lowerCols = other.lowerCols;
        data = other.data;
        other.data = nullptr;
    }
    ~Tensor() {
        delete[] data;
    }
    T& operator() (const int x, const int y, const int z) {
        return data[x*lowerRows*lowerCols + y*lowerCols+z];
    }

    T& operator[] (const int x) {
        return data[x];
    }

    const T* getData()
    {
        return data;
    }

    void resize(const int x, const int y, const int z){
        upper = x;
        lowerRows = y;
        lowerCols = z;
        delete[] data;
        data = new T[x*y*z]();
    }

    int size() {
        return upper*lowerRows*lowerCols;
    }

private:
    T* data = nullptr;
    int upper = 0;
    int lowerRows = 0;
    int lowerCols = 0;
};