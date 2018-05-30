all: recommender

evaluate: clean stringMethods
	g++ evaluate.cpp -o evaluate.out stringMethods.o -O2 -O3 -O4 -Wall -std=c++11 -pedantic

recommender: clean stringMethods graph data prediction
	g++ recommender.cpp -o recommender.out graph.o stringMethods.o data.o -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

graph:
	g++ -c graph.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

data:
	g++ -c data.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

stringMethods:
	g++ -c stringMethods.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

prediction:
	g++ -c prediction.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

clean:
	rm -f *.out
	rm -f *.o
