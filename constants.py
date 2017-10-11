import math
import operator
import random
#import main

# Seed used to control the tests
random.seed()

# A set of nodes composes a binary tree
class Node(object):
	def __init__(self, item, right=None, left=None, level=1):
		self.right = right # Right child
		self.left = left # Left child
		self.item = item
		self.level = level # The level of the node in the tree

# The representation of an individual
class Individual(object):
	def __init__(self, tree=None, fitness=None, fitness_niching=None, size=0):
		self.tree = tree
		self.fitness = fitness
		self.fitness_niching = fitness_niching # The f' function
		self.size = size # The number of nodes in the tree

################## CONSTANTS ####################
MAX_DEPTH = 7 # Max depth of an individual, a binary tree
SIGMA = 15000 # Limit to calculate de relative distance between two individuals, used in fitness sharing
POPULATION_SIZE = 100
NUMBER_OF_GENERATIONS = 500
CROSSOVER_RATE = 20 # In percentage
MUTATION_RATE = 75 # In percentage
REPRODUCTION_RATE = 5 # In percentage
K = 10 # Tournament selection
ELITISM = 1 # The number of individuals will be passed to the next generation
# MIN and MAX are the limits of the interval to get one random number when 'num' is called in terminals_set
MIN = -1
MAX = 1

################## DATABASE ####################
FILE_NAME = None # The database which will be used
file_lines = None # List with all the data
file_size = None # Number of examples
variables_amount = None # Number of variables in the database

#################### SETS #######################
functions_set = ['+', '-', '*', '/', 'log']
functions_for_two_terminals = ['+', '-', '*', '/']
function_for_one_terminal = ['log'] # For example, sin, cos, log
variables_set = []
terminals_set = ['num'] #'num' is a real number between -1 and 1
t_and_f_set = None #Both sets, used in grow method

############################################## RANDOM ELEMENTS ##################################################
# Each function below returns one random element from a set
def random_function():
	return functions_set[random.randint(0, (len(functions_set)-1))]

def random_terminal():
	return terminals_set[random.randint(0, (len(terminals_set)-1))]		

def random_terminal_and_function():
	return t_and_f_set[random.randint(0, (len(t_and_f_set)-1))]	
