PCOORDS_LIB_DIR = .
CC = gcc
LD = ld
INSTALL = /usr/bin/install
CFLAGS = -fPIC -O2 -c -g -Wall -Wformat-security -fno-strict-aliasing -I../../../include
LDFLAGS = -x --shared
PCOORDSLIB = -lpcoords
CPPFLAGS =

all: outsimple.so

outsimple.so: outsimple.o
	$(LD) $(LDFLAGS) -o outsimple.so outsimple.o $(PCOORDSLIB)

outsimple.o: outsimple.c
	$(CC) $(CFLAGS) outsimple.c

install: outsimple.so
	$(INSTALL) -m 0755 -d $(PCOORDS_LIB_DIR)
	$(INSTALL) -m 0644 outsimple.so $(LIBVIZ_LIB_DIR)

clean:
	rm -f outsimple.o outsimple.so

