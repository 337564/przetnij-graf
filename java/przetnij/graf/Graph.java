package przetnij.graf;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class Graph {
    public final int numVertices;
    public final List<Edge> edges;
    public int splitCount;
    
    // Store layout information
    public final int maxNodesPerRow;
    public final int[] nodeIndexes;
    public final int[] rowPointers;
    public final Set<Integer> presentNodes;

    // Store segment information for each node
    private final int[] segments;

    public Graph(int numVertices, int maxNodesPerRow, int[] nodeIndexes, int[] rowPointers, Set<Integer> presentNodes) {
        this.numVertices = numVertices;
        this.edges = new ArrayList<>();
        this.splitCount = -1;
        this.segments = new int[numVertices];
        this.maxNodesPerRow = maxNodesPerRow;
        this.nodeIndexes = nodeIndexes;
        this.rowPointers = rowPointers;
        this.presentNodes = presentNodes;
    }

    // Legacy constructor for compatibility
    public Graph(int numVertices, int numEdges) {
        this(numVertices, 0, new int[0], new int[0], null);
    }

    // Legacy constructor for compatibility
    public Graph(int numVertices, int numEdges, Set<Integer> presentNodes) {
        this(numVertices, 0, new int[0], new int[0], presentNodes);
    }

    public void addEdge(int src, int dest) {
        // Ensure we don't add duplicate edges in case the input format is redundant
        for (Edge edge : edges) {
            if ((edge.src == src && edge.dest == dest) || (edge.src == dest && edge.dest == src)) {
                return;
            }
        }
        edges.add(new Edge(src, dest));
    }
    
    // Segment management
    public void setSegment(int nodeIndex, int segment) {
        if (nodeIndex >= 0 && nodeIndex < numVertices) {
            segments[nodeIndex] = segment;
        }
    }
    
    public int getSegment(int nodeIndex) {
        if (nodeIndex >= 0 && nodeIndex < numVertices) {
            return segments[nodeIndex];
        }
        return -1;
    }
    
    public int getNodeCount() {
        return numVertices;
    }
    
    public int getEdgeCount() {
        return edges.size();
    }
    
    public Edge getEdge(int index) {
        return edges.get(index);
    }

    @Override
    public String toString() {
        return "Graph{" +
                "vertices=" + numVertices +
                ", edges=" + getEdgeCount() +
                ", splitCount=" + splitCount +
                '}';
    }
}
