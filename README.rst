.. image:: http://www.w3.org/Icons/w3c_home

===========================
microdata/RDFa-Lite library
===========================

* Light weight streaming parser
  
  - Parse chunked HTML and extract both `microdata`_/`RDFa-Lite`_ via corresponding event handlers
  - Applications can reduce memory foot print because huge HTML tree isn't internally created

* Data model builder
  
  - Create a data model based on the underlying streaming parser result
  - All semantics of microdata/RDFa-Lite can be accessed via HTML5 microdata API

How to build binary
-------------------

1. Install ``libxml`` because the streaming parser works on libxml ``HTMLparser``

2. Execute configure script::
     
    $ ./configure

3. Build library::
     
    $ make
    
How to serialize API specification
----------------------------------

1. Install ``doxygen``

2. Execute doxygen command under ``$LIBRARY_ROOT/csem``::
     
    $ doxygen
    
3. API specification is serialized under ``$LIBRARY_ROOT/doc`` as both html and man
 
Sample programs
---------------

* Examples for each module can be found under ``$LIBRARY_ROOT/examples``
* A sample program based on `Chrome extension`_

Code quality
------------

* No memory leak with ``valgrind``
* Nice code coverage (more than 80%)

Related prototypes
------------------

`SemWeb for HTML5`_.

License
-------

`W3C Software Notice and License`_

Author
------

Hitoshi Uchida <uchida@w3.org>

.. _`microdata`: http://www.w3.org/TR/2012/WD-microdata-20121025/
.. _`RDFa-Lite`: http://www.w3.org/TR/2012/REC-rdfa-lite-20120607/
.. _`Chrome extension`: https://chrome.google.com/webstore/detail/html5-annotation-viewer/omopnhbadhcofdohecpodpngdeepjjek
.. _`SemWeb for HTML5`: http://www.w3.org/2013/04/semweb-html5/
.. _`W3C Software Notice and License`: http://www.w3.org/Consortium/Legal/2002/copyright-software-20021231
