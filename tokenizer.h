#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stdbool.h>
#include <stdio.h>

#include "file_to_buf.h"

typedef struct {
  char * word;
  unsigned int length;
}Token;
  
#define TOKENSTRNCMP_NOT_EQUAL 1
#define TOKENSTRNCMP_EQUAL 0

bool tokenStrcmp(Token tok1, Token tok2);
void printToken(Token token, FILE * stream);
void tokenizeBuffer (buffer buf, void * tokens);


#endif
