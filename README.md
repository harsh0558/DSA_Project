# DSA_Project

A C++ graph-based campus navigation system designed to model buildings and paths, then compute routes and connectivity using common data structures and algorithms.

## Overview

This project simulates a university or campus map where each building is a node and each walkway/path is an edge with a distance. The application lets users:

- add and remove buildings
- add and delete paths between buildings
- display the full graph
- find the shortest path using Dijkstra's algorithm
- find reachable buildings using DFS
- save graph data to a file and load it back later

## Core Features

- Graph representation using adjacency lists
- Building and path management
- Shortest path calculation between two buildings
- Reachability analysis from a starting building
- File persistence for campus data
- Menu-driven command-line interface

## Tech Stack

- C++
- Standard Template Library concepts and manual graph structures
- File I/O for saving/loading campus data

## Project Structure

```text
DSA_Project/
├── project.cpp
├── project_final.cpp
├── project.exe
├── campus.txt
├── large_graph.txt
├── Harsh/
└── README.md
```

## What the Program Does

The main program in `project.cpp` implements a campus navigation system with these menu options:

1. Add Building
2. Add Path
3. Delete Building
4. Delete Path
5. Display Graph
6. Find Shortest Path (Dijkstra)
7. Show Reachable Buildings (DFS)
8. Save to File
9. Load from File
0. Exit

This makes it a practical example of:

- adjacency list graph design
- graph traversal
- shortest path algorithm
- connected component / reachability analysis
- data persistence

## Compilation

Compile the project with:

```bash
g++ project.cpp -o campus_app
```

On Windows, you can also run the included executable:

```bash
project.exe
```

## Running

```bash
./campus_app
```

Then choose an option from the menu and interact with the campus map.

## Example Workflow

- Add buildings like `Library`, `Admin`, `Cafeteria`, `Hostel`
- Add paths between buildings with distances
- View the graph
- Ask for the shortest route from one building to another
- Save the data to a file such as `campus.txt`
- Load the data again later

## Notes

- `project_final.cpp` appears to be a later or alternate version of the same project.
- `campus.txt` and `large_graph.txt` are example graph datasets used for testing or demonstration.
- The implementation is command-line based and suitable for learning graph algorithms and data structures.

## Learning Outcomes

This project demonstrates several important DSA concepts:

- Graph modeling
- Weighted graph traversal
- Dijkstra's shortest path algorithm
- DFS-based reachability
- File-based persistence
- Dynamic data handling in C++

## License

No explicit license file was found in the repository. If this project is being reused or published, it is recommended to add a license and review usage rights.

## Author

@harsh0558
