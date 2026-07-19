#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "tokenizer.h"
#include "dynamic_array.h"

bool tokenStrcmp(Token tok1, Token tok2)
{
  //  trying to make the return value like strcmp 
  //  like aforementioned library function, but friendly to tokens
  //  returns nonzero if incompatible strings
  size_t i = 0;
  for (; i < tok1.length && i < tok2.length; i++)
  {
    if (tok1.word[i] != tok2.word[i])
    {
      return TOKENSTRNCMP_NOT_EQUAL; 
    }
  }

  if (i == tok1.length && i == tok2.length)
  {
    return TOKENSTRNCMP_EQUAL;
  }

  return TOKENSTRNCMP_NOT_EQUAL;
}

void printToken(Token token, FILE * stream)
{
  for (size_t i = 0; i < token.length; i++)
  {
    fputc(token.word[i], stream);
  }
  fflush(stream);
}

void tokenizeBuffer (buffer buf, void * tokens)
{
  for (size_t i = 0; i < buf.buf_size; i++)
  {
    //  Tokenize the text file, splitting it into tokens by spaces and newlines
    Token temp = {0};
    size_t j = i;
    for (; j < buf.buf_size; j++)
    {
      if (buf.buf[j] == ' ' || buf.buf[j] == '\n')
      {
        if (!temp.word)
        {
          continue;
        }
        else
        {
          temp.length = (buf.buf + j) - temp.word;
          break;
        }
      }
      else
      {
        if (!temp.word)
        {
          temp.word = buf.buf + j;
        }
      }
    }
    i = j;

    if (temp.word && temp.length)
    {
      dynArrayAdd(tokens, &temp);
    }
  }
}
