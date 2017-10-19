import constants as c
import aco
import sys

def main():
	# Data read
	# Point is a list with c.Point instances, and it represents all the points in the space.
	f = open(sys.argv[1], 'r')
	line = f.readline().split()
	c.N, c.P = int(line[0]), int(line[1])
	c.NUMBER_OF_ANTS = c.N - c.P
	points = []
	
	line = f.readline().split()
	points = points + [c.Point(int(line[0]), int(line[1]), int(line[2]), int(line[3]))]
	# It contains the max value of X, max of Y, min of X and min of Y, respectively
	max_and_min = {'max_x':int(line[0]), 'max_y':int(line[1]), 'min_x':int(line[0]), 'min_y':int(line[1])}

	for line in f:
		line = line.split()
		points = points + [c.Point(int(line[0]), int(line[1]), int(line[2]), int(line[3]))]
		if max_and_min['max_x'] < int(line[0]):
			max_and_min['max_x'] = int(line[0])
		if max_and_min['max_y'] < int(line[1]):
			max_and_min['max_y'] = int(line[1])
		if max_and_min['min_x'] > int(line[0]):
			max_and_min['min_x'] = int(line[0])
		if max_and_min['min_y'] > int(line[1]):
			max_and_min['min_y'] = int(line[1])

	im_point = aco.imaginaryPoint(max_and_min)
	graph = aco.buildGraph(points, im_point)

""" Print the graph
	for i in range(0, c.N):
		for j in range(0, i+1):
			print('X ', end='')
		for j in range(0, c.N-i-1):
			print('{0:.0f} '.format(graph[i][j].weight), end='')
		print('{0:.0f} '.format(graph[i][-1].pheromone))
	print('')
"""
	
	aco.aco(im_point, graph, points)

main()