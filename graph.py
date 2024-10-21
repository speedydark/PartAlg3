import subprocess
import matplotlib.pyplot as plt
import networkx as nx
import re

def run_cpp_program(input_file, output_file):
    compile_cmd = ['g++', 'Pseudo-knots-corr5-01-19.cpp', '-o', 'pseudo_knots']
    subprocess.run(compile_cmd, check=True)
    
    run_cmd = ['./pseudo_knots']
    process = subprocess.Popen(run_cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    
    vertices = input("Enter the number of vertices: ")
    motif_id = input("Enter the motif ID: ")
    
    process.communicate(f"{vertices}\n{motif_id}\n".encode())

def extract_edges_from_output(output_file):
    edges = []
    with open(output_file, 'r') as file:
        lines = file.readlines()
        block_started = False
        for line in lines:
            if "New Block" in line:
                block_started = True
            elif block_started and re.match(r'\(\d+,\d+\)', line):
                matches = re.findall(r'\((\d+),(\d+)\)', line)
                for match in matches:
                    v1, v2 = int(match[0]), int(match[1])
                    edges.append((v1, v2))
            elif "Summary information" in line:
                break
    return edges

def draw_graph(edges):
    G = nx.Graph()
    G.add_edges_from(edges)
    pos = nx.spring_layout(G)
    nx.draw_networkx_nodes(G, pos, node_size=700)
    nx.draw_networkx_edges(G, pos)
    nx.draw_networkx_labels(G, pos)
    plt.show()

input_file = "/Users/just/Documents/petingi/PKB236.txt" 
output_file = "/Users/just/Documents/petingi/PKB236_out.txt"  

run_cpp_program(input_file, output_file)
edges = extract_edges_from_output(output_file)
draw_graph(edges)
