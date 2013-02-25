# Super Huge, Super Fast, MarkLogic Thesauri


## WHY RE-INVENT THE WHEEL?!

**Why write a MarkLogic Thesaurus XQuery library when MarkLogic [already provide one](http://docs.marklogic.com/thsr)?**

MarkLogic has shipped with an Thesaurus XQuery library for years. Written in `0.9-ml` it does the job, OK. The library works on the principle that a Thesaurus, including all root terms and term synonyms exist in on one single XML document which can be placed in a Database.

The larger and more complex your Thesaurus, the larger the XML document needs to be. This can pose performance and scaleability issues in MarkLogic Server, to the point of it not even being a viable solution for performing Thesaurus Lookups.

MarkLogic on the other hand is brilliant at storing smaller XML documents, it's also brilliant at handling small structured documents, if we just altered the principle of having one huge XML document by splitting each root term and it's associated synonyms into separate XML files, while also making use of MarkLogic's path range indexes, we could scale Thesauri to a super huge sizes while getting serious performance gains!

The Engineers at MarkLogic are very clever and will almost certainly get round to re-vamping their Thesaurus library in the future, but in the meantime, this project should help you create your own custom MarkLogic Thesauri which can be very large and complex, while at the same time getting serious performance benefits for doing thesaurus look-ups as this version will never even touch the disk!

## GETTING THIS INSTALLED IN MY PROJECT

### Multiple Thesaurus Files

You will need to create multiple MarkLogic XML Thesaurus files, where each Thesaurus file has one single `entry` item with it's associated synonyms.

You can create multiple thesaurus XML files from a CSV file by using the `thes2xml` program included in this project, where the first cell of each row is considered to be the root term and all subsequent cells on the same row are considered to be the root term's associated synonyms.

If you have one big XML thesaurus right now, you could create an XSLT to split them out into separate files, you will need to make sure that the filenames of each XML file is a MD5 hash of the lower-cased root term name. If you need help with this, contact me.

### Setting up required indexes

Before you ingest any XML files, make sure somethings are setup first.

* Make sure the uri lexion is enabled.
* Add a path range index

```
  path: //thsr:synonym/thsr:term
  type: string
  
  Where the namespace of 'thsr' is http://marklogic.com/xdmp/thesaurus
```

### Ingesting XML thesaurus files

Your Thesaurus which should now be split into multiple single entry XML files should be placed in a meaningful MarkLogic directory, such as `/thesaurus/en-GB` for a British English Thesaurus.

To get the XML documents ingested, you could use RecordLoader, XML Shell (XSH), Information Studio or Content Pump, it's up to you. Just make sure they happen to be in a suitable directory within MarkLogic.

### Using the alternatie Thesaurus Library

The XQuery library called `thesaurus.xqy` in this project provides just one function which has the exact same signature found in the original MarkLogic Thesaurus library:

```xquery

declare function lookup(
  $uri as xs:string*,
  $term as xs:string) as element(thsr:entry)*;

```

The only difference is that instead of specifying a paths to 1 or more thesaurus XML documents, you specify the directory which holds the split Thesaurus XML files, (*without the terminating forward slash*).

So for instance:

```xquery
import module namespace thsr =
  "http://marklogic.com/xdmp/thesaurus" at "/util/thesaurus.xqy";

thsr:lookup('/thesaurus','tango')
```

Could produce:

```
<entry xmlns="http://marklogic.com/xdmp/thesaurus">
  <term>tango</term>
  <synonym>
    <term>boogie</term>
  </synonym>
  <synonym>
    <term>bust a move</term>
  </synonym>
  <synonym>
    <term>dance</term>
  </synonym>
  <synonym>
    <term>get down</term>
  </synonym>
  <synonym>
    <term>make some shapes</term>
  </synonym>
</entry>
```

