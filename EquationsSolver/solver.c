#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "terms.h"
#include "solver.h"

void cannot_solve_equation() {
	printf("Can't solve this equation yet...");
	exit(EXIT_FAILURE);
}

equation *parse_equation(char *equation_string) {
	char* equal_ptr = strchr(equation_string, '=');
	int left_expression_len = equal_ptr - equation_string + 1;
	char* left_expression = (char*)malloc_or_die(left_expression_len * sizeof(char));
	memcpy(left_expression, equation_string, left_expression_len);
	left_expression[left_expression_len - 1] = '\0';
	char* right_expression = equal_ptr + 1;

	/*printf("left_expression: '%s'\n", left_expression);
	printf("right_expression: '%s'\n", right_expression);*/

	node_t *left_terms = create_terms(left_expression);
	node_t *right_terms = create_terms(right_expression);

	free(left_expression);

	equation *e = (equation*)malloc_or_die(sizeof(equation));
	e->left_terms = left_terms;
	e->right_terms = right_terms;
	return e;
}

void print_equation(equation *e) {
	print_terms(e->left_terms);
	printf(" = ");
	print_terms(e->right_terms);
	printf("\n");
}

bool is_solved(equation *e) {
	// sample solved equation: X = 1
	term *left = e->left_terms->val;
	term *right = e->right_terms->val;
	return e->left_terms->next == NULL && e->right_terms->next == NULL &&
		   left->coefficient == 1 && left->variable != NULL && right->variable == NULL;
}

void solve_equation(equation *e) {
	if (is_solved(e)) {
		return;
	}
	char var = get_first_var(e->left_terms);
	if (!var) {
		var = get_first_var(e->right_terms);
	}
	printf("Solving for: %c\n", var);
	print_equation(e);

	node_t **ptr_left_terms = &e->left_terms;
	node_t **ptr_right_terms = &e->right_terms;

	// move to the right side all the terms that	don't contain	the variable 'var'
	move_terms(&term_isnot_var, var, ptr_left_terms, ptr_right_terms);

	// move to the left side all the terms that		contain			the variable 'var'
	move_terms(&term_is_var, var, ptr_right_terms, ptr_left_terms);

	print_equation(e);

	double divider = e->left_terms->val->coefficient;
	e->left_terms->val->coefficient = 1;

	if (divider != 1) {
		// divide all the terms in the right side with the left term's coefficient
		node_t *current = e->right_terms;
		while (current) {
			current->val->coefficient /= divider;
			current = current->next;
		}
		print_equation(e);
	}
}

// Substitues var = var_terms in the list *ptr_other_terms.
// Returns true if the variable was substitued, false if it wan't in the equation.
bool substitue_variable(char var, node_t *var_terms, node_t **ptr_other_terms) {
	bool substitued = false;

	node_t *dummy_head = (node_t*) malloc_or_die(sizeof(node_t));
	node_t *previous = dummy_head;
	node_t *current = *ptr_other_terms;
	dummy_head->next = current;

	while (current) {
		if (current->val->variable == var) {
			current->val->variable = NULL;
			substitute_expression('*', copy_terms(var_terms), previous, &current); 
			substitued = true;
		}
		previous = current;
		current = current->next;
	}

	*ptr_other_terms = dummy_head->next;
	free(dummy_head);
	return substitued;
}

// Substitues each equation in the array equations until equations_num,
// in the equation at e_index in the array.
void substitue_equations(equation **equations, int equations_num, int e_index) {
	equation *e1 = equations[e_index];
	for (int i = 0; i < equations_num; i++) {
		if (i == e_index) {
			continue;
		}
		equation *e2 = equations[i];
		// subtitue e2 in e1
		// at this point e2->left_terms->val->coefficient = 1
		char subtitued_var = e2->left_terms->val->variable;
		bool sub_in_left = substitue_variable(subtitued_var, e2->right_terms, &e1->left_terms);
		bool sub_in_right = substitue_variable(subtitued_var, e2->right_terms, &e1->right_terms);
		if (sub_in_left || sub_in_right) {
			printf("Substituing equation (%d) in equation (%d)\n", (i+1), (e_index+1));
		}
	}
}

void solve_all_equations(char **equations_strings, int equations_num) {
	equation **equations = (equation*) malloc_or_die(equations_num * sizeof(equation*));
	for (int i = 0; i < equations_num; i++) {
		printf("Parsing equation (%d): '%s'\n", (i+1), equations_strings[i]);
		equations[i] = parse_equation(equations_strings[i]);
		// the equations array is filled during the loop, 
		// so at each iteration there are just i equations
		substitue_equations(equations, i, i);
		solve_equation(equations[i]);
		printf("\n");
	}

	for (int i = 0; i < equations_num; i++) {
		substitue_equations(equations, equations_num, i);
		solve_equation(equations[i]);
		printf("\n");
	}

	printf("================================================\n");
	printf("Final answer:\n");

	for (int i = 0; i < equations_num; i++) {
		print_equation(equations[i]);
		free_terms(equations[i]->left_terms);
		free_terms(equations[i]->right_terms);
	}
}