#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> split(const std::string &str, const std::string &delim = ",") {
    std::vector<std::string> tokens;
    size_t beg, end = 0;
    while ((beg = str.find_first_not_of(delim, end)) != std::string::npos) {
        end = str.find_first_of(delim, beg);
        tokens.emplace_back(str.substr(beg, end - beg));
    }
    return tokens;
}

template<typename Iter>
std::string join(Iter beg, Iter end, char sep = ',') {
    std::ostringstream result;
    if (beg != end)
        result << *beg++;
    while (beg != end)
        result << sep << *beg++;
    return result.str();
}

int main() {
    std::string csv = "2020-04-02,SH000001,2780.64";
    auto tok = split(csv);
    for (auto &val : tok)
        std::cout << val << std::endl;
    assert(join(tok.begin(), tok.end()) == csv);
    return 0;
}

