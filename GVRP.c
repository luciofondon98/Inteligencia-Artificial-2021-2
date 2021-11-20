#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct instanceParams { // estructura que contiene los parametros de la instancia
    int numCostumers;
    int numtStations;
    int maxTime;
    int maxDistance;
    double vehicleSpeed;
    int serviceTime;
    int refuelTime;

} instanceParams;

instanceParams* getParametros(FILE* fp) { // funcion que retorna un struct con los parametros almacenados
    char linea[1000];
    instanceParams* parametros = (instanceParams*)malloc(sizeof(instanceParams));

    fseek(fp, 8, SEEK_SET); // movemos el cursor pasada la posicion
                            // del nombre de la instancia
                        
    fgets(linea, 1000, fp); // primera linea del archivo

    if (linea[0] == ' ') { 
        sscanf(linea," %d   %d   %d   %d    %lf   %d   %d",
        &(parametros->numCostumers),&(parametros->numtStations),&(parametros->maxTime),
        &(parametros->maxDistance), &(parametros->vehicleSpeed), &(parametros->serviceTime),&(parametros->refuelTime));
    }

    else {
        sscanf(linea,"%d   %d   %d   %d    %lf   %d   %d",
        &(parametros->numCostumers),&(parametros->numtStations),&(parametros->maxTime),
        &(parametros->maxDistance), &(parametros->vehicleSpeed), &(parametros->serviceTime),&(parametros->refuelTime));
    }

    return parametros;
}

void mostrarParametros(instanceParams* parametros) {
    printf(" Número de clientes: %d\n Número de estaciones: %d\n Tiempo máximo de recorrido: %d\n Distancia máxima de recorrido: %d\n Velocidad límite: %lf\n Tiempo de servicio: %d\n Tiempo de recarga: %d\n",
    parametros->numCostumers, parametros->numtStations, 
    parametros->maxTime,parametros->maxDistance,parametros->vehicleSpeed,
    parametros->serviceTime, parametros->refuelTime);

}

int main(){
    instanceParams* parametros; // variable para almacenar los parametros

    FILE *fp = fopen("Instancias/AB101.dat","r");
    
    parametros = getParametros(fp);
    mostrarParametros(parametros);
    fclose(fp);
    free(parametros);
    return 0;
}
