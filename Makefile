all: clean recommender

recommender: graph.o prediction.o
	g++ recommender.cpp -o recommender graph.o  prediction.o -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

graph.o:
	g++ -c graph.cpp -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

prediction.o:
	g++ -c prediction.cpp -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

clean:
	rm -f recommender
	rm -f *.o