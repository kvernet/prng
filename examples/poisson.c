#include "prng.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	if(argc < 3) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "\t%s LAMBDA N\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	const double lambda = strtod(argv[1], NULL);
	const long n = atol(argv[2]);
	
	prng * prng = mim_prng_init(0);
	
	long i;
	for(i = 0; i < n; i++) {
		double r = prng->poisson(prng, lambda);
		printf("%.8f\n", r);
	}
	
	/* free allocated memories */
	prng->destroy(&prng);
	
	return EXIT_SUCCESS;
}
