#include <stdio.h>
#include <cstdlib>
#include <iostream>
using namespace std;
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(){
	long long int number_of_tosses = 1000000;//numero muy alto
	long long int number_in_circle = 0;
	#pragma omp parallel for reduction(+:number_in_circle)
	for (long long int toss = 0; toss < number_of_tosses; toss++) {
		double x = fRand((double)-1,(double)1);
		double y = fRand((double)-1,(double)1);
		//double x = random double between -1 and 1;
		//double y = random double between -1 and 1;
		double distance_squared = x*x + y*y;
		if (distance_squared <= 1) number_in_circle++;
	}
	double pi_estimate = 4*number_in_circle/((double)number_of_tosses);	
	cout<<"pi: "<<pi_estimate<<endl;
}
