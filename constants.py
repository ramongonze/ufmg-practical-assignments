class Ant(object):
	def __init__(self, point=None):
		self.point = point

class Point(object):
	remaining_points = None

	def __init__(self, id, x, y, c, d, cur_c=0, attended_by=None, random_prob=None, nearest_median=None, point_distance=None):
		self.id = id
		self.x = x
		self.y = y
		self.c = c
		self.d = d
		self.cur_c = cur_c # Current capacity used for this point
		self.attended_by = attended_by # Median which attend the point in analysis. A median attends itself.
		self.random_prob = random_prob # Used to select points randomly.
		self.nearest_median = nearest_median # Used to create a sorted list of points in ascending order, in function sortRemPoints().
		self.point_distance = point_distance # Used to create a sorted list of medians in ascending order, in function sortMedians().

#Parameters
N = None # Number of Points
P = None # Number of p-median
ITERATIONS = 1 # Number of iterations, used as "generations"
INITIAL_PHEROMONE = 10 # Initial pheromone
NUMBER_OF_ANTS = None # The number of ants is equals to the number of nodes - number of p-median.
IM_POINT = -1 # Index of the imaginary point in the graph
K = 3 # It's the KNN (K-Nearest-neighbor) of a point.