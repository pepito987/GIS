/* Author Giuseppe Quartarone mat.: 408661
 * Implementazione di un albero binario
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#define DEBUG fprintf(stderr,"%s\n","eccomi")
#define	 ABORT_1	  "out of memory on malloc()"
#define	 MACRO(x,y)	if( x == NULL ){ fprintf(stderr,"%s\n",y); exit(EXIT_FAILURE);}
#define NewNode new=(node_t *)malloc(sizeof(node_t)); MACRO(new,ABORT_1)\
		    new->key=(char *)malloc(sizeof(char)*strlen(key)+1); MACRO(new,ABORT_1)\
		    strcpy(new->key, key); \
		    new->data=data; \
		    new->left = NULL; \
		    new->right = NULL


/* Aggiunge un nodo all'albero, se il nodo esiste gia` assegna a ret il suo puntatore.
 * Restituisce -1 in caso di errore 1 altrimenti
 */
int addNode(node_t **r, char *key, void *data, node_t **ret ){
   node_t *new;
   node_t *att;
   att=*r;
   if(key == NULL || key =='\0')
      return -1;
   int i=0;
   while( att != NULL){
      i = strcmp(key, att->key);
      if(i>0){
	 if(att->right != NULL )
	    att=att->right;
	 else{
	    NewNode;
	    att->right = new;
	    return 1;
	 }
      }
      else if(i<0){
	    if(att->left != NULL)
	       att=att->left;
	    else{
	       NewNode;
	       att->left = new;
	       return 1;
	    }
      }
      else{
	 *ret=att;
	 return 1;
      }
   }
   NewNode;
   *r=new;
   return 1;
}


/* Elimina un nodo dall'albero, restituice -1 in caso di errore 1 altrimenti */
int delNode(node_t **r, char *key){
   if(r == NULL || key == NULL || key == '\0')
      return -1;
   node_t *canc, *prec, *att2, *prec2;
   int i;
   int type=0;
   canc=*r;
   prec=*r;
   while(canc != NULL && (i=strncmp(key,canc->key,MAXKEYLEN)) != 0){
      if(i>0){
	 prec=canc;
	 canc=canc->right;
	 type=1;
      }
      else{
	 prec=canc;
	 canc=canc->left;
	 type=-1;
      }
   }
   if(canc == NULL)
      return -1;
   if(canc->right == NULL && canc->left == NULL){	// un solo nodo
      free(canc->data);
      free(canc);
      if(prec == canc)
	 *r=NULL;
      return -1;
   }
   att2=canc->right;
   prec2=att2;
   while(att2->left != NULL){
      prec2=att2;
      att2=att2->left;
   }
   att2->left=canc->left;
   if(prec2 != att2){
      prec2->left = NULL;
      att2->right=canc->right;
   }
   if(canc = prec)
      *r=att2;
   else{
      if(type == 1)
	 prec->right = att2;
      else
	 prec->left = att2;
   } 
  free(canc->data);
  free(canc);
  return 1;
}
/* Ricerca un nodo e ne restituisce il suo puntatore, NULL in caso di errore */
node_t *findNode(node_t *r, char *key){
   if(r == NULL || key == NULL || key == '\0')
      return NULL;
   node_t *att;
   att=r;
   int i=0;
   while( att != NULL && (i=strncmp(key,att->key,MAXKEYLEN)) != 0 ){
      if(i>0)
	 att=att->right;
      else
	 att=att->left;
   }
   return att;
}
/* Stampa l'intero albero */
void printTree(node_t *r){
   if(r != NULL){
      printf("key: %s\n",r->key);
      printTree(r->left);
      printTree(r->right);
   }  
}
