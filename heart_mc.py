import os, time
from numba import njit, prange, uint64

@njit(fastmath=True, inline='always')
def is_inside(x: float, y: float, z: float) -> bool:
    return ((x**2+2.25*y**2+z**2-1.0)**3-(x**2+0.1125*y**2)*z**3)<0.0

@njit(fastmath=True, inline='always')
def next_rand(s: uint64) -> tuple[float, uint64]:
    s = s + uint64(0x9E3779B97F4A7C15)
    z = (s ^ (s >> uint64(30))) * uint64(0xBF58476D1CE4E5B9)
    z = (z ^ (z >> uint64(27))) * uint64(0x94D049BB133111EB)
    z = z ^ (z >> uint64(31))
    u01 = float(z >> uint64(11)) * 1.1102230246251565e-16
    return u01, s

@njit(fastmath=True, inline='always')
def count_hits(samples: int, init_state: uint64) -> int:
    cnt = 0
    state = init_state
    for _ in range(samples):
        ux, state = next_rand(state)
        uy, state = next_rand(state)
        uz, state = next_rand(state)
        x, y, z = -1.5+3.0*ux, -1.5+3.0*uy, -1.5+3.0*uz
        cnt += is_inside(x, y, z)
    return cnt

@njit(parallel=True, fastmath=True)
def mc_vol(n: int, nthr: int) -> int:
    chunk = n // nthr
    rem = n % nthr
    total = 0
    for tid in prange(nthr):
        samples = chunk + (1 if tid < rem else 0)
        total += count_hits(samples, uint64(12345+tid*31337))
    return total

if __name__ == "__main__":
    N = 10_000_000_000
    T = os.cpu_count() or 8
    _ = mc_vol(10_000, T)  # warmup
    t0 = time.perf_counter()
    hits = mc_vol(N, T)
    dt = time.perf_counter() - t0
    p = hits / N
    volume = p * 27.0
    abs_err = 27.0 * (p * (1.0 - p) / N) ** 0.5
    print(f"Volume: {volume:.4f}  ± {abs_err:.6f}")
    print(f"Time:   {dt:.3f}s")