#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "md5.h"
#include "csv.h"
#include "thes2xml.h"

int main (int args, char *argv[])
{
  FILE *file;
  int i;

  bool readInputFilename = false,
       readOutputFilename = false;

  if(args == 1) {
    printf("%s",FIRST_USE_MESSAGE);
    return 0;
  }

  for(i=1;i<args;i++)
  {
    if(strcmp_seq(argv[i], 3, OPTION_HELP)) {
      printf("%s", HELP_MESSAGE); return 0;
    }
    else if(strcmp_seq(argv[i], 3, OPTION_VERSION)) {
      char buffer[1024];
      sprintf(buffer, VERSION_MESSAGE, VERSION_MAJOR, VERSION_MINOR);
      printf("%s", buffer);  return 0;
    }
    else if(strcmp_seq(argv[i], 3, OPTION_KEEPCASE))
      keepLetterCase = true;
    else if(strcmp_seq(argv[i], 3, OPTION_SPLIT))
      split = true;
    else if(strcmp_seq(argv[i], 3, OPTION_VERBOSE))
      debug = true;
    else if(!readInputFilename) {
      inputFilename = argv[i];
      readInputFilename = true;
    }
    else if(!readOutputFilename) {
      outputFilename = argv[i];
      readOutputFilename = true;
    }
  }

  if(!(readInputFilename && readOutputFilename)) {
    printf("%s", HELP_MESSAGE); return 0;
  }

  // ---- read parameters complete ----

  csv_init(&parser, CSV_STRICT);

  file = fopen ( inputFilename, "r" );

  if(file == NULL) {
    printf("Can not read %s\n", inputFilename);
    perror ( inputFilename );
    return 1;
  }

  if(split) // write to a directory
    op_split(file);
  else // write to a single file
    op_single(file);

  fclose(file);
  csv_free(parser);

  return 0;
}

