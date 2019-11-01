#include "partida.h"
#include "ia.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

static int verFilas(tPartida p,int mov_y);
static int verColumnas(tPartida p,int mov_x);
static int verDiagonales(tPartida p,int mov_x,int mov_y);


typedef struct tablero * tTablero;
typedef struct partida * tPartida;

/**
Inicializa una nueva partida, indicando:
 - Modo de partida (Usuario vs. Usuario o Usuario vs. Agente IA).
 - Jugador que comienza la partida (Jugador 1, Jugador 2, o elecci�n al azar).
 - Nombre que representa al Jugador 1.
 - Nombre que representa al Jugador 2.
**/
void nueva_partida(tPartida * p, int modo_partida, int comienza, char * j1_nombre, char * j2_nombre){
int ran;
*p=(tPartida) malloc(sizeof(struct partida));
if ((*p) == NULL) exit(PART_ERROR_MEMORIA);
tTablero tab=(tTablero) malloc(sizeof(struct tablero));
if(tab==NULL) exit((PART_ERROR_MEMORIA));
(*p)->tablero=tab;
for(int i=0;i<3;i++)
  for(int j=0;j<3;j++)
  tab->grilla[i][j]=PART_SIN_MOVIMIENTO;

(*p)->modo_partida=modo_partida;
(*p)->estado=PART_EN_JUEGO;
if(comienza==PART_JUGADOR_RANDOM){
    srand(time(NULL));
    ran=rand()%2;
    if (ran==0)ran=PART_JUGADOR_1;
    else ran=PART_JUGADOR_2;
    comienza=ran;
}
(*p)->turno_de=comienza;
strcpy((*p)->nombre_jugador_1,j1_nombre);
strcpy((*p)->nombre_jugador_2,j2_nombre);
}

/**
Actualiza, si corresponde, el estado de la partida considerando que el jugador al que le corresponde jugar, decide hacerlo en la posici�n indicada (X,Y).
En caso de que el movimiento a dicha posici�n sea posible, retorna PART_MOVIMIENTO_OK; en caso contrario, retorna PART_MOVIMIENTO_ERROR.
Las posiciones (X,Y) deben corresponderse al rango [0-2]; X representa el n�mero de fila, mientras Y el n�mero de columna.
**/
int nuevo_movimiento(tPartida p, int mov_x, int mov_y){
int toRet=PART_MOVIMIENTO_OK;
int diag,filas,cols,empate=0;

int jug_actual;
if(p->turno_de==PART_JUGADOR_1) jug_actual=PART_JUGADOR_1;
else jug_actual=PART_JUGADOR_2;
if((p->tablero)->grilla[mov_x][mov_y]!=PART_SIN_MOVIMIENTO||mov_x<0||mov_x>2||mov_y<0||mov_y>2) toRet= PART_MOVIMIENTO_ERROR;
else{
    p->tablero->grilla[mov_x][mov_y]=p->turno_de;
    for(int i=0;i<3&&empate!=9;i++)
        for(int j=0;j<3&&empate!=9;j++)
    if(p->tablero->grilla[i][j]==PART_SIN_MOVIMIENTO) {empate++;}

    if(empate==9)p->estado=PART_EMPATE;
    else{
        if((mov_x+mov_y)%2==0) diag=verDiagonales(p,mov_x,mov_y);
        else diag=0;
        filas=verFilas(p,mov_x);
        cols=verColumnas(p,mov_y);

        if(diag+filas+cols>0){
            if(jug_actual==PART_JUGADOR_1) p->estado=PART_GANA_JUGADOR_1;
            else p->estado=PART_GANA_JUGADOR_2;

        }

    }


}
if(toRet==PART_MOVIMIENTO_OK){
if(jug_actual==PART_JUGADOR_1) p->turno_de=PART_JUGADOR_2;
else p->turno_de=PART_JUGADOR_1;
}
return toRet;

}

/**
Finaliza la partida referenciada por P, liberando toda la memoria utilizada.
**/
void finalizar_partida(tPartida * p){
free((*p)->tablero);
free((*p)->nombre_jugador_1);
free((*p)->nombre_jugador_2);
free(*p);
(*p)->tablero=NULL;
(*p)=NULL;

}

static int verFilas(tPartida p,int y){
 int ret=1;
for(int i=0;i<3&&ret==1;i++)
 if(p->tablero->grilla[i][y]!=p->turno_de) ret=0;
return ret;
}
static int verColumnas(tPartida p,int x){
int ret=1;
for(int i=0;i<3&&ret==1;i++)
 if(p->tablero->grilla[x][i]!=p->turno_de) ret=0;
return ret;

}
static int verDiagonales(tPartida p,int x,int y){
int valor=p->turno_de;
int ret=0;

if((p->tablero->grilla[1][1]==p->tablero->grilla[0][0]) && (p->tablero->grilla[1][1]==p->tablero->grilla[2][2])&& (p->tablero->grilla[1][1]==valor)) ret=1;
else if((p->tablero->grilla[1][1]==p->tablero->grilla[0][2]) && (p->tablero->grilla[1][1]==p->tablero->grilla[2][0])&& (p->tablero->grilla[1][1]==valor)) ret=1;


return ret;
}
