#ifndef PRNG_H
#define PRNG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct prng prng;

struct prng {
    const unsigned long seed;
    
    void (*set_seed)(prng * prng,
            const unsigned long s);
    void (*destroy)(prng ** self_ptr);
    
    double (*uniform1)(prng * prng);
    double (*uniform2)(prng * prng);
    double (*uniform3)(prng * prng);
    double (*normal)(prng * prng,
            const double mu, const double sigma);
    double (*poisson)(prng * prng,
            const double lambda);
    double (*weight)(prng * prng);    
};

prng * mim_prng_init(const unsigned long seed);

#ifdef __cplusplus
}
#endif

#endif
