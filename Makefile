all: clean recommender

recommender: graph.o prediction.o
	g++ recommender.cpp -o recommender graph.o  prediction.o

graph.o:
	g++ -c graph.cpp

prediction.o:
	g++ -c prediction.cpp

clean:
	rm -f recommender
	rm -f *.o