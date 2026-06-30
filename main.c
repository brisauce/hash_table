/*
 *  The purpose of this project is to implement a hash table to bucket a long text file.
 *  The hash table should:
 *  - Use a simple but effective hash function (tsoding made a good implementation) to create 
 *    a table of entries which are data structures which includes the word (the key) and number of 
 *    occurences of the word in the text file
 *  - handle collisions when two different words hash to the same index on the table
 *  - handle when all available spaces in the hash table are occupied
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include "build/hash_tableConfig.h"
#include "file_to_buf.h"
#include "CLI.h"
#include "arena.h"
#include "dynamic_array.h"

typedef struct {
  uint32_t hash;
  Token key;
  unsigned int frequency;
} hash_index;

#define TOKENSTRNCMP_NOT_EQUAL 1
#define TOKENSTRNCMP_EQUAL 0

#define ARRAY_START_SIZE 400

#define TABLE_INDICES 2000

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

void printHashTable (hash_index * table, bool verbose, size_t num_to_print, FILE * stream)
{
  for (size_t i = 0; i < num_to_print; i++)
  {
    if (verbose)
    {
      fprintf(stream, "#%4zu: hash: 0x%08X, freq: %3u, key:", i, table[i].hash, table[i].frequency);
    }

    if (!table[i].key.word && verbose)
    {
      fprintf(stream, "--");
    }
    else
    {
      printToken(table[i].key, stream);
    }

    fputc('\n', stream);
  }
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

void tokenizeBuffer (buffer buf, void ** tokens)
{
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
      dynArrayAdd(tokens, &temp);
    }
  }

}

void buildHashTable(hash_index * table, Token * tokens, size_t num_tokens)
{
  for (size_t i = 0; i < num_tokens; i++)
  {
    hash_index temp = {
      .hash = hashToken(tokens[i]),
      .key = tokens[i]
    };

    unsigned int index = temp.hash % TABLE_INDICES;

    if (table[index].frequency == 0)
    {
      table[index].key = temp.key;
      table[index].hash = temp.hash;
      table[index].frequency = 1;
    }
    else if (!tokenStrcmp(temp.key, table[index].key))
    {
      table[index].frequency++;
    }
    else
    {
      //  collision handling
      size_t i = 1; //  start chacking the one immediately after the hash collision 
      for (; i < TABLE_INDICES; i++)
      {
        size_t wraparound_index = (index + i) % TABLE_INDICES;
        if (table[wraparound_index].frequency == 0)
        {
          table[wraparound_index].key = temp.key;
          table[wraparound_index].hash = temp.hash;
          table[wraparound_index].frequency = 1;
          break;
        }
        else if (!tokenStrcmp(temp.key, table[wraparound_index].key))
        {
          table[wraparound_index].frequency++;
          break;
        }
      }

      if (i == TABLE_INDICES)
      {
        printf("hash table overflow\n");
        printHashTable(table, true, TABLE_INDICES, stdout);
        exit(EXIT_FAILURE);
      }
    }
  }

}

int HT_freqCompare(const void * index1, const void * index2)
{
  const hash_index * index1_fr = index1;
  const hash_index * index2_fr = index2;
  return (int)index2_fr->frequency - (int)index1_fr->frequency;
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

  tokenizeBuffer(buf, (void**)&tokens);

  // hash each Tokenized word and check for collisions
  size_t num_tokens = dynArrayGetArraySize(tokens);
  hash_index * table = calloc(TABLE_INDICES, sizeof(hash_index));
  buildHashTable(table, tokens, num_tokens);

  if (a.output_presorted_hash_table)
  {
    FILE * jared_fp = fopen("goof.txt", "w");
    assert(jared_fp);
    printHashTable(table, false, TABLE_INDICES, jared_fp);
    fclose(jared_fp);
  }

  qsort(table, TABLE_INDICES, sizeof(hash_index), HT_freqCompare);
  printHashTable(table, true, 100, stdout);

  dynArrayDestroy((void**)&tokens);
  free(buf_start);
  free(table);
}
