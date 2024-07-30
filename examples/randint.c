#include "prng.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	if(argc < 2) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "\t%s N\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	const long n = atol(argv[1]);
	
	prng * prng = prng_init(0);
	
	long i;
	for(i = 0; i < n; i++) {
		unsigned long r = prng->randint(prng);
		printf("%ld\n", r);
	}
	
	/* free allocated memories */
	prng->destroy(&prng);
	
	return EXIT_SUCCESS;
}
