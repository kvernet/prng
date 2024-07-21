#ifndef MTWISTER_H
#define MTWISTER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void init_genrand(uint32_t s);
void init_by_array(uint32_t init_key[], int key_length);
uint32_t genrand_int32(void);
double genrand_real1(void);
double genrand_real2(void);
double genrand_real3(void);

#ifdef __cplusplus
}
#endif

#endif
