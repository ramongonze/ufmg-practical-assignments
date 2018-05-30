all: clean recommender

recommender: graph stringMethods data
	g++ recommender.cpp -o recommender graph.o stringMethods.o data.o -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

graph:
	g++ -c graph.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

data:
	g++ -c data.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

stringMethods:
	g++ -c stringMethods.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

clean:
	rm -f recommender
	rm -f *.o
