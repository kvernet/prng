#include "prng.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	if(argc < 2) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "\t%s N\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	const int n = atoi(argv[1]);
	
	prng * prng = mim_prng_init(0);
	
	int i;
	for(i = 0; i < n; i++) {
		double r1 = prng->uniform1(prng);
		double r2 = prng->uniform2(prng);
		double r3 = prng->uniform3(prng);
		printf("%.8f %.8f %.8f\n", r1, r2, r3);
	}
	
	/* free allocated memories */
	prng->destroy(&prng);
	
	return EXIT_SUCCESS;
}
