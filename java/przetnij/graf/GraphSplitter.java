package przetnij.graf;

import java.util.*;

public class GraphSplitter {
    
    public static Graph splitGraph(Graph original, int number, float margin) {
        // Increment number to work on number+1 groups
        int internalGroupCount = number + 1;
        
        // Create a copy of the original graph
        Graph balancedGraph = new Graph(original.numVertices, original.edges.size());
        
        // Initialize arrays
        boolean[] visited = new boolean[original.numVertices];
        int[] component = new int[original.numVertices];
        Arrays.fill(component, -1);
        
        // Calculate desired group sizes
        int totalV = original.numVertices;
        int baseSize = totalV / internalGroupCount;
        int remainder = totalV % internalGroupCount;
        int[] desiredSizes = new int[internalGroupCount];
        for (int i = 0; i < internalGroupCount; i++) {
            desiredSizes[i] = baseSize + (i < remainder ? 1 : 0);
        }
        
        // Precompute degrees
        int[] degree = new int[original.numVertices];
        for (Edge edge : original.edges) {
            degree[edge.src]++;
            degree[edge.dest]++;
        }
        
        // Seed groups with highest degree nodes
        int[] groupSizes = new int[internalGroupCount];
        Queue<int[]> queue = new LinkedList<>();  // [component, node]
        
        for (int g = 0; g < internalGroupCount; g++) {
            int bestNode = -1;
            int bestDegree = -1;
            for (int i = 0; i < original.numVertices; i++) {
                if (!visited[i] && degree[i] > bestDegree) {
                    bestDegree = degree[i];
                    bestNode = i;
                }
            }
            if (bestNode == -1) break;
            
            visited[bestNode] = true;
            component[bestNode] = g;
            groupSizes[g]++;
            queue.add(new int[]{g, bestNode});
        }
        
        // Multi-source BFS expansion
        while (!queue.isEmpty()) {
            int[] entry = queue.poll();
            int comp = entry[0];
            int u = entry[1];
            
            // Skip if group is full
            if (groupSizes[comp] >= desiredSizes[comp]) continue;
            
            // Explore neighbors
            for (Edge edge : original.edges) {
                int v = -1;
                if (edge.src == u) v = edge.dest;
                else if (edge.dest == u) v = edge.src;
                
                if (v != -1 && !visited[v]) {
                    visited[v] = true;
                    component[v] = comp;
                    groupSizes[comp]++;
                    queue.add(new int[]{comp, v});
                    
                    // Stop if group is full
                    if (groupSizes[comp] >= desiredSizes[comp]) break;
                }
            }
        }
        
        // Assign unconnected nodes through edges
        boolean changed;
        int iter = 0;
        do {
            changed = false;
            for (Edge edge : original.edges) {
                int u = edge.src;
                int v = edge.dest;
                
                if (component[u] != -1 && component[v] == -1) {
                    component[v] = component[u];
                    changed = true;
                } else if (component[v] != -1 && component[u] == -1) {
                    component[u] = component[v];
                    changed = true;
                }
            }
            iter++;
        } while (changed && iter < original.numVertices);
        
        // Filter edges to keep only intra-group edges
        for (Edge edge : original.edges) {
            if (component[edge.src] == component[edge.dest]) {
                balancedGraph.addEdge(edge.src, edge.dest);
            }
        }
        
        // Verify group sizes against margin
        for (int i = 0; i < internalGroupCount; i++) {
            float percentage = groupSizes[i] * 100.0f / totalV;
            if (percentage > margin) {
                return null;  // Group exceeds margin
            }
        }
        
        balancedGraph.splitCount = number;
        return balancedGraph;
    }
}
