// Lab5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>

#define NUM_THREADS 4 
#define ROWS 10000
#define COLS 100000
using namespace std;

long long* findMatrixSum(int*[]);
long long findSum(int*[], int);
void initArr(int* []);

int main()
{
	int** arr  = new int* [ROWS];
    initArr(arr);
    /*for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
			cout << arr[i][j] << " ";
        }
        cout << endl;
    }*/
    long long sum = 0;
    double t1 = omp_get_wtime();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
			sum += arr[i][j];
        }
    }
    cout << sum << endl;
    double t2 = omp_get_wtime();
    cout << "time: " << t2 - t1 << endl;
    t1 = omp_get_wtime();
    long long* res = findMatrixSum(arr);
    t2 = omp_get_wtime();
    cout << "time: " << t2 - t1 << endl;
	cout << "sum: " << res[0] << endl;
    cout << "minsum: " << res[2] << "minsum row indx: "<<res[1] << endl;
    delete[] arr;
    delete[] res;

}

long long* findMatrixSum(int* arr[] ) {
    omp_set_nested(1);
    long long* res = new long long[3];
	long long sum = 0;
    int minRowSumIndx = 0;
	long long minRowSum = LONG_MAX;
#pragma omp parallel for reduction(+:sum) num_threads(NUM_THREADS)
    for (int i = 0; i < ROWS; i++) {
		long long newSum = findSum(arr, i);
		sum += newSum;
        if (newSum < minRowSum) {
#pragma omp critical
            {
                if (newSum < minRowSum) {
                    minRowSum = sum;
                    minRowSumIndx = i;
                }
            }
        }
    }
#pragma omp barrier
    res[0] = sum;
	res[1] = minRowSumIndx;
	res[2] = minRowSum;
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

