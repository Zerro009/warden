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
	int_64 *digits;
	uint_32 size;
	uchar sign;
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

struct bigint *bigint_construct_from_int(int_64 number) {
	struct bigint *result = bigint_construct(18);

	if (result == NULL) {
		return NULL;
	}

	if (number < 0) {
		result->sign = 1;
		number = -number;
	} else {
		result->sign = 0;
	}

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

void bigint_swap_sign(struct bigint *bignumber) {
	bignumber->sign = bignumber->sign ^ 1;
}

struct bigint *bigint_abs(const struct *bignumber) {
	struct bigint result = bigint_copy(bigumber);
	result->sign = 0;
	return result;
}

void bigint_copy(struct bigint *dest, const struct *source) {
	dest->sign = source->sign;
	dest->size = source->sign;
	for (int i = 0; i < source->size; i++) {
		dest->digits[i] = source->digits[i];
	}
}

int bigint_equal(const struct bigint *left, const struct bigint *right) {
	if (left->sign != right->sign) {
		return 0;
	}

	if (left->size == right->size) {
		for (int i = 0; i < left->size; i++) {
			if (left->digits[i] != right->digits[i]) {
				return 0;
			}
		}
	} else {
		int lesser_size = (left->size < right->size) ? left->size : right->size;
		const struct bigint *P = (left->size > right->size) ? left : right;
		for (int i = 0; i < lesser_size; i++) {
			if (left->digits[i] != right->digits[i]) {
				return 0;
			}
		}
		for (int i = lesser_size; i < P->size; i++) {
			if (P->digits[i] != 0) {
				return 0;
			}
		}
	}
	return 1;
}

int bigint_greater(const struct bigint *left, const struct bigint *right) {
	if (left->sign == 1 && right->sign == 0) {
		return 0;
	} else if (left->sign == 0 && right->sign == 1) {
		return 1;
	}
	if (left->size == right->size) {
		for (int i = 0; i < left->size; i++) {
			if (left->digits[i] > right->digits[i]) {
				return 1;
			}
		}
	} else {
		int lesser_size = (left->size < right->size) ? left->size : right->size;
		const struct bigint *P = (left->size > right->size) ? left : right;
		
		for (int i = lesser_size; i < P->size; i++) {
			if (P->digits[i]) {
				return 1;
			}
		}
		for (int i = lesser_size - 1; i >= 0; i--) {
			if (left->digits[i] > right->digits[i]) {
				return 1;
			}
		}
	}
	return 0;
}

struct bigint *bigint_addition(const struct bigint *left, const struct bigint *right) {
	if (left->sign == 1 && right->sign == 1) {
		struct bigint *p_left;
		bigint_copy(p_left, left);
		bigint_swap_sign(p_left);

		struct bigint *p_right;
		bigint_copy(p_right, right);
		bigint_swap_sign(p_right);

		struct bigint *result = bigint_addition(p_left, p_right);
		bigint_swap_sign(result);
		return result;
	} else if (left->sign == 0 && right->sign == 0) {
		int carry = 0;
		int bigger_size = (left->size >= right->size) ? left->size : right->size;

		struct bigint *result = bigint_construct(bigger_size * 9);
		result->sign = 0;

		for (int i = 0; i < bigger_size; i++) {
			if (i < left->size) {
				carry += left->digits[i];
			}
			if (i < right->size) {
				carry += right->digits[i];
			}
			result->digits[i] = carry % BIGINT_BASE;
			carry /= BIGINT_BASE;
		}
		if (carry > 0) {
			int_64 copy = (int_64 *) malloc((bigger_size + 1) * sizeof(int_64));
			for (int i = 0; i < bigger_size; i++) {
				copy[i] = result->digits[i];
			}
			copy[bigger_size] = carry;
			
			result->digits = copy;
			result->size++;
		}
		return result;
	} else if (left->sign == 1 && right->sign == 0) {
		return bigint_addition(right, left);
	} else if (left->sign == 0 && right->sign == 1) {
		
	}
}

int main(int argc, char *argv[]) {
	struct bigint *X = bigint_construct_from_int(1234567891);
	struct bigint *Y = bigint_construct_from_int(-1234567891);
	bigint_print(X);
	bigint_print(Y);
	return 0;
}
