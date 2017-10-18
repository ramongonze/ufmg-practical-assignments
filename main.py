import constants as c
import aco
import sys

def main():
	# Data read
	# Point is a list with c.Point instances, and it represents all the points in the space.
	f = open(sys.argv[1], 'r')
	line = f.readline().split()
	c.N, c.P = int(line[0]), int(line[1])
	c.NUMBER_OF_ANTS = c.N
	points = []
	for line in f:
		line = line.split()
		points = points + [c.Point(int(line[0]), int(line[1]), int(line[2]), int(line[3]))]

	graph = aco.buildGraph(points)

	aco.aco()

main()