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
	int size = digits;
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
	for (int i = 0; i < size; i++) result->digits[i] = 0;
	result->sign = 0;
	return result;
}

struct bigint *bigint_construct_from_int(int_64 number) {
	struct bigint *result = bigint_construct(2);

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

void bigint_resize(struct bigint *bignumber, int size) {
	bignumber->size = size;

	bignumber->digits = (int_64 *) malloc(size * sizeof(int_64));
	for (int i = 0; i < size; i++) {
		bignumber->digits[i] = 0;
	}
}

struct bigint *bigint_copy(struct bigint *source) {
	struct bigint *result = (struct bigint*) malloc(sizeof(struct bigint));
	
	result->digits = (int_64 *) malloc(source->size * sizeof(int_64));
	result->size = source->size;
	result->sign = source->sign;

	for (int i = 0; i < source->size; i++) {
		result->digits[i] = source->digits[i];
	}
	return result;
}

void bigint_allocation_normalize(struct bigint *bignumber) {
	int garbage = 0;
	for (int i = bignumber->size - 1; i > 0 && bignumber->digits[i] == 0; i--, garbage++)
		{}
	if (garbage) {
		bignumber->size -= garbage;
		bignumber = bigint_copy(bignumber);
	}
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

struct bigint *bigint_abs(struct bigint *bignumber) {
	struct bigint *result = bigint_copy(bignumber);
	result->sign = 0;
	return result;
}

int bigint_is_zero(struct bigint *bignumber) {
	for (int i = 0; i < bignumber->size; i++) {
		if (bignumber->digits[i] != 0) {
			return 0;
		}
	}
	return 1;
}

int bigint_equal(struct bigint *left, struct bigint *right) {
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
		struct bigint *P = (left->size > right->size) ? left : right;
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

int bigint_greater(struct bigint *left, struct bigint *right) {
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
		struct bigint *P = (left->size > right->size) ? left : right;
		
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

struct bigint *bigint_addition(struct bigint *left, struct bigint *right) {
	if (left->sign == 1 && right->sign == 1) {
		struct bigint *p_left = bigint_copy(left);
		bigint_swap_sign(p_left);

		struct bigint *p_right = bigint_copy(right);
		bigint_swap_sign(p_right);

		struct bigint *result = bigint_addition(p_left, p_right);
		bigint_swap_sign(result);

		bigint_allocation_normalize(result);
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
			int_64 *copy = (int_64 *) malloc((bigger_size + 1) * sizeof(int_64));
			for (int i = 0; i < bigger_size; i++) {
				copy[i] = result->digits[i];
			}
			copy[bigger_size] = carry;
			
			result->digits = copy;
			result->size++;
		}

		bigint_allocation_normalize(result);
		return result;

	} else if (left->sign == 1 && right->sign == 0) {
		struct bigint *result = bigint_addition(right, left);
		bigint_allocation_normalize(result);
		return result;

	} else if (left->sign == 0 && right->sign == 1) {
		if (!(bigint_greater(left, right)) && !(bigint_equal(left, right))) {
			struct bigint *p_left = bigint_copy(left);
			bigint_swap_sign(p_left);

			struct bigint *p_right = bigint_copy(right);
			p_right = bigint_abs(p_right);

			struct bigint *result = bigint_addition(right, left);
			bigint_swap_sign(result);
			bigint_allocation_normalize(result);
			return result;
		}
		struct bigint *result = bigint_copy(left);

		for (int i = 0; i < right->size; i++) {
			if (right->digits[i] > result->digits[i]) {
				result->digits[i] += BIGINT_BASE;
				result->digits[i+1]--;
			}
			result->digits[i] -= right->digits[i];
		}

		for (int i = 0; i < right->size; i++) {
			if (result->digits[i] < 0) {
				result->digits[i] += BIGINT_BASE;
				result->digits[i+1]--;
			}
		}
		bigint_allocation_normalize(result);
		return result;
	}
	return NULL;
}

struct bigint *bigint_addition_int(struct bigint *bignumber, int_64 number) {
	return bigint_addition(bignumber, bigint_construct_from_int(number));
}

struct bigint *bigint_subtraction(struct bigint *left, struct bigint *right) {
	struct bigint *p_right = bigint_copy(right);
	bigint_swap_sign(p_right);
	return bigint_addition(left, p_right);
}

struct bigint *bigint_multiplication(struct bigint *left, struct bigint *right) {
	if (bigint_is_zero(left) || bigint_is_zero(right)) {
		return bigint_construct_from_int(0);
	}
	
	int size = (left->size * right->size) + 1;
	struct bigint *result = bigint_construct(size);
	result->size = size;
	result->digits = (int_64 *) malloc(size * sizeof(int_64));

	if (left->sign != right->sign) {
		result->sign = 1;
	}

	for (int i = 0; i < left->size; i++) {
		for (int j = 0; j < right->size; j++) {
			int_64 temp = result->digits[i+j];
			temp += 1 * left->digits[i] * right->digits[j];
			result->digits[i+j] = temp % BIGINT_BASE;
			temp /= BIGINT_BASE;
			result->digits[i + j + 1] += temp;
		}
	}

	for (int i = 0; i < result->size; i++) {
		if (result->digits[i] > BIGINT_BASE) {
			result->digits[i + 1] += result->digits[i] / BIGINT_BASE;
			result->digits[i] %= BIGINT_BASE;
		}
	}
	bigint_allocation_normalize(result);
	return result;
}

struct bigint *bigint_multiplication_int(struct bigint *bignumber, int_64 number) {
	return bigint_multiplication(bignumber, bigint_construct_from_int(number));
}

struct bigint *bigint_power_int(struct bigint *base, int number) {
	if (number == 0) {
		return bigint_construct_from_int(1);
	} else if (number == 1) {
		return base;
	} else {
		struct bigint *result = bigint_copy(base);
	
		for (int i = 0; i < number - 1; i++) {
			result = bigint_multiplication(result, base);
			bigint_allocation_normalize(result);
		}
	return result;
	}
	return NULL;
}

struct bigint *bigint_division_int(struct bigint *bignumber, int_64 number) {
	struct bigint *result = bigint_copy(bignumber);
	int value = number;

	if (value < 0) {
		bigint_swap_sign(result);
		value = -value;
	}

	int_64 carry = 0;
	for (int i = result->size - 1; i >= 0; i--) {
		carry *= BIGINT_BASE;
		carry += result->digits[i];
		result->digits[i] = carry / value;
		carry %= value;
	}

	bigint_allocation_normalize(result);
	return result;
}

struct bigint *bigint_division_modulo(struct bigint *left, struct bigint *right, uchar mode) {
	int norm = BIGINT_BASE / (right->digits[right->size - 1] + 1);
	struct bigint *X_left = bigint_multiplication_int(bigint_abs(left), norm);
	struct bigint *X_right = bigint_multiplication_int(bigint_abs(right), norm);

	struct bigint *q = bigint_construct(1);
	struct bigint *r = bigint_construct(1);

	q->sign = 0;
	bigint_resize(q, left->size);

	for (int i = X_left->size - 1; i >= 0; i--) {
		r = bigint_multiplication_int(r, BIGINT_BASE);
		r = bigint_addition_int(r, X_left->digits[i]);

		int s1 = r->size <= X_right->size ? 0 : r->digits[X_right->size];
		int s2 = r->size <= X_right->size - 1 ? 0 : r->digits[X_right->size - 1];
		int d = (1 * BIGINT_BASE * s1 + s2) / X_right->digits[X_right->size-1];

		r = bigint_subtraction(r, bigint_multiplication_int(X_right, d));
		while (r->sign == 1) {
			r = bigint_addition(r, X_right);
			d--;
		}
		q->digits[i] = d;
	}

	q->sign = X_left->sign ^ X_right->sign;
	r->sign = X_left->sign;

	bigint_allocation_normalize(q);
	bigint_allocation_normalize(r);

	if (mode == 0) {
		return q;
	} else if (mode == 1) {
		return bigint_division_int(r, norm);
	} else {
		return NULL;
	}
	return NULL;
}

struct bigint *bigint_division(struct bigint *left, struct bigint *right) {
	return bigint_division_modulo(left, right, 0);
}

struct bigint *bigint_modulo(struct bigint *left, struct bigint *right) {
	return bigint_division_modulo(left, right, 1);
}

int main(int argc, char *argv[]) {
	struct bigint *X = bigint_construct_from_int(999999999);
	struct bigint *Z = bigint_power_int(X, 1000);
	bigint_print(Z);
	return 0;
}
