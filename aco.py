import constants as c
import random
import operation as op

random.seed()

def euclidianDistance(x0, x1, y0, y1):
	return ((x0 - x1)**2 + (y0 - y1)**2)**(1/2)


"""
	- It builds a graph with the points using an adjacency matrix. 
	- The graph is undirected and there isn't an edge from one node to itself.
	- The last matrix column represents the quantity of pheromone among the immaginary point with all the points of the graph.
"""
def buildGraph(points):
	graph = []

	for i in range(0, c.N):
		graph += [[]]
		for j in range(0, c.N):
			graph[i] += [c.Edge(euclidianDistance(points[i].x, points[j].x, points[i].y, points[j].y), c.INITIAL_PHEROMONE)]
		graph[i] += [c.Edge(pheromone=c.INITIAL_PHEROMONE)]

	return graph

# Let a point x: sumKNN calculates the sum of the distances of the K nearest points of x.
def sumKNN(graph, points):
	
	for i in range(0, c.N):
		neighbors = []
		for j in range(0, c.N):
			neighbors.append(graph[i][j].distance)
		neighbors.sort()
		points[i].s = sum(neighbors[1:(c.K+1)]) # It starts in '1' because the first element is the point itself (distance = 0).

# Returns the index of a point in the array "points". Used to choose randomly a point.
def binarySearch(item, points):
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

# According to pheronomes among the imaginary point and the other points, choose 'c.P' medians and spread the ants into them.
def spreadAnts(ants, graph, points):
	number_of_choosed_points, choosed_points, choosed_points_i = 0, [], []

	points_aux = points[:] # Objects
	points_aux_i = list(range(0, c.N)) # Indexes

	# Choose p medians and store in choosed_points (objects), choosed_points_i (indexes) 
	while number_of_choosed_points < c.P:
		points_aux[0].random_prob = graph[0][c.IM_POINT].pheromone
		for i in range(1, len(points_aux)):
			points_aux[i].random_prob = points_aux[i-1].random_prob + graph[i][c.IM_POINT].pheromone
		total_pheromone = points_aux[-1].random_prob
		# Choose a point
		p = binarySearch(random.randint(1, total_pheromone), points_aux)
		choosed_points_i += [points_aux_i[p]] 
		choosed_points += [points_aux[p]]
		number_of_choosed_points += 1
		del points_aux[p], points_aux_i[p]

	# Distribute all ants in the medians choosed, based in the sum of KNN, inversely proportional.
	factor = 0
	for p in choosed_points:
		factor += 1/p.s
	factor = c.NUMBER_OF_ANTS/factor


	total_ants = 0
	for i in range(0, c.P):
		choosed_points[i].ants += int((1/choosed_points[i].s) * factor)
		ants[total_ants:total_ants+choosed_points[i].ants] = [choosed_points_i[i]] * choosed_points[i].ants
		total_ants += choosed_points[i].ants

	# Check if all the ants were selected and moved to one median
	if total_ants < c.NUMBER_OF_ANTS:
		while total_ants < c.NUMBER_OF_ANTS:
			choosed_points[0].random_prob = graph[0][c.IM_POINT].pheromone
			for i in range(1, c.P):
				choosed_points[i].random_prob = choosed_points[i-1].random_prob + graph[i][c.IM_POINT].pheromone
			total_pheromone = choosed_points[-1].random_prob
			p = binarySearch(random.randint(1, total_pheromone), choosed_points)
			choosed_points[p].ants += 1
			ants[total_ants] = choosed_points_i[p]
			total_ants += 1

	return choosed_points_i # Index list of the p-medians choosed.

def aco(graph, points):
	# List with the ants starting in the imaginary point in the space.
	ants = [c.N] * c.NUMBER_OF_ANTS # c.N is the index of the imaginary point.

	for t in range(1, c.ITERATIONS + 1):
		ants_copy = ants[:]
		p_medians = spreadAnts(ants_copy, graph, points) # ants_copy contains indexes of the p-medians choosed
		c.Point.remaining_points = c.N - c.P
		
		buildSolution(p_medians, points, graph)

		# for i in range(0, c.NUMBER_OF_ANTS):
		# 	buildSolution(ants_copy[i], graph, points) # Each ant in ants_copy receive its p-median: i.e. [2,2,2,4,4,5]

		# if c.Point.remaining_points > 0: # There is at least one point whitout any ant.
		# 	empty_points, medians = [], []
		# 	for i in range(0, c.N):
		# 		if points[i].ants == 0 and points[i].attended_by != i:
		# 			empty_points += [i]

		# 	# Fill empty points. stuffed_points != {}
		# 	for p in empty_points:
		# 		# Choose the closest median
		# 		if p_medians[0] < p:
		# 			min_w = graph[p_medians[0]][p].distance
		# 		else:
		# 			min_w = graph[p][p_medians[0]].distance
		# 		median_i = 0 # Closest median
		# 		i = 0
		# 		while points[p_medians[i]].cur_c + points[p].d > points[p_medians[i]].c:
		# 			i += 1
		# 			if p_medians[i] < p:
		# 				min_w = graph[p_medians[i]][p].distance
		# 			else:
		# 				min_w = graph[p][p_medians[i]].distance
		# 			median_i = i # Closest median

		# 		for i in range(1, c.P):
		# 			if p_medians[i] < median_i:
		# 				if min_w > graph[p_medians[i]][median_i].distance:
		# 					min_w = graph[p_medians[i]][median_i].distance
		# 					median_i = i
		# 			elif min_w > graph[median_i][p_medians[i]].distance:
		# 				min_w = graph[median_i][p_medians[i]].distance
		# 				median_i = i
		# 		points[p].attended_by = median_i
		# 		points[median_i].ants += 1


def sortRemPoints(p_medians, points, graph):
	sorted_points = [] # All the points aren't medians
	for i in range(0, c.N):
		if points[i].attended_by != i: # Ignore the medians
			aux_list = [] # aux_list will contain all the distances among the point 'i' and the p_medians
			for j in p_medians:
				aux_list += [graph[i][j].distance]
				points[i].medians_distances += [points[j]]
			points[i].nearest_median = min(aux_list)
			sorted_points += [points[i]]

	sorted_points.sort(key=op.attrgetter('nearest_median')) # Order the points according the nearest median

	return sorted_points

def sortMedians(p, p_medians):
	sorted_medians = []	


	

def buildSolution(p_medians, points, graph):
	ordered_rem_points = sortRemPoints(p_medians, points, graph) # Ordered remaining points, according to the nearest median

	for p in ordered_rem_points:
		ordered_medians = p.medians



	