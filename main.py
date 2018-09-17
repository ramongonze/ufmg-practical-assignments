import sys
import numpy as np

"""
	Definitions:

	A = Matrix m x n that represents the constraints
	b = Vetor of size m that represents the right side of the constraints
	c = Vector of coeficients
	x = Vector of solution

	The linear programming will be transformed to the standard form:
		- Is a maximization problem;
		- All the constraints are equalities
		- All the variables are non-negatives

	The L.P has this shape:

		 max cx
		s.t. Ax = b
			 x >= 0
"""

def readLP(inp_file):

	F = open(inp_file, 'r')
	m = int(F.readline())
	n = int(F.readline())

	F.readline() # Ignores the third line

	c = np.array(F.readline().split(' ')).astype("double")

	A = []
	for i in range(m):
		row = F.readline().split(' ')
		if row[-1][-1] == '\n':
			row[-1] = row[-1][:-1] # Removes the \n
		A.append(row)
	F.close()

	A = np.array(A);
	b = np.array(A[:,-1]).astype("double")
	signals = A[:,-2]
	A = np.array(A[:,0:-2]).astype("double")
	
	return A,b,signals	

def main():

	if len(sys.argv) < 3:
		print("Invalid number of arguments!")
		sys.exit()

	inp_file = sys.argv[1]
	out_file = sys.argv[2]

	A,b,signals = readLP(inp_file);

main()