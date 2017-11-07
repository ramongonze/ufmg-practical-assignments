import operator as op
import sys
import aco

class Point(object):
	def __init__(self, id, x, y, c, d, density=None, cur_c=0, attended_by=None, random_prob=None, nearest_median=None, point_distance=None):
		self.id = id # Point id according the 'points' array
		self.x = x # Point x in the cartesian plane
		self.y = y # Point y in the cartesian plane
		self.c = c # Point capacity
		self.d = d # Point demand
		self.density = density # Density based in how many points are next to the point. Used to be a factor to choose a median.
		self.cur_c = d # Current capacity used for this point.
		self.attended_by = attended_by # Median which attend the point in analysis. A median attends itself.
		self.random_prob = random_prob # Used to select points randomly.
		self.nearest_median = nearest_median # Used to create a sorted list of points in ascending order, in function sortRemPoints().
		self.point_distance = point_distance # Used to create a sorted list of medians in ascending order, in function sortMedians().


def dataRead():

	f = open(sys.argv[1], 'r')
	line = f.readline().split()
	aco.N, aco.P = int(line[0]), int(line[1])
	
	points, id = [], 0
	for line in f:
		line = line.split()
		points.append(Point(id, int(line[0]), int(line[1]), int(line[2]), int(line[3])))
		id += 1

	f.close()
	return points

def buildGraph(points):
	"""
	- It builds a graph with the points using an adjacency matrix.
	- graph[i][j] contains the Euclidian distance between the nodes i and j. 
	- The graph is undirected and there isn't an edge from one node to itself.
	- The last matrix column represents the quantity of pheromone among the immaginary point with all the points of the graph.
	"""
	graph = []

	for i in range(0, aco.N):
		graph.append([])
		for j in range(0, aco.N):
			graph[i].append(euclidianDistance(points[i].x, points[j].x, points[i].y, points[j].y))
		graph[i].append(aco.INITIAL_PHEROMONE)

	return graph

def euclidianDistance(x0, x1, y0, y1):
	return ((x0 - x1)**2 + (y0 - y1)**2)**(1/2)
########################################################

def calculateDensity(points, graph):
	for p in range(0, aco.N):
		ordered_points = sortPoints(p, points, graph)
		all_points, sum_distance = allocatePoints(p, ordered_points, points, graph)
		points[p].density = all_points / sum_distance

def sortPoints(p, points, graph):
	# Sort all points based on their distance to node p(p = point.id).

	ordered_points = points[:]
	del ordered_points[p] # Remove the point p itself

	for i in ordered_points:
		i.point_distance = graph[p][i.id]

	ordered_points.sort(key=op.attrgetter('point_distance'))

	return ordered_points

def allocatePoints(p, ordered_points, points, graph):
	all_points, sum_distance, i = 0, 0, 0
	while i < (aco.N-1) and points[p].cur_c <= points[p].c: # While the capacity have not exceeded
		if (points[p].cur_c + ordered_points[i].d) <= points[p].c:
			points[p].cur_c += ordered_points[i].d
			all_points += 1
			sum_distance += graph[p][ordered_points[i].id]
		i += 1
	aco.resetCapacity(points)
	return all_points, sum_distance
