#include <iostream>
#include <cstdint>
#include <omp.h>
#include <chrono>

// g++ -O3 -march=native -fopenmp -ffast-math heart_mc.cpp

namespace {
    constexpr double BOX_VOL  = 27.0;
    constexpr double INV_2P32 = 1.0 / 4294967296.0;
    constexpr double SCALE    = 3.0 * INV_2P32;
    constexpr double MIN_VAL  = -1.5;
    constexpr double C9_4     = 2.25;
    constexpr double C9_80    = 0.1125;
    constexpr uint64_t MASK32 = 0xFFFFFFFFULL;
}

// (x^2 + 9/4*y^2 + z^2 - 1)^3 - x^2*z^3 - 9/80*y^2*z^3 < 0
inline bool inside(double x, double y, double z) {
    double x2 = x * x, y2 = y * y, z2 = z * z;
    double z3 = z2 * z;
    double b  = x2 + C9_4 * y2 + z2 - 1.0;
    return (b * b * b < (x2 + C9_80 * y2) * z3);
}

// xoshiro256** PRNG
struct Rng {
    uint64_t s[4];
    explicit Rng(uint64_t seed) {
        for (auto &v : s) {
            uint64_t z = (seed += 0x9E3779B97F4A7C15ULL);
            z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
            z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
            v = z ^ (z >> 31);
        }
    }
    uint64_t operator()() {
        const uint64_t res = ((s[1] * 5) << 7 | (s[1] * 5) >> 57) * 9;
        const uint64_t t   = s[1] << 17;
        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];
        s[2] ^= t;
        s[3]  = (s[3] << 45 | s[3] >> 19);
        return res;
    }
};

int main() {
    const int64_t N = 10000000000LL;
    int64_t hits = 0;
    auto t0 = std::chrono::high_resolution_clock::now();
    #pragma omp parallel reduction(+:hits)
    {
        Rng rng(12345ULL + omp_get_thread_num() * 31337ULL);
        #pragma omp for schedule(static)
        for (int64_t i = 0; i < N; i += 8) {
            uint64_t r0  = rng(), r1  = rng(), r2  = rng(),
                     r3  = rng(), r4  = rng(), r5  = rng(),
                     r6  = rng(), r7  = rng(), r8  = rng(),
                     r9  = rng(), r10 = rng(), r11 = rng();
            #define U(v) (MIN_VAL + SCALE * (double)(uint32_t)(v))
            double x0 = U(r0 & MASK32), y0 = U(r1 & MASK32), z0 = U(r2 & MASK32);
            double x1 = U(r0 >> 32),    y1 = U(r1 >> 32),    z1 = U(r2 >> 32);
            double x2 = U(r3 & MASK32), y2 = U(r4 & MASK32), z2 = U(r5 & MASK32);
            double x3 = U(r3 >> 32),    y3 = U(r4 >> 32),    z3 = U(r5 >> 32);
            double x4 = U(r6 & MASK32), y4 = U(r7 & MASK32), z4 = U(r8 & MASK32);
            double x5 = U(r6 >> 32),    y5 = U(r7 >> 32),    z5 = U(r8 >> 32);
            double x6 = U(r9 & MASK32), y6 = U(r10 & MASK32), z6 = U(r11 & MASK32);
            double x7 = U(r9 >> 32),    y7 = U(r10 >> 32),    z7 = U(r11 >> 32);
            hits += inside(x0,y0,z0) + inside(x1,y1,z1)
                  + inside(x2,y2,z2) + inside(x3,y3,z3)
                  + inside(x4,y4,z4) + inside(x5,y5,z5)
                  + inside(x6,y6,z6) + inside(x7,y7,z7);
        }
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double dt = std::chrono::duration<double>(t1 - t0).count();
    std::cout << "Estimated Volume: " << (double)hits / N * BOX_VOL << "\n"
              << "Total Hits:       " << hits << " / " << N << "\n"
              << "Time elapsed:     " << dt << " seconds" << std::endl;
    return 0;
}
