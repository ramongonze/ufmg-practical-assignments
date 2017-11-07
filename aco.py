import operator as op
import random
import graph as g

random.seed(5)

#Parameters
N = None # Number of Points
P = None # Number of p-median
ITERATIONS = 10000 # Number of iterations, used as "generations"
INITIAL_PHEROMONE = 0.5 # Initial pheromone
IM_POINT = -1 # Index of the imaginary point in the graph
RANDOM_FACTOR = 1 # A factor used to change the point[p].random_prob to > 0.
DECAY = 0.99 # Its the decay rate of the pheromones trails.
CONST_FOR_INVALID_SOLUTION = -100000
ALPHA = 1 # A factor to give emphasis to the current pheromone level
BETA = 1 # A factor to give emphasis to the point density
BEST_SOLUTION = None # The sum of all distances in the best solution found until that moment.
B_SOLUTION_MATRIX = []# A matrix N x N (number of points) built by:
					  # m[i][j] = 1 if the point i is allocated to the median j or i == j;
					  #           0 otherwise


def binarySearch(item, points):
	# Returns the index of a point in the array "points". Used to choose randomly a point.
	begin, end = 0, len(points)

	while (end - begin) > 1:
		mid = int((begin+end)/2)
		if item > points[mid].random_prob:
			begin = mid
		elif item < points[mid].random_prob:
			end = mid
		else:
			return mid

	if item <= points[begin].random_prob:
		return begin
	else:
		return end

def chooseMedians(points, graph):
	# According to pheronomes among the imaginary point and the other points, choose P medians.
	medians = []

	points_aux, min_value = points[:], None
	for i in range(0, P): # Choose p medians.

		# Put the probabilities in a roullete of integers, and take one random integer.
		points_aux[0].random_prob = (graph[0][IM_POINT])**ALPHA * (points_aux[0].density)**BETA
		if min_value == None or min_value > points_aux[0].random_prob:
			min_value = points_aux[0].random_prob
		
		for j in range(1, (N-i)):
			points_aux[j].random_prob = points_aux[j-1].random_prob + ((graph[j][IM_POINT])**ALPHA * (points_aux[j].density)**BETA)
			if min_value > points_aux[j].random_prob - points_aux[j-1].random_prob:	
				min_value = points_aux[j].random_prob - points_aux[j-1].random_prob
		
		# Change all the probabilities to > 0.
		global RANDOM_FACTOR
		try:
			while int(min_value*RANDOM_FACTOR) == 0:
				RANDOM_FACTOR *= 100
		except:
			None
		
		for p in range(0, (N-i)):
			points_aux[p].random_prob *= RANDOM_FACTOR
		total_pheromone = points_aux[-1].random_prob

		# Choose a point
		p = binarySearch(random.randint(1, int(total_pheromone)), points_aux)
		medians.append(points_aux[p].id)
		points_aux[p].attended_by = points_aux[p].id
		del points_aux[p]

	return medians # Return an index list of the points choosed as medians.

########################################################

def saveBestSolution(s_q, points, graph):
	global BEST_SOLUTION
	global B_SOLUTION_MATRIX
	if s_q > 0:
		if BEST_SOLUTION == None:
			BEST_SOLUTION = s_q
			for i in range(0, N):
				B_SOLUTION_MATRIX.append([])
				for j in range(0, N):
					if points[i].attended_by == j:
						B_SOLUTION_MATRIX[i].append(1)
					else:
						B_SOLUTION_MATRIX[i].append(0)

		elif s_q < BEST_SOLUTION:
			BEST_SOLUTION = s_q
			for i in range(0, N):
				for j in range(0, N):
					if points[i].attended_by == j:
						B_SOLUTION_MATRIX[i][j] = 1
					else:
						B_SOLUTION_MATRIX[i][j] = 0

def buildSolution(p_medians, points, graph):
	ordered_rem_points = sortRemPoints(p_medians, points, graph) # Ordered remaining points, according to the nearest median

	for p in ordered_rem_points:
		ordered_medians = sortMedians(p, p_medians, points, graph)
		for m in ordered_medians:
			if (m.cur_c + p.d) <= m.c:
				p.attended_by = m.id
				m.cur_c += p.d
				break

def sortRemPoints(p_medians, points, graph):
	sorted_points = [] # All the points aren't medians
	for i in range(0, N):
		if points[i].attended_by != i: # Ignore the medians
			aux_list = [] # aux_list will contain all the distances among the point 'i' and the p_medians
			for j in p_medians:
				aux_list.append(graph[i][j])
			
			points[i].nearest_median = min(aux_list) # Gets the distance between the point i and its nearest neighbor.
			sorted_points.append(points[i])

	sorted_points.sort(key=op.attrgetter('nearest_median')) # Order the points according the nearest median

	return sorted_points # Returns a list of objects

def sortMedians(p, p_medians, points, graph):
	sorted_medians = []	

	for i in p_medians:
		points[i].point_distance = graph[i][p.id]
		sorted_medians.append(points[i])

	sorted_medians.sort(key=op.attrgetter('point_distance'))

	return sorted_medians # Returns a list of objects

def solutionQuality(points, graph):
	# Test if its a valid solution or not
	distances_sum = 0
	for p in points:
		if p.attended_by == None:
			return CONST_FOR_INVALID_SOLUTION
		else:
			distances_sum += graph[p.id][p.attended_by]

	return distances_sum

def updatePheromones(s_q, p_medians, points, graph, t):
	# Factors used to set the maximum and minimum pheromone value.
	phe_sum = 1
	for i in range(1, N+1):
		phe_sum += DECAY**(t-i)
	if len(g.sys.argv) > 2: # The optimal solution is known.
		F = float(g.sys.argv[2])
	elif t == 1:
		F = 1
	else:
		F = 1/BEST_SOLUTION
	
	MAX_PHEROMONE = (1/(1-DECAY))*(1/F)
	MIN_PHEROMONE = MAX_PHEROMONE/ (2*N)

	for p in points:
		graph[p.id][-1] = DECAY * graph[p.id][-1]
		if p.id in p_medians:
			graph[p.id][-1] += 1/s_q

		if graph[p.id][-1] < MIN_PHEROMONE :# Test the minimum
			graph[p.id][-1] = MIN_PHEROMONE
		elif graph[p.id][-1] > MAX_PHEROMONE: # Test the maximum
			graph[p.id][-1] = MAX_PHEROMONE

def resetCapacity(points):
	for p in points:
		p.cur_c = p.d
		p.attended_by = None
		p.random_prob = None
		p.nearest_median = None
		p.point_distance = None
		RANDOM_FACTOR = 1

def printBestSolution():
	
	if BEST_SOLUTION == None:
		print('No valid solution generated!')
	else:
		print(BEST_SOLUTION)
		for i in range(0, N):
			for j in range(0, N):
				if j != (N-1):
					print(str(B_SOLUTION_MATRIX[i][j]) + ',', end='')
				else:
					print(B_SOLUTION_MATRIX[i][j])
