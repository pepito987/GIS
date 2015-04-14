/* Author Giuseppe Quartarone mat.: 408661
 * Implementzine di una lista 
 */
#define MAXKEYLEN 100

/* Struttura rappresentate un elemento di una lista */
typedef struct list{
   int key;
   void *data;
   struct list *next;
}elem_t;

/* Aggiunge un elemento in testa alla lista */
elem_t *addTopElem(elem_t *l, int k, void *d);

/* Cerca un elemento nella lista */
elem_t *findElem(elem_t *l, int k);

/* Stampa l'intera lista */
void printList(elem_t *l);
