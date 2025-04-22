import struct
import networkx as nx
import matplotlib.pyplot as plt

def read_graph_from_binary_file(filename):
    with open(filename, "rb") as file:
        num_vertices, num_edges = struct.unpack("ii", file.read(8))

        vertices = []
        for _ in range(num_vertices):
            weight = struct.unpack("f", file.read(4))[0]
            vertices.append(weight)

        edges = []
        for _ in range(num_edges):
            src, dest = struct.unpack("ii", file.read(8))
            edges.append((src, dest))

    return num_vertices, num_edges, vertices, edges

def print_graph(num_vertices, num_edges, vertices, edges):
    print(f"Number of vertices: {num_vertices}")
    print(f"Number of edges: {num_edges}")
    print("Vertex weights:")
    for i, weight in enumerate(vertices):
        print(f"  Vertex {i}: {weight}")
    print("Edges:")
    for src, dest in edges:
        print(f"  {src} -> {dest}")

def visualize_graph(num_vertices, vertices, edges):
    G = nx.Graph()

    for i, weight in enumerate(vertices):
        G.add_node(i, weight=weight)

    G.add_edges_from(edges)

    pos = nx.spring_layout(G)
    node_labels = {i: f"{i}\n({weight:.2f})" for i, weight in enumerate(vertices)}
    nx.draw(G, pos, with_labels=True, labels=node_labels, node_color="lightblue", node_size=2000, font_size=10)
    nx.draw_networkx_edge_labels(G, pos, edge_labels={(src, dest): "" for src, dest in edges})
    plt.title("Graph Visualization")
    plt.show()

if __name__ == "__main__":
    filename = "program\\graphs\\graph_output.bin"
    num_vertices, num_edges, vertices, edges = read_graph_from_binary_file(filename)
    print_graph(num_vertices, num_edges, vertices, edges)
    visualize_graph(num_vertices, vertices, edges)