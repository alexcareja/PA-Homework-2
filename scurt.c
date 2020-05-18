#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COST 1000
#define MAX_VERTEXES 5000

typedef struct edge {
	int u;
	int v;
	int index;
} edge;  // tip de date muchie

typedef struct solution {
	edge w;
	int status;
} solution;

typedef struct VertexListNode {
	int nod;
	struct VertexListNode *next;
} VertexListNode;  // tip de date nod de lista de noduri

typedef struct VertexList {
	VertexListNode *start;
	VertexListNode *end;
} VertexList;  // tip de date lista de noduri

typedef struct forest {
	int e[MAX_VERTEXES + 1];
	VertexList *v;
	int size;
	edge last_edge;
	int opt;
} forest;  // tip de date padure (de adiacenta pentru un nod)

typedef struct EdgeListNode {
	edge w;
	struct EdgeListNode *next;
} EdgeListNode;  // tip de date nod de lista de muchii

typedef struct EdgeList {
	EdgeListNode *start;
	EdgeListNode *end;
} EdgeList;  // tip de date lista de muchii

VertexList *initVertexList() {  // initializeaza o lista de noduri goala
	VertexList *l = (VertexList *) malloc(sizeof(VertexList));
	l->start = NULL;
	l->end = NULL;
	return l;
}

void insertInVertexList(VertexList *l, int nod) {
	VertexListNode *node = (VertexListNode *) malloc(sizeof(VertexListNode));
	node->nod = nod;
	node->next = NULL;
	if (l->start == NULL) {  // lista e goala
		// insereaza la inceputul listei
		l->start = node;
		l->end = node;
		return;
	}
	// insereaza la finalul listei
	l->end->next = node;
	l->end = node;
}

void deleteVertexFirst(VertexList *l) {  // sterge primul nod din lista
	if (l->start == NULL) {
		return;
	}
	VertexListNode *aux = l->start;
	l->start = aux->next;
	if (l->start == NULL) {  // daca am eliminat ultimul element din lista
		l->end = NULL;
	}
	free(aux);
}

void destroyVertexList(VertexList *l) {  // distruge lista, elibereaza memoria
	while (l->start != NULL) {
		deleteVertexFirst(l);
	}
	free(l);
}

EdgeList *initEdgeList() {  // initializeaza o lista de muchii goala
	EdgeList *l = (EdgeList *) malloc(sizeof(EdgeList));
	l->start = NULL;
	l->end = NULL;
	return l;
}

void insertInEdgeList(EdgeList *l, edge w) {  // insereaza un nod in lista
	EdgeListNode *node = (EdgeListNode *) malloc(sizeof(EdgeListNode));
	node->w = w;
	node->next = NULL;
	if (l->start == NULL) {  // lista e goala
		// insereaza la inceputul listei
		l->start = node;
		l->end = node;
		return;
	}
	// insereaza la finalul listei
	l->end->next = node;
	l->end = node;
}

void deleteEdgeFirst(EdgeList *l) {  // sterge primul nod din lista
	if (l->start == NULL) {
		return;
	}
	EdgeListNode *aux = l->start;
	l->start = aux->next;
	if (l->start == NULL) {  // daca am eliminat ultimul element din lista
		l->end = NULL;
	}
	free(aux);
}

void destroyEdgeList(EdgeList *l) {  // distruge lista, elibereaza memoria
	while (l->start != NULL) {
		deleteEdgeFirst(l);
	}
	free(l);
}

forest *initPadure(int s) {
	forest *p = (forest *) malloc(sizeof(forest));
	memset(&(p->e), 0, (MAX_VERTEXES + 1) * sizeof(int));
		p->v = initVertexList();
	insertInVertexList(p->v, s);
	p->e[s] = 1;
	p->size = 1;
	p->opt = 0;
	return p;
}

int intersectie(forest *p1, forest *p2, int c, edge w, int **max_cost);
void reuniune(forest **pdr, edge w, int c, int **max_cost);
void interclasare(solution *v, int i, int m, int j);
void sortare_pi(solution *v, int i, int j);

int main() {
	int c, i, n, m, max_vertex = 0, r;
	// deschide fisierul de input
	FILE *fin = fopen("scurt.in", "r");
	// citeste N si M
	fscanf(fin, "%d %d\n", &n, &m);

	// citeste muchiile intr-un vector de liste, ce retine la indicele i numai
	// muchii de cost i intr-o lista
	EdgeList **e = (EdgeList **) calloc(MAX_COST + 1, sizeof(EdgeList **));
	edge w;
	for (i = 0; i < m; i++) {
		fscanf(fin, "%d %d %d\n", &(w.u), &(w.v), &c);
		w.index = i;
		if (e[c] == NULL) {
			e[c] = initEdgeList();
		}
		if (w.u > max_vertex) {
			max_vertex = w.u;
		}
		if (w.v > max_vertex) {
			max_vertex = w.v;
		}
		insertInEdgeList(e[c], w);
	}
	fclose(fin);
	for (i = 0; i <= MAX_COST; i++) {
		if (e[i] != NULL) {
			EdgeListNode *aux = e[i]->start;
			while (aux != NULL) {
				printf("%d %d %d\t index = %d\n", aux->w.u, aux->w.v, i, aux->w.index);
				aux = aux->next;
			}
		}
	}

	int **max_cost = (int **) malloc((max_vertex + 1) * sizeof(int *));
	forest **pdr = (forest **) calloc(max_vertex + 1 , sizeof(forest *));
	for (i = 1; i <= max_vertex; i++) {
		max_cost[i] = (int *) calloc(max_vertex + 1, sizeof(int));
		pdr[i] = initPadure(i);
	}
	solution *sol = (solution *) calloc(3 * max_vertex, sizeof(solution));
	int sc = 0, ss = 0, nec = 0, opt = 0;
	EdgeListNode *aux;
	for (c = 1; c <= MAX_COST; c++) {
		if (e[c] == NULL) {
			continue;
		}
		ss = sc;
		aux = e[c]->start;
		while (aux != NULL) {
			w = aux->w;
			// printf("muchie %d-%d c = %d\n", w.u, w.v, c);
			r = intersectie(pdr[w.u], pdr[w.v], c, w, max_cost);
			if (r == -1) {
				aux = aux->next;
				continue;
			}
			if (r == 0) {
				reuniune(pdr, w, c, max_cost);
				// adauga muchia in solutie
				sol[sc].w = w;
				// sol[sc].status = 1;  // deocamdata o consider necesara
				sc++;
			} else {
				// marcheaza padurea ca fiind ciclica
				pdr[w.u]->opt = 1;
				// adauga muchia in solutie
				sol[sc].w = w;
				// sol[sc].status = 2;  // muchie optionala
				sc++;
			}
			aux = aux->next;
		}
		// nodurile adaugate la cost c adaugate in vector/lista
		for (i = ss; i < sc; i++) {
			if (pdr[sol[i].w.u]->opt) {
				sol[i].status = 2;  // muchie optionala
				opt++;
			} else {
				sol[i].status = 1;  // muchie necesara
				nec++;
			}
			// printf("%d-%d status = %d\n", sol[i].w.u, sol[i].w.v, sol[i].status);
		}
	}

	sortare_pi(sol, 0, ss -1);

	FILE *fout = fopen("scurt.out", "w");
	fprintf(fout, "%d %d\n", nec, opt);
	for (i = 0; i < ss; i++) {
		fprintf(fout, "%d %d\n", sol[i].w.u, sol[i].w.v);
	}
	fclose(fout);
	for (i = 1; i <= MAX_COST; i++) {
		if (e[c] == NULL) {
			destroyEdgeList(e[c]);
		}
	}
	free(e);
	return 0;
}

int intersectie(forest *p1, forest *p2, int c, edge w, int **max_cost) {
	if (p1 != p2) {
		return 0;
	}
	if (p1 == p2 && c > max_cost[w.u][w.v]) {
		return -1;  // muchia nu poate fi folosita
	}
	// p1 == p2 && maxcost == c
	// avem un ciclu care are cel putin 2 muchii de cost c
	return 1;
}

void reuniune(forest **pdr, edge w, int c, int **max_cost) {
	forest *p1 = pdr[w.u];
	forest *p2 = pdr[w.v];
	forest *aux;
	if (p1->size < p2->size) {
		aux = p1;
		p1 = p2;
		p2 = aux;
	}
	// p1 este padurea de dimensiune mai mare
	// copiez nodurile din p2 in p1
	VertexListNode *auxv = p2->v->start;
	VertexListNode *auxv2;
	while (auxv != NULL) {
		insertInVertexList(p1->v, auxv->nod);
		p1->e[auxv->nod] = 1;
		// fiecare nod din p2 trebuie sa pointeze catre p1 acum
		pdr[auxv->nod] = p1;
		p1->size += 1;
		auxv2 = p1->v->start;
		while (auxv2 != NULL) {
			// printf("%d %d \n", auxv->nod, auxv2->nod);
			max_cost[auxv->nod][auxv2->nod] = c;
			max_cost[auxv2->nod][auxv->nod] = c;
			auxv2 = auxv2->next;
		}
		auxv = auxv->next;
	}
	p1->last_edge = w;
	// eliberez memoria alocata pentru p2
	free(p2);
}

void interclasare(solution *v, int i, int m, int j) {
	int iinit = i;
	solution *u = (solution *) calloc(j - i + 1, sizeof(solution));
	int l = 0;
	int k = m + 1;
	while (i <= m && k <= j) {
		if (v[i].status == v[k].status) {
			if (v[i].w.index < v[k].w.index) {
				u[l++] = v[i++];
			} else {
				u[l++] = v[k++];
			}
			continue;
		}
		if (v[i].status < v[k].status) {
			u[l++] = v[i++];
		} else {
			u[l++] = v[k++];
		}
	}
	while (i <= m) {
		u[l++] = v[i++];
	}
	while (k <= j) {
		u[l++] = v[k++];
	}
	l = 0;
	while (iinit <= j) {
		v[iinit++] = u[l++];
	}
	free(u);
}

void sortare_pi(solution *v, int i, int j) {
	if (i < j) {
		int m = (i + j) / 2;
		sortare_pi(v, i, m);
		sortare_pi(v, m + 1, j);
		interclasare(v, i, m, j);
	}
}
