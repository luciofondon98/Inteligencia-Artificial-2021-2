#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tamArray; // variable global para mantener el tamaño del arreglo

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



nodo* getInfoNodo(FILE* fp, int numCostumers, int numStations) { // funcion que retorna un struct nodo con la informacion del nodo
    char linea[1000];

    // creamos arreglo de nodos que almacenara la info
    int totalNodes = numCostumers + numStations; // +1 por el depot
    nodo* infoNodes = (nodo*)malloc(sizeof(nodo) * totalNodes);
    // while(fgets(linea, sizeof(linea), fp)){
    //     printf("%s", linea);

    // }

    tamArray = totalNodes;

    fgets(linea, sizeof(linea), fp); // obtenemos la info del nodo deposito
    if (linea[0] == ' ') { 
        sscanf(linea," %d  %c     %lf   %lf",
        &(infoNodes[0].nodeId),&(infoNodes[0].nodeType),&(infoNodes[0].longitude),
        &(infoNodes[0].latitude));
    }

    else {
        sscanf(linea,"%d  %c     %lf   %lf",
        &(infoNodes[0].nodeId),&(infoNodes[0].nodeType),&(infoNodes[0].longitude),
        &(infoNodes[0].latitude));
    }
    fgets(linea, sizeof(linea), fp); // dummy para leer una linea mas

    // printf("%d %c %lf %lf",
    // infoNodes[0].nodeId, infoNodes[0].nodeType, 
    // infoNodes[0].longitude,infoNodes[0].latitude);

    for (size_t i = 1; i < totalNodes; i++) {
        fgets(linea, sizeof(linea), fp); // dummy para leer una linea mas
        if (linea[0] == ' ') { 
            sscanf(linea," %d  %c     %lf   %lf",
            &(infoNodes[i].nodeId),&(infoNodes[i].nodeType),&(infoNodes[i].longitude),
            &(infoNodes[i].latitude));
        }

        else {
            sscanf(linea,"%d  %c     %lf   %lf",
            &(infoNodes[i].nodeId),&(infoNodes[i].nodeType),&(infoNodes[i].longitude),
            &(infoNodes[i].latitude));
        }
        // printf("%d %c %lf %lf\n",
        // infoNodes[i].nodeId, infoNodes[i].nodeType, 
        // infoNodes[i].longitude,infoNodes[i].latitude);
    }

    return infoNodes;
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
    printf(" Número de clientes: %d\n Número de estaciones: %d\n Tiempo máximo de recorrido: %d\n Distancia máxima de recorrido: %d\n Velocidad límite: %lf\n Tiempo de servicio: %d\n Tiempo de recarga: %d\n",
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

int main() {
    instanceParams* parametros; // variable para almacenar los parametros

    FILE *fp = fopen("Instancias/AB101.dat","r");
    
    parametros = getParametros(fp);
    mostrarParametros(parametros);
    nodo* infoNodes = getInfoNodo(fp, parametros->numCostumers, parametros->numStations);
    mostrarInfoNodos(infoNodes, tamArray);
    fclose(fp);
    free(parametros);
    return 0;
}
