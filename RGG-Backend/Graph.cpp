#include "Graph.h"

// Vertex
Vertex::Vertex() noexcept : label(NOLABEL_CHAR), mark(NOMARK) {}
Vertex::Vertex(const char& _label, const int& _mark) : label(_label), mark(_mark) {}
Vertex::Vertex(const Vertex& _vertex) : label(_vertex.label), mark(_vertex.mark) {}
bool Vertex::operator ==(const Vertex& v)
{
	return this->label == v.label;
}

// Node
Node::Node() noexcept : id(NOID), is_terminal(false), label(NOLABEL_STR) {}
Node::Node(const int& _id, const bool& _is_terminal, const std::string& _label, const std::vector<Vertex>& _vertices) : id(_id), is_terminal(_is_terminal), label(_label) {
	vertices.assign(_vertices.begin(), _vertices.end());
}
Node::Node(const Node& _node) : id(_node.id), is_terminal(_node.is_terminal), label(_node.label) {
	vertices.assign(_node.vertices.begin(), _node.vertices.end());
}
bool Node::operator ==(const Node& n) {
	return this->id == n.id;
}

// Edge
Edge::Edge() noexcept : id(NOID) {}
Edge::Edge(const int& _id, const std::pair<Node, Vertex>& _node1, const std::pair<Node, Vertex>& _node2) : id(_id), mark(0) {
	node1.first = _node1.first;
	node1.second = _node1.second;
	node2.first = _node2.first;
	node2.second = _node2.second;
}
Edge::Edge(const Edge& _edge) : id(_edge.id), mark(_edge.mark) {
	node1.first = _edge.node1.first;
	node1.second = _edge.node1.second;
	node2.first = _edge.node2.first;
	node2.second = _edge.node2.second;
}
bool Edge::operator ==(const Edge& e) {
	return this->id == e.id;
}


// Graph
Graph::Graph(const Graph & _graph) {
	nodes.assign(_graph.nodes.begin(), _graph.nodes.end());
	edges.assign(_graph.edges.begin(), _graph.edges.end());
}
Graph::Graph(const std::vector<Node>&_nodes, const std::vector<Edge>&_edges) : nodes(_nodes), edges(_edges) {}

// Production
Production::Production(const Graph& _l_graph, const Graph& _r_graph) : l_graph(_l_graph), r_graph(_r_graph) {}


// find the redex
std::vector<Graph> find_redex(Graph& host_graph, Graph& sub_graph) {
	throw - 1;
}

Graph replace_redex(const Graph& host_graph, const Graph& redex, const Graph& sub_graph) {
	Graph temp = host_graph;
	delete_redex(temp, redex);
	add_subgraph(temp, sub_graph);
	return temp;
}

std::vector<Graph> replace_redex(const Graph& host_graph, const std::vector<Graph>& redexes, const Graph& sub_graph) {
	std::vector<Graph> graphs;
	for (auto& redex : redexes) {
		graphs.push_back(replace_redex(host_graph, redex, sub_graph));
	}
	return graphs;
}

void delete_redex(Graph& host_graph, const Graph& redex) {
	delete_redex_edges(host_graph, redex);
	delete_redex_nodes(host_graph, redex);
	add_mark_on_edge(host_graph, redex);
}

void delete_redex_edges(Graph& host_graph, const Graph& redex) {
	std::unordered_set<int> edges_set;
	for (const auto& edge : redex.edges) {
		edges_set.insert(edge.id);
	}
	host_graph.edges.erase(
		std::remove_if(host_graph.edges.begin(), host_graph.edges.end(),
			[edges_set](Edge edge) -> bool {return edges_set.count(edge.id); }),
		host_graph.edges.end());
}

void delete_redex_nodes(Graph& host_graph, const Graph& redex) {
	// delete node
	std::unordered_set<int> nodes_set;
	for (const auto& node : redex.nodes) {
		nodes_set.insert(node.id);
	}
	host_graph.nodes.erase(
		std::remove_if(host_graph.nodes.begin(), host_graph.nodes.end(),
			[nodes_set](Node node) -> bool {return nodes_set.count(node.id); }),
		host_graph.nodes.end());
}

void add_mark_on_edge(Graph& host_graph, const Graph& redex) {
	// add mark on edge and change id on edge's node
	std::unordered_set<int> used_vertices;
	for (auto& host_edge : host_graph.edges) {
		for (const auto& redex_node : redex.nodes) {
			bool is_dangle = false;
			if (host_edge.node1.first == redex_node) {
				host_edge.node1.first.id = -1; // means this node is deleted
				is_dangle = true;
			}
			else if (host_edge.node2.first == redex_node) {
				host_edge.node2.first.id = -1; // means this node is deleted
				is_dangle = true;
			}
			if (is_dangle) {
				// find a vertex not be used before
				auto it = std::find_if(redex_node.vertices.begin(), redex_node.vertices.end(),
					[used_vertices](Vertex v) -> bool {return used_vertices.count(v.mark) == 0; });
				used_vertices.insert(it->mark);
				host_edge.mark = it->mark; // mark this vertex for connecting
			}
		}
	}
}

void add_subgraph(Graph& host_graph, const Graph& sub_graph) {
	connect_nodes_on_dangle_edges(host_graph, sub_graph);
	add_subgraph_nodes(host_graph, sub_graph);
	add_subgraph_edges(host_graph, sub_graph);
	remove_vertex_on_edges_and_nodes(host_graph);
	reset_id_on_edges_and_nodes(host_graph);
}

void connect_nodes_on_dangle_edges(Graph& host_graph, const Graph& sub_graph) {
	// put the marked vertex into map(mark == 0 means unmarked),
	// such that we can quickly find the connecting node
	std::unordered_map<int, std::pair<Node, Vertex>> vertex_mark2node;
	for (const auto& node : sub_graph.nodes){
		for (const auto& vertex : node.vertices) {
			if (vertex.mark != 0) {
				vertex_mark2node[vertex.mark] = std::make_pair(node, vertex);
			}
		}
	}
	for (auto& host_edge : host_graph.edges) {
		if (host_edge.mark != 0) {
			if (host_edge.node1.first.id == -1) {
				host_edge.node1 = vertex_mark2node[host_edge.node1.first.id];
			}
			else if (host_edge.node2.first.id == -1) {
				host_edge.node2 = vertex_mark2node[host_edge.node2.first.id];
			}
		}
	}
}

inline void add_subgraph_nodes(Graph& host_graph, const Graph& sub_graph) {
	host_graph.nodes.insert(host_graph.nodes.end(), sub_graph.nodes.begin(), sub_graph.nodes.end());
}

inline void add_subgraph_edges(Graph& host_graph, const Graph& sub_graph) {
	host_graph.edges.insert(host_graph.edges.end(), sub_graph.edges.begin(), sub_graph.edges.end());
}

void remove_vertex_on_edges_and_nodes(Graph& graph) {
	for (auto& edge : graph.edges) {
		edge.node1.second = {};
		edge.node2.second = {};
		edge.mark = 0;
	}
	for (auto& node : graph.nodes) {
		node.vertices = {};
	}
}

void reset_id_on_edges_and_nodes(Graph& graph) {
	int id = 0;
	for (auto& edge : graph.edges) {
		edge.id = id++;
	}
	id = 0;
	for (auto& node : graph.nodes) {
		node.id = id++;
	}
}

inline std::string get_host_graph_path() {
	return "../host_graph.txt";
}

inline std::string get_production_path() {
	return "../productions.txt";
}
