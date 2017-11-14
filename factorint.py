#!/usr/bin/env python3
from bisect import bisect_left
from collections import Counter
from random import SystemRandom


# Python 3.5 introduced math.gcd, but OJ uses old version
# TODO : binary GCD algorithm
def i_gcd(a, b):
    if a and b:
        if b < 0:
            b = -b
        while b:
            a, b = b, a % b
    else:
        a = abs(a or b)
    return a


# https://oeis.org/A014233
pseudo_primes = [2047, 1373653, 25326001, 3215031751, 2152302898747, 3474749660383, 341550071728321, 341550071728321,
                 3825123056546413051, 3825123056546413051, 3825123056546413051, 318665857834031151167461,
                 3317044064679887385961981]
# Mathematica : Table[Prime[n], {n, PrimePi[1000]}]
primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
          109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
          233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
          367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491,
          499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641,
          643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787,
          797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
          947, 953, 967, 971, 977, 983, 991, 997]


def test_composite(a, n, d, s):
    x = pow(a, d, n)
    if x == 1 or x == n - 1:
        return False
    for i in range(1, s):
        x = pow(x, 2, n)
        if x == n - 1:
            return False
    return True  # n definitely composite


# Miller–Rabin primality test can be deterministic for n below certain limit with proper base
# TODO : Lucas–Lehmer primality test
def miller_rabin(n):
    assert n <= pseudo_primes[-1], "Miller-Rabin primality test may fail for large n = %r" % n
    if n < 2:
        return False
    elif n in primes:
        return True
    elif n in pseudo_primes:
        return False
    if any((n % p == 0) for p in primes):
        return False
    pos = bisect_left(pseudo_primes, n) + 1
    prime_bases = primes[:pos]
    d = n - 1
    s = 0
    while d % 2 == 0:
        d >>= 1
        s += 1
    for a in prime_bases:
        if test_composite(a, n, d, s):
            return False
    return True


# TODO : SQUFOF - Square Form Factorization algorithm
def pollard_rho(n):
    for p in primes:
        if n % p == 0:
            return p
    rng = SystemRandom()
    x = 2
    a = 1
    while True:
        y = x
        f = lambda t: (pow(t, 2, n) + a) % n
        while True:
            x = f(x)
            y = f(f(y))
            d = i_gcd(y - x, n)
            if d == 1:
                continue
            elif d == n:
                break
            else:
                return d
        x = rng.randint(0, n - 1)
        a = rng.randint(1, n - 3)  # a%n should not be 0 or -2


def factor(n):
    assert n >= 0, "factor(n): negative input n = %r !" % n
    if n <= 3:
        return [n]
    if miller_rabin(n):
        return [n]
    else:
        c = pollard_rho(n)
        factor_list = factor(c) + factor(n // c)
    # factor_list.sort()
    return factor_list


def print_factors(factor_list):
    divisor_list = []
    cnt = Counter(factor_list)
    for fac, expo in sorted(cnt.items()):
        divisor = str(fac)
        if expo > 1:
            divisor += '^' + str(expo)
        divisor_list.append(divisor)
    return '*'.join(divisor_list)


num = int(input())
expr = str(num) + '=' + print_factors(factor(num))
print(expr)
