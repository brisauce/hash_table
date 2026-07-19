#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "dynamic_array.h"
#define DEREF_VOID_PTR_PTR *(void **)


typedef struct {

  //  size in bytes of the individual elements of the array.
  size_t unit_size;

  //  The next element in the array to write data to. 
  size_t w_pointer;

  //  the total number of indices in the array which can be filled before the 
  //  dynamic array needs to be resized
  size_t capacity;
  
} dynArrayHeader;

void * dynArrayInit(size_t num_items, size_t item_size)
{
  void * retval = calloc(1, sizeof(dynArrayHeader) + (num_items * item_size));
  
  dynArrayHeader * header = retval;

  header->capacity = num_items;
  header->unit_size = item_size;
  header->w_pointer = 0;

  return (dynArrayHeader *) retval + 1;
}

void dynArrayAdd(void * array, void * item)
{
  //  Move the pointer to where the header data is, i.e. one header-sized amount of data before
  //  the array
  dynArrayHeader * header = *(void **) array;
  header --;

  if (header->w_pointer == header->capacity)
  {
    header->capacity *= 2;
    void * temp = realloc(header, (header->capacity * header->unit_size) + 
                          sizeof(dynArrayHeader)); 
    assert(temp);
    header = temp;
    (DEREF_VOID_PTR_PTR array) = (dynArrayHeader *) header + 1;
    array = (dynArrayHeader *) header + 1;
  } 

  void * current_element = (DEREF_VOID_PTR_PTR array) + (header->w_pointer * header->unit_size);

  memcpy(current_element, item, header->unit_size);
  header->w_pointer ++;
}

void dynArrayDestroy (void * array)
{
  dynArrayHeader * a = *(void **)array;
  free(a - 1);
  (DEREF_VOID_PTR_PTR array) = NULL;
}

size_t dynArrayGetArraySize(void * array)
{
  dynArrayHeader * header = (dynArrayHeader *) array - 1;
  return header->w_pointer;
}

