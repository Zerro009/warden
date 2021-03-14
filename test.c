#include <stdio.h>

#include "bigint.h"

int main(int argc, char *argv[]) {
	struct bigint *X = bigint_construct_from_int(999999999);
	struct bigint *Y = bigint_power_int(X, 5000);
	bigint_print(Y);
	return 0;
}
