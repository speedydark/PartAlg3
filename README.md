# RNA Pseudoknot Analysis: Usage Guide

This repository contains two main components:

1. **C++ program** (`Pseudo-knots-corr5-01-19.cpp`)  
   - Detects and classifies “pseudoknot blocks” in RNA-like structures based on an adjacency matrix.
   - Outputs text describing the identified blocks (regular, pseudoknot, or recursive pseudoknot).

2. **Python script** (`graph.py`)  
   - Automates compilation and execution of the C++ program.
   - Reads the generated output and visualizes each block using NetworkX and Matplotlib.
   - Produces a combined graph visualization showing block groupings and edge weights.

This document shows you how to set up and use both components together. If you prefer, you can also run the C++ program by itself, but you will lose the Python visualization features.

## Table of Contents
1. [Prerequisites](#prerequisites)  
2. [Repository Structure](#repository-structure)  
3. [Installation](#installation)  
4. [How the Process Works](#how-the-process-works)  
5. [Usage (Quick Start)](#usage-quick-start)  
6. [Detailed Steps](#detailed-steps)  
   - [1. Preparing the Input Files](#1-preparing-the-input-files)  
   - [2. Running the Python Script](#2-running-the-python-script)  
   - [3. Interpreting the Output](#3-interpreting-the-output)  
7. [Example Adjacency Matrix File](#example-adjacency-matrix-file)  
8. [Troubleshooting](#troubleshooting)  

---

## Prerequisites

- **C++ Compiler**  
  You need a C++ compiler (such as `g++`) to compile `Pseudo-knots-corr5-01-19.cpp`.  
  - On most Linux and macOS systems, you can install via your package manager (e.g., `sudo apt-get install g++` on Ubuntu).
  - On Windows, install [MinGW](http://www.mingw.org/) or use another environment that provides `g++`.

- **Python 3.7+**  
  Make sure you have Python 3 installed.

- **Python packages**  
  The Python script uses:
  - `matplotlib`
  - `networkx`
  - `numpy`

  To install them, run:
  ```bash
  pip install matplotlib networkx numpy
  ```

---

## Repository Structure

```
.
├── Pseudo-knots-corr5-01-19.cpp   # C++ source
├── graph.py                       # Python script that compiles and runs the C++ program, then visualizes
├── matrix.txt                     # (Example) adjacency matrix input file for testing
├── matrix_out.txt                 # (Example) output file that the C++ program generates
└── README.md                      # This usage guide (you are reading now :D)
```

---

## Installation

1. **Clone or Download**  
   - Clone with Git:
     ```bash
     git clone https://github.com/jkiddin/PartAlg3.git
     ```
   - Or download the ZIP from the repo.

2. **Install Python packages**  

   Ensure you have all required packages installed:
   ```bash
   pip install matplotlib networkx numpy
   ```

3. **Check C++ compiler**  
   Make sure `g++` is accessible. For example:
   ```bash
   g++ --version
   ```
   If that shows a valid response, you should be ready to compile the C++ code.

---

## How the Process Works

1. **You provide an adjacency matrix** for your graph (RNA structure). This is typically stored in a text file (`matrix.txt` by our default).
2. **`graph.py`** will:
   1. Compile `Pseudo-knots-corr5-01-19.cpp` into an executable named `pseudo_knots`.
   2. Run that executable with the specified input/output paths.
   3. **The C++ program** reads the adjacency matrix and:
      - Detects pseudoknot structures (blocks).
      - Classifies them as:
        - **Regular region**  
        - **Pseudoknot** (non-recursive)  
        - **Recursive pseudoknot**
      - Summarizes the counts in an output file (e.g., `matrix_out.txt`).
   4. **Back in Python,** the script reads:
      - The same adjacency matrix.
      - The block definitions from `matrix_out.txt`.
   5. **The Python script** constructs separate subgraphs (blocks) and uses NetworkX to arrange them visually. Blocks are offset so that they don’t overlap. Finally, it shows a window with the combined graph.

---

## Usage (Quick Start)

1. Place your adjacency matrix in `matrix.txt`.
2. From the command line, run:
   ```bash
   python graph.py
   ```
3. When prompted:
   - **Input file**: Press Enter (to accept `matrix.txt`) or type another path.
   - **Output file**: Press Enter (to accept `matrix_out.txt`) or type another path.
4. Wait for the script to compile and run the C++ code. Then it will open a visualization window showing each discovered block in the adjacency matrix.

---

## Detailed Steps

### 1. Preparing the Input Files

- **Adjacency Matrix File**  
  A text file where each line corresponds to a row of the matrix, and each number in a row corresponds to the weight/edge count between two vertices.  
  - Example (for a 4×4 matrix):
    ```
    0 1 0 0
    1 0 1 0
    0 1 0 2
    0 0 2 0
    ```
  - Make sure the matrix size matches the number of vertices you will later provide in the C++ program’s prompt (`Please enter the number of vertices:`). In the default automated workflow, the script will also pass that number in.

### 2. Running the Python Script

- **Command:**  
  ```bash
  python graph.py
  ```
  or
  ```bash
  python3 graph.py
  ```
  if `python3` is the name of your Python 3 executable.

- **Prompts:**  
  1. **Input file path:**  
     - If you leave it blank, it uses `<current_dir>/matrix.txt`.  
  2. **Output file path:**  
     - If you leave it blank, it uses `<current_dir>/matrix_out.txt`.

- **What Happens:**  
  1. `graph.py` compiles `Pseudo-knots-corr5-01-19.cpp` using `g++ -o pseudo_knots`.
  2. It executes `pseudo_knots <input_file> <output_file>`.
  3. `pseudo_knots`:
     - Asks for the **number of vertices** (which must match your matrix size).
     - Reads in the adjacency matrix from `input_file`.
     - Writes analysis text to `output_file` describing each discovered block.
     - Prints a summary (regular vs. pseudoknot vs. recursive).
  4. After the C++ run completes, `graph.py`:
     - Reads your adjacency matrix again in Python.
     - Reads `output_file` for the block definitions.
     - Displays a combined graph using NetworkX and Matplotlib in a pop-up window.

### 3. Interpreting the Output

1. **In the console:**  
   - The C++ code may output lines such as:
     ```
        (0, 1) weight is 1
        (1, 2) weight is 1
        (2, 3) weight is 1
        (3, 0) weight is 1
        (3, 1) weight is 1
        (3, 4) weight is 1
        (4, 11) weight is 1
        (11, 1) weight is 1
        removed edges (0,1) and (1,2)
        these two edges are not a disconnecting set:
        removed edges (0,1) and (2,3)
        these two edges are not a disconnecting set:
        removed edges (0,1) and (3,0)
     ```
   - A final summary might look like:
     ``` 
        --------------------- Motif :PartAlg3 -----------------------------

        ===================== New Block ================== 

        (9,10) - (9,10) - 

        ---- this block represents a regular-region ---- 

        ===================== New Block ================== 

        (8,9) - (8,9) - 

        ---- this block represents a regular-region ---- 

        ===================== New Block ================== 

        (7,8) - (7,8) - 

        ---- this block represents a regular-region ---- 

        ===================== New Block ================== 

        (7,5) - (6,7) - (5,6) - 

        ---- this block represents a regular-region ---- 

        ===================== New Block ================== 

        (4,5) - (4,5) - 

        ---- this block represents a regular-region ---- 

        ===================== New Block ================== 

        (11,1) - (4,11) - (3,4) - (3,1) - (3,0) - (2,3) - (1,2) - (0,1) - 

        degree of 3 is 3
        degree of 3 is 4
        degree of 1 is 3
        degree of 1 is 4
        removed edges (0,1) and (3,0)
        these two edges are a disconnecting set:
        The block is a recursive PK.
        ---- this pseudoknot is recursive ---- 

        ----------- Summary information for Motif :PartAlg3 --------------------------------
        ----------- Total number of blocks: 6
        ----------- number of non-recursive PK blocks: 0
        ----------- number of recursive PK blocks: 1
        ----------- number of regular blocks : 5
     ```

2. **In the `output_file` (e.g., `matrix_out.txt`):**  
   - Similar block-by-block breakdowns, plus the same summary at the bottom.

3. **In the visualization window (Python):**  
   - Each subgraph or “block” is drawn in a separate cluster.  
   - Blocks are labeled with the node indices (e.g., `0`, `1`, etc.).  
   - Edges show their weight in red.  
   - You will see multiple clusters if the code identified multiple blocks.

---

## Example Adjacency Matrix File

Below is an example of a small adjacency matrix (`matrix.txt`) you can use to test the workflow. This represents a graph of 5 vertices (0 to 4):

```
0 2 0 0 1
2 0 1 0 0
0 1 0 1 0
0 0 1 0 2
1 0 0 2 0
```

Here’s how you might run the script with this matrix:

1. **Create `matrix.txt`** containing the 5×5 matrix above.
2. **Run**:  
   ```bash
   python graph.py
   ```
3. **Input file path?** Press Enter (to accept `matrix.txt`).  
4. **Output file path?** Press Enter (to accept `matrix_out.txt`).   
5. When the C++ program asks for the number of vertices, type `5`.  
6. Watch for blocks or pseudoknot detection, and then see the network visualization pop up.

---

## Troubleshooting

1. **`g++` not found**  
   - Make sure you have installed a C++ compiler (e.g., MinGW on Windows, `build-essential` on Ubuntu).
2. **Module not found (`networkx`, `matplotlib`, `numpy`)**  
   - Run `pip install networkx matplotlib numpy`.
3. **Graph window not showing**  
   - Sometimes interactive backends in Matplotlib can be slow to appear. Check if another window is behind your terminal. Otherwise, you may want to run `plt.show()` in an interactive environment (like Jupyter).
4. **Input file not found**  
   - Ensure you put the correct path when asked. If in doubt, place `matrix.txt` in the same directory as `graph.py`, then press Enter at the prompt without typing anything.
5. **Mismatch in number of vertices**  
   - The adjacency matrix size must match the integer you input in the C++ prompt. If not, the program may behave incorrectly or produce partial results.

---

