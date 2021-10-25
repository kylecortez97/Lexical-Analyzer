#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdio.h>

#define NUMBER_OF_KEYWORDS  13
#define NUMBER_OF_OPERATORS  8
#define NUMBER_OF_SEPARATORS 7

#define NUMBER_OF_STATES     32
#define NUMBER_OF_CHARACTERS 15

#define LENGTH_OF_TYPE   18
#define LENGTH_OF_LEXEME 31

extern char * arr_keywords[NUMBER_OF_KEYWORDS];
extern char arr_operators[NUMBER_OF_OPERATORS];
extern char arr_separators[NUMBER_OF_SEPARATORS];

extern int state_table[NUMBER_OF_STATES][NUMBER_OF_CHARACTERS];

struct token {
  char type[LENGTH_OF_TYPE + 1];
  char lexeme[LENGTH_OF_LEXEME + 1];
};

bool is_keyword(char *);
bool is_operator(char);
bool is_separator(char);

int get_character(char);
void append_type(char *, char);

struct token lexer(FILE *);

#endif
