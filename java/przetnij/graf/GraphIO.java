package przetnij.graf;

import java.io.*;
import java.util.*;

public class GraphIO {
    
    public static Graph fromTextFile(String filename) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            StringBuilder content = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                content.append(line).append("\n");
            }
            return fromString(content.toString());
        }
    }

    public static Graph fromString(String str) {
        String[] lines = str.split("\\r?\\n");
        if (lines.length < 3) return null;  // Reduced from 5 to 3 since we only need 3 lines now

        int numVertices = Integer.parseInt(lines[0]);
        
        // Parse group nodes
        String[] grpNodesStr = lines[1].split(";");  // Changed from lines[3] to lines[1]
        int[] grpNodes = Arrays.stream(grpNodesStr).mapToInt(Integer::parseInt).toArray();

        // Parse group positions
        String[] grpPosStr = lines[2].split(";");  // Changed from lines[4] to lines[2]
        int[] grpPos = Arrays.stream(grpPosStr).mapToInt(Integer::parseInt).toArray();

        // Create adjacency matrix
        boolean[][] adj = new boolean[numVertices][numVertices];
        int numGroups = grpPos.length - 1;
        
        for (int g = 0; g < numGroups; g++) {
            int start = grpPos[g];
            int end = grpPos[g + 1];
            for (int i = start; i < end; i++) {
                for (int j = i + 1; j < end; j++) {
                    int v1 = grpNodes[i];
                    int v2 = grpNodes[j];
                    if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
                        adj[v1][v2] = true;
                        adj[v2][v1] = true;
                    }
                }
            }
        }

        // Count edges
        int edgeCount = 0;
        for (int i = 0; i < numVertices; i++) {
            for (int j = i + 1; j < numVertices; j++) {
                if (adj[i][j]) edgeCount++;
            }
        }

        // Create graph
        Graph graph = new Graph(numVertices, edgeCount);
        for (int i = 0; i < numVertices; i++) {
            for (int j = i + 1; j < numVertices; j++) {
                if (adj[i][j]) {
                    graph.addEdge(i, j);
                }
            }
        }

        return graph;
    }

    public static void toTextFile(Graph graph, String filename) throws IOException {
        try (PrintWriter writer = new PrintWriter(new FileWriter(filename))) {
            writer.print(toString(graph));
        }
    }

    public static String toString(Graph graph) {
        StringBuilder sb = new StringBuilder();
        sb.append(graph.numVertices).append("\n");
        
        // Simplified representation for demo
        for (Edge edge : graph.edges) {
            sb.append(edge.src).append(";").append(edge.dest).append("\n");
        }
        
        return sb.toString();
    }
}
