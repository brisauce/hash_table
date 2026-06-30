#ifndef ARENA_H
#define ARENA_H
#include <stdio.h>
#include <stdbool.h>

typedef struct {
  char * name;
  FILE * fp;
  bool output_presorted_hash_table; 
} arena;

#endif
