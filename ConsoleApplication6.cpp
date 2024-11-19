#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <locale.h>

// Структура для элемента списка смежности
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

// Функция для добавления вершины в список смежности
void addEdge(Node** adjacencyList, int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = *adjacencyList;
    *adjacencyList = newNode;
}

// Функция для генерации графа в виде матрицы смежности
int** generG(int size) {
    int** G = NULL;
    G = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        G[i] = (int*)malloc(size * sizeof(int));
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            G[i][j] = 0;
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            int edge = rand() % 2;
            G[i][j] = edge;
            G[j][i] = edge; // Для неориентированного графа
        }
    }
    return G;
}

// Функция для вывода матрицы смежности
int** printG(int** G, int size) {
    printf("Матрица смежности для неориентированного графа:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", G[i][j]);
        }
        printf("\n");
    }
    return G;
}

// Функция для создания списка смежности из матрицы смежности
Node** createAdjList(int** G, int size) {
    Node** adjList = (Node**)malloc(size * sizeof(Node*));

    // Инициализация всех списков смежности как пустых
    for (int i = 0; i < size; i++) {
        adjList[i] = NULL;
    }

    // Переводим матрицу смежности в список смежности
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (G[i][j] == 1) {
                addEdge(&adjList[i], j);  // Добавляем вершину j в список вершины i
            }
        }
    }

    return adjList;
}
// Функция для сортировки списка смежности по возрастанию
void sortAdjList(Node* list) {
    // Проверка на пустой список
    if (list == NULL || list->next == NULL) {
        return; // Если список пуст или состоит из одного элемента, то сортировка не требуется
    }

    // Алгоритм сортировки "пузырьком"
    for (Node* i = list; i != NULL; i = i->next) {
        for (Node* j = i->next; j != NULL; j = j->next) {
            if (i->vertex > j->vertex) {
                // Меняем значения вершин местами
                int temp = i->vertex;
                i->vertex = j->vertex;
                j->vertex = temp;
            }
        }
    }
}



// Функция для вывода списка смежности
void printAdjList(Node** adjList, int size) {
    printf("Список смежности:\n");
    for (int i = 0; i < size; i++) {
        printf("%d: ", i);
        Node* temp = adjList[i];

        // Сортируем список смежности для вершины i
        sortAdjList(temp);

        while (temp != NULL) {
            printf("%d ", temp->vertex);
            temp = temp->next;
        }
        printf("\n");
    }
}



int** unionG(int** G1, int** G2, int size1, int size2) {
    int sizemax = (size1 > size2) ? size1 : size2;
    int sizemin = (size1 < size2) ? size1 : size2;

    int** Gmax = (size1 > size2) ? G1 : G2;
    int** Gmin = (size1 < size2) ? G1 : G2;

    int** Gtemp = generG(sizemax);

    for (int i = 0; i < sizemin; i++) {
        for (int j = 0; j < sizemin; j++) {
            Gtemp[i][j] = Gmin[i][j] | Gmax[i][j];
        }
    }
    for (int i = 0; i < sizemax; i++) {
        for (int j = sizemin; j < sizemax; j++) {
            Gtemp[i][j] = Gmax[i][j];
            Gtemp[j][i] = Gtemp[i][j];
        }
    }

    return Gtemp;

}

int** intersectoinG(int** G1, int** G2, int size1, int size2) {
    int sizemin = (size1 < size2) ? size1 : size2;

    int** Gtemp = generG(sizemin);

    for (int i = 0; i < sizemin; i++) {
        for (int j = 0; j < sizemin; j++) {
            Gtemp[i][j] = G1[i][j] & G2[i][j];
        }
    }
    return Gtemp;
}

int** xorG(int** G1, int** G2, int size1, int size2) {
    int size = (size1 > size2) ? size1 : size2;

    // Создаём результирующую матрицу и заполняем её нулями
    int** Gxor = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        Gxor[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            int val1 = (i < size1 && j < size1) ? G1[i][j] : 0;
            int val2 = (i < size2 && j < size2) ? G2[i][j] : 0;
            Gxor[i][j] = val1 ^ val2; // Применение XOR
        }
    }
    return Gxor;
}


// Функция для добавления вершины в список смежности
void addEdgeToAdjList(Node** adjacencyList, int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = *adjacencyList;
    *adjacencyList = newNode;
}

// Функция для объединения двух списков смежности
Node** unionAdjLists(Node** adjList1, Node** adjList2, int size1, int size2) {
    int size = (size1 > size2) ? size1 : size2;
    Node** result = (Node**)malloc(size * sizeof(Node*));

    // Инициализация всех списков смежности результата
    for (int i = 0; i < size; i++) {
        result[i] = NULL;
    }

    // Процесс объединения
    for (int i = 0; i < size1; i++) {
        Node* temp1 = adjList1[i];
        while (temp1 != NULL) {
            addEdge(&result[i], temp1->vertex);
            temp1 = temp1->next;
        }
    }

    for (int i = 0; i < size2; i++) {
        Node* temp2 = adjList2[i];
        while (temp2 != NULL) {
            // Добавляем только уникальные вершины
            bool found = false;
            Node* tempResult = result[i];
            while (tempResult != NULL) {
                if (tempResult->vertex == temp2->vertex) {
                    found = true;
                    break;
                }
                tempResult = tempResult->next;
            }
            if (!found) {
                addEdge(&result[i], temp2->vertex);
            }
            temp2 = temp2->next;
        }
    }

    return result;
}

// Функция для пересечения двух списков смежности
Node** intersectionAdjLists(Node** adjList1, Node** adjList2, int size1, int size2) {
    int size = (size1 < size2) ? size1 : size2;
    Node** result = (Node**)malloc(size * sizeof(Node*));

    for (int i = 0; i < size; i++) {
        result[i] = NULL;
    }

    for (int i = 0; i < size; i++) {
        Node* temp1 = (size1 < size2) ? adjList2[i] : adjList1[i];
        while (temp1 != NULL) {
            Node* temp2 = (size1 < size2) ? adjList1[i] : adjList2[i];
            while (temp2 != NULL) {
                if (temp1->vertex == temp2->vertex) {
                    addEdge(&result[i], temp1->vertex);
                    break;
                }
                temp2 = temp2->next;
            }
            temp1 = temp1->next;
        }
    }

    return result;
}


Node* createNode(int vertex) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

// Функция для добавления элемента в конец списка
void appendNode(Node** head, int vertex) {
    Node* newNode = createNode(vertex);
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Функция для проверки, содержит ли список вершину
bool contains(Node* list, int vertex) {
    Node* temp = list;
    while (temp != NULL) {
        if (temp->vertex == vertex) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// Функция для XOR двух списков смежности
Node* xorAdjacencyLists(Node* list1, Node* list2) {
    Node* result = NULL;

    // Добавляем элементы из первого списка, которых нет во втором
    Node* temp = list1;
    while (temp != NULL) {
        if (!contains(list2, temp->vertex)) {
            appendNode(&result, temp->vertex);
        }
        temp = temp->next;
    }

    // Добавляем элементы из второго списка, которых нет в первом
    temp = list2;
    while (temp != NULL) {
        if (!contains(list1, temp->vertex)) {
            appendNode(&result, temp->vertex);
        }
        temp = temp->next;
    }

    return result;
}

// Функция для вычисления кольцевой суммы двух списков смежности
Node** ring(Node** adjList1, Node** adjList2, int size1, int size2) {
    int maxSize = (size1 > size2) ? size1 : size2;
    Node** result = (Node**)malloc(maxSize * sizeof(Node*));

    for (int i = 0; i < maxSize; i++) {
        Node* list1 = (i < size1) ? adjList1[i] : NULL;
        Node* list2 = (i < size2) ? adjList2[i] : NULL;
        result[i] = xorAdjacencyLists(list1, list2);
    }

    return result;
}




// Основная функция
int main(void) {
    int sizeG1 = 3, sizeG2 = 5;
    setlocale(LC_ALL, "");

    printf("Введите количество вершин графа 1: ");
    scanf("%d", &sizeG1);
    printf("Введите количество вершин графа 2: ");
    scanf("%d", &sizeG2);

    int** G1 = generG(sizeG1);
    int** G2 = generG(sizeG2);

    printf("граф 1 ");
    printG(G1, sizeG1);
    printf("граф 2 ");
    printG(G2, sizeG2);


    int** G3 = unionG(G1, G2, sizeG1, sizeG2);
    int sizeG3 = (sizeG1 > sizeG2) ? sizeG1 : sizeG2;
    printf("\nОбъединение графов\n");
    printG(G3, sizeG3);

    int** G4 = intersectoinG(G1, G2, sizeG1, sizeG2);
    int sizeG4 = (sizeG1 < sizeG2) ? sizeG1 : sizeG2;
    printf("\nПересечение графов\n");
    printG(G4, sizeG4);

    int** Gxor = xorG(G1, G2, sizeG1, sizeG2);
    printf("\nКольцевая сумма графов (матрица смежности):\n");
    printG(Gxor, (sizeG1 > sizeG2) ? sizeG1 : sizeG2);

    // Генерация и вывод списка смежности для графа 1
    Node** adjList1 = createAdjList(G1, sizeG1);
    printAdjList(adjList1, sizeG1);

    // Генерация и вывод списка смежности для графа 2
    Node** adjList2 = createAdjList(G2, sizeG2);
    printAdjList(adjList2, sizeG2);

    printf("\nОбъединение списков смежности:\n");
    Node** unionList = unionAdjLists(adjList1, adjList2, sizeG1, sizeG2);
    printAdjList(unionList, sizeG1);

    printf("\nПересечение списков смежности:\n");
    Node** intersectionList = intersectionAdjLists(adjList1, adjList2, sizeG1, sizeG2);
    int sizeG6 = (sizeG1 < sizeG2) ? sizeG1 : sizeG2;
    printAdjList(intersectionList, sizeG6);

    printf("\nКольцевая сумма списков смежности:\n");
    Node** xorList = ring(adjList1, adjList2, sizeG1, sizeG2);
    printAdjList(xorList, sizeG1);

    return 0;
}
