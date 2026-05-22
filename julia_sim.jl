using Base.Threads

@inline @fastmath function is_inside(x, y, z)
    ((x^2 + 2.25y^2 + z^2 - 1.0)^3 - (x^2 + 0.1125y^2) * z^3) < 0
end

function init_rng(seed::UInt64)
    ntuple(4) do _
        seed += 0x9e3779b97f4a7c15
        z = xor(seed, seed >> 30) * 0xbf58476d1ce4e5b9
        z = xor(z, z >> 27) * 0x94d049bb133111eb
        xor(z, z >> 31)
    end
end
# Xoshiro256** PRNG
@inline function next_rand((s1, s2, s3, s4)::NTuple{4, UInt64})
    u01 = Float64((bitrotate(s2 * 5, 7) * 9) >> 11) * 2.0^-53
    return u01, (
        xor(s1, s2, s4),
        xor(s1, s2, s3),
        xor(s1, s3, s2 << 17),
        bitrotate(xor(s2, s4), 45)
    )
end

function mc_vol(n, nthr = Threads.nthreads())
    chunk, remainder = divrem(n, nthr)
    hits = zeros(Int64, nthr)
    @fastmath Threads.@threads for tid in 1:nthr
        samples = chunk + (tid <= remainder ? 1 : 0)
        state, count = init_rng(UInt64(12345 + tid * 31337)), 0
        for _ in 1:samples
            ux, state = next_rand(state)
            uy, state = next_rand(state)
            uz, state = next_rand(state)
            count += is_inside(3ux - 1.5, 3uy - 1.5, 3uz - 1.5)
        end
        @inbounds hits[tid] = count
    end
    sum(hits)
end

mc_vol(10_000)  # warmup
let N = 100_000_000_000
    elapsed = @elapsed hits = mc_vol(N)
    volume = (hits / N) * 27.0
    println("Volume: $(round(volume, digits=4))")
    println("Time:   $(round(elapsed, digits=3))s")
end