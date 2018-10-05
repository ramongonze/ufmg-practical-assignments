import sys
import numpy as np
import fractions as frac

"""
	Definitions:

	A = Matrix m x n that represents the constraints
	b = Vetor of size m that represents the right side of the constraints
	c = Vector of coeficients
	x = Vector of variables

	The Linear Programming (LP) will be transformed to the standard form:
		- Is a maximization problem;
		- All the constraints are equalities
		- All the variables are non-negatives

	The L.P has this shape:

		 max cx
		s.t. Ax = b
			 x >= 0

	GUIDE (from PA-Description):
	
	X (a) Read input.
	
	X (b) Tranform it in standard form.
	
	(c) Run the auxiliar LP to find a base, and verify if it is a feasible problem (this step can be jumped if you find a straightfoward base).
	
	X (d) If the problem is feasible, run Simplex and find the optimal solution, or verify that the problem is unlimited.
	
	(e) In the end you must write an output file. One extra point will be given for who include certificates in the output file.
"""

################### TESTS ######################

def feasible(A,b,c,y,z):
	A = np.array(A)
	b = np.array(b)
	c = np.array(c)
	y = y[:(A.shape)[1]]
	z = z[:(A.shape)[1]]

	print(A)
	print(b)
	print(c)
	print(y)
	print(z)

	ans = len(np.where(((c-(y@A)) <= 0) == 0)[0]) == 0
	print(c-(y@A))
	print("LALALA: " + str((y@A)))
	print("c - yA <= 0: " + str(ans))
	ans = (y@b) == (c@z)
	print("yb = cz: " + str(ans))

def unfeasible(A,b,c,certificate):
	A = np.array(A)
	b = np.array(b)
	c = np.array(c)
	certificate = certificate[:(A.shape)[1]]

	ans = len(np.where(((certificate@A) >= 0) == 0)[0]) == 0
	print("yA >= 0: " + str(ans))
	ans = len(np.where(((certificate@b) < 0) == 0)[0]) == 0
	print("yb < 0: " + str(ans))

def unlimited(A,b,c,d):
	A = np.array(A)
	b = np.array(b)
	c = np.array(c)
	d = d[:(A.shape)[1]]

	ans = len(np.where(((A@d) == 0) == 0)[0]) == 0
	print("Ad = 0: " + str(ans))
	ans = len(np.where((d >= 0) == 0)[0]) == 0
	print("d >= 0: " + str(ans))
	ans = len(np.where(((c@d) > 0) == 0)[0]) == 0
	print("cd > 0: " + str(ans))

################################################

def readLP(inp_file):
	"""
		@Parameter inp_file: directory and name of the input file with the LP
	"""

	F = open(inp_file, 'r')
	m = int(F.readline())
	n = int(F.readline())

	# Read non-negative constraints for variables
	neg_constraints = np.array(F.readline().split(' ')).astype("int")

	#c = np.array(F.readline().split(' ')).astype("double")
	c = F.readline().split(' ')
	if c[-1] == '\n':
		c = c[:-1]
	
	c = [int(c[i]) for i in range(len(c))]
	A = []
	for i in range(n):
		row = F.readline().split(' ')
		if row[-1][-1] == '\n':
			row[-1] = row[-1][:-1] # Removes the \n
		A.append(row)
	F.close()

	b = [A[i][-1] for i in range(len(A))]
	signals = [A[i][-2] for i in range(len(A))]
	A = [A[i][:-2] for i in range(len(A))]

	b = np.array([int(b[i]) for i in range(len(b))])
	for i in range(len(A)):
		for j in range(len(A[i])):
			A[i][j] = int(A[i][j])
		
	return A,b,c,signals,neg_constraints

def transformFPI(A,c,signals,neg_constraints):
	"""
		@Parameter A: Matrix of constraints
		@Parameter c: Vector of coeficients
		@Parameter signals: Signals of constraints ("<=",">=" or "==")
	"""

	mask = neg_constraints == 0
	position = {}

	A = list(A)
	c = list(c)

	i, var = 0, 0
	for var in range(len(neg_constraints)):
		if neg_constraints[var] == 0:
			# Free variable
			p1 = c[:i+1] + [-c[i]]
			if i+1 < len(c):
				c = p1 + c[i+1:]
			else:
				c = p1

			for line in range(len(A)):
				p1 = A[line][:i+1] + [-A[line][i]]
				if i+1 < len(A[line]):
					A[line] = p1 + A[line][i+1:]
				else:
					A[line] = p1

			position[var] = (i,i+1)
			i += 2
		else:
			position[var] = i
			i += 1

	for i in range(len(signals)):
		n = len(A)
		if signals[i] != "==":
			c = np.hstack((c,[0]))
			
			v = 1
			if signals[i] == ">=":
				v = -1
			
			x = np.hstack((np.hstack((np.zeros(i),[v])) , np.zeros(n-i-1)))
			A = np.column_stack((A,x))

	A = np.array(A)
	c = np.array(c)

	return A,c,position

def pivot(T,T_cert,x,y):
	"""
		@Parameter T: Tableau
		@Parameter x,y: Pair (x,y) in the tableau T that must be pivoted
	"""
	v = (1/T[x,y])
	T[x,:] *= v # Transform the pair x,y in 1
	T_cert[x,:] *= v

	for i in range((T.shape)[0]):
		if i != x: # For each line != pivot
			co = -T[i,y]
			for j in range((T.shape)[1]): # For each column in the tableau
				T[i,j] += (co*T[x,j])
			for j in range((T_cert.shape)[1]):
				T_cert[i,j] += (co*T_cert[x,j])

	return T,T_cert

def makeCanonical(T,T_cert,columns):
	"""
		@Parameter T: Tableau
		@Parameter columns: Columns that make up a base in the LP
	"""	
	for i in range(len(columns)):
		T,T_cert = pivot(T,T_cert,i+1,columns[i])

	return T,T_cert

def minValue(T,col_index,neg):
	"""
		@Parameter T: Tableau
		@Parameter col_index: Index of a negative value in the vector c of 
							  coeficients
		@Parameter neg: Boolean vector corresponding to the column 'col_index'
						in the tableau T. neg[i] will be False if the value 
						T[i,col_index] <= 0, or True otherwise.
	"""

	for v in range(len(neg)):
		if neg[v] == False:
			MIN = T[v+1,-1]/T[v+1,col_index]
			line = v+1
			break

	for v in range(v+1,len(neg)):
		if neg[v] == False and MIN > T[v+1,-1]/T[v+1,col_index]:
			MIN = T[v+1,-1]/T[v+1,col_index]
			line = v+1

	return line

def simplex(T,columns):
	"""
		@Parameter columns: The vector with indexes of a base of LP
		
		Return value: 
			- Pair (optimal_solution,x), where optimal_solution is the value 
			  of the optimal solution for the given LP; x is the vector of 
			  variables, with their values in the optimal solution.

			- If the LP is unlimited, -1 will be returned.
	"""
	T_cert = np.vstack((np.zeros((T.shape)[0]-1),np.identity((T.shape)[0]-1))) 

	# Transform all integers in fractions
	T_cert = T_cert.astype("object")
	old_shape = T_cert.shape
	T_cert = np.array([frac.Fraction(int(T_cert[i][j]),1) for i in range((T_cert.shape)[0]) for j in range((T_cert.shape)[1])]).reshape(old_shape)

	# Matrix used to find optimality certificate
	T,T_cert = makeCanonical(T,T_cert,columns)
	no_negative = True

	while(no_negative):
		no_negative = False
		# Select a column with negative coeficient in c
		for col_index in range((T.shape)[1]-1):
			if T[0,col_index] < 0:
				non_positve = (T[1:,col_index] <= 0)
				neg = (np.where(non_positve == False))[0]
				if len(neg) == 0:
					# Unlimited LP
					certificate = np.array([]).astype("object")
					certificate = np.array([frac.Fraction(0,1) for i in range((T.shape)[1]-1)])
					# certificate = np.zeros((T.shape)[1]-1).astype("object")
					for i in range(len(columns)):
						certificate[columns[i]] = -T[i+1,col_index]
					certificate[col_index] = frac.Fraction(1,1);
					return False,0,[],certificate,[]
				else:
					no_negative = True
					line = minValue(T,col_index,non_positve)
					columns[line-1] = col_index # Swap a column in the base
					T,T_cert = makeCanonical(T,T_cert,columns)
					break

	x = np.array([])
	for variable in range((T.shape)[1]-1):
		if variable not in columns:
			x = np.hstack((x,[0]))
		else:
			x = np.hstack((x,T[list(columns).index(variable)+1,-1]))
	
	optimal_solution = T[0,-1]
	certificate = T_cert[0]

	return True, optimal_solution, x, certificate, columns

def findBasicSolution(A,b,c):
	# Remove negative values in b
	for line in range(len(b)):
		if b[line] < 0:
			A[line,:] *= -1
			b[line] *= -1

	# Vector of coeficients
	T = np.hstack((np.zeros(len(c)), np.ones(len(b)),np.zeros(1)))

	# Build the complete tableau
	T = np.vstack((T,np.column_stack((np.hstack((A,np.identity(len(b)))),b))))

	T = T.astype("object")
	old_shape = T.shape
	T = np.array([frac.Fraction(int(T[i][j]),1) for i in range((T.shape)[0]) for j in range((T.shape)[1])]).reshape(old_shape)

	columns = [x for x in range(len(c),len(c)+len(b))]

	# Select the columns that make up a feasible solution
	_, feasible_solution, x, certificate, columns = simplex(T,columns)

	return feasible_solution == 0, columns, certificate

def printSolution(out_file,status,solution=None,x=None,certificate=[],position=None):
	output = "Status: " + str(status) + "\n"

	if status == "otimo":
		output += "Objetivo: " + str(solution) + "\n" + "Solucao:\n"
		for _,value in position.items():
			if type(value) == type((0,0)):
				output += str(x[value[0]]-x[value[1]]) + " "
			else:
				output += str(x[value]) + " "

	output = output[:-1] + "\n" # Switch the last space by \n
	output += "Certificado:\n"
	for i in range(len(certificate)):
		output += str(certificate[i]) + " "

	output = output[:-1] + "\n" # Switch the last space by \n
	
	f = open(out_file, "w")
	f.write(output)
	f.close()

def main():
	np.set_printoptions(precision=2)
	if len(sys.argv) < 3:
		print("Invalid number of arguments!")
		sys.exit()

	inp_file = sys.argv[1]
	out_file = sys.argv[2]

	A,b,c,signals,neg_constraints = readLP(inp_file)
	number_variables = len(c)
	A,c,position = transformFPI(A,c,signals,neg_constraints)

	isFeasible, columns, certificate = findBasicSolution(A,b,c)

	if not isFeasible:
		printSolution(out_file,"inviavel",certificate=certificate)
		# unfeasible(A,b,c,certificate)
		return 0

	# Build tableau
	T = np.column_stack((np.vstack((c*-1,A)) , np.hstack(([0],b))))

	# Transform all integers in fractions
	T = T.astype("object")
	old_shape = T.shape
	T = np.array([frac.Fraction(int(T[i][j]),1) for i in range((T.shape)[0]) for j in range((T.shape)[1])]).reshape(old_shape)

	# Run simplex
	NotUnlimited, solution, x, certificate, _ = simplex(T,columns)

	if not NotUnlimited:
		printSolution(out_file,"ilimitado",[],[],certificate,position)
		# unlimited(A,b,c,certificate)
		return 0
	
	printSolution(out_file,"otimo",solution,x,certificate,position)

main()
