#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
	int elem;
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

void insertInList(List *l, int elem) {  // insereaza un nod in lista
	ListNode *node = (ListNode *) malloc(sizeof(ListNode));
	node->elem = elem;
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

void DFS_conex(List **la, List *q, int *v, int current);
int aciclic(List *q, List **la, int *v);
int DFS_ciclu(List **la, int *v, int current);

int main() {
	int ans, i, n, m, u , v;

	// citeste N si M
	FILE *fin = fopen("holograme.in", "r");  // fisier input
	fscanf(fin, "%d %d\n", &n, &m);

	// initializari
	ans = n;  // variabila raspuns initilizata cu N (nr maxim de holograme)
	int *viz = (int *) calloc(n + 1, sizeof(int));  // vector de vizite
	List **la = (List **) malloc((n + 1) * sizeof(List *));
	// ^ lista de adiacenta pentru graful orientat
	List **la_no = (List **) malloc((n + 1) * sizeof(List *));
	// ^ lista de adiacenta pentru graful neorientat
	for (i = 1; i <= n; i++) {
		la[i] = initList();
		la_no[i] = initList();
	}

	// citire perechi
	for (i = 0; i < m; i++) {
		fscanf(fin, "%d %d\n", &u, &v);

		// construieste lista de adiacente pentru graful orientat
		insertInList(la[u], v);

		// construieste lista de adiacente pentru graful neorientat
		insertInList(la_no[u], v);
		insertInList(la_no[v], u);
	}
	fclose(fin);

	// gaseste componentele conexe si introduce nodurile sale in lista q
	// considerand graful ca fiind neorientat
	for (i = 1; i <= n; i++) {
		if (viz[i] == 0) {
			List *q = initList();
			DFS_conex(la_no, q, viz, i);
			// pentru componenta conexa gasita,verifica daca este aciclica
			// considerand graful orientat de data aceasta
			if (aciclic(q, la, viz)) {
				// daca nu are ciclu, atunci scad 1 din numarul minim de muchii
				ans--;
			}
			destroyList(q);
		}
	}

	// scrie rezultat
	FILE *fout = fopen("holograme.out", "w");  // fisier output
	fprintf(fout, "%d\n", ans);
	fclose(fout);

	// elibereaza memoria
	for (i = 1; i <= n; i++) {
		destroyList(la[i]);
		destroyList(la_no[i]);
	}
	free(la);
	free(la_no);
	free(viz);
	return 0;
}

void DFS_conex(List **la, List *q, int *v, int current) {
	// construieste in q o componenta conexa prin DFS pe graful neorientat
	int succesor;
	v[current] = 1;
	insertInList(q, current);  // adauga in componenta conexa q, nodul curent
	ListNode *aux = la[current]->start;
	while (aux != NULL) {
		succesor = aux->elem;
		if (v[succesor] == 0) {
			// daca am gasit un nod nevizitat, viziteaza
			DFS_conex(la, q, v, succesor);
		}
		aux = aux->next;
	}
}

int aciclic(List *q, List **la, int *v) {
	// verifica daca o componenta conexa contine un ciclu
	ListNode *aux = q->start;
	while (aux != NULL) {
		if (v[aux->elem] == 1) {
			// daca nodul nu a fost vizitat, aplica DFS pentru a gasi ciclu
			if (DFS_ciclu(la, v, aux->elem)) {
				// daca am gasit un ciclu, returneaza 0
				return 0;
			}
		}
		aux = aux->next;
	}
	// nu am gasit niciun ciclu
	return 1;
}

int DFS_ciclu(List **la, int *v, int current) {
	// cauta daca exista un ciclu
	int succesor;
	v[current] = 2;  // nod gri
	ListNode *aux = la[current]->start;
	while (aux != NULL) {
		succesor = aux->elem;
		if (v[succesor] == 2) {  // daca am gasit un nod gri, atunci avem ciclu
			return 1;
		}
		if (DFS_ciclu(la, v, succesor)) {
			// daca functia recursiva a descoperit un ciclu, returneza 1
			return 1;
		}
		aux = aux->next;
	}
	v[current] = 3;  // nod negru
	return 0;
}
