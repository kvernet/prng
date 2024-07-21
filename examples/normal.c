#include "prng.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	if(argc < 4) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "\t%s MU SIGMA N\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	const double mu = strtod(argv[1], NULL);
	const double sigma = strtod(argv[2], NULL);
	const long n = atol(argv[3]);
	
	prng * prng = mim_prng_init(0);
	
	long i;
	for(i = 0; i < n; i++) {
		double r = prng->normal(prng, mu, sigma);
		printf("%.8f\n", r);
	}
	
	/* free allocated memories */
	prng->destroy(&prng);
	
	return EXIT_SUCCESS;
}
