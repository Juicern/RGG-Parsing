#include "Graph.h"

// Vertex
Vertex::Vertex() noexcept : label(NOLABEL_CHAR), mark(NOMARK) {}
Vertex::Vertex(const char& _label, const int& _mark) : label(_label), mark(_mark) {}
Vertex::Vertex(const Vertex& _vertex) : label(_vertex.label), mark(_vertex.mark) {}

bool Vertex::operator==(const Vertex& v)
{
	return this->label == v.label;
}

// Node
Node::Node() noexcept : id(NOID), is_terminal(false), label(NOLABEL_STR) {}
Node::Node(const int& _id, const bool& _is_terminal, const string& _label, const vector<Vertex>& _vertices) : id(_id), is_terminal(_is_terminal), label(_label) {
	vertices.assign(_vertices.begin(), _vertices.end());
}
Node::Node(const Node& _node) : id(_node.id), is_terminal(_node.is_terminal), label(_node.label) {
	vertices.assign(_node.vertices.begin(), _node.vertices.end());
}

bool Node::operator==(const Node& n) {
	return this->id == n.id;
}

// Edge
Edge::Edge() noexcept : id(NOID) {}
Edge::Edge(const int& _id, const pair<Node, Vertex>& _node1, const pair<Node, Vertex>& _node2) : id(_id) {
	mark = make_pair(0, Vertex());
	node1.first = _node1.first;
	node1.second = _node1.second;
	node2.first = _node2.first;
	node2.second = _node2.second;
}
Edge::Edge(const Edge& _edge) : id(_edge.id) {
	mark.first = _edge.mark.first;
	mark.second = _edge.mark.second;
	node1.first = _edge.node1.first;
	node1.second = _edge.node1.second;
	node2.first = _edge.node2.first;
	node2.second = _edge.node2.second;
}

bool Edge::operator==(const Edge& e) {
	return this->id == e.id;
}

// Graph
Graph::Graph(const Graph & _graph) {
	nodes.assign(_graph.nodes.begin(), _graph.nodes.end());
	edges.assign(_graph.edges.begin(), _graph.edges.end());
}
Graph::Graph(const vector<Node>&_nodes, const vector<Edge>&_edges) : nodes(_nodes), edges(_edges) {}

// Production
Production::Production(const Graph& _l_graph, const Graph& _r_graph) : l_graph(_l_graph), r_graph(_r_graph) {}

// find the redex
vector<Graph> find_redex(Graph& host_graph, Graph& sub_graph, vector<Production>& production) {
	throw - 1;
}

void delete_redex(Graph& host_graph, const Graph& redex) {
	// add mark on edge and node
	for (auto& host_edge : host_graph.edges) {
		for (auto& redex_node : redex.nodes) {
			if (host_edge.node1.first.id == redex_node.id) {
				// this edge needs to be removed
				if (host_edge.mark.first != 0) {
					host_edge.mark.first = -1;
				}
				// this edge becomes a dangle edge
				else {
					// record the deleted node(for sub_graph to connect)
					host_edge.mark.first = host_edge.node1.first.id;
					host_edge.mark.second = host_edge.node1.second;
					host_edge.node1.first.id = -1; // node1 should be removed in the redex
				}
			}
			if (host_edge.node2.first.id == redex_node.id) {
				// this edge needs to be removed
				if (host_edge.mark.first != 0) {
					host_edge.mark.first = -1;
				}
				// this edge becomes a dangle edge
				else {
					// record the deleted node(for sub_graph to connect)
					host_edge.mark.first = host_edge.node2.first.id;
					host_edge.mark.second = host_edge.node2.second;
					host_edge.node2.first.id = -1; // node2 should be removed in the redex
				}
			}
		}
	}
	// delete edges from redex
	unordered_set<int> edge_ids;
	for (const auto& edge : redex.edges) {
		edge_ids.insert(edge.id);
	}
	for (auto p = host_graph.edges.begin(); p != host_graph.edges.end();) {
		if (edge_ids.find(p->id) != edge_ids.end()) {
			p = host_graph.edges.erase(p);
		}
		else {
			++p;
		}
	}
	// delete edges which 2 nodes is removed (mark == -1)
	for (auto p = host_graph.edges.begin(); p != host_graph.edges.end();) {
		if (p->mark.first == -1) {
			p = host_graph.edges.erase(p);
		}
		else {
			++p;
		}
	}
	// delete nodes from redex
	unordered_set<int> node_ids;
	for (const auto& node : redex.nodes) {
		node_ids.insert(node.id);
	}
	for (auto p = host_graph.nodes.begin(); p != host_graph.nodes.end();) {
		if (node_ids.find(p->id) != node_ids.end()) {
			p = host_graph.nodes.erase(p);
		}
		else {
			++p;
		}
	}
}

void add_sub_graph(Graph& deleted_graph, const Graph& sub_graph) {
	// add nodes
	deleted_graph.nodes.insert(deleted_graph.nodes.end(), sub_graph.nodes.begin(), sub_graph.nodes.end());

	// handle dangle edge
	unordered_map<int, Node> nodes;
	for (const auto& node : sub_graph.nodes) {
		nodes[node.id] = node;
	}
	for (auto& edge : deleted_graph.edges) {
		if (edge.mark.first != 0) {
			if (nodes.find(edge.mark.first) != nodes.end()) {
				if (edge.node1.first.id == -1) {
					edge.node1.first = nodes[edge.mark.first];
					edge.node1.second = edge.mark.second;
				}
				else if (edge.node2.first.id == -1) {
					edge.node2.first = nodes[edge.mark.first];
					edge.node2.second = edge.mark.second;
				}
			}
			else {
				edge.mark = make_pair(-1, Vertex());
			}
		}
	}
	// delete –¸±ﬂ
	for (auto p = deleted_graph.edges.begin(); p != deleted_graph.edges.end();) {
		if (p->mark.first == -1) {
			// need to be deleted
			p = deleted_graph.edges.erase(p);
		}
		else {
			p->mark.first = 0;
			++p;
		}
	}
	// add edges
	deleted_graph.edges.insert(deleted_graph.edges.end(), sub_graph.edges.begin(), sub_graph.edges.end());
}

vector<Graph> replace_redex(const Graph& host_graph, const vector<Graph>& redexes, const Graph& sub_graph) {
	vector<Graph> graphs;
	for (auto& redex : redexes) {
		graphs.push_back(replace_redex(host_graph, redex, sub_graph));
	}
	return graphs;
}
Graph replace_redex(const Graph& host_graph, const Graph& redex, const Graph& sub_graph) {
	Graph temp = host_graph;
	delete_redex(temp, redex);
	add_sub_graph(temp, sub_graph);
	return temp;
}
// “‘œ¬Œ¥≤‚ ‘
vector<Vertex> get_vertices(const vector<char>& labels, const vector<int>& marks) {
	auto n = labels.size();
	vector<Vertex> vertices;
	for (int i = 0; i < n; ++i) {
		vertices.push_back(Vertex(labels[i], marks[i]));
	}
	return vertices;
}

vector<Node> get_nodes(const vector<int>& ids, const vector<bool>& flags, const vector<string>& labels, const vector<vector<Vertex>>& vertices) {
	auto n = labels.size();
	vector<Node> nodes;
	for (int i = 0; i < n; ++i) {
		nodes.push_back(Node(ids[i], flags[i], labels[i], vertices[i]));
	}
	return nodes;
}

vector<Edge> get_edges(const vector<int>& ids, const vector<Node>& node1s, const vector<Vertex>& vertex1s, const vector<Node>& node2s, vector<Vertex>& vertex2s) {
	auto n = ids.size();
	vector<Edge> edges;
	for (int i = 0; i < n; ++i) {
		edges.push_back(Edge(ids[i], { node1s[i], vertex1s[i] }, { node2s[i], vertex2s[i] }));
	}
	return edges;
}

vector<Edge> get_edges(const vector<int>& ids, const vector<pair<Node, Vertex>>& node1s, const vector<pair<Node, Vertex>>& node2s) {
	auto n = ids.size();
	vector<Edge> edges;
	for (int i = 0; i < n; ++i) {
		edges.push_back(Edge(ids[i], { node1s[i].first, node2s[i].second }, { node2s[i].first, node2s[i].second }));
	}
	return edges;
}

