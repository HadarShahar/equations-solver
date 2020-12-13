#ifndef SOLVER_H
#define SOLVER_H
#include "terms.h"

typedef struct {
	node_t *right_terms;
	node_t *left_terms;
} equation;

void cannot_solve_equation();
equation* parse_equation(char*);
void print_equation(equation*);

bool is_solved(equation*);
void solve_equation(equation*);
bool substitue_variable(char, node_t*, node_t**);
void substitue_equations(equation**, int, equation*);
void solve_all_equations(char**, int);

#endif