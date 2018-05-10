all: clean recommender

recommender: predict.o
	g++ -o recommender recommender.cpp predict.o

predict.o:
	g++ -c predict.cpp

clean:
	rm -f recommender
	rm -f *.o