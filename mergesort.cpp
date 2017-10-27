#include <boost/timer/timer.hpp>
#include <algorithm>

using namespace std;

template <typename Iterator>
inline void merge_sort(Iterator first, Iterator last) {
    auto len = last - first;
    if (len > 1) {
        Iterator middle = first + len / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        inplace_merge(first, middle, last);
    }
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
