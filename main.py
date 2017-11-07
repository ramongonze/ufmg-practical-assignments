import graph as g
import aco
import matplotlib.pyplot as plt

def main():
	# Data read
	points = g.dataRead() # Point is a list with 'Point' instances, and it represents all the points in the space.
	graph = g.buildGraph(points) # N is the index of the imaginary point
	g.calculateDensity(points, graph) # Each point has a density, based in the distance of the nearests neighbors.
	
	y = []
	for t in range(1, aco.ITERATIONS + 1):
		p_medians = aco.chooseMedians(points, graph)
		aco.buildSolution(p_medians, points, graph)
		s_q = aco.solutionQuality(points, graph)
		aco.saveBestSolution(s_q, points, graph)
		aco.updatePheromones(s_q, p_medians, points, graph, t)
		aco.resetCapacity(points)
		y.append(aco.BEST_SOLUTION)
		#print('BEST: ' + str(aco.BEST_SOLUTION))

	x = range(1, 10000)

	plt.plot(x, y)
	plt.show()
	

main()	