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
Edge::Edge() noexcept : id(NOID), mark(NOMARK){}
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
std::vector<Graph> find_redex(const Graph& host_graph, const Graph& sub_graph) {
	auto comb_edges = comb_edge(host_graph.edges, sub_graph.edges.size());
}

std::vector<std::vector<Edge>> comb_edge(const std::vector<Edge>& host_edges, int n) {
	std::vector<std::vector<Edge>> comb_edges;
	std::vector<Edge> cur_edges;
	comb_edge(host_edges, n, 0, comb_edges, cur_edges);
	return comb_edges;
}

void comb_edge(const std::vector<Edge>& host_edges, int n, int index, std::vector<std::vector<Edge>>& comb_edges, std::vector<Edge>& cur_edges) {
	if (n == 0) {
		comb_edges.emplace_back(cur_edges);
		return;
	}
	if (host_edges.size() - index < n) {
		return;
	}
	cur_edges.emplace_back(host_edges[index]);
	comb_edge(host_edges, n - 1, index + 1, comb_edges, cur_edges);
	cur_edges.pop_back();
	comb_edge(host_edges, n, index + 1, comb_edges, cur_edges);
}

// is_ok, host_edge2sub_edge, host_node2sub_node
std::tuple<bool, std::unordered_map<int, int>, std::unordered_map<int, int>> is_node_margin(const std::vector<Edge>& host_edges, const std::vector<Edge>& sub_edges) {
	// 1. label相同的node数量的一样
	auto host_lable_count = get_lable_count(host_edges);
	auto sub_lable_count = get_lable_count(sub_edges);
	for (const auto& [key, value] : host_lable_count) {
		if (sub_lable_count.count(key) == 0 || sub_lable_count[key] != value) {
			return { false, {}, {} };
		}
	}
	// 2. 再去匹配
	std::vector<Edge> temp_edges(host_edges.begin(), host_edges.end());
	do {
		auto [is_ok, host_node2sub_node] = is_one_to_one(host_edges, sub_edges);
		if (is_ok) {
			std::unordered_map<int, int> host_edge2sub_edge;
			for (int i = 0; i < host_edges.size(); ++i) {
				host_edge2sub_edge[host_edges[i].id] = sub_edges[i].id;
			}
			return { false,  host_edge2sub_edge,  host_node2sub_node };
		}
	} while (next_permutation(host_edges.begin(), host_edges.end(), [](const Edge& edge1, const Edge& edge2) {return edge1.id < edge2.id; }));
	return { false, {}, {} };
}

std::pair<bool, std::unordered_map<int, int>> is_one_to_one(const std::vector<Edge>& host_edges, const std::vector<Edge>& sub_edges) {
	std::unordered_map<int, int> host_node2sub_node;
	return is_one_to_one(host_edges, sub_edges, 0, host_node2sub_node);
}

std::pair<bool, std::unordered_map<int, int>> is_one_to_one(const std::vector<Edge>& host_edges, const std::vector<Edge>& sub_edges, int index, std::unordered_map<int, int>& host_node2sub_node) {
	if (index == host_edges.size()) {
		return { true, host_node2sub_node };
	}
	if (host_edges[index].node1.first.label == sub_edges[index].node1.first.label && host_edges[index].node2.first.label == sub_edges[index].node2.first.label) {
		if (host_node2sub_node.count(host_edges[index].node1.first.id) == 0) {
			host_node2sub_node[host_edges[index].node1.first.id] = sub_edges[index].node1.first.id;
		}
		else {
			if (host_node2sub_node[host_edges[index].node1.first.id] == sub_edges[index].node1.first.id) {
				if (host_node2sub_node.count(host_edges[index].node2.first.id) == 0) {
					host_node2sub_node[host_edges[index].node2.first.id] = sub_edges[index].node2.first.id;
				}
				else {
					if (host_node2sub_node[host_edges[index].node2.first.id] == sub_edges[index].node2.first.id) {
						auto [is_ok, m] = is_one_to_one(host_edges, sub_edges, index + 1, host_node2sub_node);
						if (is_ok) {
							return { true, m };
						}
					}
				}
			}
		}
		if (host_node2sub_node.count(host_edges[index].node1.first.id)) {
			host_node2sub_node.erase(host_edges[index].node1.first.id);
		}
		if (host_node2sub_node.count(host_edges[index].node2.first.id)) {
			host_node2sub_node.erase(host_edges[index].node2.first.id);
		}
	}
	if (host_edges[index].node1.first.label == sub_edges[index].node2.first.label && host_edges[index].node2.first.label == sub_edges[index].node1.first.label) {
		if (host_node2sub_node.count(host_edges[index].node1.first.id) == 0) {
			host_node2sub_node[host_edges[index].node1.first.id] = sub_edges[index].node2.first.id;
		}
		else {
			if (host_node2sub_node[host_edges[index].node1.first.id] == sub_edges[index].node2.first.id) {
				if (host_node2sub_node.count(host_edges[index].node2.first.id) == 0) {
					host_node2sub_node[host_edges[index].node2.first.id] = sub_edges[index].node1.first.id;
				}
				else {
					if (host_node2sub_node[host_edges[index].node2.first.id] == sub_edges[index].node1.first.id) {
						auto [is_ok, m] = is_one_to_one(host_edges, sub_edges, index + 1, host_node2sub_node);
						if (is_ok) {
							return { true, m };
						}
						
					}
				}
			}
		}
		if (host_node2sub_node.count(host_edges[index].node1.first.id)) {
			host_node2sub_node.erase(host_edges[index].node1.first.id);
		}
		if (host_node2sub_node.count(host_edges[index].node2.first.id)) {
			host_node2sub_node.erase(host_edges[index].node2.first.id);
		}
	}
	return { false, {} };
}


std::unordered_map<std::string, int> get_lable_count(const std::vector<Edge>& edges) {
	std::unordered_map<std::string, int> lable_count;
	std::unordered_set<int> used_ids;
	for (const auto& edge : edges) {
		if (used_ids.count(edge.node1.first.id) == 0) {
			lable_count[edge.node1.first.label]++;
		}
		if (used_ids.count(edge.node2.first.id) == 0) {
			lable_count[edge.node2.first.label]++;
		}
	}
	return lable_count;
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
	std::unordered_set<int> used_vertices{0};
	for (const auto& edge : redex.edges) {
		used_vertices.insert(edge.node1.second.mark);
		used_vertices.insert(edge.node2.second.mark);
	}
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
				//used_vertices.insert(it->mark);
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

