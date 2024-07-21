#include "prng.h"
#include "mtwister.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include <time.h>

typedef struct wprng {
    prng pub;    
    const double weight;    /* Monte Carlo weight */
}wprng;

static unsigned long randint(prng * prng);
static double uniform1(prng * prng);
static double uniform2(prng * prng);
static double uniform3(prng * prng);
static double normal(prng * prng,
        const double mu, const double sigma);
static double poisson(prng * prng,
        const double lambda);
static double weight(prng * prng);

static void set_seed(prng * prng,
        const unsigned long s) {
    *(unsigned long*)&prng->seed = s;
    /* set seed */
    init_genrand(s);
}

static void mim_prng_destroy(prng ** self_ptr) {
    if(*self_ptr == NULL) return;
    free(*self_ptr);
    self_ptr = NULL;
}

prng * mim_prng_init(const unsigned long seed) {
    wprng * prng = malloc(sizeof(*prng));
    
    if(seed > 0) {
        *(unsigned long*)&prng->pub.seed = seed;
    }
    else {
        FILE * stream = fopen("/dev/urandom", "rb");
        if(stream == NULL) *(unsigned long*)&prng->pub.seed = time(NULL);
        else {
            unsigned int seeds[2];
            size_t size = fread(seeds, sizeof seeds, 1, stream);
            if(size <= 0) *(unsigned long*)&prng->pub.seed = time(NULL);
            *(unsigned long*)&prng->pub.seed = seeds[0];
            fclose(stream);
        }
    }
    
    prng->pub.set_seed = &set_seed;
    prng->pub.destroy = &mim_prng_destroy;
    
    prng->pub.randint = &randint;
    prng->pub.uniform1 = &uniform1;
    prng->pub.uniform2 = &uniform2;
    prng->pub.uniform3 = &uniform3;
    prng->pub.normal = &normal;
    prng->pub.poisson = &poisson;
    prng->pub.weight = &weight;    
    
    /* default weight */
    *(double*)&prng->weight = 1.;
    
    /* Set seed */
    init_genrand(prng->pub.seed);
    
    return &prng->pub;
}


static unsigned long randint(prng * prng) {
	wprng * p = (void*)prng;
    *(double*)&p->weight = 1;
    return genrand_int32();
}

static double uniform1(prng * prng) {
    wprng * p = (void*)prng;
    *(double*)&p->weight = 1;
    return genrand_real1();
}

static double uniform2(prng * prng) {
    wprng * p = (void*)prng;
    *(double*)&p->weight = 1;
    return genrand_real2();
}

static double uniform3(prng * prng) {
    wprng * p = (void*)prng;
    *(double*)&p->weight = 1;
    return genrand_real3();
}

double normal(prng * prng,
        const double mu, const double sigma) {
    wprng * p = (void*)prng;
    
    if(sigma == 0) {
        *(double*)&p->weight = 1.;
        return mu;
	}
	
	const double u = uniform1(prng);
	const double v = uniform1(prng);
	
	const double two_pi = 2 * M_PI;
	const double mag = sqrt(-2 * log(u));
	
	double x = sigma * mag * cos(two_pi * v) + mu;
	
	// compute weight = inv(pdf)
	const double xi = (x - mu) / sigma;
	const double nume = exp(-0.5 * xi * xi);
	const double deno = sigma * sqrt(two_pi);
	*(double*)&p->weight = deno / nume;
	
	return x;
}

static double poisson_knuth(prng * prng,
        const double lambda);
static double poisson_cook(prng * prng,
        const double lambda);
double poisson(prng * prng,
        const double lambda) {
    if(isnan(lambda)) return lambda;
    if(fabs(lambda) >= DBL_MAX) return DBL_MAX;
    else if(lambda < 30) return poisson_knuth(prng, lambda);
    else return poisson_cook(prng, lambda);
}

static double weight(prng * prng) {
    wprng * p = (void*)prng;
    return p->weight;
}

long long compute_factorial_minus_one(long long k) {
    long long fact = 1, i;
    for(i = 2; i < k - 1; i++) {
        fact *= i;
    }
    return fact;
}

double compute_poisson_weight(const double lambda,
        const long long k, const double x) {
    long long fact = compute_factorial_minus_one(k);
    
    const double pdf = exp(-lambda) * pow(lambda, x) / (double)fact;
    return 1 / pdf;
}

// algorithm https://fr.wikipedia.org/wiki/Loi_de_Poisson
double poisson_knuth(prng * prng,
        const double lambda) {
    wprng * rng = (void*)prng;
    
    double p = 1;
    long long k = 0;
    
    while(p > exp(-lambda)) {
        const double u = uniform1(prng);
        if((u <= 0.) || (u >= 1.)) continue;
        
        p *= u;
        k += 1;
    }
    
    double x = (double) (k - 1);
    
    *(double*)&rng->weight = compute_poisson_weight(
            lambda, k, x);
    
    return x;
}
// reference: https://www.johndcook.com/blog/2010/06/14/generating-poisson-random-values/
double poisson_cook(prng * prng,
        const double lambda) {
    wprng * rng = (void*)prng;
    
    const double c = 0.767 - 3.36 / lambda;
    const double beta = M_PI / sqrt(3.0 * lambda);
    const double alpha = beta * lambda;
    const double k = log(c) - lambda - log(beta);
    
    while(true) {
        const double u = uniform1(prng);
        const double x = (alpha - log((1.0 - u)/u)) / beta;
        const long n = floor(x + 0.5);
        if(n < 0) continue;
        
        const double v = uniform1(prng);
        const double y = alpha - beta * x;
        const double t = 1.0 + exp(y);
        const double lhs = y + log(v / (t*t));
        // lgamma c++ function https://en.cppreference.com/w/cpp/numeric/math/lgamma
        // N.B log(n!) ~= lgamma(n + 1)
        const double rhs = k + n*log(lambda) - lgamma(n + 1);
        
        if(lhs <= rhs) {
            /* FIXME */
            *(double*)&rng->weight = compute_poisson_weight(
                    lambda, n + 1, n);
            
            return n;
        }
    }
}
