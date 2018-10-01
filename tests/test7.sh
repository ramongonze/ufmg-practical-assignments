#!/bin/bash
set -eu

if [ ! -s sh ] ; then exit 1 ; fi

cat > tests/script.sh <<-EOF
cat sh.c | sort | uniq | wc -l
cat sh.c | sort | uniq | wc -l
cat sh.c | sort | uniq | wc -l
cat sh.c | sort | uniq | wc -l
cat sh.c | sort | uniq | wc -l
EOF

cat tests/script.sh | bash > tests/bash.out
cat tests/script.sh | ./sh > tests/sh.out
rm -f tests/script.sh

if ! diff tests/bash.out tests/sh.out &> /dev/null ; then
    echo "[7] output for sequence of pipe commands does not match"
    rm -f tests/bash.out tests/sh.out
    exit 1
else
    rm -f tests/bash.out tests/sh.out
    exit 0
fi

