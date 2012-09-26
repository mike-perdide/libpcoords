#!/bin/sh

CC="gcc"
CFLAGS="-D_UNIT_TEST_ -I./include -ggdb"
LIBS="-lpicviz -lpcre"

function compile()
{
        base=$1
	addflags=$2
	CFLAGS="$CFLAGS $addflags"

        echo "Compiling $base..."
        $CC $base.c -o $base $CFLAGS $LIBS
}

#compile values-mapping
#compile fifo-read -levent
#compile filter
compile utils
compile correlation
compile picviz-hash
