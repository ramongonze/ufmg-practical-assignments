#!/usr/bin/env python

import sys
import os
import re


MARK_START = 'MARK START'
MARK_END = 'MARK END'
MARK_NAME = 'MARK NAME'

def main():
	printing = False
	w = sys.stdout.write
	regexp1 = re.compile('%s (\w*)' % MARK_START)
	regexp2 = re.compile('%s (\w*)' % MARK_END)
	regexp3 = re.compile('%s (\w*)' % MARK_NAME)

	for line in sys.stdin:
		match = regexp3.search(line)
		if match:
			w(line.strip())
			x = len(line.strip())
			w((79-x)*'*' + '\n')
		match = regexp1.search(line)
		if match:
			printing = True
			w(79 * '*' + '\n\n')
		if printing:
			w(line)
		match = regexp2.search(line)
		if match:
			printing = False
			w(79 * '*' + '\n')
			w('\n\n')


if __name__ == '__main__':
	sys.exit(main())
