#!/bin/bash
set -u

srcfn=dccthread.c

zero () {
    echo $1
    exit 1
}

rm -f gcc.log
test -s dlist.c || zero "[0] file dlist.c does not exist"
gcc -g -Wall -c dlist.c &>> gcc.log
test -s dccthread.c || zero "[0] file dccthread.c does not exist"
gcc -g -Wall -c dccthread.c &>> gcc.log
test -s dlist.o || zero "[0] compilation error"
test -s dccthread.o || zero "[0] compilation error"
if [ -s gcc.log ] ; then
    echo "[0] compilation has warnings"
    exit 1
fi
exit 0


