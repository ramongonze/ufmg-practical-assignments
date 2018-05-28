all: clean recommender

recommender: graph.o data.o
	g++ recommender.cpp -o recommender graph.o  data.o -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

graph.o:
	g++ -c graph.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

data.o:
	g++ -c data.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

clean:
	rm -f recommender
	rm -f *.o
