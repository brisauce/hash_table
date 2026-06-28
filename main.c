/*
 *  The purpose of this project is to implement a hash table to bucket a long text file.
 *  The hash table should:
 *  - Use a simple but effective hash function (tsoding made a good implementation) to create 
 *    a table of entries which are data structure which includes the word (the key) and number of 
 *    occurences of the word in the text file
 *  - handle collisions when two different words hash to the same index on the table
 *  - handle when all available spaces in the hash table are occupied
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "build/hash_tableConfig.h"
#include "file_to_buf.h"
#include "CLI.h"
#include "arena.h"
#include "dynamic_array.h"

#define ARRAY_START_SIZE 400
#define TABLE_INDICES 1000

typedef struct {
  unsigned int hash;
  Token key;
  unsigned int frequency;
} hash_index;


void printToken(Token token)
{
  for (size_t i = 0; i < token.length; i++)
  {
    putc(token.word[i], stdout);
  }
  fflush(stdout);
}

unsigned int hashToken(Token token)
{
  size_t retval = 0;
  for (size_t i = 0; i < token.length; i++)
  {
    retval = retval * 31 + token.word[i];
  }
  return retval;
}


int main(int argc, char ** argv)
{

  arena a = {0};
  parseCLI(argc, argv, &a);  
  // Tokenize all of the words in a text file 

  buffer buf;
  buf.buf_size = BUF_SIZE;
  buf.buf = calloc(1, buf.buf_size);
  
  copyFileToBuf(&buf, a.fp);
  char * buf_start = buf.buf;

  Token * tokens = dynArrayInit(ARRAY_START_SIZE, sizeof(Token));

  while (*buf.buf != '\0')
  {
    //  Tokenize the text file, splitting it into tokens by spaces and newlines
    Token temp = {0};
    for (;*buf.buf != '\0'; buf.buf++)
    {
      if (*buf.buf == ' ' || *buf.buf == '\n')
      {
        if (!temp.word)
        {
          continue;
        }
        else
        {
          temp.length = buf.buf - temp.word;
          buf.buf++;
          break;
        }
      }
      else
      {
        if (!temp.word)
        {
          temp.word = buf.buf;
        }
      }
    }

    if (temp.word && temp.length)
    {
      dynArrayAdd((void **)&tokens, &temp);
    }
  }


  // hash each Tokenized word and check for collisions
  size_t num_tokens = dynArrayGetArraySize(tokens);
  hash_index * table = calloc(TABLE_INDICES, sizeof(hash_index));
  for (size_t i = 0; i < num_tokens; i++)
  {
    hash_index temp = {
      .hash = hashToken(tokens[i]) % TABLE_INDICES,
      .key = tokens[i]
    };

    if (table[temp.hash].frequency == 0)
    {
      table[temp.hash] = temp;
    }
    else
    {
      if (!strncmp(temp.key.word, table[temp.hash].key.word, temp.key.length))
      {
        table[temp.hash] = temp;
      }
      else 
      {
        //  collision handling
        for (size_t i = 1; i < TABLE_INDICES; i++)
        {
          if (table[temp.hash + i].frequency == 0)
          {
            table[temp.hash + i] = temp; 
          }
        }
      }
    }
  }

  dynArrayDestroy((void**)&tokens);
  free(buf_start);
  free(table);
}

