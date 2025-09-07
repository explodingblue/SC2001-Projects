#include <iostream>
#include <random>
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
        if (a[i] <= a[j]) {      // compare first elements
            tmp[k++] = a[i++];      // 1st element of 1st half joins merged list
        } else {
            tmp[k++] = a[j++];       // 1st element of 2nd half joins merged list
        }
    }

    // copy remaining elements from the first half
    while (i <= mid) tmp[k++] = a[i++];

    // copy remaining elements from the second half
    while (j <= m) tmp[k++] = a[j++];

    // copy back to original array
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

void InsertionSort (ALIST slot[ ], int n)
{ // input slot is an array of n records;
// assume n > 1;
for (int i=1; i < n; i++)
    for (int j=i; j > 0; j--) {
        key_comparisons++; // count comparison
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
        InsertionSort(slot + n, m - n + 1); // switch to insertion sort
        return;
    }

    int mid = (n + m) / 2;
    hybrid_mergesort(slot, n, mid, S);
    hybrid_mergesort(slot, mid + 1, m, S);

    // Merge step (simplified for ALIST)
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
    random_device rd; // generates a random seed
    mt19937 gen(rd()); // Mersenne Twister RNG, seeded with rd
    uniform_int_distribution<> dist(1, xmax); // produces integers in [1, xmax]

    for (int i = 0; i < n; i++) {
        arr[i].key = dist(gen);
        arr[i].data = i; // can store original index
    }
}

// Main 
int main() {
    int sizes[] = {1000, 10000, 100000, 1000000, 10000000}; // can extend to 10 million
    int S = 32; // threshold for hybrid. When a subarray has S or fewer elements, instead of recursively calling merge sort, we just use Insertion Sort on that subarray.
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int idx = 0; idx < num_sizes; idx++) {
        int n = sizes[idx];
        ALIST arr[n];
        generate_dataset(arr, n, n);

        // Display Insertion Sort for smaller n
        if (n <= 10000) {
            key_comparisons = 0;
            ALIST temp1[n];
            for (int i = 0; i < n; i++) temp1[i] = arr[i];
            InsertionSort(temp1, n);
            cout << "InsertionSort n=" << n << ", comps=" << key_comparisons << endl;
            cout.flush();
        } else {
            cout << "InsertionSort n=" << n << " -> too long" << endl;
            cout.flush();
        }

        // Display Original Merge Sort
        key_comparisons = 0;
        mergesort(0, n - 1);
        cout << "MergeSort n=" << n << ", comps=" << key_comparisons << endl;
        cout.flush();

        // Display Hybrid Merge Sort
        key_comparisons = 0;
        ALIST temp2[n];
        for (int i = 0; i < n; i++) temp2[i] = arr[i];
        hybrid_mergesort(temp2, 0, n - 1, S);
        cout << "HybridMergeSort n=" << n << ", S=" << S << ", comps=" << key_comparisons << endl;
        cout.flush();

        cout << "----------------------------------------" << endl;
    }

    return 0;
}