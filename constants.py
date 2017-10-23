class Ant(object):
	def __init__(self, point=None):
		self.point = point

class Edge(object):
	def __init__(self, weight=None, pheromone=None):
		self.weight = weight
		self.pheromone = pheromone

class Point(object):
	remaining_points = None

	def __init__(self, x, y, c, d, cur_c=0, ants=0, attended_by=None, random_prob=None, s=None):
		self.x = x
		self.y = y
		self.c = c
		self.d = d
		self.cur_c = cur_c # Total sum of the points attended, if this point is a median.
		self.ants = ants # Number of ants in this point.
		self.attended_by = attended_by # Median which attend the point in analysis. A median attends itself.
		self.random_prob = random_prob # Used to select points randomly.
		self.s = s # It's the sum of the distance among the K nearest neighbor of a point.

#Parameters
N = None # Number of Points
P = None # Number of p-median
ITERATIONS = 30 # Number of iterations, used as "generations"
INITIAL_PHEROMONE = 10 # Initial pheromone
NUMBER_OF_ANTS = None # The number of ants is equals to the number of nodes - number of p-median.
IM_POINT = -1 # Index of the imaginary point in the graph
K = 1 # It's the KNN (K-Nearest-neighbor) of a point.