#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <regex>
#include <iterator>
constexpr auto NOLABEL_CHAR = '-';
constexpr auto NOLABEL_STR = "-";
constexpr auto NOMARK = -1;
constexpr auto NOID = -1;

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


class Edge {
public:
	Edge() noexcept;
	Edge(const int&, const std::pair<Node, Vertex>&, const std::pair<Node, Vertex>&);
	Edge(const Edge&);
	bool operator==(const Edge&);
public:
	int id;
	int mark; // 不需要删除：0， 悬边：连接的被删顶点的id， 需要被删的边： -1
	std::pair<Node, Vertex> node1;
	std::pair<Node, Vertex> node2;
};

class Graph {
public:
	Graph() = default;
	Graph(const Graph&);
	Graph(const std::vector<Node>&, const std::vector<Edge>&);
public:
	std::vector<Node> nodes;
	std::vector<Edge> edges;
};

class Production {
public:
	Production() = default;
	Production(const Graph&, const Graph&);
public:
	Graph l_graph;
	Graph r_graph;
};
std::vector<Graph> find_redex(const Graph&, const Graph&, const std::vector<Production>&);
std::vector<Graph> replace_redex(const Graph&, const std::vector<Graph>&, const Graph&);
Graph replace_redex(const Graph&, const Graph&, const Graph&);
void delete_redex(Graph&, const Graph&);
void delete_redex_edges(Graph&, const Graph&);
void delete_redex_nodes(Graph&, const Graph&);
void add_mark_on_edge(Graph&, const Graph&);
void add_subgraph(Graph&, const Graph&);
void connect_nodes_on_dangle_edges(Graph&, const Graph&);
inline void add_subgraph_nodes(Graph&, const Graph&);
inline void add_subgraph_edges(Graph&, const Graph&);
void remove_vertex_on_edges_and_nodes(Graph&);
void reset_id_on_edges_and_nodes(Graph&);
