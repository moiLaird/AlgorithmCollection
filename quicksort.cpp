#include <boost/timer/timer.hpp>
#include <algorithm>

using namespace std;

inline void swap(int v[], int i, int j)
{
    int tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

constexpr int THRESHOLD = 16;

inline void quicksort(int v[], int lo, int hi)
{
    int len = hi - lo;
    // insertion sort on small array
    if (len < THRESHOLD) {
        int i, j;
        for (i = lo + 1; i <= hi; ++i) {
            int tmp = v[i];
            for (j = i; j > lo && v[j - 1] > tmp; --j)
                v[j] = v[j - 1];
            v[j] = tmp;
        }
        return;
    }
    // median-of-three
    int mid = lo + len / 2;
    if (v[lo] > v[mid])
        swap(v, lo, mid);
    if (v[lo] > v[hi])
        swap(v, lo, hi);
    if (v[mid] > v[hi])
        swap(v, mid, hi);
    swap(v, mid, hi - 1);  // store pivot in v[hi-1] as sentinel
    int pivot = v[hi - 1];
    // begin partitioning
    int i = lo, j = hi - 1;
    while (true) {
        while (v[++i] < pivot) {}
        while (pivot < v[--j]) {}
        if (i >= j) break;
        swap(v, i, j);
    }
    swap(v, i, hi - 1);  // restore pivot
    quicksort(v, lo, i - 1);
    quicksort(v, i + 1, hi);
}

int main()
{
    constexpr size_t n = 100000000;
    vector<int> a(n);

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> uid;
    generate(begin(a), end(a), bind(uid, mt));
    vector<int> b = a;

    boost::timer::cpu_timer timer;
    quicksort(a.data(), 0, n - 1);
    cout << "quicksort:" << timer.format();

    timer.start();
    sort(begin(b), end(b));
    cout << "std::sort:" << timer.format();
}
