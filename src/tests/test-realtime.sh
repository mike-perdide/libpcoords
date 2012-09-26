#!/bin/bash
#
# Test for real-time capabilities
#

PCV=pcv
REG_PATH=./regressions
REG_VER=""
TMP_DIR=""


function run_check_basic()
{
        file_check=$1

        echo "Run check with $file_check (listen for 30s)..."
        $PCV -Tsvg -s listen.sock -t realtime/template.pgdt > $TMP_DIR/$file_check.svg &
	echo "time=\"11:00\",str=\"blah\",foo=\"123\";" > listen.sock
	sleep 3
	echo "time=\"01:00\",str=\"foo\",foo=\"10000\";" > listen.sock
	echo "time=\"01:10\",str=\"foo\",foo=\"10000\";" > listen.sock
	echo "time=\"01:20\",str=\"foo\",foo=\"10000\";" > listen.sock
	echo "time=\"01:30\",str=\"bar\",foo=\"4000\";" > listen.sock
	echo "time=\"01:40\",str=\"bar\",foo=\"2000\";" > listen.sock
	sleep 17
	echo "time=\"09:40\",str=\"this is life\",foo=\"20000\";" > listen.sock
	sleep 10
        diff -Nudb $REG_PATH/$REG_VER/$file_check.svg $TMP_DIR/$file_check.svg > $TMP_DIR/$file_check.diff

        if [ -s $TMP_DIR/$file_check.diff ]
        then
                echo "*********************************"
                echo "*** WARNING: Regression found ***"
                echo "*********************************"
                cat $TMP_DIR/$file_check.diff
        else
                echo " |-> No regression"
        fi

}

function run_freeze()
{
        file_freeze=$1

        echo "Freezing $file_freeze..."
        $PCV -Tsvg -s listen.sock -t realtime/template.pgdt > $REG_PATH/$REG_VER/$file_freeze.svg &
	echo "time=\"11:00\",str=\"blah\",foo=\"123\";" > listen.sock
	sleep 3
	echo "time=\"01:00\",str=\"foo\",foo=\"10000\";" > listen.sock
	echo "time=\"01:10\",str=\"foo\",foo=\"10000\";" > listen.sock
	echo "time=\"01:20\",str=\"foo\",foo=\"10000\";" > listen.sock
	echo "time=\"01:30\",str=\"bar\",foo=\"4000\";" > listen.sock
	echo "time=\"01:40\",str=\"bar\",foo=\"2000\";" > listen.sock
	sleep 17
	echo "time=\"09:40\",str=\"this is life\",foo=\"20000\";" > listen.sock
}

function main()
{
        if [ $# -lt 2 ] || [ "$1" == "--help" ]
        then
                echo ""
                echo " Syntax: $0 --regression-with [version]"
                echo "         $0 --freeze [version]"
                echo "         $0 --help"
                echo ""

                exit 1
        fi

	if [ "$1" == "--regression-with" ]
	then
        	TMP_DIR=`mktemp -d -p $REG_PATH/`
        	REG_VER=$2
        	run_check rl-basic

        	rm -rf $TMP_DIR
	fi

	if [ "$1" == "--freeze" ]
	then
        	REG_VER=$2
		mkdir -p $REG_PATH/$REG_VER
        	run_freeze rl-basic
		echo "SVG files generated in: $REG_PATH/$REG_VER"
	fi

	exit 1
}

main $@

