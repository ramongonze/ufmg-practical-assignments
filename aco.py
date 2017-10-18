import constants as c

"""
	It builds a graph with the points using an adjacency matrix. 
	The graph is undirected and there isn't an edge from one node to itself, so the adjacency matrix
	is build only above the main diagonal.
"""
def buildGraph(points):
	graph = []

	for i in range(0, c.N):
		graph = graph + [[]]
		for j in range(i+1, c.N):
			graph[i] = graph[i] + [((points[i].x - points[j].x)**2 + (points[i].y - points[j].y)**2)**(1/2)]

	return graph
