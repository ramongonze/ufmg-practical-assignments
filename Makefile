all: clean recommender

recommender: predict.o
	g++ -o recommender recommender.cpp predict.o -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

predict.o:
	g++ -c predict.cpp -O2 -O3 -O4 -Wall -Wextra -Werror -std=c++11 -pedantic

clean:
	rm -f recommender
	rm -f *.o