xquery version "1.0-ml";

(:~
 : thesaurus.xqy
 :
 : Scalable and lightning-fast thesaurus synonym look-ups.
 :
 : Requires:
 :
 : 1. The uri lexicon enabled.
 :
 : 2. A path range index added:
 :
 :   path: /thsr:thesaurus/thsr:entry/thsr:synonym/thsr:term
 :   type: string
 :   namespace prefix: thsr=http://marklogic.com/xdmp/thesaurus
 :
 : @author Charles Foster
 :)

module namespace thsr = "http://marklogic.com/xdmp/thesaurus";

declare default element namespace "http://marklogic.com/xdmp/thesaurus";

declare variable $TUPLE-PATH as xs:string :=
  "//thsr:synonym/thsr:term";

(:~
 : @param $uri Thesaurus directory URI, without the terminating '/',
 :             can be multiple URIs.
 :
 : @param $term The term from which to look-up related synonyms.
 :)
declare function lookup(
  $uri as xs:string*,
  $term as xs:string) as element(thsr:entry)*
{
  entry(
    $term,
    synonym(
      cts:values(
        cts:path-reference(
          $TUPLE-PATH,
          ('type=string')
        ),
        (), (: [start] :)
        (), (: [options] :)
        cts:document-query(
          fn:concat($uri, '/', xdmp:md5(fn:lower-case($term)), '.xml')
        )
      )
    )
  )
};


(:~
 : synonym constructor
 :
 : produces a synonym and child term element for a given value.
 :)
declare private function synonym(
  $value as xs:anyAtomicType
) as element(synonym) {
  element synonym {
    element term {
      $value
    }
  }
};

(:~
 : entry constructor
 :
 : Produces a a single entry element providing there is at least 1 synonym.
 : If no synonym items are given, this returns an empty sequence.
 :)
declare private function entry(
  $term-name as xs:string,
  $synonym as element(synonym)*) as element(entry)?
{
  element entry {
    element term { $term-name },
    $synonym
  }[$synonym]
};

