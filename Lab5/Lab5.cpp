// Lab5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>

#define NUM_THREADS 32
#define ROWS 10000
#define COLS 100000
using namespace std;

long long findMatrixSum(int*[]);
long long findSum(int*[], int);
void initArr(int* []);
long long* findMinRowSum(int* []);

int main()
{
    omp_set_nested(1);
	int** arr  = new int* [ROWS];
    initArr(arr);
    /*for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
			cout << arr[i][j] << " ";
        }
        cout << endl;
    }*/
    /*long long sum = 0;
    double t1 = omp_get_wtime();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
			sum += arr[i][j];
        }
    }

    cout << sum << endl;
    double t2 = omp_get_wtime();
    cout << "time: " << t2 - t1 << endl;*/
#pragma omp parallel sections
    {
#pragma omp section
        {
        double t1 = omp_get_wtime();
        long long res = findMatrixSum(arr);
        double t2 = omp_get_wtime();
#pragma omp critical
        {
            cout << "time: " << t2 - t1 << endl;
            cout << "sum: " << res << endl;
        }
        }
#pragma omp section
        {
    double t1 = omp_get_wtime();
    long long* minSumAndIndx = findMinRowSum(arr);
    double t2 = omp_get_wtime();
#pragma omp critical
    {
        cout << "time: " << t2 - t1 << endl;
        cout << "min row sum: " << minSumAndIndx[1] << " at row: " << minSumAndIndx[0] << endl;
    }
    delete[] minSumAndIndx;
        }
    }
    delete[] arr;
}

long long findMatrixSum(int* arr[] ) 
{
	long long sum = 0;
#pragma omp parallel for reduction(+:sum) num_threads(NUM_THREADS)
    for (int i = 0; i < ROWS; i++) {
		long long newSum = findSum(arr, i);
		sum += newSum;
    }
    return sum;
}

long long* findMinRowSum(int* arr[]) {
    long long* res = new long long[2];
    res[0] = 0;
    res[1] = LONG_MAX;
#pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < ROWS; i++) {
        long long newSum = findSum(arr, i);
        if(newSum < res[1])
        #pragma omp critical
        {
            if (newSum < res[1]) {
                res[0] = i;
                res[1] = newSum;
            }
        }
    }
	return res;
}

long long findSum(int* arr[], int row) {
	long long sum = 0;
    #pragma omp parallel for reduction(+:sum) num_threads(NUM_THREADS)
    for (int j = 0; j < COLS; j++) {
        sum += arr[row][j];
    }
    return sum;
}

void initArr(int* arr[]) {
    for (int i = 0; i < ROWS; i++) {
        arr[i] = new int[COLS];
        for (int j = 0; j < COLS; j++) {
            arr[i][j] = rand() % 100;
        }
    }
}

