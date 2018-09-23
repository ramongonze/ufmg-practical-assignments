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
	X (a) Ler a entrada
	
	X (b) Transforma-la em FPI
	
	(c) Rodar a PL auxiliar para encontrar uma base e verificar se o problema é viável
	(este passo pode ser pulado caso você identifique uma base óbvia).
	
	(d) Se o problema for viável, rodar o Simplex e ou encontrar a solução ótima, ou
	verificar que o problema é ilimitado.
	
	(e) Ao final, deve escrever um arquivo de saı́da. Será dado 1 ponto extra para quem
	incluir certificados no arquivo de saı́da.
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
			c = np.hstack((c,np.array([0])))
			
			v = 1
			if signals[i] == ">=":
				v = -1
			
			x = np.hstack((np.hstack((np.zeros(i),np.array([v]))) , np.zeros(n-i-1)))
			A = np.column_stack((A,x))

	return A,c

def main():

	if len(sys.argv) < 3:
		print("Invalid number of arguments!")
		sys.exit()

	inp_file = sys.argv[1]
	out_file = sys.argv[2]

	A,b,c,signals = readLP(inp_file)
	A,c = transformFPI(A,c,signals)

main()