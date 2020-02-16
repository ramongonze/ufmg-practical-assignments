"""
	Program to print a maze as a graphic, using matplotlib.
	It uses the format generated by the function printMaze() from algs.hpp module
	Each wall is a line segment, that is, two coordinates.

	Format:
	
	wall1;wall2;wall3; ...
	
	Each wall is a pair of coordinates:
	x1,x2,y1,y2;x3,x4,y3,y4; ...
"""

import matplotlib.pyplot as plt

inp = input().split(',')
n = int(inp[0])
m = int(inp[1])

l = input().split(';')
del l[-1]

for i in range(0,len(l),1):
	c = l[i].split(',')
	x = [c[0],c[1]]
	y = [c[2],c[3]]
	plt.plot(x,y,color="blue")

plt.xlim(0, m)
plt.ylim(0, n)
plt.show()