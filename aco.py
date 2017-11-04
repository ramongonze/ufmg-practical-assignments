import constants as c
import random
import operator as op

random.seed(3)

def euclidianDistance(x0, x1, y0, y1):
	return ((x0 - x1)**2 + (y0 - y1)**2)**(1/2)

def buildGraph(points):
	"""
	- It builds a graph with the points using an adjacency matrix.
	- graph[i][j] contains the Euclidian distance between the nodes i and j. 
	- The graph is undirected and there isn't an edge from one node to itself.
	- The last matrix column represents the quantity of pheromone among the immaginary point with all the points of the graph.
	"""
	graph = []

	for i in range(0, c.N):
		graph.append([])
		for j in range(0, c.N):
			graph[i].append(euclidianDistance(points[i].x, points[j].x, points[i].y, points[j].y))
		graph[i].append(c.INITIAL_PHEROMONE)

	return graph

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

def chooseMedians(graph, points):
	# According to pheronomes among the imaginary point and the other points, choose P medians.
	medians = []

	points_aux, min_value = points[:], None
	for i in range(0, c.P): # Choose 'P' medians.
		points_aux[0].random_prob = (graph[0][c.IM_POINT])**c.ALPHA * (points_aux[0].density)**c.BETA
		if min_value == None:
			min_value = points_aux[0].random_prob
		elif min_value > points_aux[0].random_prob:
			min_value = points_aux[0].random_prob
		for i in range(1, len(points_aux)):
			points_aux[i].random_prob = points_aux[i-1].random_prob + ((graph[i][c.IM_POINT])**c.ALPHA * (points_aux[i].density)**c.BETA)
			if min_value > ((graph[i][c.IM_POINT])**c.ALPHA * (points_aux[i].density)**c.BETA):
				min_value = ((graph[i][c.IM_POINT])**c.ALPHA * (points_aux[i].density)**c.BETA)
		
		# Change all the probabilities to > 0.
		while int(min_value*c.RANDOM_FACTOR) == 0:
			c.RANDOM_FACTOR *= 10
		for p in range(0, len(points_aux)):
			points_aux[p].random_prob *= c.RANDOM_FACTOR
		total_pheromone = points_aux[-1].random_prob

		# Choose a point
		p = binarySearch(random.randint(1, int(total_pheromone)), points_aux)
		medians.append(points_aux[p].id)
		points_aux[p].attended_by = points_aux[p].id
		del points_aux[p]

	return medians # Return an index list of the points choosed as medians.

def calculateDensity(points, graph):
	for p in range(0, c.N):
		ordered_points = sortPoints(p, points, graph)
		all_points, sum_distance = allocatePoints(p, ordered_points, points, graph)
		points[p].density = all_points / sum_distance

def allocatePoints(p, ordered_points, points, graph):
	all_points, sum_distance, i = 0, 0, 0
	while i < (c.N-1) and points[p].cur_c <= points[p].c: # While the capacity have not exceeded
		if (points[p].cur_c + ordered_points[i].d) <= points[p].c:
			points[p].cur_c += ordered_points[i].d
			all_points += 1
			sum_distance += graph[p][ordered_points[i].id]
		i += 1
	resetCapacity(points)
	return all_points, sum_distance

def sortPoints(p, points, graph):
	# Sort all points based on their distance to node p(p = point.id).

	ordered_points = points[:]
	del ordered_points[p] # Remove the point p itself

	for i in ordered_points:
		i.point_distance = graph[p][i.id]

	ordered_points.sort(key=op.attrgetter('point_distance'))

	return ordered_points

def aco(graph, points):

	for t in range(1, c.ITERATIONS + 1):
		p_medians = chooseMedians(graph, points)
		buildSolution(p_medians, points, graph)
		s_q = solutionQuality(points, graph)
		updatePheromones(s_q, p_medians, graph)
		saveBestSolution(s_q, points, graph)
		print('BEST: ' + str(c.BEST_SOLUTION))
		resetCapacity(points)


def saveBestSolution(s_q, points, graph):
	if c.BEST_SOLUTION == None:
		c.BEST_SOLUTION = s_q
		for i in range(0, c.N):
			c.B_SOLUTION_MATRIX.append([])
			for j in range(0, c.N):
				if points[i].attended_by == j:
					c.B_SOLUTION_MATRIX[i].append(1)
				else:
					c.B_SOLUTION_MATRIX[i].append(0)

	elif s_q < c.BEST_SOLUTION:
		c.BEST_SOLUTION = s_q
		for i in range(0, c.N):
			for j in range(0, c.N):
				if points[i].attended_by == j:
					c.B_SOLUTION_MATRIX[i][j] = 1
				else:
					c.B_SOLUTION_MATRIX[i][j] = 0

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
	for i in range(0, c.N):
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
	distances_sum = 0
	for p in points:
		distances_sum += graph[p.id][p.attended_by]

	return distances_sum

def updatePheromones(s_q, p_medians, graph):
	for m in p_medians:
		graph[m][-1] = (c.DECAY * graph[m][-1]) + (1/s_q)

def resetCapacity(points):
	for p in points:
		p.cur_c = p.d

