all: clean recommender

recommender: graph.o functions.o
	g++ recommender.cpp -o recommender graph.o  functions.o

graph.o:
	g++ -c graph.cpp

functions.o:
	g++ -c functions.cpp

clean:
	rm -f recommender
	rm -f *.o