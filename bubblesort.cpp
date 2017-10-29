#include <boost/timer/timer.hpp>
#include <algorithm>

using namespace std;

void bubblesort(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = n - 1; j > i; j--)
            if (v[j] < v[j - 1]) {
                swap(v[j], v[j - 1]);
                swapped = true;
            }
        if (!swapped) return;
    }
}

int main() {
    constexpr size_t n = 100000;
    int a[n], b[n];

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> uid;
    generate_n(a, n, bind(uid, mt));
    copy(begin(a), end(a), b);

    boost::timer::cpu_timer timer;
    bubblesort(a, n);
    cout << "bubblesort:" << timer.format() << endl;

    timer.start();
    sort(begin(b), end(b));
    cout << "std::sort:" << timer.format() << endl;
}
