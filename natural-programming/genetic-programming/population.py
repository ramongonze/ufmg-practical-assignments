import constants as c

################################################## FULL ########################################################
"""
	The full method creates complete binary trees (the individuals) with the 'max_individual_depth' passed.
	All the leaves of the tree are elements of c.terminals_set, and the inter nodes are all elements of
	c.functions_set. 
"""

# If a terminal element choosed is 'num', replace it with a real number in the interval [c.MIN,c.MAX]
def build_full(node, max_individual_depth, current_depth, tree_size):
	tree_size[0] = tree_size[0] + 1
	node.level = current_depth - 1
	if current_depth <= max_individual_depth:
		if node.item in c.function_for_one_terminal:
			node.left = c.Node(c.random_function())
			build_full(node.left, max_individual_depth, current_depth+1, tree_size)
		else:
			node.left = c.Node(c.random_function())
			node.right = c.Node(c.random_function())
			build_full(node.left, max_individual_depth, current_depth+1, tree_size)
			build_full(node.right, max_individual_depth, current_depth+1, tree_size)
	else: 
		node.item = c.random_terminal()
		if node.item == 'num': 
			node.item = c.random.uniform(c.MIN, c.MAX)

def full(max_individual_depth, tree_size):
	root, root.level = c.Node(c.random_function()), 1
	build_full(root, max_individual_depth, 2, tree_size)
	return root

################################################## GROW #########################################################
"""
	The grow method creates irregular binary trees (the individuals) with the 'max_individual_depth' passed.
	In this creation method, each node is a random element from the c.terminals_set or c.functions_set. This build
	individuals with equal or less size (total number of nodes in the tree) of the individuals created with 'full'
	method above. 
	- 50% to choose one set (between terminals and functions set), and after, choose one element in the set.
	- If a terminal element choosed is 'num', replace it with a real number in the interval [c.MIN,c.MAX]
"""
def build_grow(node, max_individual_depth, current_depth, tree_size):
	tree_size[0] = tree_size[0] + 1
	node.level = current_depth - 1
	if current_depth <= max_individual_depth and node.item in c.functions_set:
		if node.item in c.function_for_one_terminal:
			node.left = c.Node(c.random_terminal_and_function())
			if node.left.item == 'num': 
				node.left.item = c.random.uniform(c.MIN, c.MAX)
			build_grow(node.left, max_individual_depth, current_depth+1, tree_size)
		else:		
			node.left = c.Node(c.random_terminal_and_function())
			if node.left.item == 'num': 
				node.left.item = c.random.uniform(c.MIN, c.MAX)
			node.right = c.Node(c.random_terminal_and_function())
			if node.right.item == 'num': 
				node.right.item = c.random.uniform(c.MIN, c.MAX)
			build_grow(node.left, max_individual_depth, current_depth+1, tree_size)
			build_grow(node.right, max_individual_depth, current_depth+1, tree_size)
	elif current_depth > max_individual_depth:
		node.item = c.random_terminal()
		if node.item == 'num': 
			node.item = c.random.uniform(c.MIN, c.MAX)

def grow(max_individual_depth, tree_size):
	root, root.level = c.Node(c.random_function()), 1
	build_grow(root, max_individual_depth, 2, tree_size)
	return root
#################################################################################################################

################################################## RAMPED HALF AND HALF #########################################
"""
	This method uses the methods 'grow' and 'full' together to generate individuals. Let n the population size and max the
	max depth of an individual (max depth = max number of levels in a tree). Will be created n/(max-1) individuals for each
	max depth among 2,3,...,max. For example, when max = 5 and n = 32, will be created 32/4 = 8 individuals with 2 levels, 
	8 individuals with 3 levels, and so on. And to increase the diversity, among these 8 individuals, half will be created 
	using the grow method and the other half using the full method.
"""

# Population size has to be >= c.MAX_DEPTH. It returns a list of trees
def ramped_half_and_half(population_size):
	individuals_amount = int((population_size/(c.MAX_DEPTH-1))) #Amount of individuals in each level of the tree
	rest = population_size - (c.MAX_DEPTH-1) * individuals_amount

	rest_amounts = []
	for i in range(0, rest):
		rest_amounts.append(c.random.randint(2, c.MAX_DEPTH))

	population = []
	for i in range(2, c.MAX_DEPTH+1):
		for j in range(0, int((individuals_amount + rest_amounts.count(i))/2)): #Half of individuals_amount are grow and half full
			s = [0]
			population.append(c.Individual(tree=grow(i, s)))
			population[-1].size = s[0]
			s = [0]
			population.append(c.Individual(tree=full(i, s)))
			population[-1].size = s[0]
		if (individuals_amount + rest_amounts.count(i))%2 == 1:
			if c.random.randint(0,1) == 1:
				s = [0]
				population.append(c.Individual(tree=grow(i, s)))
				population[-1].size = s[0]
			else:
				s = [0]
				population.append(c.Individual(tree=full(i, s)))
				population[-1].size = s[0]
	return population
