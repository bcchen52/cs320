all: caches

cache-sim: cache-sim.o
	g++ cache-sim.o -o cache-sim

cache-sim.o: caches.cpp
	g++ -c caches.cpp

clean: 
	rm -f *o cache-sim