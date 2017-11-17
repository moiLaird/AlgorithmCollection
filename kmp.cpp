#include <boost/timer/timer.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

inline vector<size_t> preKMP(const string &pat) {
    size_t m = pat.length();
    vector<size_t> next(m);
    next[0] = 0;
    size_t k = 0;
    for (size_t i = 1; i < m; ++i) {
        while (k > 0 && pat[k] != pat[i])
            k = next[k - 1];
        if (pat[k] == pat[i])
            k++;
        next[i] = k;
    }
    return next;
}

inline size_t kmp_match(const string &txt, const string &pat) {
    size_t n = txt.length();
    size_t m = pat.length();
    vector<size_t> next = preKMP(pat);
    size_t k = 0;
    for (size_t i = 0; i < n; ++i) {
        while (k > 0 && pat[k] != txt[i])
            k = next[k - 1];
        if (pat[k] == txt[i])
            k++;
        if (k == m)
            return i - m + 1;
    }
    return static_cast<size_t>(-1);
}

int main() {
    ifstream ifs("tale.txt");
    stringstream buffer;
    buffer << ifs.rdbuf();
    string txt = buffer.str();
    string pat("It is a far, far better thing that I do, than I have ever done");

    boost::timer::cpu_timer timer;
    auto pos = kmp_match(txt, pat);
    cout << "kmp:" << timer.format();
    cout << pos << endl;

    timer.start();
    auto loc = txt.find(pat);
    cout << "find:" << timer.format();
    cout << loc << endl;
}
