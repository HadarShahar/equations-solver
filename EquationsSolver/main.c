#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <ctype.h>
#include "main.h"
#include "terms.h"
#include "solver.h"

// Counts non empty lines in a given file.
int count_lines(FILE *fptr) {
	int lines = 0;
	char last = '\n';
	char c;
	while (EOF != (c = fgetc(fptr))) {
		if (c == '\n' && last != '\n') {
			lines++;
		}
		last = c;
	}
	if (last != '\n') {
		lines++;
	}
	// reset the file pointer to the start of the file
	rewind(fptr);
	return lines;
}

// Reads equations from a given file.
char **read_equations(FILE *fptr, int equations_num) {
	char **equations_strings = (char*) malloc_or_die(equations_num * sizeof(char*));
	int i = 0;
	while (i < equations_num) {
		equations_strings[i] = (char*) malloc_or_die(MAX_LINE_LEN * sizeof(char));
		fgets(equations_strings[i], MAX_LINE_LEN, fptr);

		char *ret = strstr(equations_strings[i], "\n");
		if (ret) {
			*ret = '\0';
		}
		//equations_strings[i][MAX_LINE_LEN - 1] = '\0';

		if (!strlen(equations_strings[i])) {
			continue;
		}
		//printf("(%d), '%s'\n", strlen(equations_strings[i]), equations_strings[i]);

		i++;
	}
	return equations_strings;
}


int main() {
	FILE *fptr;
	if (fopen_s(&fptr, EQUATIONS_FILENAME, "r") != 0 || fptr == NULL) {
		printf("Cannot open file '%s'.", EQUATIONS_FILENAME);
		return EXIT_FAILURE;
	}
	int equations_num = count_lines(fptr);
	printf("Reading %d equations...\n", equations_num);
	char **equations_strings = read_equations(fptr, equations_num);
	fclose(fptr);

	solve_all_equations(equations_strings, equations_num);

	for (int i = 0; i < equations_num; i++) {
		free(equations_strings[i]);
	}
	free(equations_strings);
	return 0;
}