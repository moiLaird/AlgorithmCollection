#include <iostream>
#include <bitset>

using namespace std;
#define likely(x) (__builtin_expect(!!(x), 1))

// ZigZag maps signed integers to unsigned, effectively used with varint encoding

uint64_t encodeZigzag(int64_t n) {
    // right-shift must be arithmetic
    return (static_cast<uint64_t>(n) << 1) ^ static_cast<uint64_t>(n >> 63);
}

int64_t decodeZigzag(uint64_t n) {
    return static_cast<int64_t>((n >> 1) ^ (~(n & 1) + 1));
}

int parseVarint(const char *buf, uint64_t &val) {
    val = 0;
    for (int i = 0; i < 10; ++i) {
        uint8_t byte = buf[i];
        val |= static_cast<uint64_t>(byte & 0x7F) << (7 * i);
        if (likely(byte < 0x80))
            return i + 1;
    }
    return -1;
}

size_t encodeVarint(uint64_t val, char *buf) {
    if (val < 128) {
        buf[0] = val;
        return 1;
    }
    size_t i = 0;
    while (val) {
        uint8_t byte = val & 0x7F;
        val >>= 7;
        if (val) byte |= 0x80;
        buf[i++] = byte;
    }
    return i;
}

const char *decodeVarint(const char *ptr, uint64_t &val) {
    uint8_t byte;
    uint8_t pos = 0;
    val = 0;
    do {
        if (pos >= 70) return nullptr;
        byte = *ptr;
        val |= static_cast<uint64_t>(byte & 0x7F) << pos;
        ++ptr;
        pos += 7;
    } while (byte & 0x80);
    return ptr;
}

string str2bin(const string &str) {
    string res;
    for (char c : str)
        res += bitset<8>(c).to_string() + ' ';
    return res;
}

int main() {
    int64_t num = 123456789;
    uint64_t zz = encodeZigzag(num);
    cout << zz << " < ZigZag > " << decodeZigzag(zz) << endl;

    char vi[10];
    size_t n = encodeVarint(zz, vi);
    uint64_t val;
    decodeVarint(vi, val);
    cout << str2bin(string(vi, n)) << " < VarInt > " << val << endl;

    return 0;
}

