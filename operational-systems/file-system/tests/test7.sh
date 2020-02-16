#!/bin/bash
set -u

# This test script contributed by Yuri Niitsuma

i=7

function create_dir() {
	if [[ ! -d "extract" ]]; then
		mkdir extract
	fi

	if [[ ! -d "files" ]]; then
		mkdir files
	fi
}

function create_file_structure() {
	# dirs
	if [[ ! -d "files/bin" ]]; then
		mkdir files/bin
	fi
	if [[ ! -d "files/usr" ]]; then
		mkdir files/usr
	fi

	# test deep dir struct
	if [[ ! -d "files/usr/share" ]]; then
		mkdir files/usr/share
	fi

	# copy and past entire structure dirs
	cp -r files/* extract/

	# files
	# test tiny file
	if [[ ! -f "files/bin/foo" ]]; then
		touch files/bin/foo
		for (( index = 0; index < 3; index++ )); do
			echo -n $RANDOM >> files/bin/foo
		done
	fi
	# test longer file
	if [[ ! -f "files/usr/share/bar" ]]; then
		touch files/usr/share/bar
		for (( index = 0; index < 10000; index++ )); do
			echo -n $RANDOM >> files/usr/share/bar
		done
	fi
}

function clean_file_structure() {
	if [[ -d "files/" ]]; then
		rm -rf files/
	fi
	if [[ -d "extract/" ]]; then
		rm -rf extract/
	fi

	if [[ -f "file_list.txt" ]]; then
		rm -rf file_list.txt
	fi
}

function create_file_list() {
	echo "DIRS:" > file_list.txt
	(cd files/ && find -type d ) >> file_list.txt
	echo "FILES:" >> file_list.txt
	(cd files/ && find -type f ) >> file_list.txt
}


# main
clean_file_structure
create_dir
create_file_structure
create_file_list


gcc -g -Wall -I. tests/test$i.c fs.o -o test$i &>> gcc.log

if [ ! -x test$i ] ; then
    echo "[$i] compilation error"
    exit 1 ;
fi

if ! ./test$i 24 1024 > test$i.out 2> test$i.err ; then
    echo "[$i] error"
    exit 1
fi

# diff files
state=0

cat file_list.txt | while read file_name; do
	if [ $state == 1 ]; then
		if ! diff files/$file_name extract/$file_name > test$i.err; then
			echo "error in $file_name"
			break
		fi
		continue
	fi

	if [ $file_name == "FILES:" ]; then
		state=1
	fi
done

clean_file_structure

size=$(wc -c < test$i.err)
if [ $size -gt 0 ] ; then
	echo "[$i] error in diff"
	exit 1
fi

rm -f file_list.txt
rm -f test$i test$i.out test$i.err
exit 0
