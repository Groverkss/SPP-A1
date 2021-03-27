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

/* void asmMul(void* matIn, void* matAIn, void* matBIn, int32_t colA, int32_t */
/* colB, */
/*             int32_t col, int32_t row) { */
/*     int64_t(*mat)[col] = matIn; */
/*     int64_t(*matA)[colA] = matAIn; */
/*     int64_t(*matB)[colB] = matBIn; */

/*     for (int k = 0; k < colA; k++) { */
/*         for (int i = 0; i < row; i++) { */
/*             for (int j = 0; j < col; j++) { */
/*                 mat[i][j] += matA[i][k] * matB[k][j]; */
/*             } */
/*         } */
/*     } */
/* } */

__asm__(
    "asmMul:\n.S:\n pushq %r12\n testl %ecx, %ecx\n pushq %rbp\n pushq %rbx\n "
    "movl "
    "32(%rsp), %ebx\n jle .X1\n leal -1(%rcx), %eax\n movq %rdi, %rbp\n movslq "
    "%r9d, %rdi\n salq $3, %rdi\n movq %rsi, %r8\n movq %rdx, %r10\n leaq "
    "8(%rsi,%rax,8), %r12\n.X2:\n testl %ebx, %ebx\n jle .X2E\n xorl %esi, "
    "%esi\n xorl %r11d, %r11d\n.X3:\n testl %r9d, %r9d\n jle .X1E\n leaq "
    "0(%rbp,%rsi), %rcx\n xorl %eax, %eax\n.Mul:\n movq (%r8,%rsi), %rdx\n "
    "imulq (%r10,%rax,8), %rdx\n addq %rdx, (%rcx,%rax,8)\n addq $1, %rax\n "
    "cmpl %eax, %r9d\n jg .Mul\n.X1E:\n addl $1, %r11d\n addq %rdi, %rsi\n "
    "cmpl %ebx, %r11d\n jne .X3\n.X2E:\n addq $8, %r8\n addq %rdi, %r10\n cmpq "
    "%r12, %r8\n jne .X2\n.X1:\n popq %rbx\n popq %rbp\n popq %r12\n ret\n");

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

    clockStart();
    for (int i = 1; i < n; i++) {
        matrixMultiply(i - 1, i);
    }

    clockEnd();

    printMat(n - 1);
}
