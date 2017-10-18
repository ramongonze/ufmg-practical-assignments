import constants as c
import random

def euclidianDistance(x0, x1, y0, y1):
	return ((x0 - x1)**2 + (y0 - y1)**2)**(1/2)


"""
	It builds a graph with the points using an adjacency matrix. 
	The graph is undirected and there isn't an edge from one node to itself, so the adjacency matrix
	is build only above the main diagonal. Each weight is calculated and the INITIAL PHEROMONE is set as the same for all edges.
"""
def buildGraph(points):
	graph = []

	for i in range(0, c.N):
		graph = graph + [[]]
		for j in range(i+1, c.N):
			graph[i] = graph[i] + [c.Edge(euclidianDistance(points[i].x, points[j].x, points[i].y, points[j].y), c.INITIAL_PHEROMONE)]

	return graph

# Choose 'c.P' points from the graph, and spread the ants into them.
# Returns a list of c.Ant objects, with their own points choosed randomly.
def spreadAnts():
	ants = []
	number_of_choosed_points, choosed_points = 0, []
	for i in range(0, c.NUMBER_OF_ANTS):
		p = random.randint(0, c.N)
		if number_of_choosed_points < c.P:
			if p not in choosed_points:
				choosed_points.append(p)
			number_of_choosed_points += 1
		else:
			p = (random.sample(choosed_points, 1))[0]

		ants.append(c.Ant(p))
	return ants

def aco():
	ants = spreadAnts()
	
	for t in range(1, c.ITERATIONS + 1):
		for i in range(0, c.NUMBER_OF_ANTS):

"""

ACO (maxIt, N, τ0 )
	OK Inicializa τij (igualmente para cada aresta)
	OK Distribui cada uma das k formigas em um nó selecionado aleatoriamente
	OK t = 1
	OK while (t < maxIt) do //número de iterações
		OK for i = 0 to N do //para cada formiga
			1. Constrói uma solução aplicando uma regra de transição
			probabilística (e-1) vezes // e é o número de arestas do grafo
			2. Avalia o custo de cada solução construída
			3. Atualiza melhor solução
		OK end for
		Atualiza as trilhas de feromônio
	OK t = t + 1
	OK end while
"""