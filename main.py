import sys
import numpy as np

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

def readLP(inp_file):
	"""
		@Parameter inp_file: directory and name of the input file with the LP
	"""

	F = open(inp_file, 'r')
	m = int(F.readline())
	n = int(F.readline())

	F.readline() # Ignore the third line

	c = np.array(F.readline().split(' ')).astype("double")

	A = []
	for i in range(n):
		row = F.readline().split(' ')
		if row[-1][-1] == '\n':
			row[-1] = row[-1][:-1] # Removes the \n
		A.append(row)
	F.close()

	A = np.array(A);
	b = np.array(A[:,-1]).astype("double")
	signals = A[:,-2]
	A = np.array(A[:,0:-2]).astype("double")
	
	return A,b,c,signals

def transformFPI(A,c,signals):
	"""
		@Parameter A: Matrix of constraints
		@Parameter c: Vector of coeficients
		@Parameter signals: Signals of constraints ("<=",">=" or "==")
	"""

	for i in range(len(signals)):
		n = A.shape[0]
		if signals[i] != "==":
			c = np.hstack((c,[0]))
			
			v = 1
			if signals[i] == ">=":
				v = -1
			
			x = np.hstack((np.hstack((np.zeros(i),[v])) , np.zeros(n-i-1)))
			A = np.column_stack((A,x))

	return A,c

def pivot(T,x,y):
	"""
		@Parameter T: Tableau
		@Parameter (x,y): Pair in the tableau T that must be pivoted
	"""

	T[x,:] *= 1.0/T[x,y] # Transform the pair x,y in 1

	for i in range((T.shape)[0]): # For each line != pivot
		co = -T[i,y]
		if i != x:
			for j in range((T.shape)[1]): # For each column in the tableau
				T[i,j] += (co*T[x,j])

	return T

def makeCanonical(T,columns):
	"""
		@Parameter T: Tableau
		@Parameter columns: Columns that make up a base in the LP
	"""

	for i in range(len(columns)):
		T = pivot(T,i+1,columns[i])

	return T

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

def simplex(A,b,c,columns):
	"""
		@Parameter columns: The vector with indexes of a base of LP
		
		Return value: 
			- Pair (optminal_solution,x), where optimal_solution is the value 
			  of the optimal solution for the given LP; x is the vector of 
			  variables, with their values in the optimal solution.

			- If the LP is unlimited, -1 will be returned.
	"""
		
	T = np.column_stack( (np.vstack((c*-1,A)) , np.hstack(([0],b))) )
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
					return -1
				else:
					no_negative = True
					line = minValue(T,col_index,non_positve)
					columns[line-1] = col_index # Swap a column in the base
					T = makeCanonical(T,columns)
					break

	x = np.array([])
	for variable in range((T.shape)[1]-1):
		if variable not in columns:
			x = np.hstack((x,[0]))
		else:
			x = np.hstack((x,T[list(columns).index(variable)+1,-1]))
	
	optminal_solution = T[0,-1]

	return optminal_solution,x

def main():

	if len(sys.argv) < 3:
		print("Invalid number of arguments!")
		sys.exit()

	inp_file = sys.argv[1]
	out_file = sys.argv[2]

	A,b,c,signals = readLP(inp_file)
	A,c = transformFPI(A,c,signals)

	columns = [2,3,4]
	solution,x = simplex(A,b,c,columns)

main()