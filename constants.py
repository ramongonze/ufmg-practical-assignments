class Ant(object):
	def __init__(self, point=None):
		self.point = point

class Edge(object):
	def __init__(self, weight=None, pheromone=None):
		self.weight = weight
		self.pheromone = pheromone

class Point(object):
	remaining = 0

	def __init__(self, x=None, y=None, c=None, d=None, ant=False):
		self.x = x
		self.y = y
		self.c = c
		self.d = d
		self.ant = ant




#Parameters
N = None # Number of Points
P = None # Number of p-median
ITERATIONS = 10 # Number of iterations, used as "generations"
INITIAL_PHEROMONE = 10 # Initial pheromone
NUMBER_OF_ANTS = None # The number of ants is equals to the number of nodes - number of p-median.
