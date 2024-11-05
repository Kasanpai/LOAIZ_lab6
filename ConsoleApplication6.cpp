#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <locale.h>


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

int** delv(int** G, int size, int v) {
    int** Gtemp = generG(size - 1);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i < v && j < v) Gtemp[i][j] = G[i][j];
            if (i > v && j > v) Gtemp[i - 1][j - 1] = G[i][j];
            if (i > v && j < v) Gtemp[i - 1][j] = G[i][j];
            if (i < v && j > v) Gtemp[i][j - 1] = G[i][j];


        }
    }
    for (int i = 0; i < size; i++) {
        free(G[i]);
    }
    free(G);
    G = NULL;
    return Gtemp;
}

int** unionv(int** G, int size, int v1, int v2) {
    for (int i = 0; i < size; i++) {
        if (G[v2][i] == 1) {
            G[v1][i] = G[v2][i];
            G[i][v1] = G[i][v2];
        }
    }
    G = delv(G, size, v2);
    return G;

}




int** contrE(int** G, int size, int v1, int v2) {
    if (G[v1][v2] != 1) {
        printf("нет ребра");
        return G;

    }
    else {
        G[v1][v2] = 0;
        G[v2][v1] = 0;
        for (int i = 0; i < size; i++) {
            if (G[v2][i] == 1) {
                G[v1][i] = G[v2][i];
                G[i][v1] = G[i][v2];
            }
        }
        G = delv(G, size, v2);
        return G;


    }
}

int** splitV(int** G, int size, int v) {
    int** Gtemp = generG(size + 1);

    for (int i = 0; i < size; i++) {
        if (G[v][i] == 1) {
            Gtemp[i][size] = G[i][v];
            Gtemp[size][i] = G[i][v];


        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Gtemp[i][j] = G[i][j];
        }
    }

    Gtemp[v][v] = 0;
    Gtemp[size][size] = 0;
    for (int i = 0; i < size; i++) {
        free(G[i]);
    }

    free(G);
    G = NULL;

    return Gtemp;
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
    int sizemax = (size1 > size2) ? size1 : size2;
    int sizemin = (size1 < size2) ? size1 : size2;

    int** Gmax = (size1 > size2) ? G1 : G2;
    int** Gmin = (size1 < size2) ? G1 : G2;

    int** Gtemp = generG(sizemax - sizemin);

    for (int i = sizemin; i < sizemax; i++) {
        for (int j = sizemin; j < sizemax; j++) {
            Gtemp[i - sizemin][j - sizemin] = Gmax[i][j];
        }
    }
    return Gtemp;

}

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

    /*  int** G3 = unionG(G1, G2, sizeG1, sizeG2);
      int sizeG3 = (sizeG1 > sizeG2) ? sizeG1 : sizeG2;
      printf("\nОбъединение графов\n");

      printG(G3, sizeG3);*/

    int** G4 = intersectoinG(G1, G2, sizeG1, sizeG2);
    int sizeG4 = (sizeG1 < sizeG2) ? sizeG1 : sizeG2;
    printf("\nПересечение графов\n");

    printG(G4, sizeG4);

    ////G2 = delV(G2, g2, 2);
    //nG2--;
    ////printf("\nГраф Godel\n");
    ////print(G2, g2);
    /*G2 = unionv(G2, sizeG2, 2, 3);
    sizeG2--;
    printG(G2, sizeG2);
    G2 = contrE(G2, sizeG2, 2, 3);
    sizeG2--;
    printG(G2, sizeG2);
    return 0;*/
    int** G5 = xorG(G1, G2, sizeG1, sizeG2);
    int sizeG5 = (sizeG1 < sizeG2) ? sizeG2 - sizeG1 : sizeG1 - sizeG2;
    printf("\nКольцевая сумма\n");

    printG(G5, sizeG5);

    return 0;




}