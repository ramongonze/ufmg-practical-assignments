import constants as c

# Post order traversal in binary tree. Return the equation in 'equation'
def post_order(node, equation):
	if node.left != None:
		post_order(node.left, equation)
	if node.right != None:
		post_order(node.right, equation)
	equation.append(node.item)

# Calculates the equation created by 'post_order' function
def evaluate(equation, terminal_values): # terminal_values is a dictionary with the variable and its value
	i, stack = 0, []

	while i < len(equation):
		#push
		stack.append(equation[i])
		i = i + 1

		if stack[-1] in c.functions_set:
			if stack[-1] == '+':
				stack[-3] = stack[-3] + stack[-2]
			elif stack[-1] == '-':
				stack[-3] = stack[-3] - stack[-2]
			elif stack[-1] == '*':
				stack[-3] = stack[-3] * stack[-2]
			elif stack[-1] == '/':
				if stack[-2] != 0: # If the denominator is == 0, returns 1 
					stack[-3] = stack[-3] / stack[-2]
				else:
					return 1
			elif stack[-1] == 'sin':
				stack[-2] = c.math.sin(stack[-2])
			elif stack[-1] == 'cos':
				stack[-2] = c.math.cos(stack[-2])
			elif stack[-1] == 'tan':
				stack[-2] = c.math.tan(stack[-2])
			elif stack[-1] == 'log' and stack[-2] > 0: # If x <= 0, is out of log domain, returns 1
				stack[-2] = c.math.log(stack[-2])
			else:
				return 1

			if stack[-1] not in c.function_for_one_terminal:
				del stack[-2]
			del stack[-1]

		elif type(stack[-1]) == str: # It's a variable, not a constant
			stack[-1] = terminal_values[stack[-1]]

	return stack[0]

def fitness(individual):
	total_sum = 0
	terminal_values = {}
	for i in range(0, c.file_size):
		#Update the dictionary with the new inputs in line 'i' from file_lines
		for j in range(0, c.variables_amount):
			terminal_values[str(j+1)] = float(c.file_lines[i][j])
		#Fitness equation
		equation = []
		post_order(individual.tree, equation)
		x = evaluate(equation, terminal_values)
		total_sum = total_sum + (x - float(c.file_lines[i][-1]))**2

	individual.fitness = (total_sum/c.file_size)**(1/2)

#Calculates the fitness for all individuals, and returns a list with them
def fitness_all(population):
	for ind in population:
		fitness(ind)

############## NICHING ##############
# Sharing function measures the similarity level between the individuals i and j
def sharing(distance_i_j):
	if distance_i_j < c.SIGMA:
		return 1 - (distance_i_j / c.SIGMA)
	else:
		return 0

# Calculates approximately the number of individuals which the fitness is shared
def niche_count(i, population):
	n_count = 0

	for j in population:
		n_count = n_count + sharing(abs(i.fitness - j.fitness))

	return n_count

def fitness_niching(ind, population):
	m = niche_count(ind, population)
	if m == 0:
		return ind.fitness
	else:
		return (ind.fitness * m)

def fitness_niching_all(population):
	for ind in population:
		ind.fitness_niching = fitness_niching(ind, population)