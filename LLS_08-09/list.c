#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#define	 ABORT_1	  "out of memory on malloc()"
#define	 MACRO(x,y,z)	if( x == NULL ){ fprintf(stderr,"%s\n",y); return z;}



/* Aggiunge un elemento alla lista, e ne restituisce la testa, NULL in caso di errore */
elem_t *addTopElem(elem_t *l, int k, void *d){
   elem_t *new;
   new=(elem_t *)malloc(sizeof(elem_t));
   MACRO(new,ABORT_1,NULL)
   new->key=k;
   new->data=d;
   new->next=l;
   return new;
}

/* Ricerca un elemento e ne restituice il suo puntatore */
elem_t *findElem(elem_t *l, int k){
   if(l == NULL )
      return NULL;
   while(l != NULL && (k != l->key)){
      l=l->next;
   }
   return l;
}

/* Stampa la lista */
void printList(elem_t *l){
   if(l == NULL)
      printf("lista vuota");
   while(l != NULL){
      printf("key: %d\n",l->key);
      l=l->next;
   }
   printf("fine lista\n");
}

