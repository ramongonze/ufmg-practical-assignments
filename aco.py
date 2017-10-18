import constants as c
import random

def euclidianDistance(x0, x1, y0, y1):
	return ((x0 - x1)**2 + (y0 - y1)**2)**(1/2)


"""
	- It builds a graph with the points using an adjacency matrix. 
	- The graph is undirected and there isn't an edge from one node to itself, so the adjacency matrix
	  is build only above the main diagonal. Each weight is calculated and the INITIAL PHEROMONE is set as the same for all edges.
	- The last matrix column represents the quantity of pheromone among the immaginary point with all the points of the graph.
"""
def buildGraph(points, im_point):
	graph = []

	for i in range(0, c.N):
		graph += [[]]
		for j in range(i+1, c.N):
			graph[i] += [c.Edge(euclidianDistance(points[i].x, points[j].x, points[i].y, points[j].y), c.INITIAL_PHEROMONE)]
		graph[i] += [c.Edge(pheromone=c.INITIAL_PHEROMONE)]

	return graph

# According to pheronomes among the imaginary point and the other points, choose 'c.P' medians.
def spreadAnts(ants, graph, points):

	number_of_choosed_points, choosed_points = 0, []
	# Calculates the probability to choose each point.
	pot = []
	for i in range(0, c.N):
		pot += [i] * graph[i][-1].pheromone

	for i in range(0, c.NUMBER_OF_ANTS):
		p = (random.sample(pot, 1))[0]
		if number_of_choosed_points < c.P:
			if p not in choosed_points:
				choosed_points.append(p)
			number_of_choosed_points += 1
			if number_of_choosed_points == c.P:
				pot2 = []
				print(choosed_points)
				for j in range(0, c.P):
					pot2 += [choosed_points[j]] * graph[choosed_points[j]][-1].pheromone
		else:
			p = (random.sample(pot2, 1))[0]
		ants[i] = (points[p].x, points[p].y)
	

# Returns a point in the middle of the space.
def imaginaryPoint(max_and_min):
	return (int((max_and_min['max_x']+max_and_min['min_x'])/2), int((max_and_min['max_y']+max_and_min['min_y'])/2))

def aco(im_point, graph, points):
	# List with the ants starting in the imaginary point in the space.
	ants = [im_point] * c.NUMBER_OF_ANTS
	print('NUMBER_OF_ANTS: ' + str(c.NUMBER_OF_ANTS))
	print('im_point: ' + str(im_point))
	print('ANTS:')
	print(ants)
	#for t in range(1, c.ITERATIONS + 1):
	ants_copy = ants[:]
	spreadAnts(ants_copy, graph, points)
	print('AFTER:')
	print(ants_copy)
		#for i in range(0, c.NUMBER_OF_ANTS):


"""

ACO (maxIt, N, τ0 )
	*Inicializa τij (igualmente para cada aresta)
	*Distribui cada uma das k formigas em um nó selecionado aleatoriamente
	*t = 1
	*while (t < maxIt) do //número de iterações
		*for i = 0 to N do //para cada formiga
			1. Constrói uma solução aplicando uma regra de transição
			probabilística (e-1) vezes // e é o número de arestas do grafo
			2. Avalia o custo de cada solução construída
			3. Atualiza melhor solução
		*end for
		Atualiza as trilhas de feromônio
		*t = t + 1
	*end while
"""