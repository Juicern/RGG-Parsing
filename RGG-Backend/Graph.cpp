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
std::vector<Graph> find_redex(const Graph& host_graph, 
							  const Graph& sub_graph) {
	std::vector<Graph> redexes;
	auto comb_edges = comb_elem(host_graph.edges, sub_graph.edges.size());
	for (const auto& comb_elem : comb_edges) {
		auto [is_ok, sub_edge2host_edge, sub_node2host_node] = is_node_matched(host_graph.edges, sub_graph.edges);
		if (is_ok) {
			// if there are isolated nodes
			if (sub_node2host_node.size() != sub_graph.nodes.size()) {
				auto [flag, isolated_id2host_ids] = handle_isolated_nodes(host_graph.nodes, sub_graph.nodes, sub_node2host_node);
				if (!flag) continue;
				for (const auto& isolated_id2host_id : isolated_id2host_ids) {
					// merge two map
					sub_node2host_node.insert(isolated_id2host_id.begin(), isolated_id2host_id.end());
					redexes.emplace_back(get_redex_by_matched(sub_graph, sub_edge2host_edge, sub_node2host_node));
					sub_node2host_node.erase(isolated_id2host_id.begin(), isolated_id2host_id.end());
				}
			}
			else {
				redexes.emplace_back(get_redex_by_matched(sub_graph, sub_edge2host_edge, sub_node2host_node));
			}
		}
	}
	return redexes;
}

Graph get_redex_by_matched(const Graph& sub_graph, 
						   std::unordered_map<int, int>& edge_map, 
						   std::unordered_map<int, int>& node_map) {
	auto redex(sub_graph);
	// modify edge id
	for (auto& edge : redex.edges) {
		edge.id = edge_map[edge.id];
	}
	// modify node id
	for (auto& node : redex.nodes) {
		node.id = node_map[node.id];
	}
	return redex;
}

template<class T>
std::vector<std::vector<T>> comb_elem(const std::vector<T>& host_elems, 
									  int n) {
	std::vector<std::vector<T>> comb_elems;
	std::vector<T> cur_elems;
	comb_elem<T>(host_elems, n, 0, comb_elems, cur_elems);
	return comb_elems;
}

template<class T>
void comb_elem(const std::vector<T>& host_elems, 
			   int n, 
			   int index, 
			   std::vector<std::vector<T>>& comb_elems, 
			   std::vector<T>& cur_elems) {
	if (n == 0) {
		comb_elems.emplace_back(cur_elems);
		return;
	}
	if (host_elems.size() - index < n) {
		return;
	}
	cur_elems.emplace_back(host_elems[index]);
	comb_elem(host_elems, n - 1, index + 1, comb_elems, cur_elems);
	cur_elems.pop_back();
	comb_elem(host_elems, n, index + 1, comb_elems, cur_elems);
}

// is_ok, sub_edge2host_edge, sub_node2host_node
std::tuple<bool, std::unordered_map<int, int>, std::unordered_map<int, int>> is_node_matched(const std::vector<Edge>& host_edges, 
																							 const std::vector<Edge>& sub_edges) {
	// 1. if their lable is same, the label count should be the same
	auto host_lable_count = get_lable_count(host_edges);
	auto sub_lable_count = get_lable_count(sub_edges);
	for (const auto& [key, value] : host_lable_count) {
		if (sub_lable_count.count(key) == 0 || sub_lable_count[key] != value) {
			return { false, {}, {} };
		}
	}
	// 2. check the match
	std::vector<Edge> temp_edges(host_edges.begin(), host_edges.end());
	std::sort(temp_edges.begin(), temp_edges.end(), [](const Edge& edge1, const Edge& edge2) {return edge1.id < edge2.id; });
	do {
		auto [is_ok, sub_node2host_node] = is_one_to_one(temp_edges, sub_edges);
		if (is_ok) {
			std::unordered_map<int, int> sub_edge2host_edge;
			for (int i = 0; i < temp_edges.size(); ++i) {
				sub_edge2host_edge[sub_edges[i].id] = temp_edges[i].id;
			}
			return { true,  sub_edge2host_edge,  sub_node2host_node };
		}
	} while (next_permutation(temp_edges.begin(), temp_edges.end(), [](const Edge& edge1, const Edge& edge2) { return edge1.id < edge2.id; }));
	return { false, {}, {} };
}

std::pair<bool, std::unordered_map<int, int>> is_one_to_one(const std::vector<Edge>& host_edges, 
															const std::vector<Edge>& sub_edges) {
	std::unordered_map<int, int> sub_node2host_node;
	return is_one_to_one(host_edges, sub_edges, 0, sub_node2host_node);
}

std::pair<bool, std::unordered_map<int, int>> is_one_to_one(const std::vector<Edge>& host_edges, 
															const std::vector<Edge>& sub_edges, 
															int index, 
															std::unordered_map<int, int>& sub_node2host_node) {
	if (index == host_edges.size()) {
		return { true, sub_node2host_node };
	}
	if (host_edges[index].node1.first.label == sub_edges[index].node1.first.label && host_edges[index].node2.first.label == sub_edges[index].node2.first.label) {
		if (sub_node2host_node.count(sub_edges[index].node1.first.id) == 0) {
			sub_node2host_node[sub_edges[index].node1.first.id] = host_edges[index].node1.first.id;
		}
		else {
			if (sub_node2host_node[sub_edges[index].node1.first.id] == host_edges[index].node1.first.id) {
				if (sub_node2host_node.count(sub_edges[index].node2.first.id) == 0) {
					sub_node2host_node[sub_edges[index].node2.first.id] = host_edges[index].node2.first.id;
				}
				else {
					if (sub_node2host_node[sub_edges[index].node2.first.id] == host_edges[index].node2.first.id) {
						auto [is_ok, m] = is_one_to_one(host_edges, sub_edges, index + 1, sub_node2host_node);
						if (is_ok) {
							return { true, m };
						}
					}
				}
			}
		}
		if (sub_node2host_node.count(sub_edges[index].node1.first.id)) {
			sub_node2host_node.erase(sub_edges[index].node1.first.id);
		}
		if (sub_node2host_node.count(sub_edges[index].node2.first.id)) {
			sub_node2host_node.erase(sub_edges[index].node2.first.id);
		}
	}
	if (host_edges[index].node1.first.label == sub_edges[index].node2.first.label && host_edges[index].node2.first.label == sub_edges[index].node1.first.label) {
		if (sub_node2host_node.count(sub_edges[index].node1.first.id) == 0) {
			sub_node2host_node[sub_edges[index].node1.first.id] = host_edges[index].node2.first.id;
		}
		else {
			if (sub_node2host_node[sub_edges[index].node1.first.id] == host_edges[index].node2.first.id) {
				if (sub_node2host_node.count(sub_edges[index].node2.first.id) == 0) {
					sub_node2host_node[sub_edges[index].node2.first.id] = host_edges[index].node1.first.id;
				}
				else {
					if (sub_node2host_node[sub_edges[index].node2.first.id] == host_edges[index].node1.first.id) {
						auto [is_ok, m] = is_one_to_one(host_edges, sub_edges, index + 1, sub_node2host_node);
						if (is_ok) {
							return { true, m };
						}
						
					}
				}
			}
		}
		if (sub_node2host_node.count(sub_edges[index].node1.first.id)) {
			sub_node2host_node.erase(sub_edges[index].node1.first.id);
		}
		if (sub_node2host_node.count(sub_edges[index].node2.first.id)) {
			sub_node2host_node.erase(sub_edges[index].node2.first.id);
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

std::pair<bool, std::vector<std::unordered_map<int, int>>> handle_isolated_nodes(const std::vector<Node>& host_nodes, 
																			     const std::vector<Node>& sub_nodes, 
																				 const std::unordered_map<int, int>& sub_node2host_node) {
	std::unordered_set<int> used_host_nodes;
	for (const auto& [_, id] : sub_node2host_node) {
		used_host_nodes.insert(id);
	}
	auto available_host_nodes(host_nodes);
	available_host_nodes.erase(
		std::remove_if(available_host_nodes.begin(), available_host_nodes.end(), 
			[used_host_nodes](Node node) {return used_host_nodes.count(node.id) != 0; }), 
		available_host_nodes.end());

	auto isolated_nodes(sub_nodes);
	isolated_nodes.erase(
		std::remove_if(isolated_nodes.begin(), isolated_nodes.end(),
			[sub_node2host_node](Node node) {return sub_node2host_node.count(node.id) != 0; }),
		isolated_nodes.end());
	auto [is_ok, isoalated_node2host_node] = is_isolated_node_margin(available_host_nodes, isolated_nodes);
	return is_isolated_node_margin(available_host_nodes, isolated_nodes);
}

std::pair<bool, std::vector<std::unordered_map<int, int>>> is_isolated_node_margin(const std::vector<Node>& available_host_nodes, 
																				   const std::vector<Node>& isolated_nodes) {
	std::unordered_map<std::string, std::vector<Node>> isolated_lable2nodes;
	std::unordered_map<std::string, std::vector<Node>> available_lable2nodes;
	for (const auto& node : isolated_nodes) {
		isolated_lable2nodes[node.label].emplace_back(node);
	}
	for (const auto& node : available_host_nodes) {
		available_lable2nodes[node.label].emplace_back(node);
	}
	// check
	for (const auto& [lable, nodes] : isolated_lable2nodes) {
		if (nodes.size() > available_lable2nodes[lable].size()) {
			return { false, {} };
		}
	}
	std::vector<std::unordered_map<int, int>> sub_id2host_ids;
	std::unordered_map<int, int> cur_sub_id2host_id;
	std::unordered_set<std::string> available_labels;
	std::unordered_map<std::string, std::vector<std::vector<Node>>> host_label2comb;
	for (const auto& [label, nodes] : available_lable2nodes) {
		host_label2comb[label] = comb_elem(nodes, isolated_lable2nodes[label].size());
		available_labels.insert(label);
	}
	get_isolated_matched_node_maps(host_label2comb, available_labels, isolated_nodes, cur_sub_id2host_id, sub_id2host_ids);
	return { true, sub_id2host_ids };
}

void get_isolated_matched_node_maps(std::unordered_map<std::string, std::vector<std::vector<Node>>>& host_lable2comb,
									std::unordered_set<std::string>& available_labels,
									const std::vector<Node>& isolated_nodes,
									std::unordered_map<int, int>& cur_sub_id2host_id, 
									std::vector<std::unordered_map<int, int>>& sub_id2host_ids) {
	if (available_labels.size() == 0) {
		sub_id2host_ids.emplace_back(cur_sub_id2host_id);
		return;
	}
	// find an available lable(so just choose begin()) 
	auto cur_label = *available_labels.begin();
	available_labels.erase(cur_label);
	for (const auto& nodes : host_lable2comb[cur_label]) {
		for (int i = 0; i < isolated_nodes.size(); ++i) {
			cur_sub_id2host_id[isolated_nodes[i].id] = nodes[i].id;
		}
		get_isolated_matched_node_maps(host_lable2comb, available_labels, isolated_nodes, cur_sub_id2host_id, sub_id2host_ids);
		for (int i = 0; i < isolated_nodes.size(); ++i) {
			cur_sub_id2host_id.erase(isolated_nodes[i].id);
		}
	}
	available_labels.insert(cur_label);
}

Graph replace_redex(const Graph& host_graph, 
					const Graph& redex, 
					const Graph& sub_graph) {
	Graph temp = host_graph;
	delete_redex(temp, redex);
	add_subgraph(temp, sub_graph);
	return temp;
}

std::vector<Graph> replace_redex(const Graph& host_graph, 
								 const std::vector<Graph>& redexes, 
								 const Graph& sub_graph) {
	std::vector<Graph> graphs;
	for (auto& redex : redexes) {
		graphs.push_back(replace_redex(host_graph, redex, sub_graph));
	}
	return graphs;
}

void delete_redex(Graph& host_graph, 
				  const Graph& redex) {
	delete_redex_edges(host_graph, redex);
	delete_redex_nodes(host_graph, redex);
	add_mark_on_edge(host_graph, redex);
}

void delete_redex_edges(Graph& host_graph, 
						const Graph& redex) {
	std::unordered_set<int> edges_set;
	for (const auto& edge : redex.edges) {
		edges_set.insert(edge.id);
	}
	host_graph.edges.erase(
		std::remove_if(host_graph.edges.begin(), host_graph.edges.end(),
			[edges_set](Edge edge){return edges_set.count(edge.id) != 0; }),
		host_graph.edges.end());
}

void delete_redex_nodes(Graph& host_graph, 
						const Graph& redex) {
	// delete node
	std::unordered_set<int> nodes_set;
	for (const auto& node : redex.nodes) {
		nodes_set.insert(node.id);
	}
	host_graph.nodes.erase(
		std::remove_if(host_graph.nodes.begin(), host_graph.nodes.end(),
			[nodes_set](Node node) {return nodes_set.count(node.id) != 0; }),
		host_graph.nodes.end());
}

void add_mark_on_edge(Graph& host_graph, 
					  const Graph& redex) {
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

void add_subgraph(Graph& host_graph, 
				  const Graph& sub_graph) {
	connect_nodes_on_dangle_edges(host_graph, sub_graph);
	add_subgraph_nodes(host_graph, sub_graph);
	add_subgraph_edges(host_graph, sub_graph);
	remove_vertex_on_edges_and_nodes(host_graph);
	reset_id_on_edges_and_nodes(host_graph);
}

void connect_nodes_on_dangle_edges(Graph& host_graph, 
								   const Graph& sub_graph) {
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

inline void add_subgraph_nodes(Graph& host_graph, 
							   const Graph& sub_graph) {
	host_graph.nodes.insert(host_graph.nodes.end(), sub_graph.nodes.begin(), sub_graph.nodes.end());
}

inline void add_subgraph_edges(Graph& host_graph, 
							   const Graph& sub_graph) {
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

