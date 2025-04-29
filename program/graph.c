#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

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
    int splitCount, numVertices, numEdges;
    sscanf(str, "%d %d %d", &splitCount, &numVertices, &numEdges);

    Graph *graph = createGraph(numVertices, numEdges);
    graph->splitCount = splitCount;

    const char *edgesStart = strchr(str, '\n') + 1;

    char *edgesLine = strdup(edgesStart);

    char *token = strtok(edgesLine, ",");
    for (int i = 0; i < numEdges; i++) {
        sscanf(token, "%d-%d", &graph->edges[i].src, &graph->edges[i].dest);
        token = strtok(NULL, ",");
    }

    free(edgesLine);

    return graph;
}

char *graphToString(const Graph *graph) {
    int bufferSize = 128 + graph->numVertices * 16 + graph->numEdges * 8;
    char *buffer = (char *)malloc(bufferSize);
    char *ptr = buffer;

    ptr += sprintf(ptr, "%d %d %d\n", graph->splitCount, graph->numVertices, graph->numEdges);

    for (int i = 0; i < graph->numEdges; i++) {
        ptr += sprintf(ptr, "%d-%d", graph->edges[i].src, graph->edges[i].dest);
        if (i < graph->numEdges - 1) {
            ptr += sprintf(ptr, ",");
        }
    }

    return buffer;
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