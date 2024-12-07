FLAGS = -Wall -Wextra -g

all: caches

cache-sim: cache-sim.o
	g++ $(FLAGS) cache-sim.o -o cache-sim

cache-sim.o: caches.cpp
	g++ $(FLAGS) -c caches.cpp

clean: 
	rm -f *o cache-sim