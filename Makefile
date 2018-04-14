all: clean recommender

recommender: graph.o prediction.o
	g++ recommender.cpp -o recommender graph.o  prediction.o -O2 -Wall -Wextra -Werror -std=c++11 -pedantic

graph.o:
	g++ -c graph.cpp

prediction.o:
	g++ -c prediction.cpp

clean:
	rm -f recommender
	rm -f *.o