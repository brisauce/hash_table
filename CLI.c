#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <arena.h>

void printHelp(void)
{
  puts("Pass the name of the file to be tokenized and hashed");
}

void parseCLI(int argc, char ** argv, arena * a)
{
  if (argc == 1)
  {
    printHelp();
    exit(EXIT_FAILURE);
  }

  for (int i = 1; i < argc; i++)
  {
    if (!strcmp("--help", argv[i]))
    {
      printHelp();
      exit(EXIT_SUCCESS);
    }
    else if (!strcmp("--goof", argv[i])) 
    {
      a->output_presorted_hash_table = true;
    }
    else
    {
      FILE * fp = fopen(argv[i], "r");
      if (!fp)
      {
        printf("Invalid file name \"%s\", Exiting\n", argv[i]);
        exit(EXIT_FAILURE);
      }
      a->fp = fp;
    }
  }

  if (!a->fp)
  {
    puts("No file was inputted! Exiting");
    exit(EXIT_FAILURE);
  }
}
