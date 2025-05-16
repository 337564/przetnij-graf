#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include "graph.h"
#include "split.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

Graph *splitGraph(Graph *originalGraph, int number, float margin) {
    number++;
    Graph *balancedGraph = copyGraph(originalGraph);

    // Tablica do oznaczania odwiedzonych wierzchołków
    bool *visited = calloc(originalGraph->numVertices, sizeof(bool));
    int *component = calloc(originalGraph->numVertices, sizeof(int));
    // Oznacz wszystkie wierzchołki jako nieprzypisane
    for (int v = 0; v < originalGraph->numVertices; v++) {
        component[v] = -1;
    }

    // Oblicz docelowy rozmiar każdej składowej (równomierny podział)
    int totalV = originalGraph->numVertices;
    int baseSize = totalV / number;
    int remainder = totalV % number;
    int *desiredSizes = calloc(number, sizeof(int));
    for (int c = 0; c < number; c++) {
        desiredSizes[c] = baseSize + (c < remainder ? 1 : 0);
    }

    // Wieloźródłowy BFS: ziarno od wierzchołków o największym stopniu
    int *compSize = calloc(number, sizeof(int));
    int *degree = calloc(totalV, sizeof(int));
    for (int e = 0; e < originalGraph->numEdges; e++) {
        degree[originalGraph->edges[e].src]++;
        degree[originalGraph->edges[e].dest]++;
    }

    typedef struct {
        int comp;
        int u; 
    } Entry;
    Entry *queue = malloc(totalV * sizeof(*queue));
    int head = 0;
    int tail = 0;
    bool *seeded = calloc(totalV, sizeof(bool));

    for (int comp = 0; comp < number; comp++) {
        int best = -1, bestDeg = -1;
        for (int v = 0; v < totalV; v++) {
            if (!visited[v] && !seeded[v] && degree[v] > bestDeg) {
                bestDeg = degree[v];
                best = v;
            }
        }
        if (best < 0) break;
        seeded[best] = true;
        visited[best] = true;
        component[best] = comp;
        compSize[comp] = 1;
        queue[tail++] = (Entry){ comp, best };
    }
    free(seeded);
    free(degree);

    // Rozszerzanie BFS aż składowe osiągną zadane rozmiary
    while (head < tail) {
        Entry cur = queue[head++];
        int comp = cur.comp;
        int u = cur.u;
        if (compSize[comp] >= desiredSizes[comp]) continue;

        for (int e = 0; e < originalGraph->numEdges; e++) {
            int v = -1;
            if (originalGraph->edges[e].src == u) { 
                v = originalGraph->edges[e].dest;
            }
            else if (originalGraph->edges[e].dest == u) { 
                v = originalGraph->edges[e].src;
            }
            if (v >= 0 && !visited[v]) {
                visited[v] = true;
                component[v] = comp;
                compSize[comp]++;
                queue[tail++] = (Entry){ comp, v };
                if (compSize[comp] >= desiredSizes[comp]) break;
            }
        }
    }
    free(queue);
    free(compSize);
    free(desiredSizes);

    // Zachowaj tylko krawędzie wewnątrz tej samej składowej
    int newEdgeCount = 0;
    for (int e = 0; e < originalGraph->numEdges; e++) {
        int s = originalGraph->edges[e].src;
        int d = originalGraph->edges[e].dest;
        if (component[s] == component[d]) {
            balancedGraph->edges[newEdgeCount++] = originalGraph->edges[e];
        }
    }
    balancedGraph->numEdges = newEdgeCount;

    // Połącz pozostałe wierzchołki poprzez jedną z ich oryginalnych krawędzi
    bool added = true;
    int iter = 0;
    while (added && iter < totalV) {
        added = false;
        for (int e = 0; e < originalGraph->numEdges; e++) {
            int s = originalGraph->edges[e].src;
            int d = originalGraph->edges[e].dest;
            if (component[s] >= 0 && component[d] < 0) {
                component[d] = component[s];
                balancedGraph->edges[newEdgeCount++] = originalGraph->edges[e];
                added = true;
            }
            else if (component[d] >= 0 && component[s] < 0) {
                component[s] = component[d];
                balancedGraph->edges[newEdgeCount++] = originalGraph->edges[e];
                added = true;
            }
        }
        iter++;
    }
    balancedGraph->numEdges = newEdgeCount;

    // Oblicz wielkości składowych i udziały, zwróć NULL jeśli przekroczono margines
    {
        int *compCounts = calloc(number, sizeof(int));
        for (int v = 0; v < totalV; v++)
            if (component[v] >= 0)
                compCounts[component[v]]++;
        bool valid = true;
        for (int i = 0; i < number; i++) {
            float perc = compCounts[i] * 100.0f / totalV;
            if (perc > margin) { valid = false; break; }
        }
        free(compCounts);
        if (!valid) {
            free(visited);
            free(component);
            free(balancedGraph->edges);
            free(balancedGraph);
            return NULL;
        }
    }

    free(visited);
    free(component);
    balancedGraph->splitCount = number-1;
    return balancedGraph;
}