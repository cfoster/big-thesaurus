# thes2xml


Simple command line utility which accepts a Thesaurus described in CSV format and produces either:

* A single MarkLogic Thesaurus XML document, which can be used by [MarkLogic's own Thesaurus XQuery library](http://docs.marklogic.com/thsr)

* Multiple MarkLogic Thesaurus XML documents, where each document has only ONE entry.

## Program behaviour

The first cell in a CSV row is considered by thes2xml to be the root term, all subsequent cells in the row are considered to be synonym terms. So for instance, the following CSV row:

```   
tango, dance, boogie, make some shapes, get down, bust a move
```

Would produce a MarkLogic Thesaurus element:

```xml
<entry xmlns="http://marklogic.com/xdmp/thesaurus">
  <term>tango</term>
  <synonym>
    <term>dance</term>
  </synonym>
  <synonym>
     <term>boogie</term>
  </synonym>
  <synonym>
    <term>make some shapes</term>
  </synonym>
  <synonym>
    <term>get down</term>
  </synonym>
  <synonym>
    <term>bust a move</term>
  </synonym>
</entry>
```

## Case conversion

By default, all terms are written to XML documents in lower-case, regardless of what case they originally had in the CSV file.

To stop this from happening, you can pass a `-k` or `--keep-letter-case` argument when running thes2xml.

## Single or Multiple XML files

By default, thes2xml will produce one single MarkLogic Thesaurus XML file from a CSV file. To produce multiple XML documents where each document has only 1 entry, pass the `-s` or `--split` argument when running thes2xml.

When using the `--split` option, the `[output]` argument MUST be a directory that already exists on the file-system.

The filenames of single entry XML documents is decided by an [MD5 hash](http://en.wikipedia.org/wiki/MD5) of the lower-cased root term, so a CSV row with a root term of 'elephant' would produce the XML file `e4b48fd541b3dcb99cababc87c2ee88f.xml`

## Command line usage

```
usage: thes2xml [options] input.csv [output]

-k  | --keep-letter-case  do not convert all terms to lower-case.
-s  | --split             split entries into multiple Thesaurus files,
                          if using --split, [output] MUST be a directory.
-vl | --verbose           turn on verbose logging, for debugging.
-v  | --version           version / about
/?  | --help              print this message

1 big XML:    thes2xml input.csv thesaurus.xml
multiple XML: thes2xml --split input.csv thesaurus-directory
```

## Compiling and running

* Must be linked with libxml2 when compiling
* Tested successfully on a MacOS with gcc 4.2.1

To compile on MacOS, use the command:

```
gcc -I/usr/include/libxml2 -lxml2 -o thes2xml *.c
```

