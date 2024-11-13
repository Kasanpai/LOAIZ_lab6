#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "cstdlib"
#include <windows.h>


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

int** decUG(int** G1, int size1, int** G2, int size2, int** G3) {
    for (int i = 0; i < size1 * size2; i++) {
        for (int j = 0; j < size1 * size2; j++) {
            G3[i][j] = 0;
        }
    }

    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            for (int k = 0; k < size1; k++) {
                for (int l = 0; l < size2; l++) {
                    if (G1[i][k] == 1 && j == l) {
                        G3[i * size2 + j][k * size2 + l] = 1;
                    }
                    if (G2[j][l] == 1 && i == k) {
                        G3[i * size2 + j][k * size2 + l] = 1;
                    }
                }
            }
        }
    }

    return G3;
}

int main(void) {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int sizeG1 = 0, sizeG2 = 0, sizeG3 = 0;
    int** G1 = NULL;
    int** G2 = NULL;
    int** G3 = NULL;

    srand(1235235235);

    printf("Введите количество вершин графа 1: ");
    scanf("%d", &sizeG1);
    printf("Введите количество вершин графа 2: ");
    scanf("%d", &sizeG2);

    G1 = createG(sizeG1);

    printf("1 граф\n");
    printG(G1, sizeG1);

    G2 = createG(sizeG2);

    printf("2 граф\n");
    printG(G2, sizeG2);

    G3 = createG(sizeG1 * sizeG2);
    G3 = decUG(G1, sizeG1, G2, sizeG2, G3);
    printf("Матрица смежности декартова произведения графов:\n");
    printG(G3, sizeG1 * sizeG2);

}