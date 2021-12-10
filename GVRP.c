#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define R 6729.56122941 // radio en metros, que son 4182.44949 millas (4182.44949 * 1.609)
#define TO_RAD (3.1415926536 / 180)

int tamCostumers; // variable global para mantener el tamaño del arreglo
int tamStations; // variable global para mantener el tamaño del arreglo

typedef struct instanceParams { // estructura que contiene los parametros de la instancia
    int numCostumers;
    int numStations;
    int maxTime;
    int maxDistance;
    double vehicleSpeed;
    int serviceTime;
    int refuelTime;
} instanceParams;

typedef struct nodo {
    int nodeId;
    char nodeType;
    double longitude;
    double latitude;
    int visited;
} nodo;


nodo* getInfoStations(FILE* fp, int numStations) { // funcion que retorna un struct nodo con la informacion del nodo
    char linea[1000];

    // creamos arreglo de nodos que almacenara la info
    nodo* infoStations = (nodo*)malloc(sizeof(nodo) * numStations);

    fgets(linea, sizeof(linea), fp); // obtenemos la info del nodo deposito
    if (linea[0] == ' ') { 
        sscanf(linea," %d  %c     %lf   %lf",
        &(infoStations[0].nodeId),&(infoStations[0].nodeType),&(infoStations[0].longitude),
        &(infoStations[0].latitude));
    }

    else {
        sscanf(linea,"%d  %c     %lf   %lf",
        &(infoStations[0].nodeId),&(infoStations[0].nodeType),&(infoStations[0].longitude),
        &(infoStations[0].latitude));
    }
    fgets(linea, sizeof(linea), fp); // dummy para leer una linea mas

    // printf("%d %c %lf %lf",
    // infoNodes[0].nodeId, infoNodes[0].nodeType, 
    // infoNodes[0].longitude,infoNodes[0].latitude);

    for (size_t i = 1; i < numStations; i++) {
        fgets(linea, sizeof(linea), fp); // dummy para leer una linea mas
        if (linea[0] == ' ') { 
            sscanf(linea," %d  %c     %lf   %lf",
            &(infoStations[i].nodeId),&(infoStations[i].nodeType),&(infoStations[i].longitude),
            &(infoStations[i].latitude));
        }

        else {
            sscanf(linea,"%d  %c     %lf   %lf",
            &(infoStations[i].nodeId),&(infoStations[i].nodeType),&(infoStations[i].longitude),
            &(infoStations[i].latitude));
        }
        // printf("%d %c %lf %lf\n",
        // infoNodes[i].nodeId, infoNodes[i].nodeType, 
        // infoNodes[i].longitude,infoNodes[i].latitude);
    }

    return infoStations;
    // printf("%ld\n", strlen(linea));
}



nodo* getInfoCostumers(FILE* fp, int numCostumers) { // funcion que retorna un struct nodo con la informacion del nodo
    char linea[1000];

    // creamos arreglo de nodos que almacenara la info
    nodo* infoCostumers = (nodo*)malloc(sizeof(nodo) * numCostumers);

    fgets(linea, sizeof(linea), fp); // obtenemos la info del nodo deposito
    if (linea[0] == ' ') { 
        sscanf(linea," %d  %c     %lf   %lf",
        &(infoCostumers[0].nodeId),&(infoCostumers[0].nodeType),&(infoCostumers[0].longitude),
        &(infoCostumers[0].latitude));
    }

    else {
        sscanf(linea,"%d  %c     %lf   %lf",
        &(infoCostumers[0].nodeId),&(infoCostumers[0].nodeType),&(infoCostumers[0].longitude),
        &(infoCostumers[0].latitude));
    }
    // fgets(linea, sizeof(linea), fp); // dummy para leer una linea mas

    // printf("%d %c %lf %lf",
    // infoNodes[0].nodeId, infoNodes[0].nodeType, 
    // infoNodes[0].longitude,infoNodes[0].latitude);

    for (size_t i = 1; i < numCostumers; i++) {
        fgets(linea, sizeof(linea), fp); // dummy para leer una linea mas
        if (linea[0] == ' ') { 
            sscanf(linea," %d  %c     %lf   %lf",
            &(infoCostumers[i].nodeId),&(infoCostumers[i].nodeType),&(infoCostumers[i].longitude),
            &(infoCostumers[i].latitude));
        }

        else {
            sscanf(linea,"%d  %c     %lf   %lf",
            &(infoCostumers[i].nodeId),&(infoCostumers[i].nodeType),&(infoCostumers[i].longitude),
            &(infoCostumers[i].latitude));
        }
        // printf("%d %c %lf %lf\n",
        // infoNodes[i].nodeId, infoNodes[i].nodeType, 
        // infoNodes[i].longitude,infoNodes[i].latitude);
    }

    return infoCostumers;
    // printf("%ld\n", strlen(linea));
}

instanceParams* getParametros(FILE* fp) { // funcion que retorna un struct con los parametros almacenados
    char linea[1000];
    instanceParams* parametros = (instanceParams*)malloc(sizeof(instanceParams));

    fseek(fp, 8, SEEK_SET); // movemos el cursor pasada la posicion
                            // del nombre de la instancia
                        
    fgets(linea, 1000, fp); // primera linea del archivo

    if (linea[0] == ' ') { 
        sscanf(linea," %d   %d   %d   %d    %lf   %d   %d",
        &(parametros->numCostumers),&(parametros->numStations),&(parametros->maxTime),
        &(parametros->maxDistance), &(parametros->vehicleSpeed), &(parametros->serviceTime),&(parametros->refuelTime));
    }

    else {
        sscanf(linea,"%d   %d   %d   %d    %lf   %d   %d",
        &(parametros->numCostumers),&(parametros->numStations),&(parametros->maxTime),
        &(parametros->maxDistance), &(parametros->vehicleSpeed), &(parametros->serviceTime),&(parametros->refuelTime));
    }

    return parametros;
}

void mostrarParametros(instanceParams* parametros) {
    printf(" Número de clientes: %d\n Número de estaciones: %d\n Tiempo máximo de recorrido (minutos): %d\n Distancia máxima de recorrido (millas): %d\n Velocidad límite (millas/min): %lf\n Tiempo de servicio (minutos): %d\n Tiempo de recarga (min): %d\n",
    parametros->numCostumers, parametros->numStations, 
    parametros->maxTime,parametros->maxDistance,parametros->vehicleSpeed,
    parametros->serviceTime, parametros->refuelTime);
}

void mostrarInfoNodos(nodo* infoNodes, int tamArray) {
    for (int i = 0; i < tamArray; i++) {
        printf("%d %c %lf %lf\n",
            infoNodes[i].nodeId, infoNodes[i].nodeType, 
            infoNodes[i].longitude,infoNodes[i].latitude);
    }
    printf("%d\n", tamArray);
}

double haversineDist(double th1, double ph1, double th2, double ph2) { // latitud1 longitud1 latitud2 longitud2
	double dx, dy, dz;
	ph1 -= ph2;
	ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;
 
	dz = sin(th1) - sin(th2);
	dx = cos(ph1) * cos(th1) - cos(th2);
	dy = sin(ph1) * cos(th1);
	return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R;
}

int main() {
    instanceParams* parametros; // variable para almacenar los parametros

    FILE *fp = fopen("Instancias/AB101.dat","r");
    
    parametros = getParametros(fp);
    mostrarParametros(parametros);
    
    nodo* infoStations = getInfoStations(fp, parametros->numStations);
    nodo* infoCostumers = getInfoCostumers(fp, parametros->numCostumers);

    mostrarInfoNodos(infoStations, parametros->numStations);
    mostrarInfoNodos(infoCostumers, parametros->numCostumers);
    // printf("Distancia de Haversine: en km -> %.5f, en millas -> %.5f\n", d, d / 1.609344);

    fclose(fp);
    free(parametros);
    // para compilar: gcc GVRP.c -o a -Wall -lm (-lm por <math.h>)
    
    return 0;
}
