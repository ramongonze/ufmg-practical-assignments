import constants as c
import random

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
			neighbors.append(graph[i][j].weight)
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

	# Choose 'c.P' medians
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


	# Distribute all ants in the medians choosed, based in the sum of KNN.
	total_sum = 0
	for p in choosed_points:
		total_sum += p.s
	
	total_ants = 0
	for p in choosed_points:
		p.ants = int((c.NUMBER_OF_ANTS * p.s)/ total_sum)
		total_ants += p.ants

	# Check if all the ants were selected and moved to one median
	if total_ants < c.NUMBER_OF_ANTS:
		while total_ants < c.NUMBER_OF_ANTS:
			choosed_points[0].random_prob = graph[0][c.IM_POINT].pheromone
			for i in range(1, c.P):
				choosed_points[i].random_prob = choosed_points[i-1].random_prob + graph[i][c.IM_POINT].pheromone
			total_pheromone = choosed_points[-1].random_prob
			p = binarySearch(random.randint(1, total_pheromone), choosed_points)
			choosed_points[p].ants += 1
			total_ants += 1

	return choosed_points_i

def aco(graph, points):
	# List with the ants starting in the imaginary point in the space.
	ants = [c.N] * c.NUMBER_OF_ANTS # c.N is the index of the imaginary point.

	for t in range(1, c.ITERATIONS + 1):
		ants_copy = ants[:]
		c.Point.remaining_points = c.N - c.P
		p_medians = spreadAnts(ants_copy, graph, points) # ants_copy contains indexes of p-medians
		c.Point.remaining_points = c.N - c.P
		# for i in range(0, c.NUMBER_OF_ANTS):
		# 	buildSolution(ants_copy[i], graph, points)

		# if c.Point.remaining_points > 0: # There is at least one point whitout any ant.
		# 	empty_points, medians = [], []
		# 	for i in range(0, c.N):
		# 		if points[i].ants == 0 and points[i].attended_by != i:
		# 			empty_points += [i]

		# 	# Fill empty points. stuffed_points != {}
		# 	for p in empty_points:
		# 		# Choose the closest median
		# 		if p_medians[0] < p:
		# 			min_w = graph[p_medians[0]][p].weight
		# 		else:
		# 			min_w = graph[p][p_medians[0]].weight
		# 		median_i = 0 # Closest median
		# 		i = 0
		# 		while points[p_medians[i]].cur_c + points[p].d > points[p_medians[i]].c:
		# 			i += 1
		# 			if p_medians[i] < p:
		# 				min_w = graph[p_medians[i]][p].weight
		# 			else:
		# 				min_w = graph[p][p_medians[i]].weight
		# 			median_i = i # Closest median

		# 		for i in range(1, c.P):
		# 			if p_medians[i] < median_i:
		# 				if min_w > graph[p_medians[i]][median_i].weight:
		# 					min_w = graph[p_medians[i]][median_i].weight
		# 					median_i = i
		# 			elif min_w > graph[median_i][p_medians[i]].weight:
		# 				min_w = graph[median_i][p_medians[i]].weight
		# 				median_i = i
		# 		points[p].attended_by = median_i
		# 		points[median_i].ants += 1


def buildSolution(ant_median, graph, points):
	av_points = [] # Available points. A list of indexes.
	for i in range(0, c.N):
		if points[ant_median].cur_c + points[i].d <= points[ant_median].c: # The points which would overflow the capacity of the median aren't selected
			if points[i].attended_by == None: # If it's a free point.
				av_points += i
			elif i != ant_median and points[i].attended_by == ant_median: # If the point is occuped by another ant from the same median.
				av_points += i

	if len(av_points) > 1:
		# Transiction rule: It's inversely proportional to the weight and directly proportional to the pheromone.
		points[av_points[0]].random_prob = graph[ant_median][av_points[0]].pheromone * int(100/graph[ant_median][av_points[0]].weight)
		for i in range(1, len(av_points)):
			points[av_points[i]].random_prob = graph[ant_median][av_points[i]].pheromone * int(100/graph[ant_median][av_points[i]].weight)
		total_pheromone = points[av_points[-1]].random_prob
		
		# Choose a point randomly
		p = binarySearch(random.randint(1, total_pheromone), av_points)
		av_points[p].ants += 1
		av_points[i].attended_by = ant_median
		points[ant_median].cur_c += av_points[p].d
		c.Point.remaining_points -= 1

"""
ACO (maxIt, N, τ0 )
	*Inicializa τij (igualmente para cada aresta)
	*Distribui cada uma das k formigas em um nó selecionado aleatoriamente
	*t = 1
	*while (t < maxIt) do //número de iterações
		*for i = 0 to N do //para cada formiga
			Pego a mediana que a formiga i está.
			Aplico a regra de transição (feromonio * 1/peso) em todos os vértices e jogo num pote.
			Pego um ponto p aleatório do pote.
			Enquanto (demanda(p) não superar a capacidade da mediana)
				Pego um ponto p aleatório do pote.
			Movo a formiga para este ponto
		*end for
		2. Avalia o custo de cada solução construída
		3. Atualiza melhor solução
		Atualiza as trilhas de feromônio
		*t = t + 1
	*end while
"""