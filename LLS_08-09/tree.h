/* Author Giuseppe Quartarone mat.:408661
 * Implementazione di un albero binario
 */
#define MAXKEYLEN 100

/** Nodo dell'albero */
typedef struct tree_node{
   char *key;
   void *data;
   struct tree_node *left;
   struct tree_node *right;
} node_t;

/** Aggiunge un nodo all'albero */
int addNode(node_t **r, char *key, void *date, node_t **ret);

/** Cancella un nodo */
int delNode(node_t **r, char *key);

/** Cerca un nodo */
node_t *findNode(node_t *r, char *key);

/** stampa albero */
void printTree(node_t *r);
