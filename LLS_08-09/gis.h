/* Author Giuseppe Quartarone mat.: 408661
 * Implementazine di un grafo rappresentate un sistema di trasporto pubblico.
 */

#include "list.h"
#include "tree.h"
#include "pqueue.h"
#define INFINITO 65535

/* Definizione di un orario formato da ore e minuti */
typedef struct orario{
   int ore;
   int min;
}orario;

/* Rappresentazione di arco */
typedef struct gis_arch{
   char *name, *src, *dst;	    /* nome dell'arco, sorgente, destinazione */
   int keysrc,keydst,keyservice;    /* chiavi della sorgente, della destinazione, del servizio */
   int cost;   
   orario orap;			    /* ora di partenza in minuti */
   orario orad;			    /* ora di arrivo in minuti */
   struct gis_arch *nextfs;	    /* puntatore al prossimo arco della stella uscente cui appartiene */
   struct gis_arch *next;	    /* puntatore al prozzimo arco della lista */
} service;

/* Descrizione di un nodo */
typedef struct gis_node{
   char *name;
   int key;
   service *day[7];		    /* identifica i giorni della settimana, e rappresenta la stella uscente del nodo */
   struct gis_node *next;	    /* puntatore al prossimo nodo nella lista */
} station;

/* Rappresentazione di un grafo */
typedef struct gis{
   station *stationlist;	    /* lista di stazioni */
   service *servicelist;	    /* lista di servizi */
   int sizestation;		    /* dimensione lista stazioni */
   int sizeservice;		    /* dimensione lista servizi */   
   node_t *sthead;		    /* albero binario contenente i puntatori alle stazioni */
} gis_t;

/* Struttura per rappresentare un predecessore */
typedef struct predecessore{
   station *pred;		    /* puntatore alla stazione predecessore */
   station *node;		    /* puntatore al nodo stesso */
   service *type;		    /* puntatore al servizio che lega il nodo al suo predecessore */
   int etichetta;
} pred_t;

/* Stama l'intero grafo */
void printgis(gis_t *g);

/* Stampa la lista delle tazioni */
void printStationList(gis_t *g);

/* Legge il file di configurazione e crea il grafo */
int readconf(FILE *fp, gis_t *g);

/* Inizzializza il grafo */
gis_t *initializegis();

/* calcola il cammino dalla sorgente alla destinazione */
int cammino(gis_t *gis, int orap, int g, char *src, char *dest);
