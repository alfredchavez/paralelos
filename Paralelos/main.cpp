#include <iostream>
#include "Matrix.h"
using namespace std;

int main(){
    size_t size = 1000;
    Square_Matrix<int> matrix1(size, RANDOM_INIT);
    Square_Matrix<int> matrix2(size, RANDOM_INIT);
    auto ans = matrix1.product_matrix(matrix2);
    auto ans2 = matrix1.product_block_matrix(matrix2);
    //ans->print_matrix();
    //cout << "-----------\n";
    //ans2->print_matrix();
	return 0;
}
