import constants as c
import random
import operator as op

random.seed()

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

# According to pheronomes among the imaginary point and the other points, choose P medians.
def chooseMedians(graph, points):
	medians = []

	points_aux = points[:]
	for i in range(0, c.P): # Choose 'P' medians.
		points_aux[0].random_prob = graph[0][c.IM_POINT]
		for i in range(1, len(points_aux)):
			points_aux[i].random_prob = points_aux[i-1].random_prob + graph[i][c.IM_POINT]
		total_pheromone = points_aux[-1].random_prob
		
		# Choose a point
		p = binarySearch(random.randint(1, total_pheromone), points_aux)
		medians.append(points_aux[p].id)
		del points_aux[p]

	return medians # Return an index list of the points choosed as medians.

def aco(graph, points):

	for t in range(1, c.ITERATIONS + 1):
		p_medians = chooseMedians(graph, points)
		buildSolution(p_medians, points, graph)


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




	