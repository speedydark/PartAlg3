import subprocess
import os
import platform
import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
import re

def normalize_path(path):
    """
    Normalize file paths to use the correct OS-specific separator.
    For example, on Windows it replaces forward slashes with backslashes,
    and on Unix it does the reverse if needed.
    """
    return os.path.normpath(path)

def run_cpp_program(input_file, output_file):
    """
    Compile and execute the C++ program that performs some processing on
    the input file, generating an output file and a graph file.

    :param input_file: Path to the input adjacency matrix file
    :param output_file: Path to the file where the C++ program writes its analysis
    """
    # Compile the C++ source file into an executable called 'pseudo_knots'.
    compile_cmd = ['g++', 'Pseudo-knots-corr5-01-19.cpp', '-o', 'pseudo_knots']
    subprocess.run(compile_cmd, check=True)

    # Choose the correct command depending on the OS (to handle Windows vs. Unix-like systems).
    run_cmd = (
        ['./pseudo_knots', input_file, output_file]
        if platform.system() != 'Windows'
        else ['pseudo_knots', input_file, output_file]
    )
    # Execute the compiled C++ program with the specified arguments.
    subprocess.run(run_cmd, check=True)

def extract_blocks_with_types(output_file):
    """
    Parse the output file produced by the C++ program to extract blocks of edges
    (pairs of vertices) and their associated types (e.g., 'Recursive PK', 'Regular', etc.).

    :param output_file: The file that contains the textual output from the C++ program
    :return: A tuple (blocks, block_types) where:
             - blocks is a list of lists; each sub-list contains (v1, v2) pairs.
             - block_types is a parallel list of block type strings.
    """
    blocks = []
    block_types = []
    current_block = []

    # Mapping from specific strings in the output file to human-readable labels.
    type_mapping = {
        "The block is a recursive PK": "Recursive PK",
        "this block represents a regular-region": "Regular",
        "this block represents a pseudoknot": "Non-recursive PK",
    }

    with open(output_file, 'r') as file:
        block_started = False
        for line in file:
            # Detect start of a new block.
            if "New Block" in line:
                # If there is an existing block being built, close it out first.
                if current_block:
                    blocks.append(current_block)
                    # If we didn't detect a type for the previous block, mark it as 'Unknown'.
                    if len(block_types) < len(blocks):
                        block_types.append("Unknown")
                current_block = []
                block_started = True

            # Capture the edges (v1, v2) once a block is in progress.
            elif block_started and re.search(r'\(\d+,\d+\)', line):
                matches = re.findall(r'\((\d+),(\d+)\)', line)
                for match in matches:
                    v1, v2 = int(match[0]), int(match[1])
                    current_block.append((v1, v2))

            # Check if the line contains any known type keywords and assign the appropriate label.
            elif any(phrase in line for phrase in type_mapping.keys()):
                for phrase, label in type_mapping.items():
                    if phrase in line:
                        block_types.append(label)
                        break

            # If "Summary information" appears, that means we are done reading blocks.
            elif "Summary information" in line:
                if current_block:
                    blocks.append(current_block)
                    if len(block_types) < len(blocks):
                        block_types.append("Unknown")
                break

    return blocks, block_types

def read_adjacency_matrix(input_file):
    """
    Read the adjacency matrix from the provided text file. Each line in the file
    should be a space-separated list of integers.

    :param input_file: Path to the file containing the adjacency matrix
    :return: A 2D list (list of lists) of integers representing the adjacency matrix
    """
    with open(input_file, 'r') as file:
        matrix = []
        for line in file:
            row = list(map(int, line.strip().split()))
            matrix.append(row)
    return matrix

def add_edges_from_matrix(G, matrix):
    """
    Add edges to the provided graph G based on a 2D adjacency matrix.
    If matrix[i][j] > 0, that means there is an edge between node i and node j.
    Multiple edges may be added for a higher weight if using a MultiGraph.

    :param G: A NetworkX MultiGraph (or Graph) object
    :param matrix: A 2D adjacency matrix where matrix[i][j] is the weight
    """
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            weight = matrix[i][j]
            # Only add edges if weight is positive, and skip self-loops (i != j).
            if weight > 0 and i != j:
                # If weight is more than 1, we add parallel edges (in a MultiGraph).
                for _ in range(weight):
                    G.add_edge(i, j, weight=weight)

def draw_combined_graph(matrix, blocks, block_types, G):
    """
    Draw a figure containing multiple "blocks" of edges. Each block is offset in
    the 2D plane so that blocks don't overlap. We use NetworkX to layout the edges
    separately and then combine them into one figure.

    :param matrix: The adjacency matrix (not directly used here except for reference)
    :param blocks: A list of edge lists, where each edge is a (v1, v2) tuple
    :param block_types: A parallel list of block types (e.g., 'Recursive PK', 'Regular')
    :param G: The original network (MultiGraph) containing all edges with their weights
    """

    pos = nx.spring_layout(G, seed=42)

    num_blocks = len(blocks)
    cmap = plt.colormaps['tab20']
    block_colors = [cmap(i / num_blocks) for i in range(num_blocks)]

    edge_colors = {}
    for i, edges in enumerate(blocks):
        block_color = block_colors[i]
        G.add_edges_from(edges)

        for edge in edges:
            edge_colors[edge] = block_color

    plt.figure(figsize=(12, 12))
    plt.title("Combined Graph", fontsize=16)

    nx.draw_networkx_nodes(G, pos, node_size=700, node_color="skyblue")

    for edge, color in edge_colors.items():
        nx.draw_networkx_edges(G, pos, edgelist=[edge], edge_color=[color], width=2)

    nx.draw_networkx_labels(G, pos, font_size=12, font_color="black")

    edge_labels = {(u, v): G[u][v][0]['weight'] for u, v in G.edges()}
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=10, font_color="red")

    legend_handles = []
    for i, block_type in enumerate(block_types):
        handle = plt.Line2D([], [], color=block_colors[i], label=block_type, linewidth=2)
        legend_handles.append(handle)

    plt.legend(
        handles=legend_handles, 
        title="Block Types", 
        loc="center left", 
        bbox_to_anchor=(1.00, 0.5), 
        fontsize=10
    )

    plt.tight_layout()
    plt.show()
    
    # pos = {}
    # offset = 1.8  # Used to shift each block in the layout so they don't overlap.

    # # Generate a layout for each block and store its positions in 'pos' dict.
    # for i, block in enumerate(blocks):
    #     # spring_layout calculates positions for a graph's nodes in a spring-force way.
    #     block_pos = nx.spring_layout(nx.Graph(block), seed=i)
    #     for node in block_pos:
    #         # Each node is named "node_block_index" to avoid collisions between blocks.
    #         pos[f"{node}_block_{i}"] = block_pos[node] + np.array([i * offset, 0])

    # # Prepare the figure
    # plt.figure(figsize=(12, 12))
    # plt.title("Graph with Separated Blocks and Weights", fontsize=16)

    # # Plot each block separately, offset vertically so they stack on the figure.
    # for i, (block, block_type) in enumerate(zip(blocks, block_types)):
    #     # Renaming original nodes (u, v) as (u_block_i, v_block_i) for clarity.
    #     block_edges = [(f"{u}_block_{i}", f"{v}_block_{i}") for u, v in block]

    #     # Create a local graph for just this block.
    #     G_block = nx.Graph()
    #     G_block.add_edges_from(block_edges)

    #     # Offset this entire block further up or down based on the block index (i).
    #     for node in G_block.nodes:
    #         pos[node] += np.array([0, i * offset])

    #     # Draw nodes and edges for this block.
    #     nx.draw_networkx_nodes(G_block, pos, node_size=700, node_color="skyblue")
    #     nx.draw_networkx_edges(G_block, pos, edgelist=block_edges, edge_color="black", width=2)

    #     # Create labels without the "_block_i" suffix, so they display as original node IDs.
    #     labels = {node: node.split("_")[0] for node in G_block.nodes}
    #     nx.draw_networkx_labels(G_block, pos, labels=labels, font_size=12, font_color="black")

    #     # Display edge weights taken from the main graph G.
    #     edge_labels = {
    #         (f"{u}_block_{i}", f"{v}_block_{i}"): G[u][v][0]['weight'] for u, v in block
    #     }
    #     nx.draw_networkx_edge_labels(G_block, pos, edge_labels=edge_labels, font_size=10, font_color="red")

    # # Adjust spacing and show the plotted figure.
    # plt.tight_layout()
    # plt.show()

if __name__ == "__main__":
    # Prompt the user for the file paths. If blank, use defaults in the current working directory.
    input_file = input("Enter the input file (or leave blank if you have a 'matrix.txt' in the current dir): ")
    output_file = input("Enter the output file (or leave blank to use the default): ")

    # If the user did not enter a path, default to 'matrix.txt' in the current directory.
    if not input_file:
        input_file = normalize_path(os.path.join(os.getcwd(), "matrix.txt"))
    else:
        input_file = normalize_path(input_file)

    # Similarly, default to 'matrix_out.txt' if no output file is specified.
    if not output_file:
        output_file = normalize_path(os.path.join(os.getcwd(), "matrix_out.txt"))
    else:
        output_file = normalize_path(output_file)

    # Make sure the input file actually exists before proceeding.
    if not os.path.exists(input_file):
        raise FileNotFoundError(f"Input file not found: {input_file}")

    # Run the external C++ program to generate output and graph data.
    run_cpp_program(input_file, output_file)

    # Verify that the output file was created.
    if not os.path.exists(output_file):
        raise FileNotFoundError(f"Output file not found: {output_file}")

    # Extract the blocks (subgraphs) and their types from the output.
    blocks, block_types = extract_blocks_with_types(output_file)

    # Read the original adjacency matrix.
    matrix = read_adjacency_matrix(input_file)

    # Create a MultiGraph to allow parallel edges in case of multiple weights between two nodes.
    G = nx.MultiGraph()
    add_edges_from_matrix(G, matrix)

    # Visualize each block in a combined layout.
    draw_combined_graph(matrix, blocks, block_types, G)
