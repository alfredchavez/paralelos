#include <iostream>
#include <chrono>
#include "Matrix.h"
using namespace std;

int main(){
    size_t size = 100;
    Square_Matrix<int> matrix1(size, RANDOM_INIT);
    Square_Matrix<int> matrix2(size, RANDOM_INIT);
    auto started = std::chrono::high_resolution_clock::now();
    //auto ans = matrix1.product_matrix(matrix2);
    auto ans2 = matrix1.product_block_matrix(matrix2);
    auto ended = std::chrono::high_resolution_clock::now();
    std::cout <<(double)std::chrono::duration_cast<std::chrono::microseconds>(ended - started).count()/1000.0 << std::endl;
    //ans->print_matrix();
    //cout << "-----------\n";
    //ans2->print_matrix();
	return 0;
}
