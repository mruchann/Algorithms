#include <iostream>
#include "the0.h"

using namespace std;

void insertionSort(int* arr, long &comparison, long & swap, int size) 
{
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i-1;
        while (comparison++, j >= 0 && key < arr[j]) {
            arr[j+1] = arr[j];
            swap++;
            j--;
        }
        arr[j+1] = key;
    }
}

int main() {
    int arr[] = {9, -2, 3, 15};
    long comparison = 0;
    long swap = 0;
    insertionSort(arr, comparison, swap, 4);
    cout << "comparison: " << comparison << endl;
    cout << "swap: " << swap << endl;
}