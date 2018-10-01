#!/bin/bash
set -eu

if [ ! -s sh ] ; then exit 1 ; fi

cat > tests/script.sh <<-EOF
ls
ls
ls
ls
ls
EOF

cat tests/script.sh | bash > tests/bash.out
cat tests/script.sh | ./sh > tests/sh.out
rm -f tests/script.sh

if ! diff tests/bash.out tests/sh.out &> /dev/null ; then
    echo "[2] output for sequence of ls commands does not match"
    rm -f tests/bash.out tests/sh.out
    exit 1
else
    rm -f tests/bash.out tests/sh.out
    exit 0
fi

