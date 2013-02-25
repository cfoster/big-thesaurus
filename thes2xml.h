#include <stdbool.h>
#include <libxml/xmlwriter.h>

extern const int VERSION_MAJOR;
extern const int VERSION_MINOR;

extern char *OPTION_HELP[];
extern char *OPTION_VERSION[];
extern char *OPTION_KEEPCASE[];
extern char *OPTION_SPLIT[];
extern char *OPTION_VERBOSE[];

extern const char* FIRST_USE_MESSAGE;

extern const char* HELP_MESSAGE;

extern const char* VERSION_MESSAGE;

extern int current_cell;

extern bool keepLetterCase;

/** split thesaurus into Directory **/
extern bool split;

extern bool debug;

extern char* inputFilename;
extern char* outputFilename;

extern char word[];
extern xmlTextWriterPtr writer;
extern struct csv_parser *parser;
extern size_t bytes_read;
extern char buf[];

void cb1_split(char *s, size_t i);
void cb2_split(char c);
void op_split(FILE *file);

void cb1_single(char *s, size_t i);
void cb2_single(char c);
void op_single(FILE *file);

void string_tolower(char* value);
bool strcmp_seq(char* value, int amt_values, char* test_values[]);