import graph as g
import aco
#import matplotlib.pyplot as plt

def main():
	# Data read
	points = g.dataRead() # Point is a list with 'Point' instances, and it represents all the points in the space.
	graph = g.buildGraph(points) # N is the index of the imaginary point
	g.calculateDensity(points, graph) # Each point has a density, based in the distance of the nearests neighbors.
	
	x = []
	for w in range(1, 2):
		for t in range(1, aco.ITERATIONS + 1):
			p_medians = aco.chooseMedians(points, graph)
			aco.buildSolution(p_medians, points, graph)
			s_q = aco.solutionQuality(points, graph)
			aco.saveBestSolution(s_q, points, graph)
			aco.updatePheromones(s_q, p_medians, points, graph, t)
			aco.resetCapacity(points)
		x.append(aco.BEST_SOLUTION)
		aco.BEST_SOLUTION = None
		aco.B_SOLUTION_MATRIX = []
		for j in range(0, aco.N):
			graph[j][-1] = aco.INITIAL_PHEROMONE
			#print('BEST: ' + str(aco.BEST_SOLUTION))
	"""
	for i in x:
		print(i)	
	print('BEST: ' + str(sum(x)/30))
	#aco.printBestSolution()
	"""

main()	