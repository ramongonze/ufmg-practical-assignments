#!/bin/bash
set -eu

# DCC605: Userspace threading library programming assignment
# Autograding script

total=22
ecnt=0

if ! tests/test0.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test1.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test2.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test3.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test4.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test5.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test6.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

# This test is worth twice the points of a previous test:
if ! tests/test7.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test7.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test8.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test8.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test9.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test9.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test9.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test9.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test10.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test10.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test10.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test10.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test11.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test11.sh ; then ecnt=$(( $ecnt + 1 )) ; fi

if ! tests/test12.sh ; then ecnt=$(( $ecnt + 1 )) ; fi
if ! tests/test12.sh ; then ecnt=$(( $ecnt + 1 )) ; fi


echo "your code passes $(( $total - $ecnt )) of $total tests"
rm -f dlist.o dccthread.o
exit 0
