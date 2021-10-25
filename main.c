/**
 * Group: Pranav Borole, Kyle Cortez, Myles Isas
 * Course: CPSC 323 - Compilers And Languages
 * Project: The Rat20F Programming Language
 * Assignment: Assignment 1 - Writing The Lexer
 */

// GCC Command Options: -std=c11 -g -Wall -Wextra -Wpedantic

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(int argc, char ** argv) {
  // The Program Reads From One Source File

  if (argc != 2) {
    fprintf(stderr, "The Number Of Arguments Passed To The Program Is Invalid.\n");
    return EXIT_FAILURE;
  }

  FILE * ptr_source_file;

  if ((ptr_source_file = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "The Name Of The Source File Passed To The Program Is Invalid.\n");
    return EXIT_FAILURE;
  }

  struct token t;

  // Call The Lexer To Generate The Token, Write The Token To The Output File
  while (feof(ptr_source_file) == 0) {
    t = lexer(ptr_source_file);

    // Testing...
    printf("Token: \"%s\"   Lexeme: \"%s\"\n", t.type, t.lexeme);
  }

  fclose(ptr_source_file);

  return EXIT_SUCCESS;
}
