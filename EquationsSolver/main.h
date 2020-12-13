#ifndef MAIN_H
#define MAIN_H

#define MAX_LINE_LEN 300
const char *EQUATIONS_FILENAME = "Equations.txt";

int count_lines(FILE*);
char **read_equations(FILE*, int);

#endif
