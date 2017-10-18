class Ant(object):
	def __init__(self, point=None):
		self.point = point


class Edge(object):
	def __init__(self, weight, pheromone):
		self.weight = weight
		self.pheromone = pheromone

class Point(object):
	def __init__(self, x=None, y=None, c=None, d=None, ant=None):
		self.x = x
		self.y = y
		self.c = c
		self.d = d
		self.ant = ant

#Parameters
N = None # Number of Points
P = None # Number of p-median
ITERATIONS = 100 # Number of iterations, used as "generations"
INITIAL_PHEROMONE = 100 # Initial pheromone
NUMBER_OF_ANTS = None # The number of ants is equals to the number of nodes in the graph
