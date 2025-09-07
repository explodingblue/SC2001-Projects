// mergesort_project.cpp
#include <bits/stdc++.h>
using namespace std;

static long long COMPS = 0; // global key-comparison counter

// ---------------- Insertion Sort (for Hybrid) ----------------
void insertion_sort(vector<int>& a, int l, int r) {
    for (int i = l + 1; i <= r; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= l) {
            COMPS++;                     // key comparison a[j] > key ?
            if (a[j] > key) { a[j + 1] = a[j]; --j; }
            else break;
        }
        a[j + 1] = key;
    }
}

// ---------------- Original Merge (slides-style, stable) ----------------
// Takes two sorted vectors by const-ref, returns merged vector (copies like slides)
vector<int> merge_original(const vector<int>& L, const vector<int>& R) {
    vector<int> out; out.reserve(L.size() + R.size());
    size_t i = 0, j = 0;
    while (i < L.size() && j < R.size()) {
        COMPS++;                          // compare L[i] and R[j]
        if (L[i] <= R[j]) out.push_back(L[i++]); // <= keeps stability
        else               out.push_back(R[j++]);
    }
    while (i < L.size()) out.push_back(L[i++]);
    while (j < R.size()) out.push_back(R[j++]);
    return out;
}

// ---------------- Original Merge Sort (slides-style) ----------------
vector<int> merge_sort_original(const vector<int>& A) {
    if (A.size() <= 1) return A;
    size_t mid = A.size() / 2;
    vector<int> left (A.begin(), A.begin() + mid);
    vector<int> right(A.begin() + mid, A.end());
    left  = merge_sort_original(left);
    right = merge_sort_original(right);
    return merge_original(left, right);
}

// ---------------- Hybrid Merge (in-place with temp buffer per call) ----------------
// In-place merge using temporary buffer (stable, counts COMPS on element compares)
void merge_inplace(vector<int>& a, int l, int m, int r) {
    vector<int> tmp; tmp.reserve(r - l + 1);
    int i = l, j = m + 1;
    while (i <= m && j <= r) {
        COMPS++;                           // compare a[i] and a[j]
        if (a[i] <= a[j]) tmp.push_back(a[i++]);
        else              tmp.push_back(a[j++]);
    }
    while (i <= m) tmp.push_back(a[i++]);
    while (j <= r) tmp.push_back(a[j++]);
    copy(tmp.begin(), tmp.end(), a.begin() + l);
}

// Hybrid mergesort: switch to insertion when segment size <= S
void hybrid_merge_sort(vector<int>& a, int l, int r, int S) {
    int len = r - l + 1;
    if (len <= 1) return;
    if (len <= S) { insertion_sort(a, l, r); return; }
    int m = l + (r - l) / 2;
    hybrid_merge_sort(a, l, m, S);
    hybrid_merge_sort(a, m + 1, r, S);
    merge_inplace(a, l, m, r);
}

// ---------------- Helpers ----------------
vector<int> generate_dataset(int n, int xmax, uint32_t seed = 42) {
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(1, xmax);
    vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = dist(rng);
    return v;
}

template <class F>
pair<double,long long> timed_run(F&& f) {
    COMPS = 0;
    auto t0 = chrono::high_resolution_clock::now();
    f(); // run
    auto t1 = chrono::high_resolution_clock::now();
    double sec = chrono::duration<double>(t1 - t0).count();
    return {sec, COMPS};
}

// ---------------- Main ----------------
int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // defaults
    int n = 1'000'000; // 1M varsayılan
    int S = 32;        // hybrid threshold
    int xmax = n;      // values in [1..xmax]
    // very tiny flag parser
    for (int i = 1; i < argc; ++i) {
        string s = argv[i];
        if ((s == "-n" || s == "--n") && i + 1 < argc) n = stoi(argv[++i]);
        else if ((s == "-S" || s == "--S") && i + 1 < argc) S = stoi(argv[++i]);
        else if ((s == "-x" || s == "--xmax") && i + 1 < argc) xmax = stoi(argv[++i]);
    }

    // dataset
    auto data = generate_dataset(n, xmax, /*seed*/42);

    // --- Original mergesort (slides-style) ---
    auto copy1 = data; // vector copy
    auto [t_orig, comps_orig] = timed_run([&]{
        // returns a new vector, emulate slides-style
        auto sorted = merge_sort_original(copy1);
        copy1.swap(sorted); // keep result in copy1
    });

    // --- Hybrid mergesort ---
    auto copy2 = data;
    auto [t_hyb, comps_hyb] = timed_run([&]{
        hybrid_merge_sort(copy2, 0, (int)copy2.size() - 1, S);
    });

    // sanity check
    if (copy1 != copy2) {
        cerr << "WARNING: outputs differ!\n";
    }

    cout << "Input size: " << n << ", Threshold S: " << S << ", xmax: " << xmax << "\n";
    cout << "Original MergeSort -> Comparisons: " << comps_orig
         << ", Time: " << fixed << setprecision(6) << t_orig << " s\n";
    cout << "Hybrid   MergeSort -> Comparisons: " << comps_hyb
         << ", Time: " << fixed << setprecision(6) << t_hyb  << " s\n";
    return 0;
}

