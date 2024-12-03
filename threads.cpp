#include <iostream>
#include <thread>
#include <vector>
#include <algorithm> 
#include <chrono>
#include <cstdlib>   
#include <ctime>   
#include <cmath>  

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int L[n1], R[n2];
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int i = 0; i < n2; ++i) R[i] = arr[mid + 1 + i];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void threadedMergeSort(int arr[], int left, int right, int depth = 0) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (depth < 3) { 
            std::thread leftThread(threadedMergeSort, arr, left, mid, depth + 1);
            std::thread rightThread(threadedMergeSort, arr, mid + 1, right, depth + 1);

            leftThread.join();
            rightThread.join();
        } else {
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    const int ARRAY_SIZE = 100000;
    int largeArray[ARRAY_SIZE];

    
    std::srand(std::time(0));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        largeArray[i] = std::rand() % 100000; 
    }

    int arrCopy[ARRAY_SIZE];
    std::copy(std::begin(largeArray), std::end(largeArray), arrCopy);

    // Measure time for threaded merge sort
    auto start1 = std::chrono::high_resolution_clock::now();
    threadedMergeSort(largeArray, 0, ARRAY_SIZE - 1);
    auto end1 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration1 = end1 - start1;
    std::cout << "Time taken using threaded merge sort: " << duration1.count() << " seconds" << std::endl;


    // Measure time for normal merge sort
    auto start2 = std::chrono::high_resolution_clock::now();
    mergeSort(arrCopy, 0, ARRAY_SIZE - 1);
    auto end2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration2 = end2 - start2;
    std::cout << "Time taken using normal merge sort: " << duration2.count() << " seconds" << std::endl;


    std::cout << "Sorting using threads is: " 
          << (duration2.count() / duration1.count()) << " times faster" << std::endl;

    return 0;
}
