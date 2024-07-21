#include "mtwister.h"

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

/* The array for the state vector */
static uint32_t mt[N];
/* mti==N+1 means mt[N] is not initialized */
static int mti = N + 1;

void init_genrand(uint32_t s) {
    mt[0] = s & 0xffffffffUL;
    for (mti = 1; mti < N; mti++) {
        mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        mt[mti] &= 0xffffffffUL; /* for >32 bit machines */
    }
}

void init_by_array(uint32_t init_key[], int key_length) {
    int i, j, k;
    init_genrand(19650218UL);
    i = 1; j = 0;
    k = (N > key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL)) + init_key[j] + j;
        mt[i] &= 0xffffffffUL; /* for >32 bit machines */
        i++; j++;
        if (i >= N) { mt[0] = mt[N-1]; i = 1; }
        if (j >= key_length) j = 0;
    }
    for (k = N - 1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL)) - i;
        mt[i] &= 0xffffffffUL; /* for >32 bit machines */
        i++;
        if (i >= N) { mt[0] = mt[N-1]; i = 1; }
    }
    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
}

uint32_t genrand_int32(void) {
    uint32_t y;
    static uint32_t mag01[2] = {0x0UL, MATRIX_A};

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N + 1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk = 0; kk < N - M; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (; kk < N - 1; kk++) {
            y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
            mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
        mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

double genrand_real1(void) {
    return genrand_int32() * (1.0 / 4294967295.0); /* divided by 2^32-1 */
}

double genrand_real2(void) {
    return genrand_int32() * (1.0 / 4294967296.0); /* divided by 2^32 */
}

double genrand_real3(void) {
    return ((double)genrand_int32() + 0.5) * (1.0 / 4294967296.0); /* divided by 2^32 */
}

