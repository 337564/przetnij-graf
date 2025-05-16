#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

static char *strtok_r(char *s, const char *delim, char **saveptr) {
    char *token;
    if (s) *saveptr = s;
    if (!*saveptr) return NULL;
    char *start = *saveptr + strspn(*saveptr, delim);
    if (!*start) { *saveptr = NULL; return NULL; }
    token = start;
    char *end = start + strcspn(start, delim);
    if (*end) {
        *end = '\0';
        *saveptr = end + 1;
    } else {
        *saveptr = NULL;
    }
    return token;
}

Graph *createGraph(int numVertices, int numEdges) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->numEdges = numEdges;
    graph->splitCount = -1;
    graph->edges = (Edge *)malloc(numEdges * sizeof(Edge));
    for (int i = 0; i < numEdges; i++) {
        graph->edges[i].src = -1;
        graph->edges[i].dest = -1;
    }
    return graph;
}

void freeGraph(Graph *graph) {
    if (graph) {
        free(graph->edges);
        free(graph);
    }
}


Graph *graphFromString(const char *str) {
    // Kopiujemy wejściowy string, aby nie modyfikować oryginału
    char *buf = _strdup(str);
    if (!buf) return NULL;

    char *line, *save1;
    line = strtok_r(buf, "\r\n", &save1);
    if (!line) { free(buf); return NULL; }

    // Parsujemy wiersze: pobieramy ID wierzchołków z drugiej linii
    line = strtok_r(NULL, "\r\n", &save1);
    if (!line) { free(buf); return NULL; }
    int *rowNodes = NULL, rowCount = 0;
    char *tok, *save2;
    for (tok = strtok_r(line, ";", &save2); tok; tok = strtok_r(NULL, ";", &save2)) {
        rowNodes = realloc(rowNodes, sizeof(*rowNodes)*(rowCount+1));
        rowNodes[rowCount++] = atoi(tok);
    }

    line = strtok_r(NULL, "\r\n", &save1);
    if (!line) { free(rowNodes); free(buf); return NULL; }

    line = strtok_r(NULL, "\r\n", &save1);
    if (!line) { free(rowNodes); free(buf); return NULL; }
    int *grpNodes = NULL, grpNodeCount = 0;
    for (tok = strtok_r(line, ";", &save2); tok; tok = strtok_r(NULL, ";", &save2)) {
        grpNodes = realloc(grpNodes, sizeof(*grpNodes)*(grpNodeCount+1));
        grpNodes[grpNodeCount++] = atoi(tok);
    }

    // Parsujemy pozycje grup: granice w tablicy grpNodes
    line = strtok_r(NULL, "\r\n", &save1);
    if (!line) { free(grpNodes); free(rowNodes); free(buf); return NULL; }
    int *grpPos = NULL, grpPosCount = 0;
    for (tok = strtok_r(line, ";", &save2); tok; tok = strtok_r(NULL, ";", &save2)) {
        grpPos = realloc(grpPos, sizeof(*grpPos)*(grpPosCount+1));
        grpPos[grpPosCount++] = atoi(tok);
    }

    int numVertices = rowCount;
    int numGroups   = grpPosCount - 1;
    char *adj = calloc(numVertices * numVertices, 1);

    // Budujemy macierz sąsiedztwa: łączymy węzły w tej samej grupie
    for (int g = 0; g < numGroups; g++) {
        int start = grpPos[g], end = grpPos[g+1];
        for (int i = start; i < end; i++) {
            for (int j = i+1; j < end; j++) {
                int v1 = grpNodes[i], v2 = grpNodes[j];
                if (v1>=0 && v1<numVertices && v2>=0 && v2<numVertices) {
                    adj[v1*numVertices + v2] = 1;
                    adj[v2*numVertices + v1] = 1;
                }
            }
        }
    }

    // Zliczamy krawędzie w górnym trójkącie macierzy (i<j)
    int edgeCount = 0;
    for (int i = 0; i < numVertices; i++)
        for (int j = i+1; j < numVertices; j++)
            if (adj[i*numVertices + j]) edgeCount++;

    Graph *graph = createGraph(numVertices, edgeCount);

    // Przepisujemy krawędzie do struktury Graph
    int idx = 0;
    for (int i = 0; i < numVertices; i++) {
        for (int j = i+1; j < numVertices; j++) {
            if (adj[i*numVertices + j]) {
                graph->edges[idx].src  = i;
                graph->edges[idx].dest = j;
                idx++;
            }
        }
    }

    free(adj);
    free(grpPos);
    free(grpNodes);
    free(rowNodes);
    free(buf);
    return graph;
}

char *graphToString(const Graph *graph) {
    // Obliczamy stopnie wierzchołków
    int V = graph->numVertices;
    int E = graph->numEdges;

    int *degree = calloc(V, sizeof(int));
    for (int i = 0; i < E; i++) {
        degree[graph->edges[i].src]++;
        degree[graph->edges[i].dest]++;
    }
    int **adj = malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        adj[i] = malloc(degree[i] * sizeof(int));
        degree[i] = 0;
    }
    for (int i = 0; i < E; i++) {
        int u = graph->edges[i].src, v = graph->edges[i].dest;
        adj[u][degree[u]++] = v;
        adj[v][degree[v]++] = u;
    }

    // Obliczamy pozycje wierszy (CSR row pointers)
    int *rowPos = malloc((V + 1) * sizeof(int));
    rowPos[0] = 0;
    for (int i = 0; i < V; i++)
        rowPos[i + 1] = rowPos[i] + degree[i];
    int totalAdj = rowPos[V];

    // Wypełniamy tablicę rowNodes listami sąsiadów
    int *rowNodes = malloc(totalAdj * sizeof(int));
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < degree[i]; j++)
            rowNodes[rowPos[i] + j] = adj[i][j];
        free(adj[i]);
    }
    free(adj);

    // Wyznaczamy spójne składowe grafu (DFS na CSR)
    int *comp = malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) comp[i] = -1;
    int *stack = malloc(V * sizeof(int));
    int compCount = 0;
    for (int i = 0; i < V; i++) {
        if (comp[i] < 0) {
            int sp = 0;
            stack[sp++] = i;
            comp[i] = compCount;
            while (sp) {
                int u = stack[--sp];
                for (int k = rowPos[u]; k < rowPos[u + 1]; k++) {
                    int v = rowNodes[k];
                    if (comp[v] < 0) {
                        comp[v] = compCount;
                        stack[sp++] = v;
                    }
                }
            }
            compCount++;
        }
    }
    free(stack);

    // Przygotowujemy pozycje grup i listę wierzchołków grup
    int *groupSize = calloc(compCount, sizeof(int));
    for (int i = 0; i < V; i++)
        groupSize[comp[i]]++;
    int *grpPos = malloc((compCount + 1) * sizeof(int));
    grpPos[0] = 0;
    for (int g = 0; g < compCount; g++)
        grpPos[g + 1] = grpPos[g] + groupSize[g];

    int *grpNodes = malloc(V * sizeof(int));
    for (int g = 0; g < compCount; g++)
        groupSize[g] = 0;
    for (int i = 0; i < V; i++) {
        int g = comp[i];
        int idx = grpPos[g] + (groupSize[g]++);
        grpNodes[idx] = i;
    }
    free(groupSize);
    free(comp);

    // Formatujemy finalny string z reprezentacją grafu
    int totalNums = totalAdj + (V + 1) + V + (compCount + 1);
    size_t bufSize = (size_t)totalNums * 12 + 128;
    char *out = malloc(bufSize);
    int pos = 0;

    pos += snprintf(out + pos, bufSize - pos, "%d\n", V);

    for (int i = 0; i < totalAdj; i++)
        pos += snprintf(out + pos, bufSize - pos,
                        i + 1 < totalAdj ? "%d;" : "%d\n",
                        rowNodes[i]);

    for (int i = 0; i <= V; i++)
        pos += snprintf(out + pos, bufSize - pos,
                        i + 1 <= V ? "%d;" : "%d\n",
                        rowPos[i]);

    for (int i = 0; i < V; i++)
        pos += snprintf(out + pos, bufSize - pos,
                        i + 1 < V ? "%d;" : "%d\n",
                        grpNodes[i]);

    for (int i = 0; i <= compCount; i++)
        pos += snprintf(out + pos, bufSize - pos,
                        i < compCount ? "%d;" : "%d",
                        grpPos[i]);

    pos += snprintf(out + pos, bufSize - pos, "\n%d", graph->splitCount);

    free(rowPos);
    free(rowNodes);
    free(grpPos);
    free(grpNodes);

    return out;
}

Graph *graphFromTextFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *fileContent = (char *)malloc(fileSize + 1);
    fread(fileContent, 1, fileSize, file);
    fileContent[fileSize] = '\0';

    fclose(file);

    Graph *graph = graphFromString(fileContent);
    free(fileContent);

    return graph;
}

void graphToTextFile(const Graph *graph, const char *filename) {
    char *graphString = graphToString(graph);

    FILE *file = fopen(filename, "w");
    if (file) {
        fputs(graphString, file);
        fclose(file);
    }

    free(graphString);
}

void graphToBinaryFile(const Graph *graph, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return;
    }

    fwrite(&graph->numVertices, sizeof(int), 1, file);
    fwrite(&graph->numEdges, sizeof(int), 1, file);

    for (int i = 0; i < graph->numEdges; i++) {
        fwrite(&graph->edges[i].src, sizeof(int), 1, file);
        fwrite(&graph->edges[i].dest, sizeof(int), 1, file);
    }

    fclose(file);
}

Graph *copyGraph(const Graph *original) {
    if (!original) {
        return NULL;
    }

    Graph *copy = createGraph(original->numVertices, original->numEdges);

    for (int i = 0; i < original->numEdges; i++) {
        copy->edges[i].src = original->edges[i].src;
        copy->edges[i].dest = original->edges[i].dest;
    }

    copy->splitCount = original->splitCount;

    return copy;
}