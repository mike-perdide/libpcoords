PicViz - Parallel Coordinates Graph Generator
=============================================
The source code isn't available from the given URL, I used the latest
released version of picviz to create this repository.

http://www.wallinfire.net/picviz
(c) Copyright 2008-2009 Sebastien Tricaud

What is Picviz ?
----------------

Picviz is a parallel coordinates plotter, written to help people
finding a needle in a haystack when dealing with numerous events
on their system and struggling to maintain an acceptable level of
security.

It is a computer security visualization program, written in C with
high performances in mind. Python bindings are available, and are
used by the Picviz Frontend that you can use to dig into your graph.

Parallel coordinates, the core visualization technique used by Picviz
allows to represent graphs in N dimensions to see correlations among
variables, making it a useful data mining software when dealing with
large sets of logs.

Vocabulary
----------

PGDL = Picviz Graph Description Language
PGDT = Picviz Graph Description Template (used for real-time)

Dependences
-----------

Mandatory
+++++++++
- Cmake
- libpcre
- libevent 

Highly recommended
++++++++++++++++++
- Cairo

Optional
++++++++
- Python library (for bindings and the frontend)
- Python QT4 for the frontend
- Plplot

Install
-------
$ make
# make install

To build bindings
+++++++++++++++++
$ cd src/libpicviz/bindings/python
# python ./setup.py install

To build the frontend
+++++++++++++++++++++
$ cd src/frontend/
# python ./setup.py install

Use
---
console binary:
pcv -Tsvg file.pgdl > file.svg

Please read the man page pcv(1)

graphical frontend:
picviz-gui file.pgdl

