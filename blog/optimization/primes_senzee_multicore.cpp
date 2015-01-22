/**
 * 

 Primes_Senzee_MultiCore is an optimized brute force (Sieve of Eratosthenes) implementation of the prime counting problem. 
 It uses no dynamically allocated memory and approximately 90k * # threads of stack.

 A release build of this implementation enumerates and counts the primes from 2 to 2**32-1 (4294967295) in ~7.15s
 on my dual core development machine.

 (c) 2006-2007
 Paul D. Senzee
 Senzee 5
 http://senzee.blogspot.com
 
 */

unsigned long Primes_Senzee_MultiCore(unsigned long lo, unsigned long hi);

//// Implementation ////

#include <string.h>
#define _WIN32_WINNT 0x0500
#include <windows.h>

namespace
{
__forceinline void _setnthbits(unsigned long start, unsigned long sz, unsigned long n, unsigned char *array);
unsigned long      _simpleeratosthenes(unsigned long *primes, unsigned long limit, unsigned char *array);
void               _populatebitcounts(unsigned char *bitcounts);
unsigned long      _countzerobits(const unsigned char *array, unsigned long size, const unsigned char *bitcounts);
unsigned long      _countprimes(unsigned long lo, unsigned long hi, const unsigned long *primes, unsigned char *array, const unsigned char *bitcounts);
DWORD __stdcall    _primethread(void *arg);
unsigned long      _isqrt(unsigned long x);

struct arguments_t
{
    unsigned long        lo, hi, result;
    const unsigned char *counts;
    unsigned char       *array;
    const unsigned long *primes;

    arguments_t(unsigned long lo = 0, unsigned long hi = 0, const unsigned char *counts = 0, unsigned char *array = 0, const unsigned long *primes = 0) :
        lo(lo), hi(hi), counts(counts), array(array), primes(primes), result(0) {}
};

}

/**
 * 

 Primes_Senzee_MultiCore is an optimized brute force (Sieve of Eratosthenes) implementation of the prime counting problem. 
 It uses no dynamically allocated memory and approximately 90k * thread count of stack.

 A release build of this implementation enumerates and counts the primes from 2 to 2**32-1 (4294967295) in ~7.15s
 on my dual core development machine.
 
 */
unsigned long Primes_Senzee_MultiCore(unsigned long lo, unsigned long hi)
{
    enum { THREAD_COUNT = 2, NO_THREAD_THRESHOLD = 1 << 22 };    // used a binary search timing algorithm to find the optimal-ish NO_THREAD_THRESHOLD
    const unsigned long RANGE = 1024 * 1024, PRIME_COUNT = 6541; // RANGE = 1m numbers at a time; PRIME_COUNT = number of primes less than 65536
    unsigned char       counts[256], array[THREAD_COUNT][RANGE / 16 + 1];
    unsigned long       primes[PRIME_COUNT + 1], count = 0, mid = 0, r = 0;
    _simpleeratosthenes(primes, _isqrt(hi) + 1, array[0]);
    _populatebitcounts(counts);
    if (hi - lo <= NO_THREAD_THRESHOLD)
    {
        arguments_t arg(lo, hi, counts, array[0], primes);
        _primethread(&arg);
        r = arg.result;
    }
    else
    {
        for (int i = 1; i < THREAD_COUNT; i++)
            memcpy(array[i], array[0], sizeof(array[0]));
        arguments_t arg[THREAD_COUNT];
        unsigned long delta = (hi - lo) / THREAD_COUNT;
        for (int i = 0; i < THREAD_COUNT; i++)
            arg[i] = arguments_t(lo + delta * i, i == THREAD_COUNT - 1 ? hi : lo + delta * (i + 1) - 1, counts, array[i], primes);
        HANDLE threads[THREAD_COUNT];
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            threads[i] = ::CreateThread(NULL, 0, _primethread, &arg[i], CREATE_SUSPENDED, NULL);
            ::SetThreadIdealProcessor(threads[i], i);   
            ::SetPriorityClass(threads[i], REALTIME_PRIORITY_CLASS);
            ::SetThreadPriority(threads[i], THREAD_PRIORITY_TIME_CRITICAL);
            ::ResumeThread(threads[i]);
        }
        ::WaitForMultipleObjects(THREAD_COUNT, threads, true, INFINITE);
        for (int i = 0; i < THREAD_COUNT; i++)
            r += arg[i].result;
    }
    return r;
}

namespace
{

DWORD __stdcall _primethread(void *p)
{
    arguments_t *arg = (arguments_t *)p;
    enum { RANGE = 1024 * 1024 };
    unsigned count = 0, mid = 0, lo = arg->lo, hi = arg->hi;
    if (lo < 3) { lo = 3; count = 1; }
    mid = lo + RANGE;
    // calculate the primes in a given window, RANGE numbers at a time
    while (true)
    {
        if (mid > hi || mid < RANGE /** rolled over.. */) mid = hi;
        count += _countprimes(lo, mid, arg->primes, arg->array, arg->counts);
        if (mid == hi) break;
        lo = mid + 1;
        mid += RANGE;
    }
    arg->result = count;
    return 0;
}

__forceinline void _setnthbits(unsigned long start, unsigned long sz, unsigned long n, unsigned char *array)
{
    //for (unsigned long i = start; i < sz; i += n)
    //    array[i >> 3] |= 1 << (i & 7);
    __asm
    {
        mov esi, start
        mov edi, sz
        cmp esi, edi
        jae done
        mov eax, n
        mov ebx, array
repeat:
        mov ecx, esi
        mov edx, 1
        and ecx, 7
        shl edx, cl
        mov ecx, esi
        shr ecx, 3
        add ecx, ebx
        add esi, eax
        or [ecx], dl
        cmp esi, edi
        jb  repeat
done:
    }
}

unsigned long _isqrt(unsigned long x)
{
    unsigned long a = 1, b = (x >> 5) + 8, m = 0;
    if (b > 65535) b = 65535;
    do
    {
        m = (a + b) >> 1;
        if (m * m > x) b = m - 1;
        else           a = m + 1;
    } while (b >= a);
    return a - 1;
}

unsigned long _simpleeratosthenes(unsigned long *primes, unsigned long limit, unsigned char *array)
{
    memset(array, 0, limit);
    unsigned long szq = _isqrt(limit);
    for (unsigned long i = 3; i <= szq; i += 2)
        for (unsigned long j = i + i; j < limit; j += i)
            array[j] = 1;
    unsigned long c = 0;
    for (unsigned long i = 3; i < limit; i += 2)
        if (!array[i]) primes[c++] = i;
    primes[c] = 0;
    return c;
}

void _populatebitcounts(unsigned char *bitcounts)
{
    const unsigned char bits[4] = { 0, 1, 1, 2 };
    for (unsigned int i = 0; i < 256; i++)
        bitcounts[i] = bits[i >> 6] + bits[(i >> 4) & 3] + bits[(i >> 2) & 3] + bits[i & 3];
}

unsigned long _countzerobits(const unsigned char *array, unsigned long size, const unsigned char *bitcounts)
{
    unsigned long count = 0;
    for (unsigned long i = 0; i < size; i++)
        count += bitcounts[(unsigned char)~array[i]];
    return count;
}

unsigned long _countprimes(unsigned long lo, unsigned long hi, const unsigned long *primes, unsigned char *array, const unsigned char *bitcounts)
{
    if (!(lo & 1)) lo++;
    if (!(hi & 1)) hi--;
    const unsigned long sz  = hi - lo + 1, asz = (sz + 15) >> 4, innersz = (sz + 1) >> 1;
    unsigned long       szq = _isqrt(hi) + 1;
    memset(array, 0, asz);
    unsigned char fbits = (unsigned char)((asz << 4) - sz) >> 1;     // mark the end bits (representing numbers > hi) in the byte as non-prime
    array[asz - 1] |= ((1 << (fbits + 1)) - 1) << (8 - fbits);
    unsigned long i = 0, of = 0, j = 0;                              // populate
    while ((i = *primes) && *primes <= szq)
    {        
        if (i >= lo) j = i + i + i;
        else { j = (lo + (i - 1)) / i * i; if (!(j & 1)) j += i; }
        j = (j - lo) >> 1;
        _setnthbits(j, innersz, i, array);
        primes++;
    }
    return _countzerobits(array, asz, bitcounts);                    // count
}

}