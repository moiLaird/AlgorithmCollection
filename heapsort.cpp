#include <boost/timer/timer.hpp>
#include <algorithm>

using namespace std;

inline void siftDown(vector<int> &v, int root, int n) {
    while (true) {
        int child = 2 * root + 1;
        if (child >= n) break;
        if (child + 1 < n && v[child] < v[child + 1])
            ++child;
        if (v[root] >= v[child]) return;
        swap(v[root], v[child]);
        root = child;
    }
}

inline void heapsort(vector<int> &v) {
    int n = v.size();
    for (int i = n / 2 - 1; i >= 0; --i)
        siftDown(v, i, n);  // build max heap
    for (int i = n - 1; i > 0; --i) {
        swap(v[0], v[i]);
        siftDown(v, 0, i);
    }
}

int main() {
    constexpr size_t n = 1000000;
    vector<int> a(n);

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> uid;
    generate(begin(a), end(a), bind(uid, mt));
    vector<int> b = a;

    boost::timer::cpu_timer timer;
    heapsort(a);
    cout << "heapsort:" << timer.format();
    cout << boolalpha << is_sorted(begin(a), end(a)) << endl;

    timer.start();
    make_heap(begin(b), end(b));
    sort_heap(begin(b), end(b));
    cout << "sort_heap:" << timer.format();
}
