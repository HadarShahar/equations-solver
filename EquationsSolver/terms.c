#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "terms.h"
#include "solver.h"


node_t* create_terms(char *expression) {
	node_t *dummy_head = (node_t*) malloc_or_die(sizeof(node_t));
	dummy_head->next = NULL;
	node_t *previous = NULL;
	node_t *current = dummy_head;
	char last_op = '+';
	while (*expression && *expression != ')') {
		if (strchr(VALID_OPERATIONS, *expression)) {
			last_op = *expression;
		}
		else if (*expression == '(') {
			node_t *terms_in_parenthesis = create_terms(expression + 1);
			join_terms(&terms_in_parenthesis);
			substitute_expression(last_op, terms_in_parenthesis, previous, &current);
			
			// advance the pointers to the end of the subtitued expression
			while (current->next) {
				previous = current;
				current = current->next;
			}

			// skip the parenthesis in the main expression, 
			// because the expressions inside them has already been calculated
			while (*expression && *expression != ')') {
				expression++;
			}
		}
		else if (*expression != ' ') {
			term *t = (term*) malloc_or_die(sizeof(term));

			char prev_ch = *expression;
			// convert expression to a floating-point number, and update expression 
			// to the character after the last character used in the conversion.
			t->coefficient = strtod(expression, &expression);
			if (isalpha(*expression)) {
				if (!isdigit(prev_ch)) { // X => 1*X
					t->coefficient = 1;
				}
				t->variable = *expression;
			} else {
				t->variable = NULL;
				// strtod() updates expression, so if the current char isn't a variable (isn't alpha), 
				// got back to it, so it will be handled in the next iteration of the for loop.
				expression--;
			}
			if (last_op == '-') {
				t->coefficient *= -1;
			}

			if (last_op == '*') {
				mul_terms(current->val, t);
				free(t);
			} else if (last_op == '/') {
				div_terms(current->val, t);
				free(t);
			} else {
				// create a new node
				node_t *tmp = (node_t*) malloc_or_die(sizeof(node_t));
				tmp->val = t;
				tmp->next = NULL;
				// add it to the end of the list
				current->next = tmp;
				previous = current;
				current = current->next;
			}

			if (expression[1] == '(') { // 2(X+1) = 2*(X+1)
				last_op = '*';
			}
		}
		expression++;
	}
	current->next = NULL;
	node_t *ret = dummy_head->next;
	free(dummy_head);
	return ret;
}

void substitute_expression(char last_op, node_t *terms, node_t *previous, node_t **ptr_current) {

	// the pointer to the list that will contain the substitued expression
	node_t *current = *ptr_current;

	if (last_op == '/') {
		// if the list terms contains more than one value
		if (terms->next) { // can't solve 1/(X+1)
			cannot_solve_equation();
		} else {
			div_terms(current->val, terms->val);
			free_terms(terms);
		}
		return;
	}

	// the pointers to the substitued expression
	node_t *prev = NULL;
	node_t *pos = terms;
	while (pos) {
		if (last_op == '*') {
			mul_terms(pos->val, current->val);
		} else if (last_op == '-') { // -(X+1) = -1*(X+1)
			pos->val->coefficient *= -1;
		}
		prev = pos;
		pos = pos->next;
	}

	// connect the end of the substituted expression (terms) 
	// to the rest of the current expression
	if (prev) {
		prev->next = current->next;
	}

	// connect the current expression to the beginning of the substituted expression
	current->next = terms;

	if (last_op == '*') {
		// delete the coefficient of the substitued expression,
		// for example 2(X+1) => (X+1)
		node_t *tmp = current->next;
		current->next = NULL;
		free_terms(current);
		if (previous) {
			previous->next = tmp;
		}
		*ptr_current = tmp;

	}
}

// Copies a list of terms.
node_t* copy_terms(node_t *head) {
	node_t *dummy_head = (node_t*) malloc_or_die(sizeof(node_t));
	node_t *current = dummy_head;
	while (head) {
		node_t *tmp = (node_t*) malloc_or_die(sizeof(node_t));
		tmp->val = (term*) malloc_or_die(sizeof(term));;
		tmp->val->coefficient = head->val->coefficient;
		tmp->val->variable = head->val->variable;

		current->next = tmp;
		current = current->next;
		head = head->next;
	}
	current->next = NULL;
	node_t *ret = dummy_head->next;
	free(dummy_head);
	return ret;
}

// t1 *= t2
void mul_terms(term *t1, term *t2) {
	if (!t1->variable || !t2->variable) { // one of the variables must be NULL (can't solve X*X)
		t1->coefficient *= t2->coefficient;
		if (!t1->variable) { // for example 2*2X => 4x
			t1->variable = t2->variable;
		}
	} else {
		cannot_solve_equation();
	}
}

// t1 /= t2
void div_terms(term *t1, term *t2) {
	if (!t2->variable || t1->variable == t2->variable) { // can't solve 1/X, X/Y
		t1->coefficient /= t2->coefficient;
	} else {
		cannot_solve_equation();
	}
}

void print_term(term *t) {
	double c = t->coefficient;
	if (c >= 0) {
		printf("+");
	}
	if (c == 1 && t->variable) { // print X instead of 1.00x
		printf("%c ", t->variable);
	}
	else {
		printf("%.2f%c ", c, t->variable);
	}
}

void print_terms(node_t *head) {
	node_t *current = head;
	if (!current) { // if the list is empty
		printf("0");
	}
	while (current) {
		print_term(current->val);
		current = current->next;
	}
}

void free_terms(node_t *head) {
	while (head) {
		free(head->val);
		node_t *tmp = head;
		head = head->next;
		free(tmp);
	}
}

// Returns the first variable in a list of terms.
char get_first_var(node_t *head) {
	while (head) {
		char var = head->val->variable;
		if (var) {
			return var;
		}
		head = head->next;
	}
	return NULL;
}

bool add_term(term *t, node_t **ptr_head) {
	node_t *head = *ptr_head;
	while (head) {
		term *val = head->val;
		if (val == t) {
			return false;
		}
		// if there's a node in the list with the given term's variable
		if (val->variable == t->variable) {
			// add the given term coefficient to the node's coefficient
			val->coefficient += t->coefficient;
			free(t);
			return true;
		}
		head = head->next;
	}

	// create a new node and adds it to the beginning of the list
	node_t *new_node = (node_t*)malloc_or_die(sizeof(node_t));
	new_node->val = t;
	new_node->next = *ptr_head;
	*ptr_head = new_node;
	return true;
}

// Iterates over the terms in the left side and for each term (t):
//		If the function move_term_to_right(t, var) returns true, moves it to the right side.
//		Otherwise, try to add it to a term with the same variable in the left side (thus join the terms).
void move_terms(bool (*move_term_to_right)(term*, char), char var,
	node_t **ptr_left_terms, node_t **ptr_right_terms) {

	node_t *prev = NULL;
	node_t *current = *ptr_left_terms;
	while (current) {
		bool remove_current_node = true;
		term *t = current->val;
		if (move_term_to_right(t, var)) {
			t->coefficient *= -1; // move it to the other side of the equation
			// if the term wad added, remove the current node
			remove_current_node = add_term(t, ptr_right_terms);
			if (!prev) {
				*ptr_left_terms = (*ptr_left_terms)->next;
			}
		}
		else {
			remove_current_node = add_term(t, ptr_left_terms);
		}

		node_t *tmp = current;
		current = current->next;
		if (remove_current_node) {
			if (prev) {
				prev->next = tmp->next;
			}
			free(tmp);
		}
		else {
			prev = tmp;
		}
	}
}

bool term_is_var(term *t, char var) {
	return t->variable == var;
}

bool term_isnot_var(term *t, char var) {
	return t->variable != var;
}

bool dont_move_term(term *t, char var) {
	return false;
}

void join_terms(node_t **ptr_terms) {
	// if you call move_terms and tell it to not move any term to the right side,
	// it will just join the common terms in the left side.
	move_terms(&dont_move_term, NULL, ptr_terms, NULL);
}