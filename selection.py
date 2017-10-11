import constants as c
import operations as op

#It returns a new population, with the individuals choosed by a tournament
def tournament_selection(population):
	new_pop = []
	len_new_pop = len(population) - c.ELITISM
	for i in range(0, len_new_pop):
		# Choose k individuals randomly
		choosed = c.random.sample(range(0, c.POPULATION_SIZE), c.K)
		# Take the best individual among the choosed individuals above
		m = population[choosed[0]].fitness_niching
		ind_j = choosed[0]
		for j in range(1, len(choosed)):
			if population[choosed[j]].fitness_niching < m:
				m = population[choosed[j]].fitness_niching
				ind_j = choosed[j]
		# Add this new best individual to the new population
		new_pop.append(copy_individual(population[ind_j]))

	return new_pop

#Returns a new individual equals to 'original'
def copy_individual(original):
	new = c.Individual(tree=c.Node(0, level=1)) 
	op.copy_tree(original.tree, new.tree)
	new.fitness = original.fitness
	new.fitness_niching = original.fitness_niching
	new.size = original.size
	return new