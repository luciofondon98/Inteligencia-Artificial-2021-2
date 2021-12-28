ejecucion: gvrp
	./gvrp

gvrp: GVRP.o
	gcc GVRP.o -o gvrp -lm

GVRP.o: GVRP.c
	gcc -c GVRP.c -Wall -lm

clear:
	rm gvrp GVRP.o