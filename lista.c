#include "lista.h"
#include <stdlib.h>

typedef struct celda* tLista;
typedef struct celda* tPosicion;
typedef void* tElemento;

/**
 Inicializa una lista vac�a.
 Una referencia a la lista creada es referenciada en *L.
**/
void crear_lista(tLista* l){
    *l = (tLista) malloc(sizeof (struct celda));
    if(*l==NULL)exit(LST_ERROR_MEMORIA);
    (*l)->elemento=NULL;
    (*l)->siguiente=NULL;
}

/**
 Inserta el elemento E, en la posici�n P, en L.
 Con L = A,B,C,D y la posici�n P direccionando C, luego:
 L' = A,B,E,C,D
**/
void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion nw = (tPosicion) malloc(sizeof (struct celda));
    if(p==NULL)exit(LST_POSICION_INVALIDA);
    if(l==NULL)exit(LST_POSICION_INVALIDA);
    if(nw==NULL)exit(LST_ERROR_MEMORIA);
    if(e==NULL)exit(LST_ELEMENTO_NULO);
    nw->elemento=e;
    (nw->siguiente)=(p->siguiente);
    p->siguiente=nw;
}

/**
 Elimina la celda P de L. El elemento almacenado en la posici�n P es eliminado mediante la funci�n fEliminar parametrizada.
 Si P es fin(L), finaliza indicando LST_POSICION_INVALIDA.
**/
void l_eliminar(tLista l, tPosicion p, void (*fEliminar)(tElemento)){
    if(p==NULL)exit(LST_POSICION_INVALIDA);
    if(l==NULL)exit(LST_POSICION_INVALIDA);
    if(p->siguiente == NULL)exit(LST_POSICION_INVALIDA);
    tPosicion aEliminar = (p->siguiente);
    fEliminar(aEliminar->elemento);
    tPosicion nSiguiente = (aEliminar->siguiente);
    aEliminar->siguiente=NULL;
    p->siguiente=nSiguiente;
    if(aEliminar!=NULL)free(aEliminar);
    aEliminar=NULL;
}

/**
 Destruye la lista L, elimininando cada una de sus celdas. Los elementos almacenados en las celdas son eliminados mediante la funci�n fEliminar parametrizada.
**/
void l_destruir(tLista * l, void (*fEliminar)(tElemento)){
    if(*l==NULL)exit(LST_POSICION_INVALIDA);
    tPosicion p = (tPosicion) l;
    while(p->siguiente!=NULL){
        tPosicion aEliminar = (p->siguiente);
        fEliminar(aEliminar->elemento);
        tPosicion nSiguiente = (aEliminar->siguiente);
        aEliminar->siguiente=NULL;
        p->siguiente=nSiguiente;
        if(aEliminar!=NULL)free(aEliminar);
        aEliminar=NULL;
    }
    fEliminar(p->elemento);
    if(p!=NULL)free(p);
    p=NULL;
    if(p!=NULL)free(l);
    *l=NULL;
}

 /**
 Recupera y retorna el elemento en la posici�n P.
 Si P es fin(L), finaliza indicando LST_POSICION_INVALIDA.
**/
tElemento l_recuperar(tLista l, tPosicion p){
    if(p==NULL)exit(LST_POSICION_INVALIDA);
    if(l==NULL)exit(LST_POSICION_INVALIDA);
    if((p->siguiente)==NULL)exit(LST_POSICION_INVALIDA);
    tElemento toRet;
    tPosicion sig = p->siguiente;
    toRet = sig->elemento;
    return toRet;
}

/**
 Recupera y retorna la primera posici�n de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_primera(tLista l){
    if(l==NULL)exit(LST_POSICION_INVALIDA);
    return l;
}

/**
 Recupera y retorna la posici�n siguiente a P en L.
 Si P es fin(L), finaliza indicando LST_NO_EXISTE_SIGUIENTE.
**/
tPosicion l_siguiente(tLista l, tPosicion p){
    if(l==NULL)exit(LST_POSICION_INVALIDA);
    if((p->siguiente)==NULL)exit(LST_NO_EXISTE_SIGUIENTE);
    return (p->siguiente);
}

/**
 Recupera y retorna la posici�n anterior a P en L.
 Si P es primera(L), finaliza indicando LST_NO_EXISTE_ANTERIOR.
**/
tPosicion l_anterior(tLista l, tPosicion p){
    if(p==l)exit(LST_NO_EXISTE_ANTERIOR);
    tPosicion pos=l;
    while((pos->siguiente)!=p)pos=(pos->siguiente);
    return pos;
}

 /**
 Recupera y retorna la �ltima posici�n de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_ultima(tLista l){
    tPosicion toRet = l;
    tPosicion actual = l;
    while((actual->siguiente)!=NULL){
        toRet=actual;
        actual=(actual->siguiente);
    }
    return toRet;
}

 /**
 Recupera y retorna la posici�n fin de L.
 Si L es vac�a, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_fin(tLista l){
    tPosicion toRet = l;
    while((toRet->siguiente)!=NULL)
        toRet=(toRet->siguiente);
    return toRet;
}

/**
 Retorna la longitud actual de la lista.
**/
int l_longitud(tLista l){
    tPosicion aux=l;
    int i=0;
    while((aux->siguiente)!=NULL){
        i++;
        aux=(aux->siguiente);
    }
    return i;
}

