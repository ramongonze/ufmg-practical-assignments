#!/bin/bash
set -eu

if [ ! -s sh ] ; then exit 1 ; fi

ls > tests/ls1.out
echo "ls" | ./sh > tests/ls2.out

if ! diff tests/ls1.out tests/ls2.out &> /dev/null ; then
    echo "[1] output for ls does not match"
    rm -f tests/ls1.out tests/ls2.out
    exit 1
else
    rm -f tests/ls1.out tests/ls2.out
    exit 0
fi

