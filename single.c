void cb1_single (char *s, size_t i) {   // Every cell.

  if(current_cell == 0) {
    xmlTextWriterStartElement(writer, "entry"); // <entry>
    // inEntry = true;
    if(debug)
      printf("NEW ENTRY\n");
  }

  strcpy(word, s);
  word[i] = '\0';

  if(!keepLetterCase)
    string_tolower(word);
  // printf("  cb1: i = %d, s=: '%s' \n", (int)i, word);

  if(current_cell == 0) {
    xmlTextWriterWriteElement(writer, "term", word); // <term>hello</term>
    if(debug)
      printf("new term '%s'\n", word);
  }
  else
  {
    xmlTextWriterStartElement(writer, "synonym"); // <synonym>

    xmlTextWriterWriteElement(writer, "term", word); // <term>hello</term>
    xmlTextWriterEndElement(writer); // </synonym>
    if(debug)
      printf("synonym '%s'\n", word);
  }

  current_cell++;
}


void cb2_single(char c) {   // Called for every row in the CSV file

  if(debug)
    printf("END ENTRY\n");

  xmlTextWriterEndElement(writer); // </entry>
  current_cell = 0;
}

void op_single(FILE *file)
{
  writer = xmlNewTextWriterFilename(outputFilename, 0);
  xmlTextWriterSetIndent(writer, 1);
  xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);

  xmlTextWriterStartElementNS(
    writer,
    NULL,
    "thesaurus",
    "http://marklogic.com/xdmp/thesaurus"
  );

  while ((bytes_read=fread(buf, 1, 1024, file)) > 0)
  {
    if(csv_parse(parser, buf, bytes_read, cb1_single, cb2_single) != bytes_read)
    {
      printf("Error while parsing file: %s\n", csv_strerror(csv_error(parser)));
    }
  }

  csv_fini(parser, cb1_single, cb2_single);

  xmlTextWriterEndElement(writer); // </thesaurus>
  xmlTextWriterEndDocument(writer); // END OF document-node(element())

  xmlFreeTextWriter(writer);
}