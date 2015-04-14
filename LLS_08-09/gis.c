/* Author Giuseppe Quartarone mat.: 408661
 * Implementazine di un grafo rappresentate un sistema di trasporto pubblico.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gis.h"

#define MAXNAME 100
#define MAX_ERR_BUFF 100
#define MAX_SOLUZIONI	5
#define TEMPO_CAMBIO	10
#define CHECK(x,y,z,a) if(x != y){ fprintf(stderr,"%s\n",ERROR_3); exit(EXIT_FAILURE);}
#define	 ERROR_3  "Errore nel file di configurazione"
#define	 ERROR_1	  "out of memory on malloc()"
#define	 MACRO(x,y,z,t)	if( x == y ){ fprintf(stderr,"%s\n",z); return t;}

char errbuff[MAX_ERR_BUFF];

/* Inizializza il gis ai valori di default, restituisce il puntatore alla struttura, NULL altrimenti*/
gis_t *initializegis(){
   gis_t *t;
   int i;
   t=(gis_t *)malloc(sizeof(gis_t));
   MACRO(t,NULL,ERROR_1,NULL)
   t->sizestation=0;
   t->sizeservice=0;
   t->stationlist=NULL;
   t->servicelist=NULL;
   return t;
}

/* Stampa la lista delle stazioni */
void printStationList(gis_t *g){
   station *st;
   st=g->stationlist;
   while(st != NULL){
      printf("%d) %s\n",st->key,st->name);
      st = st->next;
   }
}

/* Stampa tutta la struttura */
void printgis(gis_t *g){
   station *st;
   service *next;
   st=g->stationlist;
   while(st != NULL){
      printf("%d) Stazione: %s\n",st->key,st->name);      
      int i;
      printf(" Servizi: \n");
      for(i=0; i<7; i++){
	 printf("GIORNO: %d) ",i+1);
	 if(st->day[i] != NULL){
	    next=st->day[i];
	    while(next != NULL){
	       printf("[%d] %s %d) %s --> %d) %s P: %02d:%02d A: %02d:%02d COSTO: %d|\n           ",next->keyservice,next->name,next->keysrc,next->src,next->keydst,next->dst,next->orap.ore,next->orap.min,next->orad.ore,next->orad.min,next->cost);
	       next=next->nextfs;
	    }
	 }
	 printf("\n");
      }  
      st=st->next;
   }
}
/* Aggiunge una relazione, in particolare aggiunge un nodo ed il suo relativo arco della stella uscente.
 * restituisce il puntatore all'arco, NULL in caso di errore.
 * Param: g gis, tos nome del servizio, day giorni di effettuazione,puntatore all'arco precedente,sorgente
 * orari di partenza e destinazione
 */
service *addRel(gis_t *g, char *tos, char *day, service *prec, char *sorgente, orario orap, orario orad){
   if(g == NULL )
      return 0;
   int i=0;
   int c;
   station *src;
   service *new;
   new = NULL;
   addStation(g,sorgente, &src); 
   if(orap.ore >=0 && orap.min >= 0){
      new=(service *)malloc(sizeof(service));		    
      MACRO(new,NULL,ERROR_1,NULL)
      new->name=(char *)malloc(sizeof(char)*strlen(tos)+1);
      MACRO(new,NULL,ERROR_1,NULL)
      new->name=strcpy(new->name,tos); 
      new->src=(char *)malloc(sizeof(char)*strlen(src->name)+1);
      MACRO(new,NULL,ERROR_1,NULL)
      new->src=strcpy(new->src,src->name);
      new->keysrc=src->key;
      new->orap.ore=orap.ore; 
      new->orap.min=orap.min; 
      new->orad.ore=orad.ore; 
      new->orad.min=orad.min;
      new->next=g->servicelist;
      g->servicelist=new;
      g->sizeservice++;
      new->keyservice = g->sizeservice;
      while(day[i] != '\0'){
	 c=day[i] -49 ;
	 new->nextfs=src->day[c];
	 src->day[c]=new;
	 i++;
      }
   }
   if(prec != NULL){
      prec->dst=(char *)malloc(sizeof(char)*strlen(src->name)+1);
      MACRO(prec->dst,NULL,ERROR_1,NULL)
      prec->dst=strcpy(prec->dst,src->name);
      prec->keydst=src->key;
      prec->orad.ore=orad.ore;
      prec->orad.min=orad.min;
      prec->cost=(((prec->orad.ore*60)+prec->orad.min)-(prec->orap.ore*60)-(prec->orap.min));
      //printf("COSTO: %d\n",prec->cost);
   }
   return new;
}


/* Legge il file di configurazione, creando man mano le stazioni e gli archi necessari, termina con exit in caso di errore di lettura 
 * scrivendo un messaggio di errore sullo standar error, restituisce 1 in cas positivo.
 */
int readconf(FILE *fp, gis_t *g){
   char tos[MAXNAME], day[8], src[MAXNAME], dest[MAXNAME];
   char tmp[MAXNAME], buff[MAXNAME*3];
   orario orap, orad;
   service *prec;
   int i,j;
   j=0;
   while((fgets(buff, sizeof(buff), fp) != NULL)){
      j++;
      if(isalpha(buff[0])){	 // letura TOS
	 i=sscanf(buff,"%[^|]|%s\n",tos,day);
	 day[7]='\0';
	 CHECK(2,i,buff,j)
      }else if(buff[0] == '-'){	 //lettura SRC	    
	 i=sscanf(buff,"-|%[^|]|%02d:%02d\n",src,&(orap.ore),&(orap.min));
	 CHECK(i,3,buff,j)
	 orad.ore=-1;
	 orad.min=-1;
	 prec=addRel(g,tos,day,NULL,src,orap,orad);
      }else if(buff[0] != '\n'){ //LETTURA DEST
	 i=sscanf(buff,"%d:%d|%[^|]|%s\n",&(orad.ore),&(orad.min),dest,tmp);
	 CHECK(i,4,buff,j)
	 if(isdigit(tmp[0])){
	    i=sscanf(tmp,"%02d:%02d",&(orap.ore),&(orap.min));
	    CHECK(2,i,buff,j)
	 }
	 else{
	    orap.ore=-1;	    
	    orap.min=-1;
	 }	    
	 if(prec == NULL )
	    fprintf(stderr,"%s\n","ERRORE GRAVE IN ADDREL");
	 prec=addRel(g,tos,day,prec,dest,orap,orad); 
      }
   }
}

/* Aggiunge una stazione, e completa la configurazine di quella precedente rappresentata da s,
 * restituisce -1 in caso di errore, 1 altrimenti.
 * Param: g gis, name nome stazione, s stazione precedente da completare.
 */
int addStation(gis_t *g,char *name, station **s){
   if(g == NULL || name == NULL || name == '\0' || strlen(name)>MAXNAME)
      return -1;
   station *st;
   node_t *node;
   node=NULL;
   st=(station *)malloc(sizeof(station));
   MACRO(st,NULL,ERROR_1,-1)
   addNode(&(g->sthead),name,st,&node);
   if(node == NULL){
      st->name=(char *)malloc(sizeof(char)*strlen(name)+1);
      MACRO(st,NULL,ERROR_1,-1)
      st->name=strcpy(st->name,name);
      st->key=++(g->sizestation);
      st->next=g->stationlist;
      g->stationlist=st;
      *s=st;
      return 1;
   }
   *s=(station *)node->data;
   return 1;
}


/* Calcola un cammino dalla stazione di partenza fino alla destinazione, restituisce il puntatore all struttura
 * predecessore. 
 * Param: gis gis, g giorno della settimana (1-7), orap orario di partenza in minuti, pred puntatore alla struttura predecessore,
 * dst stazione di arrivo, minserv puntatore ad un albero binario contenente gli archi da scartare, puntatore alla coda 
 * utilizzata per la visita del grafo.
 */
pred_t *spt(gis_t *gis,int g, int orap, pred_t *pred, station *src,station *dst, node_t *minserv,pqueue *q){
   int partenza,attesa,oldorap;
   node *qnode;
   service *serv,*servtmp;
   station *s;
   node_t *tmp;
   char str[MAXKEYLEN], oldnameserv[MAXNAME];
   oldorap = orap;
   attesa=0;
   partenza=0;
   s = gis->stationlist;
   while(s != NULL){
      pred[s->key-1].etichetta=INFINITO;
      pred[s->key-1].pred=src;
      pred[s->key-1].node=s;
      pred[s->key-1].type=NULL;
      s=s->next;
   }
   pred[src->key-1].etichetta=0;
   pred[src->key-1].pred=NULL;
   initpqueue(&q,5);
   enqueue(q,src->key,pred[src->key-1].etichetta);
   while((qnode=dequeue(q)) != NULL){
      s=pred[qnode->key-1].node;
      serv=s->day[g-1];
      while(serv != NULL){	
	 sprintf(str,"%d",serv->keyservice);
	 if(((tmp=findNode(minserv,str)) == NULL) ){
	    partenza=(serv->orap.ore*60)+serv->orap.min;
	    if(partenza >= oldorap){
	       if(pred[serv->keysrc-1].type != NULL){
		  attesa= partenza - ((pred[serv->keysrc-1].type->orad.ore*60) + pred[serv->keysrc-1].type->orad.min);
		  strcpy(oldnameserv,pred[serv->keysrc-1].type->name);
		  oldorap = 0;
	       }
	       if(( (attesa >= 0) && (pred[s->key-1].etichetta + serv->cost + attesa) < pred[serv->keydst-1].etichetta) ){
		  if(!((attesa < TEMPO_CAMBIO) && (pred[serv->keysrc-1].type != NULL) && (strcmp(serv->name,oldnameserv) != 0))){	  
		     pred[serv->keydst-1].etichetta = pred[s->key-1].etichetta + serv->cost + attesa;
		     pred[serv->keydst-1].pred=s;
		     pred[serv->keydst-1].type=serv;  	       
		     enqueue(q,serv->keydst,pred[serv->keydst-1].etichetta);
		     if(serv->keydst == dst->key)
			return pred;
		  }
	       }
	    }
	 }
	 serv=serv->nextfs;
      }
   }
   return pred;
}

/* Stampa ricorsivamente il percosro dalla sorgente alla destinazine. Di volta in volta aggiunge
 *  a mincost l'arco usato dal primo nodo per raggiungere la destinazine.
 * Param: p puntatore alla struttura predecessore, i indice della destinazione, key chiave della sorgente,
 * mincost struttura utilizzata per contenere gli archi da scartare nel calcolo delle succesive soluzioni 
 */
void stampasol(pred_t *p,int i, int key, node_t **mincost ){
   service *serv;
   if((p[i].pred != NULL) && (p[i].pred->key != key)){
      stampasol(p,p[i].pred->key-1,key,mincost);
   }
   serv = p[i].type;
   if(p[i].pred->key == key){
      char str[MAXKEYLEN];
      node_t *tmp;
      sprintf(str,"%d",serv->keyservice);
      addNode(mincost,str,serv,&tmp);
   }
   serv = p[i].type;
   printf("%s | %s | %02d:%02d | %s | %02d:%02d\n",serv->name,serv->src,serv->orap.ore,serv->orap.min,serv->dst,serv->orad.ore,serv->orad.min);
   return;


}

/* Ricerca un cammino sul grafo e ne esegue la stampa durante l'esecuzione. Si puo` impostare la macro MAX_SOLUZIONI
 * che intica il numero di riesecuzioni dell'algoritmo per il calcolo di cammini alternativi. Restituisce 1 in caso positivo
 * -1 altrimenti ed in questo caso scrive un messaggio di errore su errbuff.
 * Param: gis gis, orap orario di partenza in minuti, g giono della settimana (1-7), sorgente, destinazione
 */
int cammino(gis_t *gis, int orap, int g, char *sorgente, char *destinazione){
   pqueue *q;
   pred_t *pred;
   node_t *st1,*st2;
   station *src,*dst;
   service *serv, *servtmp;
   node_t *mincost,*tmp;
   int min = INFINITO;
   char *str,keyserv[100];
   int k;
   int j,i,y;
   j=0; i=0;
   str=" ";
   mincost = NULL;
   printf("Ricerca cammino %d \n",g);
   if(((st1 = findNode(gis->sthead,sorgente)) == NULL) || ((st2 = findNode(gis->sthead,destinazione)) == NULL)){
      strcpy(errbuff,"Stazione non trovata\n");
      return -1;
   }
   pred = (pred_t *)malloc(sizeof(pred_t)*gis->sizestation);
   src = (station *)st1->data;
   dst = (station *)st2->data;
   while( j <= MAX_SOLUZIONI ){
      pred = spt(gis,g,orap,pred,src,dst,mincost,q);
      k=dst->key-1;
      if(pred[k].etichetta >= INFINITO){
	 printf("fine\n");
	 break;
      }
      printf("Opzione %d | %02d:%02d\n",j+1,(pred[k].etichetta/60),(pred[k].etichetta%60));
      stampasol(pred,k,src->key,&mincost);
      printf("\n");
      j++;
   }
   return 1;
}  
