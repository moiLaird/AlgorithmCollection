#include <boost/timer/timer.hpp>
#include <algorithm>

using namespace std;

constexpr int THRESHOLD = 16;

template <typename Iterator>
inline void merge_sort(Iterator first, Iterator last) {
    auto len = last - first;
    // insertion sort on small array
    if (len <= THRESHOLD) {
        Iterator i, j;
        for (i = first + 1; i != last; ++i) {
            auto tmp = *i;
            for (j = i; j != first && *(j - 1) > tmp; --j)
                *j = *(j - 1);
            *j = tmp;
        }
        return;
    }
    Iterator middle = first + len / 2;
    merge_sort(first, middle);
    merge_sort(middle, last);
    // optimization for nearly sorted
    if (*(middle - 1) <= *middle)
        return;
    inplace_merge(first, middle, last);
}

int main() {
    constexpr size_t n = 100000000;
    vector<int> a(n);

    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> uid;
    generate(begin(a), end(a), bind(uid, mt));
    vector<int> b = a;

    boost::timer::cpu_timer timer;
    merge_sort(begin(a), end(a));
    cout << "mergesort:" << timer.format() << endl;

    timer.start();
    sort(begin(b), end(b));
    cout << "std::sort:" << timer.format() << endl;
}
