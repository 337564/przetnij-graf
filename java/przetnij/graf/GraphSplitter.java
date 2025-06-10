package przetnij.graf;

public class GraphSplitter {
    
    public static Graph splitGraph(Graph original, int number, float margin) {
        int targetGroups = number + 1;
        Graph balanced = new Graph(original.numVertices, original.numEdges);
        balanced.splitCount = number;
        
        // Simplified splitting logic
        int groupSize = original.numVertices / targetGroups;
        int remainder = original.numVertices % targetGroups;
        
        // Assign vertices to groups
        int[] groups = new int[original.numVertices];
        int currentGroup = 0;
        int countInGroup = 0;
        
        for (int i = 0; i < original.numVertices; i++) {
            groups[i] = currentGroup;
            countInGroup++;
            
            int targetSize = groupSize + (currentGroup < remainder ? 1 : 0);
            if (countInGroup >= targetSize) {
                currentGroup++;
                countInGroup = 0;
            }
        }
        
        // Keep edges within same group
        for (Edge edge : original.edges) {
            if (groups[edge.src] == groups[edge.dest]) {
                balanced.addEdge(edge.src, edge.dest);
            }
        }
        
        return balanced;
    }
}
