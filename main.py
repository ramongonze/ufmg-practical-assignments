import constants as c
import aco
import matplotlib.pyplot as plt

def main():
	# Data read
	points = aco.dataRead() # Point is a list with 'Point' instances, and it represents all the points in the space.
	graph = aco.buildGraph(points) # N is the index of the imaginary point
	aco.calculateDensity(points, graph) # Each point has a density, based in the distance of the nearests neighbors.

	for t in range(1, c.ITERATIONS + 1):
		p_medians = aco.chooseMedians(points, graph)
		aco.buildSolution(p_medians, points, graph)
		s_q = aco.solutionQuality(points, graph)
		aco.saveBestSolution(s_q, points, graph)
		print('BEST: ' + str(c.BEST_SOLUTION))
		aco.updatePheromones(s_q, p_medians, points, graph)
		aco.resetCapacity(points)

main()