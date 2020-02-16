#!/bin/bash
set -eu

if [ ! -s sh ] ; then exit 1 ; fi

cat > tests/script.sh <<-EOF
cat sh.c | sort | uniq | wc -l > tests/1.tmp
cat sh.c | sort | uniq | wc -l > tests/2.tmp
cat sh.c | sort | uniq | wc -l > tests/3.tmp
cat sh.c | sort | uniq | wc -l > tests/4.tmp
cat sh.c | sort | uniq | wc -l > tests/5.tmp
EOF

cat sh.c | sort | uniq | wc -l > tests/0.tmp
cat tests/script.sh | ./sh
rm -f tests/script.sh

for i in $(seq 1 5) ; do
    if ! diff tests/0.tmp tests/$i.tmp &> /dev/null ; then
        echo "[8] output for seq of pipe commands with redir does not match"
        rm -f tests/[012345].tmp
        exit 1
    fi
done
rm -f tests/[012345].tmp
exit 0

