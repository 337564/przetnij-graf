package przetnij.graf;

import java.util.ArrayList;
import java.util.List;

public class Graph {
    public final int numVertices;
    public final int numEdges;
    public final List<Edge> edges;
    public int splitCount;
    
    // Store segment information for each node
    private final int[] segments;

    public Graph(int numVertices, int numEdges) {
        this.numVertices = numVertices;
        this.numEdges = numEdges;
        this.edges = new ArrayList<>(numEdges);
        this.splitCount = -1;
        this.segments = new int[numVertices];
    }

    public void addEdge(int src, int dest) {
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
                ", edges=" + numEdges +
                ", splitCount=" + splitCount +
                '}';
    }
}
