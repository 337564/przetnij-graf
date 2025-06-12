package przetnij.graf;

import java.io.*;
import java.util.*;
import java.util.stream.Collectors;

public class GraphIO {
    public static Graph fromTextFile(String filename) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            // 1. Max nodes per row (layout info)
            String maxNodesPerRowLine = reader.readLine();
            int maxNodesPerRow = maxNodesPerRowLine != null ? Integer.parseInt(maxNodesPerRowLine.trim()) : 0;

            // 2. Node indexes in rows (all nodes in graph)
            String nodeIndexesLine = reader.readLine();
            if (nodeIndexesLine == null) throw new IOException("Missing node indexes line.");
            int[] nodeIndexes = parseSemicolonSeparated(nodeIndexesLine);
            
            // 3. Row pointers to node indexes
            String rowPointersLine = reader.readLine();
            if (rowPointersLine == null) throw new IOException("Missing row pointers line.");
            int[] rowPointers = parseSemicolonSeparated(rowPointersLine);

            // 4. Group nodes (nodes connected by edges)
            String groupNodesLine = reader.readLine();
            if (groupNodesLine == null) throw new IOException("Missing group nodes line.");
            int[] groupNodes = parseSemicolonSeparated(groupNodesLine);

            // 5. Group pointers (can be multiple lines for multiple graphs)
            List<int[]> groupPointersList = new ArrayList<>();
            String line;
            while ((line = reader.readLine()) != null && !line.trim().isEmpty()) {
                groupPointersList.add(parseSemicolonSeparated(line));
            }

            // Create graph with layout info
            Set<Integer> presentNodes = Arrays.stream(nodeIndexes).boxed().collect(Collectors.toSet());
            Graph graph = new Graph(nodeIndexes.length, maxNodesPerRow, nodeIndexes, rowPointers, presentNodes);
            
            // Add edges from groups
            for (int[] groupPointers : groupPointersList) {
                for (int i = 0; i < groupPointers.length - 1; i++) {
                    int start = groupPointers[i];
                    int end = groupPointers[i + 1];
                    
                    // Add edges between all nodes in this group
                    for (int j = start; j < end; j++) {
                        for (int k = j + 1; k < end; k++) {
                            if (j < groupNodes.length && k < groupNodes.length) {
                                graph.addEdge(groupNodes[j], groupNodes[k]);
                            }
                        }
                    }
                }
            }
            
            return graph;
        }
    }
    
    private static int[] parseSemicolonSeparated(String line) {
        return Arrays.stream(line.trim().split(";"))
                     .mapToInt(Integer::parseInt)
                     .toArray();
    }

    public static void toTextFile(Graph graph, String filename) throws IOException {
        try (PrintWriter writer = new PrintWriter(new FileWriter(filename))) {
            // 1. Max nodes per row
            writer.println(graph.maxNodesPerRow);
            
            // 2. Node indexes
            writer.println(Arrays.stream(graph.nodeIndexes)
                .mapToObj(String::valueOf)
                .collect(Collectors.joining(";")));
            
            // 3. Row pointers
            writer.println(Arrays.stream(graph.rowPointers)
                .mapToObj(String::valueOf)
                .collect(Collectors.joining(";")));
            
            // 4. Group nodes (simplified - all nodes)
            int[] allNodes = new int[graph.numVertices];
            for (int i = 0; i < graph.numVertices; i++) {
                allNodes[i] = i;
            }
            writer.println(Arrays.stream(allNodes)
                .mapToObj(String::valueOf)
                .collect(Collectors.joining(";")));
            
            // 5. Group pointers (single group for now)
            writer.println("0;" + graph.numVertices);
        }
    }
}
