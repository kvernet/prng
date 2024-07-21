#include "mtwister.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;

    /* Initialize the generator with a seed */
    init_genrand(1234);

    /* Print some random numbers */
    printf("Random numbers:\n");
    for (i = 0; i < 10; i++) {
        printf("%10u\n", genrand_int32());
    }

    /* Initialize the generator with an array */
    uint32_t init[4] = {0x123, 0x234, 0x345, 0x456};
    init_by_array(init, 4);

    /* Print some random numbers */
    printf("Random numbers (initialized by array):\n");
    for (i = 0; i < 10; i++) {
        printf("%10u\n", genrand_int32());
    }

    /* Print some random real numbers */
    printf("Random real numbers [0,1):\n");
    for (i = 0; i < 10; i++) {
        printf("%10.8f\n", genrand_real2());
    }

    return EXIT_SUCCESS;
}
