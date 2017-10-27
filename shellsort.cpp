#include <boost/timer/timer.hpp>
#include <algorithm>

using namespace std;

void shellsort(int v[], size_t n) {
    size_t gap, i, j;
    for (gap = 1; gap < n / 3; gap = 3 * gap + 1) {}
    while (gap) {
        for (i = gap; i < n; ++i) {
            int tmp = v[i];
            for (j = i; j >= gap && v[j - gap] > tmp; j -= gap)
                v[j] = v[j - gap];
            v[j] = tmp;
        }
        gap /= 3;
    }
}

int main() {
    constexpr size_t n = 1000000;
    int a[n], b[n];

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> uid;
    generate_n(a, n, bind(uid, mt));
    copy(begin(a), end(a), b);

    boost::timer::cpu_timer timer;
    shellsort(a, n);
    cout << "Shellsort:" << timer.format() << endl;

    timer.start();
    sort(begin(b), end(b));
    cout << "std::sort:" << timer.format() << endl;
}
