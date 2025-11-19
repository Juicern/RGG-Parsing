# RGG Parsing
Modernized C++ backend for experimenting with Reserved Graph Grammars (RGG). The engine loads a host graph and a set of productions, repeatedly rewrites matching subgraphs, and visualizes the full reduction trace in an interactive browser view.

## Overview
Context-sensitive graph grammars, and RGG in particular, are powerful enough to capture the syntax of many diagrammatic and visual languages. Each production replaces an isomorphic subgraph (a *redex*) with another graph. The parser performs a selection-free search: it enumerates candidate subgraphs, checks label-preserving isomorphism (including isolated nodes), derives all valid redexes, and rewrites them until the initial graph is reached or the search space is exhausted.

The repository is intentionally small:

```
RGG-Backend/   # Core library + console entry point
RGG-TEST/      # Visual Studio test project (Graph/IO/visualization helpers)
host_graph.txt # Sample host graph
productions.txt# Sample productions
```

## Getting Started

### Prerequisites
- C++17 compatible toolchain (Visual Studio 2022, clang, or GCC).
- CMake is optional; the provided `.vcxproj` files can be opened directly in Visual Studio.

### Build & Run
1. Edit `host_graph.txt` and `productions.txt` to describe your visual language.
2. Compile the backend:
   - **Visual Studio**: open `RGG-Backend/RGG-Backend.vcxproj` and build the `RGG-Backend` target.
   - **Command line** (from the repo root):
     ```bash
     g++ -std=c++17 RGG-Backend/Graph.cpp RGG-Backend/read_graph.cpp RGG-Backend/show_graph.cpp RGG-Backend/RGG-Backend.cpp -o rgg-parser
     ```
3. Run the executable from the repository root (`./rgg-parser` or the Visual Studio output). The program prints whether the host graph belongs to the grammar and emits an interactive `process.html` timeline next to the binary.

### Visualization
The reduction trace uses [Cytoscape.js](https://js.cytoscape.org) for a modern, responsive layout. Every intermediate graph is rendered inside `process.html`; the helper function attempts to open the file with the default browser on Windows, macOS, and Linux. If automatic opening is blocked, open the file manually after a run.

## Data Structures
- **Vertex** – label (`'-'` denotes the “super vertex”) plus a unique mark used while reconnecting dangling edges.
- **Node** – unique id, terminal flag, semantic label, and up to a handful of vertices.
- **Edge** – id, mark (non-zero when it straddles deleted nodes), and the endpoint `Node/Vertex` pairs.
- **Graph** – vector of nodes and edges.
- **Production** – pair of graphs: left (replacement) and right (match target).

## Parsing Algorithms
1. **`find_redex`** – enumerates all edge combinations the size of the production’s graph, filters by label counts, and checks one-to-one node mappings (plus isolated nodes) to produce redexes.
2. **`replace_redex`** – deletes the matched nodes/edges, records dangling connections, re-ids the production graph to avoid collisions, splices it into the host graph, reconnects marked edges, and normalizes vertices.
3. **`reduce`** – recursively applies productions (guarding against duplicate edges via `is_graph_available`) until the initial graph is reached; returns the success flag and the ordered list of intermediate graphs.

## Testing
`RGG-TEST` contains Visual Studio unit tests for the core data structures and helpers. Build the `RGG-TEST` project or integrate the tests into your preferred framework to validate future changes.

---
Questions or ideas? File an issue or tweak the grammar files and re-run the parser to explore new visual languages.
