
//created by ahbeheshti

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define BUF_SIZE 255

long arraySize ;
long primeArraySize ;
int processNum ;

typedef struct MyData {
    int *prime;
    int prePrimeNum;
    int maxNum;
    bool *temp; 
    int size;
    int threadCreated;
    int threadId;
} MYDATA, *PDATA;

void work(int *prime, int prePrimeNum,int maxNum , bool *temp , int size , int threadCreated , int threadId){
	for (int i = threadId; i < size; i += threadCreated)
	{
		int current = prime[i];
		int nextNum = (prePrimeNum/current + 1) * current;
		int start = (nextNum < current*current)? current*current : nextNum;
		for (int j = start; j <= maxNum; j+=current)
			temp[j] = false;
	}
}

void processFunction( PDATA pDataArray ) 
{ 
    work(pDataArray->prime , pDataArray->prePrimeNum, pDataArray->maxNum , pDataArray->temp , pDataArray->size ,
    	pDataArray->threadCreated , pDataArray->threadId);
} 

void distributeAndWork(int *prime,int prePrimeNum,int maxNum , bool *temp , int size){
	
	PDATA pDataArray[processNum];
	bool * sharedMemory = (bool *)mmap(NULL, sizeof(temp[0])*(maxNum+1), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	for (int i = 0; i <= maxNum; ++i)
		sharedMemory[i] = temp[i];

    for( int i=0; i<processNum; i++ )
    {
        pDataArray[i]=(MYDATA *)malloc(sizeof(MYDATA));
        pDataArray[i]->prime = prime;
        pDataArray[i]->prePrimeNum = prePrimeNum;
        pDataArray[i]->maxNum = maxNum;
        pDataArray[i]->temp = sharedMemory;
        pDataArray[i]->size = size;
        pDataArray[i]->threadCreated = processNum;
        pDataArray[i]->threadId = i;

        int pid = fork();

        if (pid < 0){
        	printf("process creation problem\n");
        }
        else if (pid == 0){
        	processFunction(pDataArray[i]);
        	exit(0);
        }
    }

    int processStatus = 0;
    for (int i = 0; i < processNum; ++i) {
        wait(&processStatus);
        free(pDataArray[i]);
    }

    for (int i = prePrimeNum+1; i <= maxNum; ++i)
		temp[i] = sharedMemory[i];

    munmap(sharedMemory , sizeof(temp[0])*(maxNum+1));
}

int eratostn(int maxNum , int *prime){
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

		//work
		distributeAndWork(prime , prePrimeNum, maxNum , temp , size);

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
	arraySize = 10000000;
	primeArraySize = 10000000;
	processNum = 1;
	for (int j = 0; j < 2; ++j)
	{
		printf("size : 10^%d\n",(int)log10(arraySize));
		processNum =1 ;
		for (int i = 0; i < 2; ++i)
		{
			/////////////////////////////////////////////////////

			clock_t firstSeconds ,secondSeconds;
   			firstSeconds =  clock();
			int *prime = new int[primeArraySize];
			int primeSize = eratostn(arraySize , prime);
			secondSeconds =  clock();
			printf("%d in %f seconds %d MB %d process\n", (int)primeSize , 
			((float)secondSeconds - (float)firstSeconds)/1000000.0 , (int)(sizeof(*prime)*arraySize*2/1000000)
			,processNum);

			/////////////////////////////////////////////////////
			processNum++;
		}
		arraySize *= 10;
		primeArraySize *= 10;
	}
	// for (int i = 0; i < primeSize; ++i)
	// {
	// 	printf("%d ", prime[i]);
	// }
	return 0;
}
