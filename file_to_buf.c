#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "build/hash_tableConfig.h"
#include "file_to_buf.h"

void copyFileToBuf(buffer * buff, FILE * source_fp)
{
  //  read data in 
  //  move pointer to next empty space
  //  check if reading more in will cause an overflow and if so, reallocate
  //  set the read size to the remaining size of the buffer
  //  repeat
  size_t read_size = buff->buf_size;
  size_t next_empty_space = 0;
  size_t read;
  while ((read = fread (buff->buf + next_empty_space, sizeof(char), read_size, source_fp)) != 0)
  {
    next_empty_space += read;
    if (next_empty_space + read_size > buff->buf_size)
    {
      buff->buf_size *= 2;
      buff->buf = realloc(buff->buf, buff->buf_size * sizeof(char));
      assert(buff->buf);
    }
    read_size = buff->buf_size - next_empty_space;
  }
}

