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
NUMBER_OF_ANTS = None
