#include <ctype.h>
#include <string.h>

#include "lexer.h"

char * arr_keywords[NUMBER_OF_KEYWORDS]   = { "boolean", "else", "false", "fi", "function", "get", "if", "int", "put", "real", "return", "true", "while" };
char arr_operators[NUMBER_OF_OPERATORS]   = { '=', '!', '>', '<', '+', '-', '*', '/' };
char arr_separators[NUMBER_OF_SEPARATORS] = { '$', '(', ')', ',', '{', '}', ';' };

/**
 * List Of States (From Up To Down):
 *  0 - Starting State
 *  1 - In Identifier
 *  2 - End Of Identifier
 *  3 - In Invalid Identifier
 *  4 - End Of Invalid Identifier
 *  5 - In Integer
 *  6 - End Of Integer
 *  7 - Found '.'
 *  8 - In Real
 *  9 - End Of Real
 * 10 - In Invalid Real
 * 11 - End Of Invalid Real
 * 12 - In Invalid String
 * 13 - End Of Invalid String
 * 14 - Found '='
 * 15 - Operator "=="
 * 16 - Operator "=>"
 * 17 - Found '!'
 * 18 - Operator "!="
 * 19 - Found '<'
 * 20 - Operator "<="
 * 21 - Found '/'
 * 22 - In Comment
 * 23 - Found '*' In Comment
 * 24 - End Of Comment
 * 25 - General Operator
 * 26 - General Invalid Operator
 * 27 - Found '$'
 * 28 - Separator "$$"
 * 29 - General Separator
 * 30 - General Invalid Separator
 * 31 - General Invalid Character
 */

/**
 * List Of Characters (From Left To Right):
 *  0 - Letter
 *  1 - Digit
 *  2 - '_'
 *  3 - '.'
 *  4 - '='
 *  5 - '!'
 *  6 - '>'
 *  7 - '<'
 *  8 - '*'
 *  9 - '/'
 * 10 - Operator
 * 11 - '$'
 * 12 - Separator
 * 13 - Whitespace
 * 14 - Invalid
 */

int state_table[NUMBER_OF_STATES][NUMBER_OF_CHARACTERS] = {
  {  1,  5,  3, 10, 14, 17, 25, 19, 25, 21, 25, 27, 29,  0, 31 },
  {  1,  1,  1, 12,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  3,  3,  3, 12,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  3,  5,  3,  7,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 12,  8, 12, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
  { 12,  8, 12, 10,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 12, 10, 12, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 25, 25, 25, 25, 15, 25, 16, 25, 25, 25, 25, 25, 25, 25, 25 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 26, 26, 26, 26, 18, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 25, 25, 25, 25, 20, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 25, 25, 25, 25, 25, 25, 25, 25, 22, 25, 25, 25, 25, 25, 25 },
  { 22, 22, 22, 22, 22, 22, 22, 22, 23, 22, 22, 22, 22, 22, 22 },
  { 22, 22, 22, 22, 22, 22, 22, 22, 23, 24, 22, 22, 22, 22, 22 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 28, 30, 30, 30 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
  {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
};

bool is_keyword(char * str) {
  for (int i = 0; i < NUMBER_OF_KEYWORDS; i++)
    if (strcmp(str, arr_keywords[i]) == 0)
      return true;
  return false;
}

bool is_operator(char ch) {
  for (int i = 0; i < NUMBER_OF_OPERATORS; i++)
    if (ch == arr_operators[i])
      return true;
  return false;
}

bool is_separator(char ch) {
  for (int i = 0; i < NUMBER_OF_SEPARATORS; i++)
    if (ch == arr_separators[i])
      return true;
  return false;
}

int get_character(char ch) {
  if (isalpha(ch))
    return 0;
  else if (isdigit(ch))
    return 1;
  else if (ch == '_')
    return 2;
  else if (ch == '.')
    return 3;
  else if (ch == '=')
    return 4;
  else if (ch == '!')
    return 5;
  else if (ch == '>')
    return 6;
  else if (ch == '<')
    return 7;
  else if (ch == '*')
    return 8;
  else if (ch == '/')
    return 9;
  else if (is_operator(ch))
    return 10;
  else if (ch == '$')
    return 11;
  else if (is_separator(ch))
    return 12;
  else if (isspace(ch))
    return 13;
  else
    return 14;
}

void append_type(char * str, char ch) {
  int len = strlen(str);

  if (len >= LENGTH_OF_TYPE) return;

  str[len] = ch;
  str[len + 1] = '\0';
}

struct token lexer(FILE * stream) {
  struct token t = { .type = { '\0' }, .lexeme = { '\0' } };

  int current_state = 0;

  while (true) {
    char ch = getc(stream);
    int current_character = get_character(ch);
    int next_state = state_table[current_state][current_character];

    switch (next_state) {
      case 0:  // Starting State
        if (current_state == 0) break;
        ungetc(ch, stream);
        return t;
      case 15: // Operator "=="
      case 16: // Operator "=>"
      case 18: // Operator "!="
      case 20: // Operator "<="
        // Lexeme Function
      case 1:  // In Identifier
      case 3:  // In Invalid Identifier
      case 5:  // In Integer
      case 7:  // Found '.'
      case 8:  // In Real
      case 10: // In Invalid Real
      case 12: // In Invalid String
      case 14: // Found '='
      case 17: // Found '!'
      case 19: // Found '<'
        append_type(t.type, ch);
        break;
      case 2:  // End Of Identifier
      case 4:  // End Of Invalid Identifier
      case 6:  // End Of Integer
      case 9:  // End Of Real
      case 11: // End Of Invalid Real
      case 13: // End Of Invalid String
        ungetc(ch, stream);
        // Lexeme Function
        break;
    }

    current_state = next_state;
  }
}
