#!/bin/sh
set -u

# DCC605F5: Filesystem implementation programming assignment
# Autograding script

total=6
ecnt=0

if ! tests/test1.sh ; then ecnt=$(( ecnt + 1 )) ; fi
if ! tests/test2.sh ; then ecnt=$(( ecnt + 1 )) ; fi
if ! tests/test3.sh ; then ecnt=$(( ecnt + 1 )) ; fi
if ! tests/test4.sh ; then ecnt=$(( ecnt + 1 )) ; fi
if ! tests/test5.sh ; then ecnt=$(( ecnt + 1 )) ; fi
if ! tests/test6.sh ; then ecnt=$(( ecnt + 1 )) ; fi

echo "your code passes $(( total - ecnt )) of $total tests"
rm -f fs.o
exit 0
