#include <iostream>
#include <random>
#include <vector>
#include <chrono>
#include <algorithm>
using namespace std;

// Global key comparison counter
static long long key_comparisons = 0;

// ------------ Kayıt tipi ------------
using KeyType  = int;
using DataType = int;

class ALIST {
public:
    KeyType  key;
    DataType data;
};

// ------------ Karşılaştırma sayaçlı yardımcılar ------------
inline bool leq(int a, int b){ key_comparisons++; return a <= b; }
inline bool lt (int a, int b){ key_comparisons++; return a  <  b; }

// ------------ Insertion Sort (aynı arayüz) ------------
void swap(ALIST &a, ALIST &b){ ALIST t=a; a=b; b=t; }

void InsertionSort (ALIST slot[], int n){
    for (int i=1; i<n; ++i){
        ALIST x = slot[i];
        int j = i-1;
        while (j>=0 && lt(x.key, slot[j].key)){
            slot[j+1] = slot[j];
            --j;
        }
        slot[j+1] = x;
    }
}

// ------------ Merge (tek kullanımlık buffer ile) ------------
static void merge_run(ALIST* a, int l, int m, int r, ALIST* buf){
    int i=l, j=m+1, k=0;
    while (i<=m && j<=r){
        if (leq(a[i].key, a[j].key)) buf[k++] = a[i++];
        else                         buf[k++] = a[j++];
    }
    while (i<=m) buf[k++] = a[i++];
    while (j<=r) buf[k++] = a[j++];
    for (int t=0; t<k; ++t) a[l+t] = buf[t];
}

// ------------ Pure MergeSort (ALIST üzerinde) ------------
static void mergesort(ALIST* a, int l, int r, ALIST* buf){
    if (l>=r) return;
    int m = l + (r-l)/2;
    mergesort(a, l, m, buf);
    mergesort(a, m+1, r, buf);
    merge_run(a, l, m, r, buf);
}

// ------------ Hybrid Merge Sort (eşik S) ------------
static void hybrid_mergesort(ALIST* slot, int n, int m, int S, ALIST* buf){
    if (m-n <= 0) return;
    if (m-n+1 <= S){ InsertionSort(slot+n, m-n+1); return; }
    int mid = n + (m-n)/2;
    hybrid_mergesort(slot, n, mid, S, buf);
    hybrid_mergesort(slot, mid+1, m, S, buf);
    merge_run(slot, n, mid, m, buf);
}

// ------------ Veri üretici ------------
void generate_dataset(ALIST arr[], int n, int xmax){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, xmax);
    for (int i=0; i<n; ++i){
        arr[i].key  = dist(gen);
        arr[i].data = i;
    }
}

// ------------ Main ------------
int main(){
    int sizes[] = {1000, 10000, 100000, 1000000, 10000000};
    int S = 32;
    int num_sizes = sizeof(sizes)/sizeof(sizes[0]);

    for (int idx=0; idx<num_sizes; ++idx){
        int n = sizes[idx];

        // Veri: HEAP üzerinde
        vector<ALIST> arr(n);
        generate_dataset(arr.data(), n, n);

        // Insertion Sort (küçük n'lerde)
        if (n <= 10000){
            key_comparisons = 0;
            auto temp1 = arr; // kopya (heap)
            auto t1 = chrono::high_resolution_clock::now();
            InsertionSort(temp1.data(), n);
            auto t2 = chrono::high_resolution_clock::now();
            double ms = chrono::duration<double, milli>(t2 - t1).count();
            cout << "InsertionSort n=" << n
                 << ", comps=" << key_comparisons
                 << ", time(ms)=" << ms << "\n";
        } else {
            cout << "InsertionSort n=" << n << " -> too long\n";
        }

        // Pure MergeSort (tek buffer)
        key_comparisons = 0;
        auto tempM = arr;
        vector<ALIST> bufM(n);
        auto t3 = chrono::high_resolution_clock::now();
        mergesort(tempM.data(), 0, n-1, bufM.data());
        auto t4 = chrono::high_resolution_clock::now();
        double msM = chrono::duration<double, milli>(t4 - t3).count();
        cout << "MergeSort n=" << n
             << ", comps=" << key_comparisons
             << ", time(ms)=" << msM << "\n";

        // Hybrid MergeSort (tek buffer)
        key_comparisons = 0;
        auto temp2 = arr;
        vector<ALIST> bufH(n);
        auto t5 = chrono::high_resolution_clock::now();
        hybrid_mergesort(temp2.data(), 0, n-1, S, bufH.data());
        auto t6 = chrono::high_resolution_clock::now();
        double msH = chrono::duration<double, milli>(t6 - t5).count();
        cout << "HybridMergeSort n=" << n
             << ", S=" << S
             << ", comps=" << key_comparisons
             << ", time(ms)=" << msH << "\n";

        cout << "----------------------------------------\n";
    }
    return 0;
}

