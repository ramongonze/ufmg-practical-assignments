#!/bin/bash
set -eu

if [ ! -s sh ] ; then exit 1 ; fi

ls > tests/ls1.out
echo "ls > tests/ls2.out" | ./sh

if ! diff tests/ls1.out tests/ls2.out &> /dev/null ; then
    echo "[3] redirected output for ls does not match"
    rm -f tests/ls1.out tests/ls2.out
    exit 1
else
    rm -f tests/ls1.out tests/ls2.out
    exit 0
fi

