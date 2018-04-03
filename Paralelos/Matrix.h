//
// Created by achavez on 4/3/18.
//

#ifndef PARALELOS_MATRIX_H
#define PARALELOS_MATRIX_H

#include <cstdlib>
#include <iostream>
#include <random>
#define ZERO_INIT 0
#define RANDOM_INIT 1
using namespace std;

template<class T>
class Square_Matrix{
public:
    T **matrix;
    size_t size_row{};

    explicit Square_Matrix(size_t size_row, int flag){
        if(flag == 0) {
            this->size_row = size_row;
            matrix = new T *[this->size_row];
            for (size_t i = 0; i < this->size_row; ++i) {
                matrix[i] = new T[this->size_row];
                for (size_t j = 0; j < size_row; ++j) {
                    matrix[i][j] = 0;
                }
            }
        }
        else if(flag == 1){
            this->size_row = size_row;
            matrix = new T *[this->size_row];
            for (size_t i = 0; i < this->size_row; ++i) {
                matrix[i] = new T[this->size_row];
                fill_random(matrix[i], this->size_row);
            }
        }
    }

    void fill_random(T* matrix_row, size_t len){
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(10, 100);
        for(size_t i = 0; i < len; ++i){
            matrix_row[i] = dist(mt);
        }
    }

    Square_Matrix<T>* product_matrix(const Square_Matrix<T>& sm2){
        if(sm2.size_row != this->size_row){
            cerr << "Matrix must be same size\n";
            return NULL;
        }
        int i, j, k;
        auto ans = new Square_Matrix<T>(size_row, ZERO_INIT);
        for(i=0; i< this->size_row; i++){
            for(j=0; j< this->size_row; j++){
                ans->matrix[i][j]=0;
                for(k=0; k< this->size_row; k++){
                    ans->matrix[i][j] += this->matrix[i][k] * sm2.matrix[k][j];
                }
            }
        }
        return ans;
    }
    Square_Matrix<T>* product_block_matrix(const Square_Matrix<T>& sm2){
        int block_size = 16;
        int temp;
        if(sm2.size_row != this->size_row){
            cerr << "Matrix must be same size\n";
            return NULL;
        }
        auto ans = new Square_Matrix<T>(size_row, ZERO_INIT);
        cout << "Post init" << endl;
        for(int jj=0;jj< this->size_row;jj+= block_size){
            for(int kk=0;kk< this->size_row;kk+= block_size){
                for(int i=0;i< this->size_row;i++){
                    for(int j = jj; j<((jj+block_size)>this->size_row?this->size_row:(jj+block_size)); j++){
                        temp = 0;
                        for(int k = kk; k<((kk+block_size)>this->size_row?this->size_row:(kk+block_size)); k++){
                            temp += this->matrix[i][k]*sm2.matrix[k][j];
                        }
                        ans->matrix[i][j] += temp;
                    }
                }
            }
        }
        return ans;
    }
    void print_matrix(){
        for(size_t i = 0; i < this->size_row; ++i){
            for(size_t j = 0; j < this->size_row; ++j){
                cout << this->matrix[i][j] << ' ';
            }
            cout << '\n';
        }
    }
};

#endif //PARALELOS_MATRIX_H
