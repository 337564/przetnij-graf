package przetnij.graf;

import java.util.ArrayList;
import java.util.List;

public class Graph {
    public final int numVertices;
    public final int numEdges;
    public final List<Edge> edges;
    public int splitCount;

    public Graph(int numVertices, int numEdges) {
        this.numVertices = numVertices;
        this.numEdges = numEdges;
        this.edges = new ArrayList<>(numEdges);
        this.splitCount = -1;
    }

    public void addEdge(int src, int dest) {
        edges.add(new Edge(src, dest));
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
