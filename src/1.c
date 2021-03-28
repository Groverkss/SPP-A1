#define _POSIX_C_SOURCE 199309L

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define MAX_N 5

struct timespec start, stop;

void* matrices[MAX_N];
int32_t size_x[MAX_N], size_y[MAX_N];

int32_t dpChoose[MAX_N][MAX_N];
int64_t dp[MAX_N][MAX_N];

void clockStart() { clock_gettime(CLOCK_REALTIME, &start); }

void clockEnd() {
    clock_gettime(CLOCK_REALTIME, &stop);
    double time_taken =
        (double)(stop.tv_sec - start.tv_sec) +
        ((double)(stop.tv_nsec - start.tv_nsec) / (double)(1e9));
    fprintf(stderr, "time taken %0.10lf seconds\n", time_taken);
}

void calculateDp(int32_t n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dp[i][j] = 0;
            dpChoose[i][j] = -1;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = i; k <= j - 1; k++) {
                int64_t newVal = dp[i][k] + dp[k + 1][j] +
                                 (size_x[i] * size_y[k] * size_y[j]);
                if (dp[i][j] == 0 || newVal < dp[i][j]) {
                    dpChoose[i][j] = k;
                    dp[i][j] = newVal;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%" PRId64 " ", dp[i][j]);
        }
        printf("\n");
    }
}

void asmMul(void* matIn, void* matAIn, void* matBIn, int32_t colA, int32_t colB,
            int32_t col, int32_t row);

void asmMul(void* matIn, void* matAIn, void* matBIn, int32_t colA, int32_t colB,
            int32_t col, int32_t row) {
    register int64_t* matK = matBIn;
    register int64_t* matI = matAIn;

    /* K loop */
    for (int k = 0; k < colA; k++) {
        register int64_t* matIJ = matIn;
        register int64_t* matIK = matI;

        /* I loop */
        for (int i = 0; i < row; i++) {
            register int64_t* matKJ = matK;

            int64_t* endMatKJ = matKJ + col;

            /* J loop */
            while (endMatKJ != matKJ) {
                (*matIJ) += (*matIK) * (*matKJ);
                matIJ++;
                matKJ++;
            }
            matIK += colA;
        }
        matK += colB;
        matI++;
    }
}

void matrixMultiply(int indexA, int indexB) {
    int32_t colA = size_y[indexA], colB = size_y[indexB];
    int32_t row = size_x[indexA], col = size_y[indexB];

    int64_t(*mat)[col] = malloc(sizeof(int64_t) * row * col);
    memset(mat, 0, sizeof(int64_t) * row * col);

    int64_t(*matA)[colA] = matrices[indexA];
    int64_t(*matB)[colB] = matrices[indexB];

    asmMul(mat, matA, matB, colA, colB, col, row);

    for (int i = indexA; i <= indexB; i++) {
        matrices[i] = mat;
        size_x[i] = row;
        size_y[i] = col;
    }
}

void printMat(int32_t index) {
    printf("%d %d\n", size_x[index], size_y[index]);
    int64_t(*mat)[size_y[index]] = matrices[index];
    for (int i = 0; i < size_x[index]; i++) {
        for (int j = 0; j < size_y[index]; j++) {
            printf("%" PRId64 " ", mat[i][j]);
        }
        printf("\n");
    }
}

int32_t main(void) {
    int32_t n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d %d", &size_x[i], &size_y[i]);

        int64_t(*mat)[size_y[i]] =
            malloc(sizeof(int64_t) * size_x[i] * size_y[i]);
        for (int j = 0; j < size_x[i]; j++) {
            for (int k = 0; k < size_y[i]; k++) {
                scanf("%" PRId64, &mat[j][k]);
            }
        }
        matrices[i] = mat;
    }

    /* calculateDp(n); */

    /* clockStart(); */
    for (int i = 1; i < n; i++) {
        matrixMultiply(i - 1, i);
    }

    /* clockEnd(); */

    printMat(n - 1);
}
