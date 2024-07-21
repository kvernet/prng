#ifndef PRNG_H
#define PRNG_H

#ifdef __cplusplus
extern "C" {
#endif

struct mim_prng {
    const unsigned long seed;
    
    void (*set_seed)(struct mim_prng * prng,
            const unsigned long s);
    void (*destroy)(struct mim_prng ** self_ptr);
    
    double (*uniform1)(struct mim_prng * prng);
    double (*normal)(struct mim_prng * prng,
            const double mu, const double sigma);
    double (*poisson)(struct mim_prng * prng,
            const double lambda);
    double (*weight)(struct mim_prng * prng);    
};

struct mim_prng * mim_prng_init(const unsigned long seed);

#ifdef __cplusplus
}
#endif

#endif
