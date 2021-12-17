#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define R 6729.56122941 // radio en metros, que son 4182.44949 millas (4182.44949 * 1.609)
#define TO_RAD (3.1415926536 / 180)

int tamCostumers; // variable global para mantener el tamaño del arreglo
int tamStations; // variable global para mantener el tamaño del arreglo

// ---------------------------------------------------------------------------------------- //


// --------------------/* Structs para almacenar los datos */------------------------ //

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

typedef struct solucionNode {
    char* nombreNodo;  // -> nombre del nodo (ya sea estacion-numero o cliente-numero) que es parte de la solución
    struct solucionNode* sig; 
} solucionNode;

typedef struct solucionList { // estructura que contiene una lista enlazada de strings, que basicamente contiene la solución candidata
    int size; // tamaño de la lista
    solucionNode* head;
} solucionList;

typedef struct structNodoMenorDistancia { // esta estructura nos servirá para el retoorno en las funciones de cliente y nodo nodoConMenorDistanciaA
    double distance; // tamaño de la lista
    char nombreNodo[3];
    int indice;
} structNodoMenorDistancia;

// ---------------------------------------------------------------------------------------- //

           
// ---------------- /* Funciones para agarrar los parámetros y datos de las instancias */----- //

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

// ---------------------------------------------------------------------------------------- //


// ---------------- /* Funciones para mostrar los parámetros y datos de las instancias */----- //

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

// ---------------------------------------------------------------------------------------- //


// ---------------- /* Funciones asociadas a la lista enlazada de la solución */----- //

solucionList *crearLista(){
    solucionList *lista = (solucionList*)malloc(sizeof(solucionList));
    lista->size = 0;
    lista->head = NULL;
    return lista;
}

solucionNode* crearNodo(char* nombreNodo){
    solucionNode* nodo = (solucionNode*)malloc(sizeof(solucionNode));
    nodo->sig = NULL;
    nodo->nombreNodo = nombreNodo;
      // allocate memory to hold word
    // nodo->nombreNodo = malloc(strlen(nombreNodo) + 1);
    // copy the current word
    // strcpy(nodo->nombreNodo, nombreNodo);
    return nodo;
}

void insertarNodo(solucionList *Lista, char* nombreNodo){ //agrega al final de la lista
    if(Lista->head == NULL) Lista->head = crearNodo(nombreNodo);
    else{
        solucionNode *current = Lista->head;
        while (current->sig != NULL) current = current->sig;
        solucionNode *nuevo = crearNodo(nombreNodo);
        current->sig = nuevo; 
    }
    Lista->size++;    
}

// ---------------------------------------------------------------------------------------- //


double haversineDist(double th1, double ph1, double th2, double ph2) { // latitud1 longitud1 latitud2 longitud2
	double dx, dy, dz;
	ph1 -= ph2;
	ph1 *= TO_RAD, th1 *= TO_RAD, th2 *= TO_RAD;
 
	dz = sin(th1) - sin(th2);
	dx = cos(ph1) * cos(th1) - cos(th2);
	dy = sin(ph1) * cos(th1);
	return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R;
}

structNodoMenorDistancia clienteConMenorDistanciaA(nodo deNodo, nodo* infoCostumers) {
    // Primero, visitaremos todos los clientes y veremos cual es el cliente no visitado con la menor distancia posible
    int indiceMenorDistancia;
    nodo nodoMenorDistancia;
    double d = 99999999; // inicializamos en una distancia muy grande 
    // Iteramos sobre los clientes
    for (size_t i = 0; i < tamCostumers; i++) {
        double distTemp = haversineDist(deNodo.latitude, deNodo.longitude, infoCostumers[i].latitude, infoCostumers[i].longitude);
        if (distTemp < d) {
            d = distTemp;
            nodoMenorDistancia = infoCostumers[i];
            indiceMenorDistancia = i;
        }
    }
    
    // almacenamos en la estructura de retorno los valores obtenidos
    structNodoMenorDistancia retorno;
    retorno.distance = d;
    retorno.indice = indiceMenorDistancia;
    sprintf(retorno.nombreNodo, "%c%d", nodoMenorDistancia.nodeType,nodoMenorDistancia.nodeId);

    return retorno;


}

void greedyGVRP(solucionList* listaSolucion, nodo* infoCostumers, nodo* infoStations, int maxTime, int maxDistance, double vehicleSpeed, int serviceTimem, int refuelTime) {
    // se pasa primero como parámetro una lista vacía, entonces añadimos d0 por ser el nodo depósito inicial 
    insertarNodo(listaSolucion, "d0");
    // seteamos las variables iniciales
    nodo nodoActual = infoStations[0];

    int actualTime = 0;
    int actualDistance = 0;
    int actualEstanque = maxDistance;

    nodo nodoVisitado; // variable para ir guardando los nodos a medida que vamos visitando

    int flag = 1; // booleano que hará que detengamos la iteración

    while(flag) {
        // visitar a un nodo que sea factible, es decir, que cumpla con todas las restricciones del problema (crear funcion que encuentre el nodo con menor distancia al nodoVisitado actual
        // para esto llamamos a la funcion clienteConMenorDistancia, que retorna la menor distancia del nodoActual a algun cliente
        structNodoMenorDistancia clienteMenorDistancia = clienteConMenorDistanciaA(nodoActual, infoCostumers);

        // ahora, debemos ver si la distancia anterior es factible
        // 1. Ver primero si tenemos combustible suficiente para ir al cliente y además volver al depósito, en caso de que no, debemos ir a una estacion de recarga
        // 2. Si se cumple lo anterior, debemos ver si nos da el tiempo para ir al cliente y además sumarle el tiempo que nos toma ir al depósito en caso de quedarse sin tiempo
        // 3. Si no se cumple, debemos chequear lo mismo que antes (ver el combustible suficiente y si nos da el tiempo) pero con alguna estación de recarga

        
        flag = 0;
    }
}

int main() {
    instanceParams* parametros; // variable para almacenar los parametros

    FILE *fp = fopen("Instancias/AB101.dat","r");
    
    parametros = getParametros(fp);
    mostrarParametros(parametros);
    
    nodo* infoStations = getInfoStations(fp, parametros->numStations);
    nodo* infoCostumers = getInfoCostumers(fp, parametros->numCostumers);

    tamStations = parametros->numStations;
    tamCostumers = parametros->numCostumers;
    // mostrarInfoNodos(infoStations, parametros->numStations);
    // mostrarInfoNodos(infoCostumers, parametros->numCostumers);
    // printf("Distancia de Haversine: en km -> %.5f, en millas -> %.5f\n", d, d / 1.609344);

    solucionList* listaSolucion = crearLista();
    greedyGVRP(listaSolucion, infoCostumers, infoStations, parametros->maxTime, parametros->maxDistance, parametros->vehicleSpeed, parametros->serviceTime, parametros->refuelTime);
    printf("El tamaño inicial de la lista es de: %d\n", listaSolucion->size);
    printf("Nodo inicial: %s\n", listaSolucion->head->nombreNodo);

    fclose(fp);
    free(parametros);
    // para compilar: gcc GVRP.c -o a -Wall -lm (-lm por <math.h>), luego valgrind ./a
    
    return 0;
}
