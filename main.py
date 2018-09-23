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

	GUIDE (from PA-Description):
	
	X (a) Read input
	
	X (b) Tranform it in standard form
	
	(c) Run the auxiliar LP to find a base, and verify if it is a feasible problem (this step can be jumped if you find a straightfoward base);
	
	(d) If the problem is feasible, run Simplex and find the optimal solution, or verify that the problem is unlimited.
	
	(e) In the end you must write an output file. One extra point will be given for who include certificates in the output file.

"""

def readLP(inp_file):

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

# @Parameter T: Tableau
# @Parameter (x,y): Pair in the matrix A that mut be pivoted
def pivot(T,x,y):

	T[x,:] *= 1.0/T[x,y] # Transform the pair x,y in 1

	for i in range((T.shape)[0]): # For each line != pivot
		co = -T[i,y]
		if i != x:
			for j in range((T.shape)[1]): # For each column in the tableau
				T[i,j] += (co*T[x,j])

	return T

def makeCanonical(T,columns):

	# Tableau
	for i in range(len(columns)):
		T = pivot(T,i+1,columns[i])

	return T

def simplex(A,b,c,columns):
	i = 0	
		
	T = np.column_stack( (np.vstack((c*-1,A)) , np.hstack(([0],b))) )
	# columns is the vector with the indexes of a base of LP
	no_negative = True
	while(no_negative);
		for i in range(len((T.shape)[1])-1):
			# Select a column with negative coeficient in c
		# Select the max value I can increase the solution
		# Pivot with the new column
		# Make T canonical


def main():

	if len(sys.argv) < 3:
		print("Invalid number of arguments!")
		sys.exit()

	inp_file = sys.argv[1]
	out_file = sys.argv[2]

	A,b,c,signals = readLP(inp_file)
	A,c = transformFPI(A,c,signals)


main()