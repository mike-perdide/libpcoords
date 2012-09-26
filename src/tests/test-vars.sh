#!/bin/bash

PCV=pcv
REG_PATH=./regressions
REG_VER=""
TMP_DIR=""


function run_check()
{
        file_check=$1

        echo "Run check with $file_check..."
        $PCV -Tsvg $file_check.pcv > $TMP_DIR/$file_check.svg
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
        $PCV -l -Tsvg $file_freeze.pcv > $REG_PATH/$REG_VER/$file_freeze.svg

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
        	run_check test-var-all
        	run_check test-var-char
        	run_check test-var-gold
        	run_check test-var-integer
        	run_check test-var-ipv4
        	run_check test-var-short
        	run_check test-var-string1
        	run_check test-var-string2
        	run_check test-var-string3
        	run_check test-prop-penwidth

        	rm -rf $TMP_DIR
	fi

	if [ "$1" == "--freeze" ]
	then
        	REG_VER=$2
		mkdir -p $REG_PATH/$REG_VER
        	run_freeze test-var-all
        	run_freeze test-var-char
        	run_freeze test-var-gold
        	run_freeze test-var-integer
        	run_freeze test-var-ipv4
        	run_freeze test-var-short
        	run_freeze test-var-string1
        	run_freeze test-var-string2
        	run_freeze test-var-string3
        	run_freeze test-prop-penwidth
		echo "SVG files generated in: $REG_PATH/$REG_VER"
	fi

	exit 1
}

main $@

