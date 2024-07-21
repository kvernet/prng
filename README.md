# Pseudo Random Number Generator (PRNG)

## Initialization
Two functions can be used to initialize the generator :
1. with a seed

```c
init_genrand(uint32_t s);
```

2. with an array of seeds

```c
init_by_array(uint32_t init_key[], int key_length);
```

## Generation
__PRNG__ can generate :

1. random 32-bit unsigned integers
```c
genrand_int32();
```
2. random real numbers in different ranges
```c
genrand_real1();
```

```c
genrand_real2();
```

```c
genrand_real3();
```
For example, the 2nd function generates random real numbers in [0, 1).