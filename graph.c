#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

Graph *createGraph(int numVertices, int numEdges) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->numEdges = numEdges;
    graph->vertices = (Vertex *)malloc(numVertices * sizeof(Vertex));
    for (int i = 0; i < numVertices; i++) {
        graph->vertices[i].weight = 0.0;
    }
    graph->edges = (Edge *)malloc(numEdges * sizeof(Edge));
    for (int i = 0; i < numEdges; i++) {
        graph->edges[i].src = -1;
        graph->edges[i].dest = -1;
    }
    return graph;
}

void freeGraph(Graph *graph) {
    if (graph) {
        free(graph->vertices);
        free(graph->edges);
        free(graph);
    }
}

Graph *graphFromString(const char *str) {
    int numVertices, numEdges;
    sscanf(str, "%d %d", &numVertices, &numEdges);

    Graph *graph = createGraph(numVertices, numEdges);

    const char *verticesStart = strchr(str, '\n') + 1;
    const char *edgesStart = strchr(verticesStart, '\n') + 1;

    size_t verticesLineLength = edgesStart - verticesStart - 1;
    char *verticesLine = (char *)malloc(verticesLineLength + 1);
    strncpy(verticesLine, verticesStart, verticesLineLength);
    verticesLine[verticesLineLength] = '\0';

    char *edgesLine = strdup(edgesStart);

    char *token = strtok(verticesLine, ",");
    for (int i = 0; i < numVertices; i++) {
        graph->vertices[i].weight = atof(token);
        token = strtok(NULL, ",");
    }

    token = strtok(edgesLine, ",");
    for (int i = 0; i < numEdges; i++) {
        sscanf(token, "%d-%d", &graph->edges[i].src, &graph->edges[i].dest);
        token = strtok(NULL, ",");
    }

    free(verticesLine);
    free(edgesLine);

    return graph;
}

char *graphToString(const Graph *graph) {
    int bufferSize = 128 + graph->numVertices * 16 + graph->numEdges * 8;
    char *buffer = (char *)malloc(bufferSize);
    char *ptr = buffer;

    ptr += sprintf(ptr, "%d %d\n", graph->numVertices, graph->numEdges);

    for (int i = 0; i < graph->numVertices; i++) {
        ptr += sprintf(ptr, "%.2f", graph->vertices[i].weight);
        if (i < graph->numVertices - 1) {
            ptr += sprintf(ptr, ",");
        }
    }
    ptr += sprintf(ptr, "\n");

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

    for (int i = 0; i < graph->numVertices; i++) {
        fwrite(&graph->vertices[i].weight, sizeof(float), 1, file);
    }

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

    for (int i = 0; i < original->numVertices; i++) {
        copy->vertices[i].weight = original->vertices[i].weight;
    }

    for (int i = 0; i < original->numEdges; i++) {
        copy->edges[i].src = original->edges[i].src;
        copy->edges[i].dest = original->edges[i].dest;
    }

    return copy;
}