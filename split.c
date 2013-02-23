md5_state_t state;
md5_byte_t digest[16];
int di;
char hex_output[16*2 + 1];
char complete_output_filename[256];

void cb1_split(char *s, size_t i) { // Every cell.

  strcpy(word, s);
  word[i] = '\0';

  if(!keepLetterCase)
    string_tolower(word);

  if(current_cell == 0)
  {
    // xmlTextWriterStartElement(writer, "entry"); // <entry>
    if(debug)
      printf("NEW ENTRY\n");

  	md5_init(&state);
  	md5_append(&state, word, strlen(word));
  	md5_finish(&state, digest);

    for ( di = 0; di < 16; ++di)
      sprintf(hex_output + di * 2, "%02x", digest[di]);

    sprintf(
      complete_output_filename,
      "%s/%s.xml",
      outputFilename,
      hex_output
    );

    writer = xmlNewTextWriterFilename(complete_output_filename, 0);
    xmlTextWriterSetIndent(writer, 1);
    xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);

    xmlTextWriterStartElementNS(
      writer,
      NULL,
      "thesaurus",
      "http://marklogic.com/xdmp/thesaurus"
    ); // <thesaurus xmlns="http://marklogic.com/xdmp/thesaurus">

    xmlTextWriterStartElement(writer, "entry"); // <entry>

    xmlTextWriterWriteElement(writer, "term", word); // <term>hello</term>

    if(debug)
      printf("new term '%s'\n", word);

  }
  else // subsequent fields
  {
    xmlTextWriterStartElement(writer, "synonym"); // <synonym>

    xmlTextWriterWriteElement(writer, "term", word); // <term>hello</term>
    xmlTextWriterEndElement(writer); // </synonym>

    if(debug)
      printf("synonym '%s'\n", word);
  }
  // printf("  cb1: i = %d, s=: '%s' \n", (int)i, word);


  current_cell++;
}

void cb2_split(char c) {   // Called for every row in the CSV file

  if(debug)
    printf("END ENTRY\n");

  xmlTextWriterEndElement(writer); // </entry>
  xmlTextWriterEndElement(writer); // </thesaurus>

  xmlTextWriterEndDocument(writer); // END OF document-node(element())
  xmlFreeTextWriter(writer);

  current_cell = 0;
}

void op_split(FILE *file)
{
  while ((bytes_read=fread(buf, 1, 1024, file)) > 0)
  {
    if(csv_parse(parser, buf, bytes_read, cb1_split, cb2_split) != bytes_read)
    {
      printf("Error while parsing file: %s\n", csv_strerror(csv_error(parser)));
    }
  }

  csv_fini(parser, cb1_split, cb2_split);
}