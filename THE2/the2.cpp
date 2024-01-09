#include "the2.h"

#define LETTER_COUNT 26

int MaxLength(std::string* arr, int n) {
    int maxLength = arr[0].length();
    for (int i = 1; i < n; i++) {
        if (arr[i].length() > maxLength) {
            maxLength = arr[i].length();
        }
    }
    return maxLength;
}

void swap(std::string& s1, std::string& s2) {
    std::string temp = s1;
    s1 = s2;
    s2 = temp;
}

void CountingSort(std::string* arr, int n, int curr, int& iteration) {
    int Count[LETTER_COUNT+1];
    std::string Result[n];

    for (int i = 0; i < LETTER_COUNT+1; i++) {
        Count[i] = 0;
    }

    for (int i = 0; i < n; i++, iteration++) {
        if (curr < arr[i].length()) {
            Count[arr[i][curr] - 'A' +1]++;
        }
        else {
            Count[0]++;
        }
    }

    for (int i = 1; i < LETTER_COUNT+1; i++, iteration++) { 
        Count[i] += Count[i-1];
    }

    for (int i = n-1; i >= 0; i--, iteration++) {
        if (curr < arr[i].length()) {
            Result[--Count[arr[i][curr] - 'A' +1]] = arr[i];
        }
        else {
            Result[--Count[0]] = arr[i];
        }
    }

    for (int i = 0; i < n; i++, iteration++) {
        arr[i] = Result[i];
    }
}

int radix_string_sort(std::string* arr, int size, bool ascending){
    if (!arr) {
        return 0;
    }

    int iteration = 0;

    int maxLength = MaxLength(arr, size);

    for (int curr = maxLength-1; curr >= 0; curr--) {
        CountingSort(arr, size, curr, iteration);
    }

    if (!ascending) {
        for (int i = 0; i < size/2; i++) {
            swap(arr[i], arr[size-i-1]);
        }
    }
    
    return iteration;
}