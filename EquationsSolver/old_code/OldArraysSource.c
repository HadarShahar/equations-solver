#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

int count_operands(char *expression) {
	int count = 0;
	//for (int i = 0; expression[i] != '\0'; ++i) {
	while (*expression) {
		if (strchr(VALID_OPERANDS, *expression++)) { // if the char is in OPERANDS
			count++;
		}
	}
	return count;
}

term** create_terms(char *expression) {
	int operands_num = count_operands(expression);

	// calloc() function allocates memory and initializes all bits to zero
	term **terms = (term**) calloc(operands_num+2, sizeof(term*));
	if (!terms) {
		exit(0);
	}
	int terms_index = 0;
	char last_operand = '+';
	while (*expression) {
		if (strchr(VALID_OPERANDS, *expression)) {
			last_operand = *expression;
		} else if (*expression != ' '){

			term *t = terms[terms_index++];
			t = (term*) malloc(sizeof(term));
			if (!t) { 
				exit(0); 
			}
			char prev_ch = *expression;
			t->coefficient = strtod(expression, &expression);
			if (isalpha(*expression)) {
				if (!isdigit(prev_ch)) { // X => 1*X
					t->coefficient = 1;
				}
				t->variable = *expression;
				expression++;
			} else {
				t->variable = NULL;
			}
			if (last_operand == '-') {
				t->coefficient *= -1;
			}
			// printf("'%lf %c'\n", t->coefficient, t->variable);	
		}
		expression++;
	}
	return terms;
}

void free_terms(term **terms) {
	while (*terms) {
		free(*terms++);
	}
	free(terms);
}

int main() {
	char *equation = "-2.5X + X + 2 = 8"; // X = -4
	printf("solving equation: %s", equation);

	char *equal_ptr = strchr(equation, '=');
	int left_expression_len = equal_ptr - equation + 1;
	char *left_expression = (char*) malloc(left_expression_len * sizeof(char));
	if (!left_expression) {
		exit(0);
	}
	memcpy(left_expression, equation, left_expression_len);
	left_expression[left_expression_len - 1] = '\0';
	char *right_expression = equal_ptr + 1;

	printf("'%s'\n", left_expression);
	printf("'%s'\n", right_expression);

	term **left_terms = create_terms(left_expression);
	term **right_terms = create_terms(right_expression);


	free_terms(left_terms);
	free_terms(right_terms);

	free(left_expression);
	free(right_expression);

	return 0;
}