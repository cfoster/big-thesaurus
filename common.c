const int VERSION_MAJOR = 1;
const int VERSION_MINOR = 0;

char *OPTION_HELP[] = { "/?", "--help", "-help" };
char *OPTION_VERSION[] = { "-v", "--version", "-version" };
char *OPTION_KEEPCASE[] = { "-k", "--keep-letter-case", "-keep-letter-case" };
char *OPTION_SPLIT[] = { "-s", "--split", "-split" };
char *OPTION_VERBOSE[] = { "-vl", "--verbose", "-verbose" };

const char* FIRST_USE_MESSAGE =
"thes2xml - Create MarkLogic thesauri from a CSV file\n"
"for usage info, try --help\n";

const char* HELP_MESSAGE =
"usage: thes2xml [options] input.csv [output]\n\n"
"-k  | --keep-letter-case  do not convert all terms to lower-case.\n"
"-s  | --split             split entries into multiple Thesaurus files,\n"
"                          if using --split, [output] MUST be a directory.\n"
"-vl | --verbose           turn on verbose logging, for debugging.\n"
"-v  | --version           version / about\n"
"/?  | --help              print this message\n\n"
"1 big XML:    thes2xml input.csv thesaurus.xml\n"
"multiple XML: thes2xml --split input.csv thesaurus-directory\n";

const char* VERSION_MESSAGE =
"Version: %d.%d\n"
"Author:  Charles Foster <charles" "@" "cfoster" ".net>\n" // anti-spam
"Source:  https://github.com/cfoster/thes2xml\n";

int current_cell = 0;

/** wheter strings to should be lower-cased **/
bool keepLetterCase = false;

/** split thesaurus into Directory **/
bool split = false;

bool debug = false;

char* inputFilename;
char* outputFilename;

// bool inEntry = false;

char word[128];

xmlTextWriterPtr writer;

struct csv_parser *parser;

size_t bytes_read;
char buf[1024];



void string_tolower(char* value)
{
  int len = strlen(value);
  int i;
  for(i=0;i<len;i++)
    value[i] = tolower(value[i]);
}

/**
 * is value equal to ANY value contained in test_values
**/
bool strcmp_seq(char* value, int amt_values, char* test_values[])
{
  int i;
  for(i=0;i<amt_values;i++)
    if(strcmp(value, test_values[i]) == 0)
      return true;
  return false;
};
