#ifndef __BIGINT_H__
#define __BIGINT_H__

#define BIGINT_BASE 1000000000

#ifndef uchar
typedef unsigned char uchar;
#endif

#ifndef uint_32
typedef unsigned int uint_32;
#endif

#ifndef uint_64
typedef unsigned long long uint_64;
#endif

#ifndef int_64
typedef long long int_64;
#endif

struct bigint {
        int_64 *digits;
        uint_32 size;
        uchar sign;
};

struct bigint *bigint_construct(int size);

struct bigint *bigint_construct_from_int(int_64 number);

void bigint_destruct(struct bigint *bignumber);

void bigint_resize(struct bigint *bignumber, int size);

struct bigint *bigint_copy(struct bigint *source);

void bigint_allocation_normalize(struct bigint *bignumber);

void bigint_print(struct bigint *bignumber);

void bigint_swap_size(struct bigint *bignumber);

struct bigint *bigint_abs(struct bigint *bignumber);

int bigint_is_zero(struct bigint *bignumber);

int bigint_equal(struct bigint *left, struct bigint *right);

int bigint_greater(struct bigint *left, struct bigint *right);

struct bigint *bigint_addition(struct bigint *left, struct bigint *right);

struct bigint *bigint_addition_int(struct bigint *bignumber, int_64 number);

struct bigint *bigint_subtraction(struct bigint *left, struct bigint *right);

struct bigint *bigint_multiplication(struct bigint *left, struct bigint *right);

struct bigint *bigint_multiplication_int(struct bigint *bignumber, int_64 number);

struct bigint *bigint_power_int(struct bigint *base, int_64 number);

struct bigint *bigint_division_int(struct bigint *bignumber, int_64 number);

struct bigint *bigint_division_modulo(struct bigint *left, struct bigint *right, uchar mode);

struct bigint *bigint_division(struct bigint *left, struct bigint *right);

struct bigint *bigint_modulo(struct bigint *left, struct bigint *right);

#endif
