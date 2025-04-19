import struct

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

if __name__ == "__main__":
    filename = "graph_output.bin"
    num_vertices, num_edges, vertices, edges = read_graph_from_binary_file(filename)
    print_graph(num_vertices, num_edges, vertices, edges)
