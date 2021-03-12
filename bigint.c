#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	uint_64 *digits;
	uint_32 size;
	uchar sign:1;
};

struct bigint *bigint_construct(int digits) {
	int size = digits / 9 + ((digits % 9 != 0) ? 1 : 0);
	if (size <= 0) {
		return NULL;
	}
	struct bigint *result = malloc(sizeof(struct bigint));
	if (result == NULL) {
		return NULL;
	}
	result->size = size;
	result->digits = malloc(size * sizeof(int_64));
	if (result->digits == NULL) {
		free(result);
		return NULL;
	}
	memset(result->digits, 0, size * sizeof(int_64));
	result->sign = 0;
	return result;
}

struct bigint *bigint_construct_from_uint(int_64 number) {
	int temp = (number >= BIGINT_BASE) ? 18 : 9;
	struct bigint *result = bigint_construct(temp);
	if (result == NULL) {
		return NULL;
	}
	result->sign = 0;
	if (number >= BIGINT_BASE) {
		result->digits[0] = number % BIGINT_BASE;
		result->digits[1] = number / BIGINT_BASE;
	} else {
		result->digits[0] = number;
	}
	return result;
}

void bigint_destruct(struct bigint *bignumber) {
	free(bignumber->digits);
	free(bignumber);
}

void bigint_swap_sign(struct bigint *bignumber) {
	bignumber->sign = bignumber->sign ^ 1;
}

void bigint_print(struct bigint *bignumber) {
	if (bignumber->sign == 1) {
		printf("-");
	}
	for (int i = 0; i < bignumber->size; i++) {
		if (i == bignumber->size - 1) {
			printf("%0.9llu", bignumber->digits[i]);
		} else {
			printf("%0.9llu.", bignumber->digits[i]);
		}
	}
	printf("\n");
}

struct bigint *bigint_addition(struct bigint *left, struct bigint *right) {
	if (left->size < right->size) {
		return bigint_addition(right, left);
	}

	struct bigint *result = bigint_construct(left->size * 9 + 1);
	if (result == NULL) {
		return NULL;
	}

	int carry = 0;
	int tmp = 0;

	int i;
	for (i = 0; i < right->size; i++) {
		tmp = left->digits[i] + right->digits[i] + carry;
		result->digits[i] = tmp % BIGINT_BASE;
		carry = tmp / BIGINT_BASE;
	}
	while (carry != 0 && i < left->size) {
		tmp = left->digits[i] + carry;
		result->digits[i] = tmp % BIGINT_BASE;
		carry = tmp / BIGINT_BASE;
		i++;
	}
	return result;

}

int main(int argc, char *argv[]) {
	struct bigint *X = bigint_construct_from_uint(1234);
	struct bigint *Y = bigint_construct_from_uint(1234);
	struct bigint *Z = bigint_addition(X, Y);
	bigint_print(X);
	bigint_print(Y);
	bigint_print(Z);
	printf("%d\n", Z->size);
	return 0;
}
