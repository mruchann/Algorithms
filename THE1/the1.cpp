#include "the1.h"


void swap_numbers(unsigned short& l, unsigned short&r, long& swap) {
    unsigned short temp = l;
    l = r;
    r = temp;
    swap++;
}

int abs(int diff) {
    return (diff > 0) ? diff : -diff;
}

double max(double max_dist, double diff) {
    return (max_dist > diff) ? max_dist : diff;
}

int lomutoPartition(unsigned short* arr, long& swap, double& avg_dist, double& max_dist, int size) {
    unsigned short x = arr[size-1];
    int i = -1;
    for (int j = 0; j < size-1; j++) {
        if (arr[j] >= x) {
            double total_dist = swap * avg_dist;

            swap_numbers(arr[++i], arr[j], swap);
            int dist = abs(i-j);
            max_dist = max(max_dist, dist);
            
            total_dist += dist;
            avg_dist = total_dist/swap;
        }
    }
    double total_dist = swap * avg_dist;

    swap_numbers(arr[i+1], arr[size-1], swap);
    int dist = abs(i+1-(size-1));
    max_dist = max(max_dist, dist);

    total_dist += dist;
    avg_dist = total_dist/swap;

    return i+1;
}

int hoarePartition(unsigned short* arr, long& swap, double& avg_dist, double& max_dist, int size) {
    unsigned short x = arr[(size-1)/2];
    int i = -1, j = size;

    while (true) {
        do {
            j--;
        } while (arr[j] < x);
        do {
            i++;
        } while (arr[i] > x);
        if (i < j) {
            double total_dist = swap * avg_dist;

            swap_numbers(arr[i], arr[j], swap);
            int dist = abs(i-j);
            max_dist = max(max_dist, dist);

            total_dist += dist;
            avg_dist = total_dist/swap;
        }
        else {
            return j;
        }
    }
}

int median(unsigned short* arr, int first, int mid, int last) {
    if (arr[first] != arr[mid] && arr[first] != arr[last] && arr[mid] != arr[last]) {
        if ((arr[first] < arr[mid] && arr[mid] < arr[last]) || (arr[last] < arr[mid] && arr[mid] < arr[first])) {
            return mid;
        }
        if ((arr[first] < arr[last] && arr[last] < arr[mid]) || (arr[mid] < arr[last] && arr[last] < arr[first])) {
            return last;
        }
        if ((arr[mid] < arr[first] && arr[first] < arr[last]) || (arr[last] < arr[first] && arr[first] < arr[mid])) {
            return first;
        }
    }
    else {
        return -1;
    }
}

int quickSort(unsigned short* arr, long& swap, double& avg_dist, double& max_dist, bool hoare, bool median_of_3, int size){

    if (size <= 1) {
        return 1;
    }

    int i;

    if (hoare) {
        if (size >= 3 && median_of_3) {
            int x = median(arr, 0, (size-1)/2, size-1);

            if (x != -1 && x != (size-1)/2) {
                double total_dist = swap * avg_dist;

                swap_numbers(arr[x], arr[(size-1)/2], swap);
                int dist = abs(x - ((size-1)/2));
                max_dist = max(max_dist, dist);

                total_dist += dist;
                avg_dist = total_dist/swap;
            }
        }
        i = hoarePartition(arr, swap, avg_dist, max_dist, size);

        return 1 + 
            quickSort(arr, swap, avg_dist, max_dist, hoare, median_of_3, i+1) + 
            quickSort(arr+i+1, swap, avg_dist, max_dist, hoare, median_of_3, size - (i+1));
    }
    else {
        if (size >= 3 && median_of_3) {
            int x = median(arr, 0, (size-1)/2, size-1);

            if (x != -1 && x != size-1) {
                double total_dist = swap * avg_dist;

                swap_numbers(arr[x], arr[size-1], swap);
                int dist = abs(x-(size-1));
                max_dist = max(max_dist, dist);

                total_dist += dist;
                avg_dist = total_dist/swap;
            }
        }
        i = lomutoPartition(arr, swap, avg_dist, max_dist, size);
        
        return 1 + 
            quickSort(arr, swap, avg_dist, max_dist, hoare, median_of_3, i) + 
            quickSort(arr+i+1, swap, avg_dist, max_dist, hoare, median_of_3, size - (i+1));
    }
}
