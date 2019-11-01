#include <stdlib.h>
#include <stdio.h>

#include "lista.h"
#include "arbol.h"
#include "ia.h"
#include "partida.h"

// Prototipos de funciones auxiliares.
static void ejecutar_min_max(tBusquedaAdversaria b);
static void crear_sucesores_min_max(tArbol a, tNodo n, int es_max, int alpha, int beta, int jugador_max, int jugador_min);
static int valor_utilidad(tEstado e, int jugador_max);
static tLista estados_sucesores(tEstado e, int ficha_jugador);
static void diferencia_estados(tEstado anterior, tEstado nuevo, int * x, int * y);
static tEstado clonar_estado(tEstado e);

void fNoEliminarIA(tElemento e){}
void fSiEliminarIA(tElemento e){

    tEstado estado = (tEstado) e;
    free(estado);
    estado=NULL;

}

void crear_busqueda_adversaria(tBusquedaAdversaria * b, tPartida p){
    int i, j;
    tEstado estado;

    (*b) = (tBusquedaAdversaria) malloc(sizeof(struct busqueda_adversaria));
    if ((*b) == NULL) exit(IA_ERROR_MEMORIA);

    estado = (tEstado) malloc(sizeof(struct estado));
    if (estado == NULL) exit(IA_ERROR_MEMORIA);

    // Se clona el estado del tablero de la partida, al estado inicial de la b�squeda adversaria.
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            estado->grilla[i][j] = p->tablero->grilla[i][j];
        }
    }

	// Se asume que el estado de la partida es PART_EN_JUEGO por lo que, la utilidad del estado
	// inicialmente es IA_NO_TERMINO
    estado->utilidad = IA_NO_TERMINO;

    // Inicializa los valores que representar�n a los jugadores MAX y MIN respectivamente.
    (*b)->jugador_max = p->turno_de;
    (*b)->jugador_min = (p->turno_de == PART_JUGADOR_1) ? PART_JUGADOR_2 : PART_JUGADOR_1;

    // Inicializa un �rbol para la b�squeda adversaria inicialmente vac�o.
    crear_arbol(&((*b)->arbol_busqueda));

    // Inicializa la ra�z del �rbol de b�squeda con el estado del tablero T.
    crear_raiz((*b)->arbol_busqueda, estado);

    // Ejecuta algoritmo Min-Max con podas Alpha-Beta.
    ejecutar_min_max((*b));
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
*/
void proximo_movimiento(tBusquedaAdversaria b, int * x, int * y){

    tArbol aba = (b->arbol_busqueda);
    tNodo raiz = a_raiz(aba);
    tEstado original = a_recuperar(aba,raiz);
    tLista posibles = a_hijos(aba,raiz);
    tPosicion actual = l_primera(posibles);
    tPosicion corte = l_fin(posibles);
    int noGana = 1;
    int noEmpata = 1;

    while(actual!=corte && noGana!=0){
        tEstado e = l_recuperar(posibles,actual);
        if((e->utilidad)==IA_GANA_MAX){
            diferencia_estados(original,e,x,y);
            noGana=0;
        }
        actual=l_siguiente(posibles,actual);
    }
    if(noGana){
        tPosicion actual = l_primera(posibles);
        noEmpata = 1;
        while(actual!=corte && noEmpata!=0){
            tEstado e = l_recuperar(posibles,actual);
            if((e->utilidad)==IA_EMPATA_MAX){
                diferencia_estados(original,e,x,y);
                noEmpata=0;
            }
            actual=l_siguiente(posibles,actual);
        }
    }


    if(noGana==noEmpata){diferencia_estados(original,l_recuperar(posibles,l_primera(posibles)),x,y);}

}

/**
>>>>>  A IMPLEMENTAR   <<<<<
**/
void destruir_busqueda_adversaria(tBusquedaAdversaria * b){
    a_destruir(&((*b)->arbol_busqueda),&fSiEliminarIA);
}

// ===============================================================================================================
// FUNCIONES Y PROCEDEMIENTOS AUXILIARES
// ===============================================================================================================

/**
Ordena la ejecuci�n del algoritmo Min-Max para la generaci�n del �rbol de b�squeda adversaria, considerando como
estado inicial el estado de la partida almacenado en el �rbol almacenado en B.
**/
static void ejecutar_min_max(tBusquedaAdversaria b){
    tArbol a = b->arbol_busqueda;
    tNodo r = a_raiz(a);
    int jugador_max = b->jugador_max;
    int jugador_min = b->jugador_min;

    crear_sucesores_min_max(a, r, 1, IA_INFINITO_NEG, IA_INFINITO_POS, jugador_max, jugador_min);
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
Implementa la estrategia del algoritmo Min-Max con podas Alpha-Beta, a partir del estado almacenado en N.
- A referencia al �rbol de b�squeda adversaria.
- N referencia al nodo a partir del cual se construye el sub�rbol de b�squeda adversaria.
- ES_MAX indica si N representa un nodo MAX en el �rbol de b�squeda adversaria.
- ALPHA y BETA indican sendos valores correspondientes a los nodos ancestros a N en el �rbol de b�squeda A.
- JUGADOR_MAX y JUGADOR_MIN indican las fichas con las que juegan los respectivos jugadores.
**/
static void crear_sucesores_min_max(tArbol a, tNodo n, int es_max, int alpha, int beta, int jugador_max, int jugador_min){

    tEstado e = a_recuperar(a,n);
    int vu = valor_utilidad(e,jugador_max);
    int val_suc;


    if(vu==IA_NO_TERMINO){
        if(es_max){

            tLista sucesores = estados_sucesores(e,jugador_max);
            tPosicion actual;
            tEstado estadoSuc;
            tNodo nSuc;
            int longitud = l_longitud(sucesores);
            int corte=0;
            int it=0;

            int val = IA_INFINITO_NEG;

            while(it<longitud && corte==0){//Trabajar con longitud o con l_fin
                actual= l_primera(sucesores);
                estadoSuc = l_recuperar(sucesores,actual);
                nSuc = a_insertar(a,n,NULL,estadoSuc);
                crear_sucesores_min_max(a,nSuc,0,alpha,beta,jugador_max,jugador_min);
                val_suc = estadoSuc->utilidad;
                val=(val>val_suc)?val:val_suc;
                alpha=(alpha>val)?alpha:val;
                corte=(beta<=alpha)?1:0;
                l_eliminar(sucesores,actual,&fNoEliminarIA);

                it++;
            }

            vu=val;

            l_destruir(&sucesores,&fSiEliminarIA);

        }else{

            tLista sucesores = estados_sucesores(e,jugador_max);
            tPosicion actual;
            tEstado estadoSuc;
            tNodo nSuc;

            int longitud = l_longitud(sucesores);
            int corte=0;
            int it=0;

            int val = IA_INFINITO_POS;

            while(it<longitud && corte==0){//Trabajar con longitud o con l_fin

                actual= l_primera(sucesores);
                estadoSuc = l_recuperar(sucesores,actual);
                nSuc = a_insertar(a,n,NULL,estadoSuc);
                crear_sucesores_min_max(a,nSuc,1,alpha,beta,jugador_max,jugador_min);
                val_suc = estadoSuc->utilidad;
                val=(val<val_suc)?val:val_suc;
                alpha=(alpha<val)?alpha:val;
                corte=(beta<=alpha)?1:0;

                l_eliminar(sucesores,actual,&fNoEliminarIA);

                it++;
            }

            vu=val;

            l_destruir(&sucesores,&fSiEliminarIA);

        }

    }

    (e->utilidad)=vu;

}

/**
>>>>>  A IMPLEMENTAR   <<<<<
Computa el valor de utilidad correspondiente al estado E, y la ficha correspondiente al JUGADOR_MAX, retornado:
- IA_GANA_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX gan� la partida.
- IA_EMPATA_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX empat� la partida.
- IA_PIERDE_MAX si el estado E refleja una jugada en el que el JUGADOR_MAX perdi� la partida.
- IA_NO_TERMINO en caso contrario.
**/
static int valor_utilidad(tEstado e, int jugador_max){

    int toRet = IA_NO_TERMINO;

    int ficha_rival=(jugador_max==PART_JUGADOR_1)?PART_JUGADOR_2:PART_JUGADOR_1;

    int T[3][3];
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            T[i][j]=(e->grilla[i][j]);
        }
    }

    int hay_fila=0;
    int hay_fila_rival=0;
    int hay_columna=0;
    int hay_columna_rival=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(T[i][j]==jugador_max)hay_fila++;
            if(T[i][j]==ficha_rival)hay_fila_rival++;
            if(T[j][i]==jugador_max)hay_columna++;
            if(T[j][i]==ficha_rival)hay_columna_rival++;
        }

        if((hay_fila==3) || (hay_columna==3)){
                toRet=IA_GANA_MAX;
                break;
        }else{
            hay_fila=0;
            hay_columna=0;
        }

        if((hay_fila_rival==3) || (hay_columna_rival==3)){
                toRet=IA_PIERDE_MAX;
                break;
        }else{
                hay_fila_rival=0;
                hay_columna_rival=0;

        }
    }

    if(((T[0][0])==(T[1][1])&&(T[1][1])==(T[2][2])) || ((T[0][2])==(T[1][1])&&(T[1][1])==(T[2][0]))){
        if(T[1][1]==jugador_max)toRet=IA_GANA_MAX;
        if(T[1][1]==ficha_rival)toRet=IA_PIERDE_MAX;
    }

    //Empataron?

    int hay_vacias=0;
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(T[i][j]==PART_SIN_MOVIMIENTO)hay_vacias++;
        }
    }
    if(hay_vacias==0)toRet=IA_EMPATA_MAX;

    return toRet;
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
Computa y retorna una lista con aquellos estados que representan estados sucesores al estado E.
Un estado sucesor corresponde a la clonaci�n del estado E, junto con la incorporaci�n de un nuevo movimiento
realizado por el jugador cuya ficha es FICHA_JUGADOR por sobre una posici�n que se encuentra libre en el estado E.
La lista de estados sucesores se debe ordenar de forma aleatoria, de forma tal que una doble invocaci�n de la funci�n
estados_sucesores(estado, ficha) retornar�a dos listas L1 y L2 tal que:
- L1 y L2 tienen exactamente los mismos estados sucesores de ESTADO a partir de jugar FICHA.
- El orden de los estado en L1 posiblemente sea diferente al orden de los estados en L2.
**/
static tLista estados_sucesores(tEstado e, int ficha_jugador){
    tLista sucesores;
    crear_lista(&sucesores);

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if((e->grilla[i][j])==PART_SIN_MOVIMIENTO){
                tEstado sucesor = clonar_estado(e);
                (sucesor->grilla[i][j])=ficha_jugador;
                if(rand()%2){
                    l_insertar(sucesores,l_fin(sucesores),sucesor);
                }else{
                    l_insertar(sucesores,l_primera(sucesores),sucesor);
                }
            }
        }
    }

    return sucesores;
}

/**
>>>>>  A IMPLEMENTAR   <<<<<
Inicializa y retorna un nuevo estado que resulta de la clonaci�n del estado E.
Para esto copia en el estado a retornar los valores actuales de la grilla del estado E, como su valor
de utilidad.
**/
static tEstado clonar_estado(tEstado e){
    tEstado clon = (tEstado) malloc(sizeof(struct estado));
    if(clon==NULL)exit(IA_ERROR_MEMORIA);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            (clon->grilla[i][j])=(e->grilla[i][j]);
        }
    }
    (clon->utilidad)=(e->utilidad);
    return clon;
}

/**
Computa la diferencia existente entre dos estados.
Se asume que entre ambos existe s�lo una posici�n en el que la ficha del estado anterior y nuevo difiere.
La posici�n en la que los estados difiere, es retornada en los par�metros *X e *Y.
**/
static void diferencia_estados(tEstado anterior, tEstado nuevo, int * x, int * y){

    int i,j, hallado = 0;
    for(i=0; i<3 && !hallado; i++){
        for(j=0; j<3 && !hallado; j++){
            if (anterior->grilla[i][j] != nuevo->grilla[i][j]){
                *x = i;
                *y = j;
                hallado = 1;
            }
        }
    }
}
