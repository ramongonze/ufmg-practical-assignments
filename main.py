import constants as c
import aco
import sys
import matplotlib.pyplot as plt

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

	for line in f:
		line = line.split()
		points = points + [c.Point(int(line[0]), int(line[1]), int(line[2]), int(line[3]))]

	graph = aco.buildGraph(points) # c.N is the index of the imaginary point
	aco.sumKNN(graph, points)

# Print the graph
	# for i in range(0, c.N):
	# 	for j in range(0, c.N):
	# 		print('{0:.0f} '.format(graph[i][j].weight), end='')
	# 	print('')

	# for i in range(0, c.N):
	# 	print('P{0}: {1}'.format(i, points[i].s))

	# x, y = [], []
	# for i in points:
	# 	x.append(i.x)
	# 	y.append(i.y)

	# plt.scatter(x, y)
	# plt.show()

	aco.aco(graph, points)

main()