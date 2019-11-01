#include "lista.h"
#include "arbol.h"
#include <stdlib.h>
#include<stdio.h>


typedef struct nodo * tNodo;
typedef struct arbol * tArbol;

void (*fElim)(tElemento) = NULL;
void fNoEliminar(tElemento e){}



/**
 Metodo auxiliar, destruye un subarbol
**/
void vaciar(tNodo n){
    tLista sons = (n->hijos);
    tPosicion actual = l_primera(sons);
    tPosicion corte = l_fin(sons);
    while(actual!=corte){
            tNodo nActual = l_recuperar(sons,actual);
            if(l_longitud((nActual->hijos))>0)vaciar(nActual);
            fElim((n->elemento));
            l_destruir(&(nActual->hijos),&fNoEliminar);
            (nActual->padre)=NULL;
            actual = l_siguiente(sons,actual);

    }

}


/**
Inicializa un �rbol vac�o.
Una referencia al �rbol creado es referenciado en *A.
**/

 void crear_arbol(tArbol * a){
    *a=(tArbol) malloc(sizeof(struct nodo));
    if((*a)==NULL) exit (ARB_ERROR_MEMORIA);
    (*a)->raiz=NULL;
}

/**
Crea la ra�z de A.
Si A no es vac�o, finaliza indicando ARB_OPERACION_INVALIDA.
**/
void crear_raiz(tArbol a, tElemento e){
    if(a==NULL)exit(ARB_POSICION_INVALIDA);
    if((a->raiz)!=NULL)exit(ARB_OPERACION_INVALIDA);
    tNodo root = (tNodo) malloc(sizeof (struct nodo));
    if(root==NULL)exit(ARB_ERROR_MEMORIA);

    root->elemento=e;
    root->padre=NULL;

    crear_lista(&(root->hijos));

    a->raiz=root;
}

/**
 Inserta y retorna un nuevo nodo en A.
 El nuevo nodo se agrega en A como hijo de NP, hermano izquierdo de NH, y cuyo r�tulo es E.
 Si NH es NULL, el nuevo nodo se agrega como �ltimo hijo de NP.
 Si NH no corresponde a un nodo hijo de NP, finaliza indicando ARB_POSICION_INVALIDA.
 NP direcciona al nodo padre, mientras NH al nodo hermano derecho del nuevo nodo a insertar.
**/
tNodo a_insertar(tArbol a, tNodo np, tNodo nh, tElemento e){
    if(a==NULL)exit(ARB_OPERACION_INVALIDA);
    if(np==NULL)exit(ARB_OPERACION_INVALIDA);
    tNodo nuevo =(tNodo) malloc(sizeof (struct nodo));
    if(nuevo==NULL)exit(ARB_ERROR_MEMORIA);

    tLista children;
    tLista hermanos = (np->hijos);
    tPosicion hermano = l_primera(hermanos);

    if(nh!=NULL){
        while((hermano!=l_fin(hermanos)) && (l_recuperar(hermanos,hermano)!=nh))
            hermano=l_siguiente(hermanos,hermano);
        if(hermano==l_fin(hermanos))exit(ARB_POSICION_INVALIDA);
    }

    (nuevo->elemento)=e;
    (nuevo->padre)=np;
    crear_lista(&children);
    (nuevo->hijos)=children;


    //Insercion en la lista de hijos

    if(nh==NULL){
        l_insertar(hermanos,l_fin(hermanos),nuevo);
    }else{
        l_insertar(hermanos,hermano,nuevo);
    }
    return nuevo;
}


/**
 Elimina el nodo N de A.
 El elemento almacenado en el �rbol es eliminado mediante la funci�n fEliminar parametrizada.
 Si N es la ra�z de A, y tiene un s�lo hijo, este pasa a ser la nueva ra�z del �rbol.
 Si N es la ra�z de A, y a su vez tiene m�s de un hijo, finaliza retornando ARB_OPERACION_INVALIDA.
 Si N no es la ra�z de A y tiene hijos, estos pasan a ser hijos del padre de N, en el mismo orden y a partir de la posici�n que ocupa N en la lista de hijos de su padre.
**/
void a_eliminar(tArbol a, tNodo n, void (*fEliminar)(tElemento)){
    if(a==NULL)exit(ARB_OPERACION_INVALIDA);
    if(n==NULL)exit(ARB_OPERACION_INVALIDA);
    if(n==(a->raiz)){
        if(l_longitud((n->hijos))>1)exit(ARB_OPERACION_INVALIDA);//Si no tiene hijos que hacer
        if(l_longitud((n->hijos))==1){
            tNodo hijo = l_recuperar((n->hijos),l_primera((n->hijos)));
            fEliminar((n->elemento));
            l_destruir(&(n->hijos),&fNoEliminar);
            if(n!=NULL)free(n);
            n=NULL;
            a->raiz=hijo;
        }else{
            fEliminar((n->elemento));
            l_destruir(&(n->hijos),fEliminar);
            if(n!=NULL)free(n);
            n=NULL;
        }
    }else{
        tNodo father= (n->padre);
        tLista brothers = (father->hijos);
        tLista sons = (n->hijos);
        tPosicion posN = l_primera(brothers);
        while(posN!=l_fin(brothers) && n!=l_recuperar(brothers,posN)){posN=l_siguiente(brothers,posN);}
        tPosicion rBro = l_siguiente(brothers,posN);
        tPosicion actual = l_primera(sons);
        while(actual!=l_fin(sons)){//EL ERROR ERA ACA!!!!
            tNodo nue = l_recuperar(sons,actual);
            l_insertar(brothers,rBro,nue);
            (nue->padre)=father;
            actual=l_siguiente(sons,actual);
            rBro=l_siguiente(brothers,rBro);
        }
        l_eliminar(brothers,posN,&fNoEliminar);


        fEliminar((n->elemento));
        l_destruir(&(n->hijos),&fNoEliminar);
        (n->padre)=NULL;
        free(n);
        n=NULL;
    }
}

/**
 Destruye el �rbol A, eliminando cada uno de sus nodos.
 Los elementos almacenados en el �rbol son eliminados mediante la funci�n fEliminar parametrizada.
**/
void a_destruir(tArbol * a, void (*fEliminar)(tElemento)){
    fElim = fEliminar;
    vaciar(((*a)->raiz));
    free(*a);
    *a=NULL;
}



/**
Recupera y retorna el elemento del nodo N.
*/
tElemento a_recuperar(tArbol a, tNodo n){
    if(a==NULL) exit(ARB_ERROR_MEMORIA);
    return n->elemento;
}

/**
Recupera y retorna el nodo correspondiente a la ra�z de A.
**/
tNodo a_raiz(tArbol a){
    if(a==NULL) exit(ARB_ERROR_MEMORIA);
    return (a->raiz);
}

/**
 Obtiene y retorna una lista con los nodos hijos de N en A.
**/
tLista a_hijos(tArbol a, tNodo n){
    if(a==NULL) exit(ARB_ERROR_MEMORIA);
    if(n==NULL) exit(ARB_ERROR_MEMORIA);
    return (n->hijos);
}


void clonar(tNodo clon,tNodo original){
    tLista oHijos = (original->hijos);
    tLista cHijos = (clon->hijos);
    tPosicion actual = l_primera(oHijos);
    tPosicion corte = l_fin(oHijos);

    while(actual!=corte){
        tNodo cNuevo = (tNodo) malloc(sizeof(struct nodo));
        if(cNuevo==NULL)exit(ARB_ERROR_MEMORIA);
        crear_lista(&(cNuevo->hijos));

        tNodo nActual = l_recuperar(oHijos,actual);
        if(l_longitud((nActual->hijos))>0)clonar(cNuevo,nActual);

        (cNuevo->elemento)=(nActual->elemento);
        (cNuevo->padre)=clon;

        l_insertar(cHijos,l_fin(cHijos),cNuevo);

        actual = l_siguiente(oHijos,actual);
    }
}

/**
 Inicializa un nuevo �rbol en *SA.
 El nuevo �rbol en *SA se compone de los nodos del sub�rbol de A a partir de N.
 El subarbol de A a partir de N debe ser eliminado de A.
**/
void a_sub_arbol(tArbol a, tNodo n, tArbol * sa){
    if(a==NULL)exit(ARB_POSICION_INVALIDA);
    if(n==NULL)exit(ARB_POSICION_INVALIDA);

    *sa = (tArbol) malloc(sizeof(struct nodo));
    if((*sa)==NULL)exit(ARB_ERROR_MEMORIA);
    (*sa)->raiz=NULL;

    tNodo saRoot = (tNodo) malloc(sizeof(struct nodo));
    if(saRoot==NULL)exit(ARB_ERROR_MEMORIA);
    (saRoot->elemento)=(n->elemento);
    (saRoot->padre)=NULL;
    crear_lista(&(saRoot->hijos));
    ((*sa)->raiz)=saRoot;

    clonar(((*sa)->raiz),n);
    vaciar(n);

    tNodo father= (n->padre);
    tLista fHijos= (father->hijos);
    tPosicion actual = l_primera(fHijos);
    tPosicion corte = l_fin(fHijos);
    while(actual!=corte && l_recuperar(fHijos,actual)!=n)actual=l_siguiente(fHijos,actual);

    l_eliminar(fHijos,actual,fNoEliminar);
    l_destruir(&(n->hijos),fNoEliminar);
    free(n);n=NULL;
}






