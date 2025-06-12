package przetnij.graf;

public class Edge {
    public final int src;
    public final int dest;

    public Edge(int src, int dest) {
        this.src = src;
        this.dest = dest;
    }

    @Override
    public String toString() {
        return "(" + src + ", " + dest + ")";
    }
}
