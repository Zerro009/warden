#include <stdlib.h>
#include <stdio.h>

#include "bigint.h"

int main(int argc, char *argv[]) {
	struct bigint *X = bigint_construct_from_int(atoi(argv[1]));
	int number = atoi(argv[2]);
	struct bigint *result = bigint_power_int(X, number);
	bigint_print(result);
	return 0;
}
