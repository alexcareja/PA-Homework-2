#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode {
	int elem;
	struct QueueNode *next;
}QueueNode;  // tipul de data nod dintr-o coada

typedef struct Queue {
	QueueNode *front;
	QueueNode *rear;
	int size;
}Queue;  // tipul de data coada

Queue* createQueue(void) {  // crearea unei cozi vide
	Queue *q = (Queue *) malloc(sizeof(Queue));
	q->front = NULL;
	q->rear = NULL;
	q->size = 0;
	return q;
}

int isQueueEmpty(Queue *q) {  // verificare coada goala
	if (q->front == NULL) {
		return 1;
	}
	return 0;
}

void enqueue(Queue *q, int elem) {  // inscriere in coada
	q->size += 1;
	QueueNode *node = (QueueNode *) malloc(sizeof(QueueNode));
	node->elem = elem;
	node->next = NULL;
	if (isQueueEmpty(q)) {
		q->front = node;
		q->rear = node;
		return;
	}
	q->rear->next = node;
	q->rear = node;
}

int front(Queue* q) {  // vezi primul element din coada
	return q->front->elem;
}

void dequeue(Queue* q) {  // scoate primul element din coada
	if (isQueueEmpty(q)) {
		return;
	}
	q->size -= 1;
	QueueNode *aux = q->front;
	q->front = q->front->next;
	if (q->front == NULL) {
		q->rear = NULL;
	}
	free(aux);
}

void destroyQueue(Queue *q) {  // distruge coada
	while(!isQueueEmpty(q)) {
		dequeue(q);
	}
	free(q);
}

void DFS_conex(Queue **va, Queue *q, int *v, int current);
int aciclic(Queue *q, Queue **va, int *v);
int DFS_ciclu(Queue **va, int *v, int current);

int main() {
	int ans, i, n, m, u , v;
	FILE *fin = fopen("holograme.in", "r");
	fscanf(fin, "%d %d\n", &n, &m);
	ans = n;
	int *viz = (int *) calloc(n + 1, sizeof(int));
	Queue **va = (Queue **) malloc((n + 1) * sizeof(Queue *));
	// ^ vector de adiacenta pentru graful orientat
	Queue **va_no = (Queue **) malloc((n + 1) * sizeof(Queue *));
	// ^ vector de adiacenta pentru graful neorientat
	for (i = 1; i <= n; i++) {
		va[i] = createQueue();
		va_no[i] = createQueue();
	}
	for (i = 0; i < m; i++) {
		fscanf(fin, "%d %d\n", &u, &v);
		enqueue(va[u], v);
		enqueue(va_no[u], v);
		enqueue(va_no[v], u);
	}
	fclose(fin);
	// gaseste componentele conexe si introduce nodurile sale in coada q
	// considerand graful ca fiind neorientat
	for (i = 1; i <= n; i++) {
		if (viz[i] == 0) {
			Queue *q = createQueue();
			DFS_conex(va_no, q, viz, i);
			// pentru componenta conexa gasita,verifica daca este aciclica
			// considerand graful orientat de data aceasta
			if (aciclic(q, va, viz)) {
				// daca nu are ciclu, atunci scad 1 din numarul minim de muchii
				ans--;
			}
		}
	}
	FILE *fout = fopen("holograme.out", "w");
	fprintf(fout, "%d\n", ans);
	for (i = 1; i <= n; i++) {
		destroyQueue(va[i]);
		destroyQueue(va_no[i]);
	}
	free(va);
	free(va_no);
	free(viz);
	return 0;
}

void DFS_conex(Queue **va, Queue *q, int *v, int current) {
	// construieste in q o componenta conexa prin DFS pe graful neorientat
	int succesor;
	v[current] = 1;
	enqueue(q, current);  // adauga in componenta conexa q, nodul curent
	QueueNode *aux = va[current]->front;
	while (aux != NULL) {
		succesor = aux->elem;
		if (v[succesor] == 0) {
			// daca am gasit un nod nevizitat, viziteaza
			DFS_conex(va, q, v, succesor);
		}
		aux = aux->next;
	}
}

int aciclic(Queue *q, Queue **va, int *v) {
	// verifica daca o componenta conexa contine un ciclu
	QueueNode *aux = q->front;
	while (aux != NULL) {
		if (v[aux->elem] == 1) {
			// daca nodul nu a fost vizitat, aplica DFS pentru a gasi ciclu
			if (DFS_ciclu(va, v, aux->elem)) {
				// daca am gasit un ciclu, returneaza 0
				return 0;
			}
		}
		aux = aux->next;
	}
	// nu am gasit niciun ciclu
	return 1;
}

int DFS_ciclu(Queue **va, int *v, int current) {
	// cauta daca exista un ciclu
	int succesor;
	v[current] = 2;  // nod gri
	QueueNode *aux = va[current]->front;
	while (aux != NULL) {
		succesor = aux->elem;
		if (v[succesor] == 2) {  // daca am gasit un nod gri, atunci avem ciclu
			return 1;
		}
		if (DFS_ciclu(va, v, succesor)) {
			// daca functia recursiva a descoperit un ciclu, returneza 1
			return 1;
		}
		aux = aux->next;
	}
	v[current] = 3;  // nod negru
	return 0;
}
