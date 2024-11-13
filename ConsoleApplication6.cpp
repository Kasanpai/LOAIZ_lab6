

#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "stdlib.h"
#include "cstdlib"
#include <windows.h>
#include <locale.h>

struct node {
	int vertex;
	struct node* next;
};

struct Graph {
	int numVertices;
	struct node** adjLists;
};

struct node* createNode(int v) {
	struct node* newNode = (node*)malloc(sizeof(struct node));
	newNode->vertex = v;
	newNode->next = NULL;
	return newNode;
}

struct Graph* createAGraph(int vertices) {
	struct Graph* graph = (Graph*)malloc(sizeof(struct Graph));
	graph->numVertices = vertices;

	graph->adjLists = (node**)malloc(vertices * sizeof(struct node*));

	for (int i = 0; i < vertices; i++) {
		graph->adjLists[i] = NULL;
	}

	return graph;
}


void addEdge(struct Graph* graph, int s, int d) {
	struct node* temp = graph->adjLists[s];

	// Проверяем, есть ли уже такое ребро
	while (temp) {
		if (temp->vertex == d) {
			return;
		}
		temp = temp->next;
	}

	struct node* newNode = createNode(d);

	// Если список пустой или новая вершина меньше первой вершины
	if (graph->adjLists[s] == NULL || graph->adjLists[s]->vertex > d) {
		newNode->next = graph->adjLists[s];
		graph->adjLists[s] = newNode;
	}
	else {
		// Иначе вставляем в нужное место
		temp = graph->adjLists[s];
		while (temp->next && temp->next->vertex < d) {
			temp = temp->next;
		}
		newNode->next = temp->next;
		temp->next = newNode;
	}

	// Повторяем аналогичную операцию для вершины d
	temp = graph->adjLists[d];
	while (temp) {
		if (temp->vertex == s) {
			return;
		}
		temp = temp->next;
	}

	newNode = createNode(s);
	if (graph->adjLists[d] == NULL || graph->adjLists[d]->vertex > s) {
		newNode->next = graph->adjLists[d];
		graph->adjLists[d] = newNode;
	}
	else {
		temp = graph->adjLists[d];
		while (temp->next && temp->next->vertex < s) {
			temp = temp->next;
		}
		newNode->next = temp->next;
		temp->next = newNode;
	}
}

struct Graph* edges(int** G, int size) {
	struct Graph* graph = createAGraph(size);

	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			if (G[i][j] == 1) {
				addEdge(graph, i, j);
			}
		}
	}
	return graph;
}

void printGraph(struct Graph* graph) {
	for (int v = 0; v < graph->numVertices; v++) {
		struct node* temp = graph->adjLists[v];
		printf("\nVertex %d: ", v + 1);
		while (temp) {
			printf("%d -> ", temp->vertex + 1);
			temp = temp->next;
		}
		printf("\n");
	}
	printf("\n");
}



int** createG(int size) {
	int** G = NULL;
	G = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		G[i] = (int*)malloc(size * sizeof(int));
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			G[i][j] = rand() % 2;
			if (i == j) G[i][j] = 0;
			G[j][i] = G[i][j];
		}
	}
	return G;
}

void printG(int** G, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%d ", G[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return;
}


void sortList(struct node* head) {
	if (!head) return;

	int swapped;
	struct node* ptr1;
	struct node* lptr = NULL;

	do {
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr) {
			if (ptr1->vertex > ptr1->next->vertex) {
				int temp = ptr1->vertex;
				ptr1->vertex = ptr1->next->vertex;
				ptr1->next->vertex = temp;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}



void unionV(struct Graph* graph, int v1, int v2) {
	if (v1 == v2) return;

	struct node* temp = graph->adjLists[v2];
	while (temp) {
		addEdge(graph, v1, temp->vertex);
		temp = temp->next;
	}

	for (int i = 0; i < graph->numVertices; i++) {
		struct node* adj = graph->adjLists[i];
		struct node* p = NULL;

		while (adj) {
			if (adj->vertex == v2) {
				if (p) {
					p->next = adj->next;
				}
				else {
					graph->adjLists[i] = adj->next;
				}
				struct node* del = adj;
				adj = adj->next;
				free(del);
			}
			else {
				p = adj;
				adj = adj->next;
			}
		}
	}

	graph->adjLists[v2] = NULL;

	for (int i = v2 + 1; i < graph->numVertices; i++) {
		graph->adjLists[i - 1] = graph->adjLists[i];
	}
	graph->adjLists[graph->numVertices - 1] = NULL;

	graph->numVertices--;

	for (int i = 0; i < graph->numVertices; i++) {
		struct node* adj = graph->adjLists[i];
		while (adj) {
			if (adj->vertex > v2) {
				adj->vertex--;
			}
			adj = adj->next;
		}
	}
}

void styag(struct Graph* graph, int v1, int v2) {
	if (v1 == v2) return;

	int flag = 0;
	struct node* temp = graph->adjLists[v1];
	while (temp) {
		if (temp->vertex == v2) {
			flag = 1;
		}
		temp = temp->next;
	}

	if (flag) {
		struct node* adj = graph->adjLists[v1];
		struct node* p = NULL;

		while (adj) {
			if (adj->vertex == v2) {
				if (p) {
					p->next = adj->next;
				}
				else {
					graph->adjLists[v1] = adj->next;
				}
				struct node* del = adj;
				adj = adj->next;
				free(del);
				break;
			}
			else {
				p = adj;
				adj = adj->next;
			}
		}

		adj = graph->adjLists[v2];
		p = NULL;

		while (adj) {
			if (adj->vertex == v1) {
				if (p) {
					p->next = adj->next;
				}
				else {
					graph->adjLists[v2] = adj->next;
				}
				struct node* del = adj;
				adj = adj->next;
				free(del);
				break;
			}
			else {
				p = adj;
				adj = adj->next;
			}
		}

		struct node* temp = graph->adjLists[v2];
		while (temp) {
			addEdge(graph, v1, temp->vertex);
			temp = temp->next;
		}

		for (int i = 0; i < graph->numVertices; i++) {
			adj = graph->adjLists[i];
			p = NULL;

			while (adj) {
				if (adj->vertex == v2) {
					if (p) {
						p->next = adj->next;
					}
					else {
						graph->adjLists[i] = adj->next;
					}
					struct node* del = adj;
					adj = adj->next;
					free(del);
				}
				else {
					p = adj;
					adj = adj->next;
				}
			}
		}

		graph->adjLists[v2] = NULL;

		for (int i = v2 + 1; i < graph->numVertices; i++) {
			graph->adjLists[i - 1] = graph->adjLists[i];
		}
		graph->adjLists[graph->numVertices - 1] = NULL;

		graph->numVertices--;

		for (int i = 0; i < graph->numVertices; i++) {
			struct node* adj = graph->adjLists[i];
			while (adj) {
				if (adj->vertex > v2) {
					adj->vertex--;
				}
				adj = adj->next;
			}
		}
	}
	else { return; }
}


void splitV(struct Graph* graph, int v) {
	if (v < 0 || v >= graph->numVertices) return;

	int newV1 = graph->numVertices;
	int newV2 = graph->numVertices + 1;

	graph->numVertices += 1;
	graph->adjLists = (struct node**)realloc(graph->adjLists, graph->numVertices * sizeof(struct node*));
	graph->adjLists[newV1] = NULL;

	struct node* temp = graph->adjLists[v];
	while (temp) {
		if (temp->vertex != v) {
			addEdge(graph, newV1, temp->vertex);
		}
		temp = temp->next;
	}
}

int main(void) {
	setlocale(LC_ALL, "");
	int sizeG1 = 0, sizeG2 = 0, sizeG3 = 0;
	int** G1 = NULL;
	int** G2 = NULL;
	int** G3 = NULL;


	printf("Введите количество вершин 1 графа: ");
	scanf("%d", &sizeG1);
	G1 = createG(sizeG1);
	printf("1 граф\n");
	printG(G1, sizeG1);
	struct Graph* graph1 = edges(G1, sizeG1);
	printGraph(graph1);

	int v1, v2;
	printf("Введите вершины для объединения в графе 1: ");
	scanf("%d %d", &v1, &v2);
	unionV(graph1, v1 - 1, v2 - 1);
	printf("Граф после объединения:\n");
	printGraph(graph1);


	printf("Введите количество вершин 2 графа: ");
	scanf("%d", &sizeG2);
	G2 = createG(sizeG2);
	printf("2 граф\n");
	printG(G2, sizeG2);
	struct Graph* graph2 = edges(G2, sizeG2);
	printGraph(graph2);

	int vert;
	printf("Введите вершину для расщепления в графе 2: ");
	scanf("%d", &vert);
	splitV(graph2, vert - 1);
	printf("Граф после расщепления:\n");
	printGraph(graph2);


	printf("Введите количество вершин 3 графа: ");
	scanf("%d", &sizeG3);
	G3 = createG(sizeG3);
	printf("1 граф\n");
	printG(G3, sizeG3);
	struct Graph* graph3 = edges(G3, sizeG3);
	printGraph(graph3);

	int v3, v4;
	printf("Введите вершины для стягивания в графе 3: ");
	scanf("%d %d", &v3, &v4);
	styag(graph3, v3 - 1, v4 - 1);
	printf("Граф после стягивания:\n");
	printGraph(graph3);
}