// From Senzee 5 post: Let Me Count The Ways
//
// Paul Senzee
//
// Lately, I've been tinkering with card games (poker) a bit and one of the 
// little questions that came up was what is the fastest way to enumerate all 
// possible combinations of 4 items out of a possible 64? Actually, it doesn't 
// have to be 4 of 64, it could be 7 of 48, or 2 specific aces from 52 cards or 
// 3 bits of 10. The field of combinatorics can tell us how many there are. This 
// is expressed as { n choose r } and has the formula factorial(n) / 
// (factorial(r) * factorial(n - r)). In the case of n = 64 and r = 4 it yields
// 635,376 different combinations.
//
// So, the task is to enumerate each of these unique combinations exactly one
// time until all 635,376 have been generated. This is one of those problems
// where the right approach makes all the difference.
//
// The Naive Approach
// 
// Let's look, for a moment, at the most basic brute force approach: iterate
// through every value that can be contained in 64 bits and check if it has four
// bits. How long would this take? On my machine it takes 1 second to check 250
// million numbers. Pretty fast, right? At this rate, however, it will take
// 2,340 years to check all 2^64 numbers! Clearly not a usable solution.
//
// A Better Approach
//
// Poking around the internet I found a function that takes a number and returns
// the next highest number with the same number of bits. I adapt it a bit and
// bam!

int enumerate_combinations(int n, int r, unsigned __int64 *v)
{
    unsigned __int64 y, r, x;
    unsigned         count = (unsigned)math::choose(n, r);
    v[0] = ((unsigned __int64)1 << r) - 1;
    for (unsigned i = 1; i < count; i++)
    {
        x    = v[i - 1];
        y    = x & -(__int64)x;
        r    = x + y;
        v[i] = r | (((x ^ r) >> 2) / y);
    }
    return count;
}

// This variation turns in 32 milliseconds for complete enumeration. Not a bad
// improvement over 2,340 years, eh?
// 
// The Best Approach
// 
// I actually developed the following function before the above. However, I
// figured (from looking at the code) that the above would blow this one away.
// How wrong I was. One problem with the above approach is that it has a nasty
// little division. Another thing is that this approach takes advantage of
// certain special cases like when r == 1 and n == r. The following approach is
// based on my initial recursive approach, but I removed the recursion so that I
// could rewrite it as an iterator. Removing the recursion did not seem to have
// a significant impact on performance. Anyhow the following runs to completion
// in just 3 milliseconds, over 10 times faster than the above version.

template <typename T = unsigned __int64>
int enumerate_combinations(int n, int r, T *v)
{
    struct { int n, r; T h; } s[sizeof(T) * 8] = { { n, r, 0 } }, q;
    int si  = 1, i = 0;
    T   one = 1;

    while (si)
    {
        q = s[--si];

    tail:

        if (q.r != 0)
        {
            one = 1;
            if (q.r == 1)
            {
                for (int j = 0; j < q.n; j++)
                {
                    v[i++] = q.h | one;
                    one <<= 1;
                }
            }
            else if (q.r == q.n)
            {
                v[i++] = q.h | (one << q.n) - 1;
            }
            else
            {
                --q.n; s[si++] = q; q.r--;
                q.h |= one << q.n;
                goto tail;
            }
        }
    }

    return i;
}