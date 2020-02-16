import constants as c
import fitness as fit
import population as pop
import operations as op
import selection as sel
import sys


"""
	This is an Evolutionary Algorithm which uses Genetic Programing to solve the symbolic regression.
	- It treat the individuals as binary trees.
	- The fitness is calculated by the Root Mean Squared Error (RMSE).
	- The operations used on individuals are crossover, mutation and reproduction.
	- The selection method used is the tournament.
	- This algorithm uses a niching method called 'fitness sharing', to increase the diversity of 
	  the population, and treats the premature convergence.
	- All the parameters like operations rates, population size, number of generations, and others 
	  can be managed in the 'constants.py' package.
"""

def main():
	# Data read
	c.FILE_NAME = sys.argv[1]
	file = open(c.FILE_NAME, 'r')
	c.file_lines = file.readlines()
	c.file_size = len(c.file_lines) # Number of examples in the database
	for i in range(0, c.file_size): # All the database is stored in a matrix
		c.file_lines[i] = c.file_lines[i].split(',')
	c.variables_amount = len(c.file_lines[0]) - 1 # Number of variables in the database
	file.close()

	for i in range(0, c.variables_amount): # Add variables into terminal_set and variables_set
		c.terminals_set.append(str(i+1))
		c.variables_set.append(str(i+1))
	c.t_and_f_set = c.terminals_set + c.functions_set

	# Initial population
	population = pop.ramped_half_and_half(c.POPULATION_SIZE)
	fit.fitness_all(population)
	
	for generation in range(0, c.NUMBER_OF_GENERATIONS):
		fit.fitness_niching_all(population)
		population.sort(key=c.operator.attrgetter('fitness')) # Sort the population, since the min fitness value until the max one

		new_pop = population[:c.ELITISM] # Move 'ELITISM' individuals to the next generation
		new_pop = new_pop + sel.tournament_selection(population) # Select the rest of the population (pop_size - elitism) in a tournament
		
		op.operations(new_pop) # Do the crossover, mutation and reproduction with the population choosed by the tournament
		population = new_pop[:] # Updates the actual population to the new, created by the operations

	# Output
	population.sort(key=c.operator.attrgetter('fitness')) # Sort the population, since the min fitness value until the max one
	best_solution_equation = []
	fit.post_order(population[0].tree, best_solution_equation)
	print('Best fitness: {0}'.format(population[0].fitness))
	print('Equation: ' + str(best_solution_equation))

main()