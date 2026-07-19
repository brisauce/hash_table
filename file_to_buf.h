#ifndef FILE_TO_BUF_H
#define FILE_TO_BUF_H
#include <stdio.h>
#include "build/hash_tableConfig.h"
typedef struct {
  // null terminated char string
  char * buf;
  size_t buf_size;
}buffer;

void copyFileToBuf(buffer * buff, FILE * source_fp);

#endif
