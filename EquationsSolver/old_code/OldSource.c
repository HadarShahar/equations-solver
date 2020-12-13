#include <stdio.h>
#include <string.h>
#include <stdbool.h> 
#include <ctype.h>

/*
typedef struct {
	char letter;
	int value;
} variablev;
*/

void switch_opernad(char *op, char operands[2]) {
	if (*op == operands[0]) {
		*op = operands[1];
	}
	else {
		*op = operands[0];
	}
}

int main() {
	char *equation = "20X + 2X - 7 = 9X + 18 +1";
	
	int x_sum = 0;
	int nums_sum = 0;
	char operands[] = { '+', '-' };
	char last_op = '+';
	bool in_left_side = true;
	int last_num = 0;

	int length = strlen(equation);
	for (int i = 0; i < length; i++) {
		if (equation[i] == ' ') {
			continue;
		}
		if (isdigit(equation[i])) {
			last_num = last_num * 10 + (equation[i] - '0');
			continue;
		}

		char op = last_op;
		int *sum_ptr = &nums_sum;
		if (equation[i] == 'X') {
			sum_ptr = &x_sum;
			if (!in_left_side) { // put the X values in left side
				switch_opernad(&op, operands);
			}
		} else {
			if (in_left_side) { // put the numbers in the right side
				switch_opernad(&op, operands);
			}
		}

		if (op == '+') {
			*sum_ptr += last_num;
		} else if (op == '-') {
			*sum_ptr -= last_num;
		}
		last_num = 0;

		if (equation[i] == '=') {
			in_left_side = false;
			last_op = '+';
		} else {
			last_op = equation[i];
		}

		// printf("%c, %d %d\n", equation[i], x_sum, nums_sum);
	}
	nums_sum += last_num;
	
	int result = nums_sum / x_sum;
	printf("result: %d", result);

	return 0;
}