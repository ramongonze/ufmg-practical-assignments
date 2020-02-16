#!/bin/bash
set -eu

if [ ! -s sh ] ; then exit 1 ; fi

cat sh.c | sort | uniq | wc -l > tests/pipe1.out
echo "cat sh.c | sort | uniq | wc -l" | ./sh > tests/pipe2.out

if ! diff tests/pipe1.out tests/pipe2.out &> /dev/null ; then
    echo "[6] output for pipe command does not match"
    rm -f tests/pipe1.out tests/pipe2.out 
    exit 1
else
    rm -f tests/pipe1.out tests/pipe2.out 
    exit 0
fi

