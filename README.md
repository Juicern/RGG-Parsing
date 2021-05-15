# RGG Parsing
This is the backend code of RGG Parsing(Reserved Graph Grammar), written in C++, which mainly includes data structures, parsing algorithms and test. 

## What is RGG？

In reading the visual language literature, or any book on software engineering, one cannot help but notice that a large variety of visual languages exists of which only a few are equipped with a proper formal syntax definition. Given a syntax definition of a visual language one can easily imagine an editor which supports the creation of diagram according to the syntax. 

Context-free graph grammar, as we all know, has productions in which every left-hand side consists of a single non-terminal node. It is straightforward but awkward to define the syntax of a large portion of visual languages and too restrictive. Therefore we allow for *context-sensitive graph grammar* , in which both left- and right-hand side of production are graphs, even though the performance penalty might be serious.

RGG is the most popular context-sensitive graph grammar, which facilitates the complexity of LGG(Layered graph grammar) by adding some mechanisms.

## Parts

This project is divided into 3 parts:

* Data Structures
* Parsing algorithms
* Test

And the productions and host graph is a local file called productions.txt and host_graph.txt, separately. There is also a way to show the process of parsing. Please run and you will see this.

## Data Structures

The main data structure of this project is currently the following:

* Vertex
* Node
* Edge
* Graph
* Production

### Vertex

A vertex is a class which consists of mark and label, while mark indicates the only vertex in the node and label is the specific semantics of the vertex. vertex is only used in process of replacing redex and definition of production. Besides, there is also a definition for 2 vertices:

* super vertex: it has no specific semantics but is only default for every node. we set its mark to 0 to distinguish it from other vertex.
* simple vertex: it indicated which dangle edge should be connected with this vertex. 

The data structure code is listed below:

```C++
class Vertex {
public:
	Vertex() noexcept;
	Vertex(const char&, const int&);
	Vertex(const Vertex&);
	bool operator==(const Vertex&);
public:
	char label; // if label == '-' : superVertex, else : simple vertex 
	int mark; // 0 -> not marked, The marks of the vertices in a node are not the same
};
```

### Node

A node is a a class which consist of id, is_terminal, label and vertices. Id indicated the only node in the graph while is_terminal represents if it is terminal node and label is the specific semantics of the node. Considering basically a node only consists less than 5 nodes, we just set vertices as vector to represent all vertices.

The data structure code is listed below:

```C++
class Node {
public:
	Node() noexcept;
	Node(const int&, const bool&, const std::string&, const std::vector<Vertex>&);
	Node(const Node&);
	bool operator==(const Node&);
public:
	int id;
	bool is_terminal;
	std::string label;
	std::vector<Vertex> vertices;
};
```

### Edge

An Edge is  a class which consisting of id, mark, node1 and node2,. Id indicated the only edge in the graph while 2 nodes represent the node and vertex of connected 2 nodes. And mark is used to indicate the only vertex when embedding subgraph to host graph. 

The data structure code is listed below:

```C++
class Edge {
public:
	Edge() noexcept;
	Edge(const int&, const std::pair<Node, Vertex>&, const std::pair<Node, Vertex>&);
	Edge(const Edge&);
	bool operator==(const Edge&);
public:
	int id;
	int mark; // 0 means no delete node, while other values mean deleted node's vertex mark
	std::pair<Node, Vertex> node1;
	std::pair<Node, Vertex> node2;
};
```

### Graph 

A graph is a class which consisting all the nodes and edge.

The data structure code is listed below:

```C++
class Graph {
public:
	Graph() = default;
	Graph(const Graph&);
	Graph(const std::vector<Node>&, const std::vector<Edge>&);
public:
	std::vector<Node> nodes;
	std::vector<Edge> edges;
};
```

### Production

A production is a class which consisting l_graph and r_graph while l_graph represents the left graph and r_graph represents the right graph, respectively. 

The data structure code is listed below:

```c++
class Production {
public:
	Production() = default;
	Production(const Graph&, const Graph&);
public:
	Graph l_graph;
	Graph r_graph;
};
```



## Parsing Algorithms

The main algorithms of this project are listed below:

* How to find all the isomorphic graphs by a given graph? (`find_redex`)
* How to replace the redex by subgraph? (`replace_redex`)
* How to parse?(`parse`)

### find_redex

A redex is a subgraph in the host graph which is isomorphic to the right graph in a R-application or to the left graph in an L-application. in other words, redex is a isomorphism to a given subgraph in host graph.

Denote subgraph's edges size as m, host graph edges size as n. So in selection-free algorithm, we should just get all the combinations of n choosing m. For each combination, firstly we just judge if the combination's nodes size is equals to subgraph non-isolated nodes,. After that, we recursively judge all the permutation of this combination to find if one of permutation is match to the subgraph edges. And we also noted that there may be isolated nodes in subgraph. So we also need to get the same count nodes in rest of host graph nodes to try to match them. After getting are redex and the map that subgraph edge/node id maps to host graph edge/node id, we use this map to modify the id of node and edge, to prepare for the replacement of redex. Finally we add all the redexes to a vector to return.

 In summary, we should define following functions:

* `comb_elem`(take m elements from host graph, with no order, to implement combination)
* `is_node_matched`(judge if given combination edges' nodes is matched to the subgraph edges' nodes)
* `is_one_to_one`(judge if given permutation edges' nodes is matched to the subgraph edges' nodes)
* `handle_isolated_nodes`(judge if isolated nodes in subgraph can be matched to rest of host graph nodes)
* `get_redex_by_matched`(get a redex by a found graph and map of node and edge)

### replace_redex

The algorithm is separated to 2 parts:

1. Delete a redex from host graph and handle dangle edges. In this part we should delete all the edges and nodes of redex in host graph, and modify dangle edges' mark to indicate to the connected vertex.
2. Add subgraph to host graph and handle dangle edge to connect to the right vertex. In this part we should firstly modify all the edges/nodes id to avoid conflict to the host graph id. After that we add all the edges and nodes of subgraph to host graph. Then it's time to connect the dangle edges to the right vertices. Finally, remove all the vertices and mark on host graph edges and nodes.

In summary, we should define following functions:

* `delete_redex`(delete redex from host graph)
* `delete_redex_edges` (delete all the edges of redex in host graph)
* `delete_redex_nodes` (delete all the nodes of redex in host graph)
* `add_mark_on_edge` (modify dangle edges' mark to indicate to the connected vertex)
* `add_subgraph` (add subgraph to host graph)
* `modify_id_on_subgraph` (modify the id of edge/node in subgraph to avoid conflict to the host graph id)
* `add_subgraph_nodes` (add subgraph nodes to host graph)
* `add_subgraph_edges` (add subgraph edges to host graph)
* `connect_nodes_on_dangle_edges`(connect the dangle edges to the right vertices)
* `remove_vertex_on_edges_and_nodes` (remove all the vertices and mark on host graph edges and nodes)

### parse

Considering using selection-free algorithm, we traverse all the productions to try recursively. if the host graph is already initial graph, the graph is the language of this grammar. One important thing is the first production is only used in the last step, and we also need a function to judge if current graph is available.

In summary, we should define following functions:

`is_initial_graph` (judge if given graph is initial graph)

`is_graph_available` (judge if given graph is available)

## Test

Skip

