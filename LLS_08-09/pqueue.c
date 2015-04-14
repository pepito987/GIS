/* Author Giuseppe Quartarone mat.: 408661 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pqueue.h"
#define MAX_ERR_BUFF 100
#define DEB fprintf(stderr,"%s\n","eccomi");
#define	 ABORT_1	  "out of memory on malloc()"
#define	 ABORT_2  "out of memory in realloc()"
#define	 MACRO(x,y,z)	if( x == NULL ){ strcpy(errbuff,y); return z;}

char errbuff[MAX_ERR_BUFF];

/* Inizializza la coda, restituisce -1 in caso di errore, 1 altrimenti.
 * Param p coda da creare, dim dimensione.
 */
int initpqueue(pqueue **p, int dim){
   if(dim <= 0 || dim > MAX_INT)
      return -1;
   pqueue *new;
   new=(pqueue *)malloc(sizeof(pqueue));
   MACRO(new,ABORT_1,-1)
   new->dim = dim;
   new->elements=(node **)malloc(sizeof(node)*dim);
   MACRO(new->elements,ABORT_1,-1);
   new->size=0;
   *p=new;
   return 1;
}

/* Ripristina la proprieta` di heaptree per la cancellazione di un nuovo nodo */
void balance(pqueue *p, int i){
   int l, n;
   while((l=LEFT(i)) < p->size-1 && i !=(n=selectNode(p,i))){
      SWAP(p->elements[i],p->elements[n]);
      i=n;
   }   
}

/* Seleziona il minimo tra il padre e i figli */
int selectNode(pqueue *p, int i){
   int j, k;
   j=LEFT(i);
   k=j;
   if(k+1 < p->size-1 ) k++;
   if(p->elements[k]->key < p->elements[j]->key)
      j=k;
   if(p->elements[i]->key < p->elements[j]->key)
      j=i;
   return j;
}

/* Ripristina la proprieta` di heaptree per l'inserimento di un nodo */
void riorganizza(pqueue *p, int i){
  while(i>0 && (p->elements[i]->etichetta < p->elements[PARENT(i)]->etichetta)){
      SWAP(p->elements[i],p->elements[PARENT(i)]);
      i=PARENT(i);
   }
}

/* Inserisce un nuovo nodo, nel caso in cui non ci sia spazio, provvede ad allocare
 * nuova memoira. In particolare se la coda e` piena, la sua dimensione verra` raddoppiata.
 * Restituisce -1 in caso di errore 1 altrimenti 
 */
int enqueue(pqueue *p, int key, int etichetta){
   node *new;
   int i,j;
   if(p == NULL || key < 0 )
      return -1; 
   if(p->size == p->dim){
      p->elements=realloc(p->elements, sizeof(node)*(p->dim*2));
      MACRO(p,ABORT_2,-1);
      p->dim=p->dim*2;
   }
   for(i=0; i<p->size;i++){
      if(p->elements[i]->key == key ){
	 p->elements[i]->etichetta=etichetta;
	 riorganizza(p,i);
	 return 1;
      }
   }
   new=(node *)malloc(sizeof(node));
   MACRO(new,ABORT_1,-1);
   new->key=key;
   new->etichetta=etichetta;
   p->elements[p->size]=new;
   p->size++;
   i=p->size-1;
   riorganizza(p,i);
   return 1;
}
/* Estrae l'elemento in testa della coda, restituisce il puntatore a quest'ulitmo, NULL in caso di errore
 * se nella sista ci sono meno di size/4 elementi, la sua dimensione verra dimezzata */
node *dequeue(pqueue *p){
   node *min;
   if(p->size == 0)
     return NULL;
   if(p->size == (p->dim/4)){
      p->elements=realloc(p->elements, sizeof(node)*(p->dim/2));
      MACRO(p,ABORT_2,NULL)
      p->dim=p->dim/2; 
   }
   min=p->elements[0];
   p->elements[0]=p->elements[p->size-1];
   p->size--;
   balance(p,0);
   return min;
}

/* Restituice il puntatore all'elemento in testa, non effettua l'estrazione */
node *first(pqueue *p){
   if(p->size > 0 )
      return p->elements[0];
   return NULL;
}

/* stampa gli elementi della coda */
void printElements(pqueue *p){
   int i=0;
   while(i < p->size){
      printf("key: %d -- etichetta: %d \n",p->elements[i]->key,p->elements[i]->etichetta);
      i++;
   }
   printf("\n");
}

