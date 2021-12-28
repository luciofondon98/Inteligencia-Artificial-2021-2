Lucio Fondon, 		rol: 201773610-0
Paralelo 201

Instrucciones para utilizar la tarea:
1- Instalar gcc
2- Abrir la terminal y entrar a la ubicación de la carpeta con los archivos en cuestión.
3- Una vez abierta la carpeta en la terminal, se deben realizar los siguientes pasos para compilar los archivos:

	En la terminal, escribir el siguiente comando:

	$ make

	Este comando crea 2 archivos nuevos, GVRP.o y el ejecutable gvrp.
	Para borrar los archivos creados y reiniciar la compilacion, ejecutar en la terminal:

	$ make clear

A tener en cuenta:

- Si se quieren probar instancias, simplemente se deben cambiar los nombres de variables globales dentro
del archivo GVRP.c, específicamente en las líneas 21, 22 y 23, es decir:

char* nombreInstancia = "Instancias/AB101.dat";   |
char* salidaInstanciaGreedy = "AB101Greedy.txt";  |   Acá debe cambiar AB101 por la instancia que desee probar
char* salidaInstanciaSA = "AB101Final.txt";       |   AB101 es la que viene por defecto
