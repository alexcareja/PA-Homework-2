#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COST 1000
#define MAX_VERTEXES 5000

typedef struct edge {
	int u;
	int v;
	int index;
	int status;
} edge;  // tip de date muchie

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

typedef struct ListNode {
	int elem;
	edge *w;
	int cost;
	struct ListNode *next;
} ListNode;  // tip de date nod de lista

typedef struct List {
	ListNode *start;
	ListNode *end;
} List;  // tip de date lista

List *initList() {  // initializeaza o lista goala
	List *l = (List *) malloc(sizeof(List));
	l->start = NULL;
	l->end = NULL;
	return l;
}

void insertInList(List *l, int elem, edge *w, int cost) {
	// insereaza un nod in lista
	ListNode *node = (ListNode *) malloc(sizeof(ListNode));
	node->elem = elem;
	node->w = w;
	node->cost = cost;
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

void deleteFirst(List *l) {  // sterge primul nod din lista
	if (l->start == NULL) {
		return;
	}
	ListNode *aux = l->start;
	l->start = aux->next;
	if (l->start == NULL) {  // daca am eliminat ultimul element din lista
		l->end = NULL;
	}
	free(aux);
}

void destroyList(List *l) {  // distruge lista, elibereaza memoria
	while (l->start != NULL) {
		deleteFirst(l);
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
	return p;
}

int intersectie(forest *p1, forest *p2, int c, edge w, int **max_cost);
void reuniune(forest **pdr, edge w, int c, int **max_cost);
void interclasare(edge **v, int i, int m, int j);
void sortare_pi(edge **v, int i, int j);
void DFS_nebun(List **la, int src, int dst, int c);
int explorare_nebuna(List **la, int *v, int s, int dst, int c);

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
	// for (i = 0; i <= MAX_COST; i++) {
	// 	if (e[i] != NULL) {
	// 		EdgeListNode *aux = e[i]->start;
	// 		while (aux != NULL) {
	// 			printf("%d %d %d\n", aux->w.u, aux->w.v, i);
	// 			aux = aux->next;
	// 		}
	// 	}
	// }
	List **la = (List **) malloc((max_vertex + 1) * sizeof(List *));
	int **max_cost = (int **) malloc((max_vertex + 1) * sizeof(int *));
	forest **pdr = (forest **) calloc(max_vertex + 1 , sizeof(forest *));
	for (i = 1; i <= max_vertex; i++) {
		la[i] = initList();
		max_cost[i] = (int *) calloc(max_vertex + 1, sizeof(int));
		pdr[i] = initPadure(i);
	}
	edge **sol = (edge **) calloc(3 * max_vertex, sizeof(edge *));
	int sc = 0, nec = 0, opt = 0;
	EdgeListNode *aux;
	for (c = 1; c <= MAX_COST; c++) {
		if (e[c] == NULL) {
			continue;
		}
		aux = e[c]->start;
		while (aux != NULL) {
			w = aux->w;
			r = intersectie(pdr[w.u], pdr[w.v], c, w, max_cost);
			if (r == -1) {
				aux = aux->next;
				continue;
			}
			if (r == 0) {
				reuniune(pdr, w, c, max_cost);
				insertInList(la[w.u], w.v, &aux->w, c);
				insertInList(la[w.v], w.u, &aux->w, c);
				// adauga muchia in solutie
				sol[sc] = &aux->w;
				sol[sc]->status = 1;  // deocamdata o consider muchie necesara
				sc++;
			} else {  // muchia este optionala => caut celelalte muchii opt.
				// adauga muchia in solutie
				sol[sc] = &aux->w;
				sol[sc]->status = 2;  // muchie optionala
				sc++;
				// cauta celelalte muchii opt. de cost egal din graful ciclic
				DFS_nebun(la, w.u, w.v, c);
				insertInList(la[w.u], w.v, &aux->w, c);
				insertInList(la[w.v], w.u, &aux->w, c);
			}
			aux = aux->next;
		}
	}
	for (int i = 0; i < sc; i++) {
		if (sol[i]->status == 1) {
			nec++;
		} else {
			opt++;
		}
	}
	sortare_pi(sol, 0, sc -1);

	FILE *fout = fopen("scurt.out", "w");
	fprintf(fout, "%d %d\n", nec, opt);
	for (i = 0; i < sc; i++) {
		fprintf(fout, "%d %d\n", sol[i]->u, sol[i]->v);
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

void DFS_nebun(List **la, int src, int dst, int c) {
	int *v = (int *) calloc(MAX_VERTEXES + 1, sizeof(int));
	explorare_nebuna(la, v, src, dst, c);
	free(v);
}

int explorare_nebuna(List **la, int *v, int s, int dst, int c) {
	if (s == dst) {
		return 1;
	}
	int r, succesor;
	v[s] = 1;
	ListNode *aux = la[s]->start;
	while (aux != NULL) {
		succesor = aux->elem;
		if (v[succesor] == 0) {
			r = explorare_nebuna(la, v, succesor, dst, c);
			if (r) {
				if (aux->cost == c) {
					// muchie optionala
					aux->w->status = 2;
				}
				return 1;
			}
		}
		aux = aux->next;
	}
	return 0;
}

int intersectie(forest *p1, forest *p2, int c, edge w, int **max_cost) {
	if (p1 != p2) {
		return 0;
	}
	if (p1 == p2 && c > max_cost[w.u][w.v]) {
		return -1;  // muchia nu poate fi folosita
	}
	// p1 == p2 && maxcost[w.u][w.v] == c
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
	int p1_size = p1->size;
	int i;
	VertexListNode *auxv = p2->v->start;
	VertexListNode *auxv2;
	while (auxv != NULL) {
		insertInVertexList(p1->v, auxv->nod);
		p1->e[auxv->nod] = 1;
		// fiecare nod din p2 trebuie sa pointeze catre p1 acum
		pdr[auxv->nod] = p1;
		p1->size += 1;
		auxv2 = p1->v->start;
		for (i = 0; i < p1_size && auxv2 != NULL; i++, auxv2 = auxv2->next) {
			max_cost[auxv->nod][auxv2->nod] = c;
			max_cost[auxv2->nod][auxv->nod] = c;
		}
		auxv = auxv->next;
	}
	p1->last_edge = w;
	// eliberez memoria alocata pentru p2
	free(p2);
}

void interclasare(edge **v, int i, int m, int j) {
	int iinit = i;
	edge **u = (edge **) calloc(j - i + 1, sizeof(edge *));
	int l = 0;
	int k = m + 1;
	while (i <= m && k <= j) {
		if (v[i]->status == v[k]->status) {
			if (v[i]->index < v[k]->index) {
				u[l++] = v[i++];
			} else {
				u[l++] = v[k++];
			}
			continue;
		}
		if (v[i]->status < v[k]->status) {
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

void sortare_pi(edge **v, int i, int j) {
	if (i < j) {
		int m = (i + j) / 2;
		sortare_pi(v, i, m);
		sortare_pi(v, m + 1, j);
		interclasare(v, i, m, j);
	}
}
