#include <iostream>
#include <random>
#include <chrono>
using namespace std;

// Global key comparison counter
static long long key_comparisons = 0;

// Merge Sort
void merge(int n, int m) {
    static int a[10000000]; // static array on global/static memory
    if (m - n <= 0) return;       // base case: 0 or 1 element

    int mid = (n + m) / 2;        // divide the list into two halves
    int size = m - n + 1;
    int* tmp = new int[size];      // temporary array for the merged list

    int i = n;        // pointer for the first half
    int j = mid + 1;  // pointer for the second half
    int k = 0;        // pointer for the temporary array

    // while both halves are not empty
    while (i <= mid && j <= m) {
        key_comparisons++; // count comparison
        if (a[i] <= a[j]) {      
            tmp[k++] = a[i++];      
        } else {
            tmp[k++] = a[j++];       
        }
    }

    while (i <= mid) tmp[k++] = a[i++];
    while (j <= m) tmp[k++] = a[j++];

    for (int p = 0; p < size; p++) a[n + p] = tmp[p];

    delete[] tmp; // free heap memory
}

void mergesort(int n, int m)
{ 
    int mid = (n+m)/2;
    if (m-n <= 0){
        return;
    }
    else if (m-n > 1) {
        mergesort(n, mid);
        mergesort(mid+1, m);
    }
    merge(n, m);
}

// Insertion Sort
using KeyType = int;
using DataType = int;

class ALIST {
public: 
    KeyType key;
    DataType data;
};

void swap(ALIST &a, ALIST &b) {
    ALIST temp = a;
    a = b;
    b = temp;
}

void InsertionSort (ALIST slot[ ], int n){
    for (int i=1; i < n; i++)
        for (int j=i; j > 0; j--) {
            key_comparisons++; 
            if (slot[j].key < slot[j-1].key){
                swap(slot[j], slot[j-1]);
            }
            else{
                break;
            }
        }
}

// Hybrid Merge Sort
void hybrid_mergesort(ALIST slot[], int n, int m, int S) {
    if (m - n <= 0) return;

    if (m - n + 1 <= S) {
        InsertionSort(slot + n, m - n + 1);
        return;
    }

    int mid = (n + m) / 2;
    hybrid_mergesort(slot, n, mid, S);
    hybrid_mergesort(slot, mid + 1, m, S);

    int size = m - n + 1;
    ALIST tmp[size];

    int i = n, j = mid + 1, k = 0;
    while (i <= mid && j <= m) {
        key_comparisons++;
        if (slot[i].key <= slot[j].key) tmp[k++] = slot[i++];
        else tmp[k++] = slot[j++];
    }
    while (i <= mid) tmp[k++] = slot[i++];
    while (j <= m) tmp[k++] = slot[j++];

    for (int p = 0; p < size; p++)
        slot[n + p] = tmp[p];
}

// Dataset generator
void generate_dataset(ALIST arr[], int n, int xmax) {
    random_device rd; 
    mt19937 gen(rd()); 
    uniform_int_distribution<> dist(1, xmax); 

    for (int i = 0; i < n; i++) {
        arr[i].key = dist(gen);
        arr[i].data = i; 
    }
}

// Main 
int main() {
    int sizes[] = {1000, 10000, 100000, 1000000, 10000000}; 
    int S = 32; 
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = sizes[idx];
        ALIST arr[n];
        generate_dataset(arr, n, n);

        // Insertion Sort (küçük n)
        if (n <= 10000) {
            key_comparisons = 0;
            ALIST temp1[n];
            for (int i = 0; i < n; i++) temp1[i] = arr[i];
            auto t1 = chrono::high_resolution_clock::now();
            InsertionSort(temp1, n);
            auto t2 = chrono::high_resolution_clock::now();
            double ms = chrono::duration<double, milli>(t2 - t1).count();
            cout << "InsertionSort n=" << n 
                 << ", comps=" << key_comparisons 
                 << ", time(ms)=" << ms << endl;
        } else {
            cout << "InsertionSort n=" << n << " -> too long" << endl;
        }

        // Merge Sort
        key_comparisons = 0;
        auto t3 = chrono::high_resolution_clock::now();
        mergesort(0, n - 1);
        auto t4 = chrono::high_resolution_clock::now();
        double msM = chrono::duration<double, milli>(t4 - t3).count();
        cout << "MergeSort n=" << n 
             << ", comps=" << key_comparisons 
             << ", time(ms)=" << msM << endl;

        // Hybrid Merge Sort
        key_comparisons = 0;
        ALIST temp2[n];
        for (int i = 0; i < n; i++) temp2[i] = arr[i];
        auto t5 = chrono::high_resolution_clock::now();
        hybrid_mergesort(temp2, 0, n - 1, S);
        auto t6 = chrono::high_resolution_clock::now();
        double msH = chrono::duration<double, milli>(t6 - t5).count();
        cout << "HybridMergeSort n=" << n 
             << ", S=" << S 
             << ", comps=" << key_comparisons 
             << ", time(ms)=" << msH << endl;

        cout << "----------------------------------------" << endl;
    }

    return 0;
}
