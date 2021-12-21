#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define R 6729.56122941 // radio en metros, que son 4182.44949 millas (4182.44949 * 1.609)
#define TO_RAD (3.1415926536 / 180)

int tamCostumers; // variable global para mantener el tamaño del arreglo
int tamStations; // variable global para mantener el tamaño del arreglo
double latitudDeposito;
double longitudDeposito;
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
    char nombreNodo[3];  // -> nombre del nodo (ya sea estacion-numero o cliente-numero) que es parte de la solución
    struct solucionNode* sig; 
} solucionNode;

typedef struct solucionList { // estructura que contiene una lista enlazada de strings, que basicamente contiene la solución candidata
    int size; // tamaño de la lista
    solucionNode* head;
    int timeSolucion;
    double distanciaSolucion;
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
    strcpy(nodo->nombreNodo, nombreNodo); //Lo que hay en apellido se copia a nombre
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

void mostrarLista(solucionList *Lista){
    if(Lista->head==NULL) printf("[ ]\n");
    else{
        // printf("[");
      	solucionNode* current = Lista->head;
 		while (current != NULL){
 			printf(" %s ",current->nombreNodo);
 			current = current->sig;
 		}
        printf("\n");
    }
}

void borrarTodos(solucionList *Lista){
    solucionNode * current = Lista->head;
    solucionNode * temp;
    while(current != NULL){
        temp = current->sig; //current será el nodo a borrar
        free(current);
        current = temp;
        Lista->size--;
    }
    // printf("Todos los elementos de la lista borrados.\n");
    Lista->head = NULL;
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

structNodoMenorDistancia clienteConMenorDistanciaA(nodo aNodo, nodo* infoCostumers) {
    // Primero, visitaremos todos los clientes y veremos cual es el cliente no visitado con la menor distancia posible
    // int indiceMenorDistancia;
    // nodo nodoMenorDistancia;
    double d = 99999999.0; // inicializamos en una distancia muy grande 
    // Iteramos sobre los clientes
    // almacenamos en la estructura de retorno los valores obtenidos
    structNodoMenorDistancia retorno;
    for (size_t i = 0; i < tamCostumers; i++) {
        double distTemp = haversineDist(aNodo.latitude, aNodo.longitude, infoCostumers[i].latitude, infoCostumers[i].longitude);
        if (distTemp < d && infoCostumers[i].visited != 1)  {
            d = distTemp;
            // nodoMenorDistancia = infoCostumers[i];
            retorno.distance = d;
            retorno.indice = i;
            sprintf(retorno.nombreNodo, "%c%d", infoCostumers[i].nodeType,infoCostumers[i].nodeId);
        }
    }
    // printf("Menor distancia nodo: %s, %f\n", retorno.nombreNodo, retorno.distance);

    return retorno;
}

structNodoMenorDistancia estacionConMenorDistanciaA(nodo aNodo, nodo* infoStations) {
    // Primero, visitaremos todas las estaciones y veremos cual es la estacion no visitado con la menor distancia posible
    // int indiceMenorDistancia;
    // nodo nodoMenorDistancia;
    double d = 99999999.0; // inicializamos en una distancia muy grande 
    // Iteramos sobre los clientes
    structNodoMenorDistancia retorno;
    for (size_t i = 1; i < tamStations; i++) {
        double distTemp = haversineDist(aNodo.latitude, aNodo.longitude, infoStations[i].latitude, infoStations[i].longitude);
        if (distTemp < d) {
            d = distTemp;
            retorno.distance = d;
            retorno.indice = i;
            sprintf(retorno.nombreNodo, "%c%d", infoStations[i].nodeType,infoStations[i].nodeId);
        }
    }
    // printf("Menor distancia nodo: %s, %f\n", retorno.nombreNodo, retorno.distance);

    return retorno;
}

double distanciaNodoADeposito(double latitudNodo, double longitudNodo) { // calcula una distancia de algun nodo al deposito.
    return haversineDist(latitudDeposito, longitudDeposito, latitudNodo, longitudNodo);
}

int tiempoEntreNodos(double distancia, double vehicleSpeed) {
    return distancia/vehicleSpeed;
}

solucionNode* copiaArreglo(solucionNode* arreglo) {
    int tam = sizeof(arreglo) / sizeof(arreglo[0]);
    solucionNode* copy = malloc(tam);

    for (int i = 0; i < tam; i++) strcpy(copy[i].nombreNodo,arreglo[i].nombreNodo);
    return copy;
}

nodo findNode(char* nombreNodo, nodo* infoCostumers, nodo* infoStations) { // encuentra al nodo por su nombre y lo retorna
    char compararNombre[3];
    if (strstr(nombreNodo, "c") !=  NULL) { // es cliente
        for (int i = 0; i < tamCostumers; i++) {
            sprintf(compararNombre, "%c%d", infoCostumers[i].nodeType,infoCostumers[i].nodeId);
            if (strcmp(nombreNodo, compararNombre) == 0) {
                return infoCostumers[i];
            }
        }
    } else if (strstr(nombreNodo, "f") != NULL) { // es estacion
        for (int i = 0; i < tamStations; i++) {
            sprintf(compararNombre, "%c%d", infoStations[i].nodeType,infoStations[i].nodeId);
            if (strcmp(nombreNodo, compararNombre) == 0) {
                return infoStations[i];
            }
        }
    }
    // return;
}

int esSolucionFactible(solucionNode* solucionCandidata, nodo* infoCostumers, nodo* infoStations, int maxTime, int maxDistance, double vehicleSpeed, int serviceTime, int refuelTime) {
    int tamArreglo = sizeof(solucionCandidata) / sizeof(solucionCandidata[0]);
    char* nodoActual = solucionCandidata[0].nombreNodo;
    
    int actualTime = 0;
    double actualDistance = 0;
    int actualEstanque = maxDistance;
    
    for (int i = 1; i < tamArreglo; i++) {
        char* nodoAVisitar = solucionCandidata[i].nombreNodo;  
        nodo tempNodoActual  =  findNode(nodoAVisitar, infoCostumers, infoStations); 
        nodo tempNodoVisitar = findNode(nodoAVisitar, infoCostumers, infoStations); 
        // ahora hallamos distancia entre nodoActual y tempNodo
        double feasibleDistancia = haversineDist(tempNodoActual.latitude, tempNodoActual.longitude, tempNodoVisitar.latitude, tempNodoVisitar.latitude);
        int feasibleTime = tiempoEntreNodos(feasibleDistancia, vehicleSpeed);

        // vemos si viola o no las restricciones
        if (feasibleDistancia <= actualEstanque) {
            if (feasibleTime + actualTime <= maxTime) { // entonces, podemos ir al cliente sin problema alguno
                actualDistance += feasibleDistancia;
                actualEstanque -= feasibleDistancia;
                if (tempNodoVisitar.nodeType == 99) { // veo si es cliente, ascii c = 99
                    actualTime += feasibleTime + serviceTime; 

                } else if (tempNodoVisitar.nodeType == 102) { // veo si es estacion, ascii f = 102
                    actualTime += feasibleTime + refuelTime; 
                    actualDistance = maxDistance;

                } else return 0;

            } else return 0; // no es factible
        }
                
    }

    return 1; 
}

void vecindarioFactibleSolucion(solucionList* solucionGreedy) {
    // recorrer la lista, agregarlos a un arreglo de solucionList y luego swapear ese arreglo
    int tam = solucionGreedy->size;
    solucionNode arreglo[tam];
    solucionNode* current = solucionGreedy->head;
    int i = 0;
    while (current != NULL){
        strcpy(arreglo[i].nombreNodo,current->nombreNodo);
        current = current->sig;
        // printf("%s\n", arreglo[i].nombreNodo);
        i++;
    }

    int cantSwaps = solucionGreedy->size - 3;

    // ahora debemos swapear consecutivos y ver que al realizar el swap sea factible la solución
    int tamArreglo = sizeof(arreglo) / sizeof(arreglo[0]);
    solucionNode matriz[cantSwaps][tam];
    solucionNode *copyArreglo = copiaArreglo(arreglo);
    for (i = 1; i <= cantSwaps; i++){
        if (strcmp(arreglo[i].nombreNodo,"d0") != 0) {
            solucionNode temp;
            temp = arreglo[i];
            arreglo[i] = arreglo[i+1];
            arreglo[i+1] = temp;
            // printf("%s\n", arreglo[i].nombreNodo);
            memcpy(matriz[i-1], arreglo, sizeof(matriz[i]));
            // memcpy(arreglo, copyArreglo, sizeof(arreglo));
        }
    }
    printf("\n"); 

    for (int i = 0; i < cantSwaps; i++) {
        for (int j = 0; j < tam; j++) {
            printf("%s\n", matriz[i][j].nombreNodo);
        }       
        printf("\n"); 
    }
    // ahora debemos ver la factibilidad de las soluciones con el swap
    // esSolucionFactible
}


void greedyGVRP(nodo* infoCostumers, nodo* infoStations, int maxTime, int maxDistance, double vehicleSpeed, int serviceTime, int refuelTime) {
    // se pasa primero como parámetro una lista vacía, entonces añadimos d0 por ser el nodo depósito inicial 

    double feasibleDistanceCliente; // variable para chequear si la distancia del cliente visitado + la distancia del cliente visitado al deposito no supera la del estanque
    double feasibleDistanceEstacion; // variable para chequear si la distancia de la estacion visitada + la distancia de la estacion visitada al deposito no supera la del estanque

    int feasibleTimeCliente; // variable para chequear factibilidad del tiempo demorado cliente
    int feasibleTimeEstacion; // variable para chequear factibilidad del tiempo demorado estacion
    
    int clientesVisitados = 0;
    int autosUtilizados = 0;
    solucionList* listaSolucion = crearLista();

    // iteraremos hasta tener al menos 3/4 de los clientes visitados para la instancia
    while (clientesVisitados < 0.9*tamCostumers) {
        insertarNodo(listaSolucion, "d0");
        // seteamos las variables iniciales
        nodo nodoActual = infoStations[0];
        int actualTime = 0;
        double actualDistance = 0;
        int actualEstanque = maxDistance;
        int flag = 1; // booleano que hará que detengamos la iteración
        while (flag) {
            // ahora, debemos ver si la distancia anterior es factible
            // ¿Qué significa que sea factible? 
            // 1-> Que desde el nodo actual donde estoy hasta el cliente sin pasarme del tiempo ni acabarme el combustible
            // 2-> Además, que pueda devolverme desde el cliente que he visitado al depósito (por si es que necesite devolverme en caso de violar las restricciones luego)
            // entonces, chequeamos que sea factible la distancia teniendo en cuenta lo anterior
            structNodoMenorDistancia clienteMenorDistancia = clienteConMenorDistanciaA(nodoActual, infoCostumers);
            // la distancia factible será -> distancia de donde estoy al cliente con menor distancia posible + distancia del cliente al que llego hasta el nodo depósito
            feasibleDistanceCliente = clienteMenorDistancia.distance + distanciaNodoADeposito(infoCostumers[clienteMenorDistancia.indice].latitude, infoCostumers[clienteMenorDistancia.indice].longitude) + distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude);
            // hacemos lo mismo, pero con el tiempo
            feasibleTimeCliente = tiempoEntreNodos(feasibleDistanceCliente, vehicleSpeed);
            if (feasibleDistanceCliente <= actualEstanque) {
                if (feasibleTimeCliente + actualTime <= maxTime) { // entonces, podemos ir al cliente sin problema alguno
                    actualDistance += clienteMenorDistancia.distance;
                    actualEstanque -= clienteMenorDistancia.distance;
                    actualTime += clienteMenorDistancia.distance/vehicleSpeed + serviceTime; 
                    infoCostumers[clienteMenorDistancia.indice].visited = 1; // visitamos al cliente
                    insertarNodo(listaSolucion, clienteMenorDistancia.nombreNodo);
                    nodoActual = infoCostumers[clienteMenorDistancia.indice];
                    clientesVisitados++;

                } else { // nos pasaremos del tiempo, entonces nos devolvemos al depósito y termina todo
                    actualDistance += distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude);
                    actualEstanque -= distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude);
                    actualTime += distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude)/vehicleSpeed;
                    insertarNodo(listaSolucion, "d0");
                    listaSolucion->distanciaSolucion = actualDistance;
                    listaSolucion->timeSolucion = actualTime;
                    printf("Distancia recorrida: %f\nTiempo utilizado: %d\n", actualDistance, actualTime);
                    flag = 0;
                }
            } else { // no nos pasamos del tiempo, pero debemos rellenar el estanque
                // debemos chequear de nuevo el tema del tiempo, pero esta vez para ir a una estación de recarga
                structNodoMenorDistancia estacionMenorDistancia = estacionConMenorDistanciaA(nodoActual, infoStations);
                // la distancia factible será -> distancia de donde estoy al cliente con menor distancia posible + distancia del cliente al que llego hasta el nodo depósito
                feasibleDistanceEstacion = estacionMenorDistancia.distance + distanciaNodoADeposito(infoStations[estacionMenorDistancia.indice].latitude, infoStations[estacionMenorDistancia.indice].longitude) +  distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude);
                // hacemos lo mismo, pero con el tiempo
                feasibleTimeEstacion = tiempoEntreNodos(feasibleDistanceEstacion, vehicleSpeed);
                if (feasibleDistanceEstacion <= actualEstanque) {
                    if (feasibleTimeEstacion + actualTime <= maxTime) { // entonces, podemos ir a la estación sin problema alguno
                        actualDistance += estacionMenorDistancia.distance;
                        actualEstanque = maxDistance;
                        actualTime += estacionMenorDistancia.distance/vehicleSpeed + refuelTime;
                        insertarNodo(listaSolucion, estacionMenorDistancia.nombreNodo);
                        // nodoActual = infoStations[estacionMenorDistancia.indice];

                        // printf("visitamos a cliente");
                        actualDistance += clienteMenorDistancia.distance;
                        // printf("%f\n",clienteMenorDistancia.distance);
                        actualEstanque -= clienteMenorDistancia.distance;
                        actualTime += serviceTime + clienteMenorDistancia.distance/vehicleSpeed;
                        // printf("%d\n", actualTime);
                        // marcamos como visitado al cliente
                        infoCostumers[clienteMenorDistancia.indice].visited = 1;
                        insertarNodo(listaSolucion, clienteMenorDistancia.nombreNodo);
                        nodoActual = infoCostumers[clienteMenorDistancia.indice]; // ahora nodo actual es la estacion que visitamos
                        clientesVisitados++;

                    } else { // nos pasaremos del tiempo, entonces nos devolvemos al depósito y termina todo
                        actualDistance += distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude);
                        actualEstanque -= distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude);
                        actualTime += distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude)/vehicleSpeed; 
                        insertarNodo(listaSolucion, "d0");
                        listaSolucion->distanciaSolucion = actualDistance;
                        listaSolucion->timeSolucion = actualTime;
                        printf("Distancia recorrida: %f\nTiempo utilizado: %d\n", actualDistance, actualTime);
                        flag = 0;
                    }
                } else {
                    actualDistance += distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude);
                    actualEstanque -= distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude);
                    actualTime += distanciaNodoADeposito(nodoActual.latitude, nodoActual.longitude)/vehicleSpeed; 
                    insertarNodo(listaSolucion, "d0");
                    listaSolucion->distanciaSolucion = actualDistance;
                    listaSolucion->timeSolucion = actualTime;
                    printf("Distancia recorrida: %f\nTiempo utilizado: %d\n", actualDistance, actualTime);
                    flag = 0;
                }  
            }
        }
    // acá hay q hacer simulated annealing
    // mostrarLista(listaSolucion);
    vecindarioFactibleSolucion(listaSolucion);
    borrarTodos(listaSolucion); 
    break; // dsp sacar este break
    }
    
}



// void simullatedAnnealing(solucionList* solucionGreedy, int T, int T_min, int iter) {
//     solucionList* S_best;
//     solucionList* S_c = solucionGreedy;
//     S_best = S_c;
//     int t;
//     // ciclo exterior, controlado por temperatura
//     while (T > T_min) {
//         t = 0;
//         solucionList* S_n;
//         // ciclo interior, controlado por iteraciones
//         while (t < iter) {
//             // Aca debemos realizar las operaciones en la solución para ver si es posible mejorarla
//             // ver como hacer operaciones, calcular la calidad de la solucion (maxmimizar distancia), ver si la solucion luego de la operacion es factible
//             // Sn se debe igualar y trabajar ciertas cosas con el, Sn guarda la solucion factible luego de la operacion
//             // if evaluar(S_n, S_c) { // se ve cual solucion tiene mejor calidad, Sn > Sc?
//             //     solucionList* S_c = S_n;
//             // } else if
//         }
        
//      }
// }


int main() {
    instanceParams* parametros; // variable para almacenar los parametros

    FILE *fp = fopen("Instancias/AB101.dat","r");
    
    parametros = getParametros(fp);
    mostrarParametros(parametros);
    
    nodo* infoStations = getInfoStations(fp, parametros->numStations);
    nodo* infoCostumers = getInfoCostumers(fp, parametros->numCostumers);

    tamStations = parametros->numStations;
    tamCostumers = parametros->numCostumers;

    latitudDeposito = infoStations[0].latitude;
    longitudDeposito = infoStations[0].longitude;
    // mostrarInfoNodos(infoStations, parametros->numStations);
    // mostrarInfoNodos(infoCostumers, parametros->numCostumers);
    // printf("Distancia de Haversine: en km -> %.5f, en millas -> %.5f\n", d, d / 1.609344);

    // solucionList* listaSolucion = crearLista();
    greedyGVRP(infoCostumers, infoStations, parametros->maxTime, parametros->maxDistance, parametros->vehicleSpeed, parametros->serviceTime, parametros->refuelTime);

    // printf("El tamaño inicial de la lista es de: %d\n", listaSolucion->size);
    // printf("Nodo inicial: %s\n", listaSolucion->head->nombreNodo);

    fclose(fp);
    free(parametros);
    // para compilar: gcc GVRP.c -o a -Wall -lm (-lm por <math.h>), luego valgrind ./a
    
    return 0;
}
