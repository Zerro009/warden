#include <stdlib.h>
#include <stdio.h>

#include "bigint.h"

int main(int argc, char *argv[]) {	
	if (argc != 4) {
		printf("Input bigint number, operation sign & another bigint!\n");
	} else {
		struct bigint *X = bigint_construct_from_int(atoll(argv[1]));
		char *operation = argv[2];
	
		if (*operation == '^') {
			int number = atoi(argv[3]);
			struct bigint *result = bigint_power_int(X, number);
			bigint_print(result);
			return 0;
		} else if (*operation == '+') {
			struct bigint *Y = bigint_construct_from_int(atoi(argv[3]));
			struct bigint *result = bigint_addition(X, Y);
			bigint_print(result);
			return 0;	
		} else if (*operation == '-') {
			struct bigint *Y = bigint_construct_from_int(atoi(argv[3]));
			struct bigint *result = bigint_subtraction(X, Y);
			bigint_print(result);
			return 0;
		} else if (*operation == '*') {
			struct bigint *Y = bigint_construct_from_int(atoi(argv[3]));
			struct bigint *result = bigint_multiplication(X, Y);
			bigint_print(result);
			return 0;
		} else if (*operation == '/') {
			struct bigint *Y = bigint_construct_from_int(atoi(argv[3]));
			struct bigint *result = bigint_division(X, Y);
			bigint_print(result);
			return 0;
		} else if (*operation == '%') {
			struct bigint *Y = bigint_construct_from_int(atoi(argv[3]));
			struct bigint *result = bigint_modulo(X, Y);
			bigint_print(result);
			return 0;
		} else {
			printf("Wrong operation!\n");
			return 1;
		}
	}
}
