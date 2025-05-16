import struct
import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.cm as cm

def read_graph_from_binary_file(filename):
    with open(filename, "rb") as file:
        num_vertices, num_edges = struct.unpack("ii", file.read(8))

        edges = []
        for _ in range(num_edges):
            src, dest = struct.unpack("ii", file.read(8))
            edges.append((src, dest))

    return num_vertices, num_edges, edges

def print_graph(num_vertices, num_edges, edges):
    print(f"Number of vertices: {num_vertices}")
    print(f"Number of edges: {num_edges}")
    print("Edges:")
    for src, dest in edges:
        print(f"  {src} -> {dest}")

def visualize_graph(num_vertices, edges):
    G = nx.Graph()

    for i in range(num_vertices):
        G.add_node(i)

    G.add_edges_from(edges)

    pos = nx.spring_layout(G)

    comps = list(nx.connected_components(G))
    cmap = cm.get_cmap("tab20")
    colors = [cmap(i) for i in range(len(comps))]
    color_map = {}
    for idx, comp in enumerate(comps):
        for node in comp:
            color_map[node] = colors[idx]
    node_colors = [color_map[node] for node in G.nodes()]

    node_labels = {i: f"{i}" for i in range(num_vertices)}
    nx.draw(
        G,
        pos,
        with_labels=True,
        labels=node_labels,
        node_color=node_colors,
        node_size=2000,
        font_size=10
    )
    nx.draw_networkx_edge_labels(
        G,
        pos,
        edge_labels={(src, dest): "" for src, dest in edges}
    )
    plt.title("Graph Visualization")
    plt.show()

if __name__ == "__main__":
    filename = "program\\graphs\\graph_output.bin"
    num_vertices, num_edges, edges = read_graph_from_binary_file(filename)
    print_graph(num_vertices, num_edges, edges)
    visualize_graph(num_vertices, edges)