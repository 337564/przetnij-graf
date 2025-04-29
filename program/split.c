#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "graph.h"

float grow_partition(Graph *originalGraph, int partitionId, int vertexCount, float minWeightThreshold, int *partitionAssignments, int *isUnassigned) {
    int *bfsQueue = malloc(vertexCount * sizeof(int));
    int *inQueue = calloc(vertexCount, sizeof(int));
    float partitionWeightSum = 0;
    int queueHead = 0, queueTail = 0;

    for (int i = 0; i < vertexCount; i++) {
        if (isUnassigned[i]) {
            bfsQueue[queueTail++] = i;
            inQueue[i] = 1;
            break;
        }
    }
    while (queueHead < queueTail && partitionWeightSum < minWeightThreshold) {
        int currentVertex = bfsQueue[queueHead++];
        if (!isUnassigned[currentVertex]) continue;
        partitionAssignments[currentVertex] = partitionId;
        isUnassigned[currentVertex] = 0;
        partitionWeightSum += 1.0f;
        for (int edgeIndex = 0; edgeIndex < originalGraph->numEdges; edgeIndex++) {
            int adjacentVertex = -1;
            if (originalGraph->edges[edgeIndex].src == currentVertex)
                adjacentVertex = originalGraph->edges[edgeIndex].dest;
            else if (originalGraph->edges[edgeIndex].dest == currentVertex)
                adjacentVertex = originalGraph->edges[edgeIndex].src;
            if (adjacentVertex >= 0 && isUnassigned[adjacentVertex] && !inQueue[adjacentVertex]) {
                inQueue[adjacentVertex] = 1;
                bfsQueue[queueTail++] = adjacentVertex;
            }
        }
    }
    free(bfsQueue);
    free(inQueue);
    return partitionWeightSum;
}

Graph *splitGraph(Graph *originalGraph, int number, float margin) {
    if (!originalGraph) {
        return NULL;
    }
    if (number < 0 || margin < 0 || margin > 100) {
        return NULL;
    }
    if (number == 0) {
        return copyGraph(originalGraph);
    }

    int vertexCount = originalGraph->numVertices;
    int edgeCount = originalGraph->numEdges;

    float totalWeight = 0;
    for (int i = 0; i < vertexCount; i++) {
        totalWeight += 1.0f;
    }
    float averageWeight = totalWeight / (number + 1);
    float minThreshold  = averageWeight * (1.0f - margin/100.0f);
    float maxThreshold  = averageWeight * (1.0f + margin/100.0f);

    int *partitionAssignments = malloc(vertexCount * sizeof(int));
    int *isUnassigned = malloc(vertexCount * sizeof(int));
    for (int i = 0; i < vertexCount; i++) {
        partitionAssignments[i] = -1;
        isUnassigned[i] = 1;
    }

    for (int partitionIndex = 0; partitionIndex < number; partitionIndex++) {
        float currentPartitionWeight = grow_partition( originalGraph, partitionIndex, vertexCount, minThreshold, partitionAssignments, isUnassigned);
        if (currentPartitionWeight > maxThreshold) {
            free(partitionAssignments);
            free(isUnassigned);
            return NULL;
        }
    }

    for (int i = 0; i < vertexCount; i++) {
        if (isUnassigned[i]) {
            partitionAssignments[i] = number;
        }
    }
    free(isUnassigned);

    Graph *newGraph = createGraph(vertexCount, edgeCount);

    int newEdgeCount = 0;
    for (int origEdgeIndex = 0; origEdgeIndex < edgeCount; origEdgeIndex++) {
        int src = originalGraph->edges[origEdgeIndex].src;
        int dest = originalGraph->edges[origEdgeIndex].dest;
        if (partitionAssignments[src] == partitionAssignments[dest]) {
            newGraph->edges[newEdgeCount++] = originalGraph->edges[origEdgeIndex];
        }
    }
    newGraph->numEdges = newEdgeCount;
    newGraph->edges = realloc(newGraph->edges, newEdgeCount * sizeof(Edge));

    free(partitionAssignments);
    newGraph->splitCount = number;
    return newGraph;
}