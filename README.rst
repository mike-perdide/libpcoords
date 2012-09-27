Pcoords - Parallel Coordinates Graph Generator
=============================================

Pcoords starts where Picviz v0.6 stopped (see below for more information)

(c) Copyright 2008-2009 Sebastien Tricaud

What is Pcoords ?
-----------------

Pcoords is a parallel coordinates plotter, written to help people
finding a needle in a haystack when dealing with numerous events
on their system and struggling to maintain an acceptable level of
security.

It is a computer security visualization program, written in C with
high performances in mind. Python bindings are available, and are
used by the Pcoords Frontend that you can use to dig into your graph.

Parallel coordinates, the core visualization technique used by Picviz
allows to represent graphs in N dimensions to see correlations among
variables, making it a useful data mining software when dealing with
large sets of logs.

Vocabulary
----------

PGDL = Pcoords Graph Description Language
PGDT = Pcoords Graph Description Template (used for real-time)

Dependencies
------------

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

Use
---
console binary:
pcv -Tsvg file.pgdl > file.svg

Please read the man page pcv(1)

graphical frontend:
picviz-gui file.pgdl

Pcoords and Picviz
------------------
Picviz is one of the first free software project I ever worked on. I
used it in one of my engineer school internships. After that, I stopped
using it for a while.

Lately, I've been thinking about visualizing network logs for one of my
clients. I naturally thought about Picviz, but when I tried to find a
project page, all I found was: http://www.wallinfire.net/picviz, which
doesn't exist anymore, since Picviz became Picviz Labs, and the project
became a product. The code is still opensource thought, but it is not a
free software project anymore.

This is why I'm creating this repository, so that anyone wanting to
contribute to this awesome tool can do it, as easy as a pull request.

Since Picviz is now a registered name, this project will be named Pcoords.

This project is bootstrapped with the version 0.6 of Picviz, as available
from http://www.picviz.com/sections/community/picviz-latest.tar.bz2 (which
is a little more than the version 0.6 of Picviz that has been packaged in
Fedora 17).
