#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

#define MAX_N 5

void *matrices[MAX_N];
int32_t size_x[MAX_N], size_y[MAX_N];

int32_t main(void) {
    int32_t n;
    scanf("%d", &n);

    /* Take input */
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &size_x[i], &size_y[i]);

        int32_t (*mat)[size_y[i]] = malloc(sizeof(int32_t*) * size_x[i] * size_y[i]);
        for (int j = 0; j < size_x[i]; j++) {
            for (int k = 0; k < size_y[i]; k++) {
                scanf("%d", &mat[j][k]);
            }
        }

        matrices[i] = mat;
    }

    for (int i = 0; i < n; i++) {
        printf("Matrices: %d %d\n", size_x[i], size_y[i]);

        int32_t (*mat)[size_y[i]] = matrices[i];
        for (int j = 0; j < size_x[i]; j++) {
            for (int k = 0; k < size_y[i]; k++) {
                printf("%d ", mat[j][k]);
            }
            printf("\n");
        }
    }
}
