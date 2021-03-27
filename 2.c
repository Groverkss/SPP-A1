#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>

#define MAXV 2500
#define MAXE 100000
#define INF 1000000000

#define max(a, e) a = a > e ? a : e
#define min(a, e) a = a < e ? a : e

struct timespec start, stop;

int32_t V, E;
int32_t dist[MAXV][MAXV];

void clockStart() { clock_gettime(CLOCK_REALTIME, &start); }

void clockEnd() {
    clock_gettime(CLOCK_REALTIME, &stop);
    double time_taken =
        (double)(stop.tv_sec - start.tv_sec) +
        ((double)(stop.tv_nsec - start.tv_nsec) / (double)(1e9));
    fprintf(stderr, "time taken %0.10lf seconds\n", time_taken);
}

void floydWarshall() {
    for (register int k = 0; k < V; k++) {
        for (register int i = 0; i < V; i++) {
            if (dist[i][k] == INF) {
                continue;
            }

            register int32_t distIK = dist[i][k];
            register int32_t *distIJ = dist[i];
            register int32_t *distKJ = dist[k];

            register int32_t *distIJEnd = (dist[i] + V);
            for ( ; distIJ != distIJEnd; ++distIJ, ++distKJ ) {
                min(*(distIJ), distIK + *(distKJ));
            }
        }
    }
}

void printDist() {
    for (register int i = 0; i < V; i++) {
        for (register int j = 0; j < V; j++) {
            if (dist[i][j] == INF) {
                printf("-1 ");
            } else {
                printf("%d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int main(void) {
    scanf("%d %d", &V, &E);

    for (register int i = 0; i < V; i++) {
        for (register int j = 0; j < V; j++) {
            dist[i][j] = INF;
        }
    }
    for (register int i = 0; i < V; i++) {
        dist[i][i] = 0;
    }

    int32_t X, Y, W;
    for (register int i = 0; i < E; i++) {
        scanf("%d %d %d", &X, &Y, &W);
        min(dist[X - 1][Y - 1], W);
    }

    clockStart();
    floydWarshall();
    clockEnd();
    printDist();
}
