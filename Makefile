all: clean recommender

recommender: graph.o prediction.o
	g++ recommender.cpp -o recommender graph.o  prediction.o -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

graph.o:
	g++ -c graph.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

prediction.o:
	g++ -c prediction.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

clean:
	rm -f recommender
	rm -f *.o
