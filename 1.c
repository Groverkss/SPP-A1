#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define MAX_N 5

void *matrices[MAX_N];
int32_t size_x[MAX_N], size_y[MAX_N];

void printMat(int32_t index) {
    printf("%d %d\n", size_x[index], size_y[index]);
    int64_t (*mat)[size_y[index]] = matrices[index];
    for (int i = 0; i < size_x[index]; i++) {
        for (int j = 0; j < size_y[index]; j++) {
            printf("%" PRId64 " ", mat[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int indexA, int indexB) {
    int64_t (*matA)[size_y[indexA]] = matrices[indexA];
    int64_t (*matB)[size_y[indexB]] = matrices[indexB];

    int32_t colA = size_y[indexA];
    int32_t row = size_x[indexA], col = size_y[indexB];

    int64_t (*mat)[col] = malloc(sizeof(int64_t) * row * col);
    memset(mat, 0, sizeof(int64_t) * row * col);

    for (int k = 0; k < colA; k++) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                mat[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }

    for (int i = indexA; i <= indexB; i++) {
        matrices[i] = mat;
        size_x[i] = row;
        size_y[i] = col;
    }
}

int32_t main(void) {
    int32_t n;
    scanf("%d", &n);

    /* Take input */
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &size_x[i], &size_y[i]);

        int64_t (*mat)[size_y[i]] = malloc(sizeof(int64_t) * size_x[i] * size_y[i]);
        for (int j = 0; j < size_x[i]; j++) {
            for (int k = 0; k < size_y[i]; k++) {
                scanf("%" PRId64, &mat[j][k]);
            }
        }
        matrices[i] = mat;
    }

    for (int i = 1; i < n; i++) {
        matrixMultiply(i - 1, i); 
    }

    printMat(0);
}
