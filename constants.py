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

#Parameters
N = None # Number of Points
P = None # Number of p-median
ITERATIONS = 20 # Number of iterations, used as "generations"
INITIAL_PHEROMONE = 0.5 # Initial pheromone
NUMBER_OF_ANTS = None # The number of ants is equals to the number of nodes - number of p-median.
IM_POINT = -1 # Index of the imaginary point in the graph
RANDOM_FACTOR = 1 # A factor used to change the point[p].random_prob to > 0.
DECAY = 0.9 # Its the decay rate of the pheromones trails.
ALPHA = 1 # A factor to give emphasis to the current pheromone level
BETA = 1 # A factor to give emphasis to the point density
BEST_SOLUTION = None # The sum of all distances in the best solution found until that moment.
B_SOLUTION_MATRIX = []# A matrix N x N (number of points) built by:
					  # m[i][j] = 1 if the point i is allocated to the median j or i == j;
					  #           0 otherwise
