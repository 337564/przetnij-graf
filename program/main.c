#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"
#include "split.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <input file> <N> <M> [-o <output file>] [-t] [-b]\n", argv[0]);
        return 1;
    }

    char *inputFile = argv[1];
    int number = atoi(argv[2]);
    float margin = atof(argv[3]);

    char *outputFile = "plik.out";
    int flagTerminal = 0;
    int flagBinary = 0;

    for (int i = 4; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0) {
            flagTerminal = 1;
        } else if (strcmp(argv[i], "-b") == 0) {
            flagBinary = 1;
        } else {
            printf("Unknown argument: %s\n", argv[i]);
            return 1;
        }
    }

    Graph *originalGraph = graphFromTextFile(inputFile);
    if (!originalGraph) {
        printf("Failed to load graph from file: %s\n", inputFile);
        return 1;
    }

    Graph *splitGrap = splitGraph(originalGraph, number, margin);

    if (!splitGrap) {
        printf("Failed to split graph into %d subgraphs with margin %.2f\n", number, margin);
        freeGraph(originalGraph);
        return 1;
    }

    if (!flagBinary) {
        graphToTextFile(splitGrap, outputFile);
        printf("Graph written to text file: %s\n", outputFile);
    }
    else {
        graphToBinaryFile(splitGrap, outputFile);
        printf("Graph written to binary file: %s\n", outputFile);
    }

    if (flagTerminal) {
        char *graphString = graphToString(splitGrap);
        printf("Graph:\n%s\n", graphString);
        free(graphString);
    }

    freeGraph(originalGraph);
    freeGraph(splitGrap);

    return 0;
}