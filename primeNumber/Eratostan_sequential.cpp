
//created by ahbeheshti

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

static long arraySize = 100000000;
static long primeArraySize = 100000000;

int eratostn(int maxNum , int prime[]){
	if (maxNum < 10){
		int currentFounded = 0;
		if (maxNum >= 2){
			prime[currentFounded] = 2;
			currentFounded++;
		}
		if (maxNum >= 3){
			prime[currentFounded] = 3;
			currentFounded++;
		}
		if (maxNum >= 5){
			prime[currentFounded] = 5;
			currentFounded++;
		}
		if (maxNum >= 7){
			prime[currentFounded] = 7;
			currentFounded++;
		}
		return currentFounded;
	}
	else{
		int prePrimeNum = sqrt(maxNum);
		int size = eratostn(prePrimeNum , prime);
		bool *temp = new bool[maxNum+1];

		for (int i = 0; i < maxNum+1; ++i)
			temp[i] = true;

		for (int i = 0; i < size; ++i)
		{
			int current = prime[i];
			int nextNum = (prePrimeNum/current + 1) * current;
			int start = (nextNum < current*current)? current*current : nextNum;
			for (int j = start; j <= maxNum; j+=current)
				temp[j] = false;
		}
		int currentFounded = size;
		for (int j = prePrimeNum+1; j <= maxNum; ++j)
		{
			if (temp[j] == true)
			{
				prime[currentFounded] = j;
				currentFounded++;
			}
		}
		return currentFounded;
	}
}


int main()
{
	clock_t firstSeconds ,secondSeconds;
   	firstSeconds =  clock();
	int *prime = new int[primeArraySize];
	int primeSize = eratostn(arraySize , prime);
	secondSeconds =  clock();
	printf("%d prime num founded from 1 to %d in %f seconds\n", primeSize , arraySize , ((float)secondSeconds - (float)firstSeconds)/1000.0);
	// for (int i = 0; i < primeSize; ++i)
	// {
	// 	printf("%d ", prime[i]);
	// }
	return 0;
}
