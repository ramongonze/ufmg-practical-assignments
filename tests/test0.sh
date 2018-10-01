#!/bin/bash
set -u

srcfn=$1

zero () {
    echo $1
    exit 1
}

test -s $srcfn || zero "[0] file $srcfn does not exist"
gcc -Wall $srcfn -o sh &> gcc.log
test -s sh || zero "[0] compilation error"
if [ -s gcc.log ] ; then
    echo "[0] compilation has warnings"
    rm -f gcc.log
    exit 1
fi
rm -f gcc.log
exit 0


