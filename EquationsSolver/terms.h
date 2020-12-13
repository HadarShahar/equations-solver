#ifndef TERMS_H
#define TERMS_H
#include <stdbool.h>

//const char *VALID_OPERATIONS = "+-*/";
#define VALID_OPERATIONS "+-*/"

typedef struct {
	double coefficient;
	char variable;
} term;

typedef struct node {
	term *val;
	struct node *next;
} node_t;

static inline void *malloc_or_die(size_t mem_size) {
	void *alloc_mem = malloc(mem_size);
	if (!alloc_mem && mem_size) {
		printf("Could not allocate memory!");
		exit(EXIT_FAILURE);
	}
	return alloc_mem;
}

node_t* create_terms(char*);
void substitute_expression(char, node_t*, node_t*, node_t**);
node_t* copy_terms(node_t*);
void mul_terms(term*, term*);
void div_terms(term*, term*);

void print_term(term*);
void print_terms(node_t*);

void free_terms(node_t*);
char get_first_var(node_t*);
bool add_term(term*, node_t**);

void move_terms(bool (*move_term_to_right)(term*, char), char, node_t**, node_t**);
bool term_is_var(term*, char);
bool term_isnot_var(term*, char);
bool dont_move_term(term*, char);
void join_terms(node_t**);

#endif
