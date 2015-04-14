/* Author Giuseppe Quartarone mat.: 408661
 * Implementazione di un heap di minimo
 */

#define MAX_INT 65535
#define MAX_STR	  200
#define LEFT(x)	  ((2*x)+1)
#define PARENT(x) ((x-1)/2)
#define SWAP(x,y) tmp=x; x=y; y=tmp

/* Rappresentazione di un nodo dello heap */
typedef struct heap_node{
   int key;
   int etichetta;
}node;

/* Rappresentazione di un heap di dimensione dim e contenente size elementi*/
typedef struct priority_queue{
   int size, dim;
   node **elements;
}pqueue;

/* nodo di supprto per effettuare lo scambio tra due nodi */
node *tmp;

/* Inizzializza la struttura, restituisce -1 in caso di errore 1 altrimenti */
int initpqueue(pqueue **p, int dim);

/* Inserisce un nuovo elemento alla struttra, i nodi sono ordinati per etichetta
 * e garantisce il bilanciamento a sinistra dello heap, restituisce -1 in caso di errore 1 altrimenti */
int enqueue(pqueue *p, int key, int etichetta);

/* Estrae l'elemento in testa (minimo) restituisce il puntatore al nodo estratto NULL in caso di errore */
node *dequeue(pqueue *p);

/* Restituisce il puntatore all'elemento in testa, ma non effettua la sua estrazione, in caso di errore restituisce NULL */
node *first(pqueue *p);

/* Stampa gli elementi della lista */
void printElements(pqueue *p);
