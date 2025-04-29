#ifndef GRAPH_H
#define GRAPH_H

typedef struct Edge {
    int src;
    int dest;
} Edge;

typedef struct Graph {
    int numVertices;
    int numEdges;
    Edge *edges;
    int splitCount;
} Graph;

Graph *createGraph(int numVertices, int numEdges);
void freeGraph(Graph *graph);
Graph *graphFromString(const char *str);
char *graphToString(const Graph *graph);
Graph *graphFromTextFile(const char *filename);
void graphToTextFile(const Graph *graph, const char *filename);
void graphToBinaryFile(const Graph *graph, const char *filename);
Graph *copyGraph(const Graph *original);

#endif // GRAPH_H