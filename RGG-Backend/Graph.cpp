/*****************************************************************************
***
Copyright 2021 by Juicern(JuicernChu@outlook.com).
All rights reserved.

Permission to use, copy, modifyand distribute this software for personaland educational use is hereby granted without fee, provided that the above copyright notice appears in all copiesand that both that copyright noticeand this permission notice appear in supporting documentation, and that the names of Addison Wesley Longman or the author are not used in advertising or publicity pertaining to distribution of the software without specific, written prior permission.Addison Wesley Longmanand the author make no representations about the suitability of this software for any purpose.It is provided "as is" without express or implied warranty.

ADDISON WESLEY LONGMAN AND THE AUTHOR DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.IN NO EVENT SHALL ADDISON WESLEY LONGMAN OR THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
***
******************************************************************************/

#pragma once
#include "Graph.h"
#include "show_graph.h"

// Vertex
Vertex::Vertex() noexcept : label(NOLABEL_CHAR), mark(NOMARK) {}
Vertex::Vertex(const char& _label, const int& _mark) : label(_label), mark(_mark) {}
Vertex::Vertex(const Vertex& _vertex) : label(_vertex.label), mark(_vertex.mark) {}
bool Vertex::operator ==(const Vertex& v){return this->label == v.label;}

// Node
Node::Node() noexcept : id(NOID), is_terminal(false), label(NOLABEL_STR) {}
Node::Node(const int& _id, const bool& _is_terminal, const std::string& _label, const std::vector<Vertex>& _vertices) : id(_id), is_terminal(_is_terminal), label(_label), vertices(_vertices.begin(), _vertices.end()) {}
Node::Node(const Node& _node) : id(_node.id), is_terminal(_node.is_terminal), label(_node.label), vertices(_node.vertices.begin(), _node.vertices.end()) {}
bool Node::operator ==(const Node& n) {return this->id == n.id;}

// Edge
Edge::Edge() noexcept : id(NOID), mark(NOMARK){}
Edge::Edge(const int& _id, const std::pair<Node, Vertex>& _node1, const std::pair<Node, Vertex>& _node2) : id(_id), mark(0), node1(_node1), node2(_node2) {}
Edge::Edge(const Edge& _edge) : id(_edge.id), mark(_edge.mark), node1(_edge.node1), node2(_edge.node2) {}
bool Edge::operator ==(const Edge& e) {return this->id == e.id;}



// Graph
Graph::Graph(const Graph & _graph): nodes(_graph.nodes), edges(_graph.edges) {}
Graph::Graph(const std::vector<Node>&_nodes, const std::vector<Edge>&_edges) : nodes(_nodes), edges(_edges) {}

// Production
Production::Production(const Graph& _l_graph, const Graph& _r_graph) : l_graph(_l_graph), r_graph(_r_graph) {}

/// <summary>
/// find redexes(part of host graph) which is isomorphic to the subgraph
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="sub_graph">subgraph</param>
/// <returns>multiple redexes</returns>
std::vector<Graph> find_redex(const Graph& host_graph, 
							  const Graph& sub_graph) {
	std::vector<Graph> redexes;
	// take n edges from host_graph.edges (n is the count of sub_graph.edges)
	auto comb_edges = comb_elem(host_graph.edges, sub_graph.edges.size());
	// traverse all combinations to find redex
	for (const auto& comb_elem : comb_edges) {
		// judge if this combination is a redex, node and edge map which mapping subgraph node/edge id to host graph node/edge id
		auto [is_ok, sub_edge2host_edge, sub_node2host_node] = is_node_matched(comb_elem, sub_graph.edges);
		if (is_ok) {
			// if there are isolated nodes
			if (sub_node2host_node.size() != sub_graph.nodes.size()) {
				// judege if any part of rest host graph(substract the redex) can be isomorphic to these isolated nodes
				// and get the node map which mapping isolated node id to host graph node id
				auto [flag, isolated_id2host_ids] = handle_isolated_nodes(host_graph.nodes, sub_graph.nodes, sub_node2host_node);
				if (!flag) continue;
				for (const auto& isolated_id2host_id : isolated_id2host_ids) {
					// merge two maps
					for (const auto& [key, value] : isolated_id2host_id) {
						sub_node2host_node[key] = value;
					}
					redexes.emplace_back(get_redex_by_matched(sub_graph, sub_edge2host_edge, sub_node2host_node));
					// recover
					for (const auto& [key, value] : isolated_id2host_id) {
						sub_node2host_node.erase(key);
					}
				}
			}
			else {
				redexes.emplace_back(get_redex_by_matched(sub_graph, sub_edge2host_edge, sub_node2host_node));
			}
		}
	}
	return redexes;
}

/// <summary>
/// get a redex by using matched edges and nodes 
/// </summary>
/// <param name="sub_graph"> sub graph </param>
/// <param name="edge_map"> sub graph edge id to host graph edge id </param>
/// <param name="node_map"> sub graph node is to host graph node id </param>
/// <returns> redex </returns>
Graph get_redex_by_matched(const Graph& sub_graph, 
						   std::unordered_map<int, int>& edge_map, 
						   std::unordered_map<int, int>& node_map) {
	auto redex(sub_graph);
	// modify edge
	for (auto& edge : redex.edges) {
		edge.id = edge_map[edge.id];
		edge.node1.first.id = node_map[edge.node1.first.id];
		edge.node2.first.id = node_map[edge.node2.first.id];
	}
	// modify node
	for (auto& node : redex.nodes) {
		node.id = node_map[node.id];
	}
	return redex;
}

/// <summary>
/// Implement combination (take n elements from host_elems, with no order)
/// </summary>
/// <typeparam name="T">the type of elements</typeparam>
/// <param name="host_elems">elements need to be take</param>
/// <param name="n">amount to be taken</param>
/// <returns>all combinations</returns>
template<class T>
std::vector<std::vector<T>> comb_elem(const std::vector<T>& host_elems, 
									  int n) {
	std::vector<std::vector<T>> comb_elems;
	std::vector<T> cur_elems;
	// get combinations recursively
	comb_elem<T>(host_elems, n, 0, comb_elems, cur_elems);
	return comb_elems;
}

/// <summary>
/// recursive version of comb_elem (call by non-recursive version)
/// </summary>
/// <typeparam name="T">the type of elements</typeparam>
/// <param name="host_elems">elements need to be take</param>
/// <param name="n">amount to be taken</param>
/// <param name="index">current index of element</param>
/// <param name="comb_elems">all combinations</param>
/// <param name="cur_elems">current combination</param>
template<class T>
void comb_elem(const std::vector<T>& host_elems, 
			   int n, 
			   int index, 
			   std::vector<std::vector<T>>& comb_elems, 
			   std::vector<T>& cur_elems) {
	if (n == 0) {
		// add current combination to combinations
		comb_elems.emplace_back(cur_elems);
		return;
	}
	// not enough elements to form a combination
	if (host_elems.size() - index < n) {
		return;
	}
	// traverse:
	// 1. put current element into combination
	cur_elems.emplace_back(host_elems[index]);
	comb_elem(host_elems, n - 1, index + 1, comb_elems, cur_elems);
	cur_elems.pop_back();
	// 2. not put
	comb_elem(host_elems, n, index + 1, comb_elems, cur_elems);
}

/// <summary>
/// judge if node can be matched, and return the mappings of edges and nodes
/// </summary>
/// <param name="host_edges">host graph edges</param>
/// <param name="sub_edges">sub graph edges</param>
/// <returns>is matched, a map subgraph edge id mapping to host graph edge id, a map subgraph node id mapping to host graph node id</returns>
std::tuple<bool, std::unordered_map<int, int>, std::unordered_map<int, int>> is_node_matched(const std::vector<Edge>& host_edges, 
																							 const std::vector<Edge>& sub_edges) {
	// if their lable is same, the label count should be the same. Otherwise return false
	auto host_lable_count = get_lable_count(host_edges);
	auto sub_lable_count = get_lable_count(sub_edges);
	for (const auto& [key, value] : host_lable_count) {
		if (sub_lable_count.count(key) == 0 || sub_lable_count[key] != value) {
			return { false, {}, {} };
		}
	}
	// traverse all the permunation, find if node is one to one matched by order
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

/// <summary>
/// judge if the nodes can be one to one matched by order
/// </summary>
/// <param name="host_edges">host graph edges</param>
/// <param name="sub_edges">host graph nodes</param>
/// <returns>is one to one matched, map subgraph node mapping to host graph node</returns>
std::pair<bool, std::unordered_map<int, int>> is_one_to_one(const std::vector<Edge>& host_edges, 
															const std::vector<Edge>& sub_edges) {
	std::unordered_map<int, int> sub_node2host_node;
	return is_one_to_one(host_edges, sub_edges, 0, sub_node2host_node);
}

/// <summary>
/// recursive version of is_one_to_one (call by non_recursive version)
/// </summary>
/// <param name="host_edges">host graph edges</param>
/// <param name="sub_edges">host graph edges</param>
/// <param name="index">current index of edge</param>
/// <param name="sub_node2host_node"></param>
/// <returns>is one to one, map subgraph node mapping to host graph node</returns>
std::pair<bool, std::unordered_map<int, int>> is_one_to_one(const std::vector<Edge>& host_edges, 
															const std::vector<Edge>& sub_edges, 
															int index, 
															std::unordered_map<int, int>& sub_node2host_node) {
	if (index == host_edges.size()) {
		return { true, sub_node2host_node };
	}
	// judge if the record is same to the old
	auto is_same_to_old_record = [](std::unordered_map<int, int>& m, int id1, int id2) {
		if (m.count(id1) == 0 || m[id1] == id2) {
			return true;
		}
		return false;
	};
	// get to next recursion if the node lable is matched and the record is not diffrent to the old record
	if (host_edges[index].node1.first.label == sub_edges[index].node1.first.label && host_edges[index].node2.first.label == sub_edges[index].node2.first.label) {
		if (is_same_to_old_record(sub_node2host_node, sub_edges[index].node1.first.id, host_edges[index].node1.first.id)
			&& is_same_to_old_record(sub_node2host_node, sub_edges[index].node2.first.id, host_edges[index].node2.first.id)) {
			sub_node2host_node[sub_edges[index].node1.first.id] = host_edges[index].node1.first.id;
			sub_node2host_node[sub_edges[index].node2.first.id] = host_edges[index].node2.first.id;
			auto [is_ok, m] = is_one_to_one(host_edges, sub_edges, index + 1, sub_node2host_node);
			if (is_ok) {
				return { true, m };
			}
			sub_node2host_node.erase(sub_edges[index].node1.first.id);
			sub_node2host_node.erase(sub_edges[index].node2.first.id);
		}
	}
	if (host_edges[index].node1.first.label == sub_edges[index].node2.first.label && host_edges[index].node2.first.label == sub_edges[index].node1.first.label) {
		if (is_same_to_old_record(sub_node2host_node, sub_edges[index].node2.first.id, host_edges[index].node1.first.id)
			&& is_same_to_old_record(sub_node2host_node, sub_edges[index].node1.first.id, host_edges[index].node2.first.id)) {
			sub_node2host_node[sub_edges[index].node2.first.id] = host_edges[index].node1.first.id;
			sub_node2host_node[sub_edges[index].node1.first.id] = host_edges[index].node2.first.id;
			auto [is_ok, m] = is_one_to_one(host_edges, sub_edges, index + 1, sub_node2host_node);
			if (is_ok) {
				return { true, m };
			}
			sub_node2host_node.erase(sub_edges[index].node2.first.id);
			sub_node2host_node.erase(sub_edges[index].node1.first.id);
		}
	}
	return { false, {} };
}

/// <summary>
/// get the amount of each node label
/// </summary>
/// <param name="edges">all the edges</param>
/// <returns>map node label mapping to amount</returns>
std::unordered_map<std::string, int> get_lable_count(const std::vector<Edge>& edges) {
	std::unordered_map<std::string, int> lable_count;
	std::unordered_set<int> used_ids;
	for (const auto& edge : edges) {
		if (used_ids.count(edge.node1.first.id) == 0) {
			lable_count[edge.node1.first.label]++;
			used_ids.insert(edge.node1.first.id);
		}
		if (used_ids.count(edge.node2.first.id) == 0) {
			lable_count[edge.node2.first.label]++;
			used_ids.insert(edge.node2.first.id);
		}
	}
	return lable_count;
}

/// <summary>
/// handle isolated nodes in sub graph, judge if it can be matched to available nodes in host graph
/// </summary>
/// <param name="host_nodes">host graph nodes</param>
/// <param name="sub_nodes">subgraph nodes</param>
/// <param name="sub_node2host_node">existing map that subgraph node mapping to host graph node</param>
/// <returns>can_be_matched, maps with all possible matched combinations</returns>
std::pair<bool, std::vector<std::unordered_map<int, int>>> handle_isolated_nodes(const std::vector<Node>& host_nodes, 
																			     const std::vector<Node>& sub_nodes, 
																				 const std::unordered_map<int, int>& sub_node2host_node) {
	// remove used host graph nodes(in redex)
	auto available_host_nodes(host_nodes);
	std::unordered_set<int> used_host_nodes;
	for (const auto& [_, id] : sub_node2host_node) {
		used_host_nodes.insert(id);
	}
	available_host_nodes.erase(
		std::remove_if(available_host_nodes.begin(), available_host_nodes.end(), 
			[used_host_nodes](Node node) {return used_host_nodes.count(node.id) != 0; }), 
		available_host_nodes.end());
	// remove used subgraph nodes(in redex)
	auto isolated_nodes(sub_nodes);
	isolated_nodes.erase(
		std::remove_if(isolated_nodes.begin(), isolated_nodes.end(),
			[sub_node2host_node](Node node) {return sub_node2host_node.count(node.id) != 0; }),
		isolated_nodes.end());
	
	auto [is_ok, isoalated_node2host_node] = is_isolated_node_matched(available_host_nodes, isolated_nodes);
	return is_isolated_node_matched(available_host_nodes, isolated_nodes);
}
/// <summary>
/// judge if all the isolated nodes in sub graph can be matched to the available nodes in host graph
/// </summary>
/// <param name="available_host_nodes"> available host graph nodes</param>
/// <param name="isolated_nodes">isolated nodes in subgraph</param>
/// <returns>can_be_matched, maps with all possible matched combinations</returns>
std::pair<bool, std::vector<std::unordered_map<int, int>>> is_isolated_node_matched(const std::vector<Node>& available_host_nodes, 
																				   const std::vector<Node>& isolated_nodes) {
	std::unordered_map<std::string, std::vector<Node>> isolated_lable2nodes;
	std::unordered_map<std::string, std::vector<Node>> available_lable2nodes;
	for (const auto& node : isolated_nodes) {
		isolated_lable2nodes[node.label].emplace_back(node);
	}
	for (const auto& node : available_host_nodes) {
		available_lable2nodes[node.label].emplace_back(node);
	}
	// check if available lables size is greater than isolated lables size
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
	get_isolated_matched_node_maps(host_label2comb, available_labels, isolated_lable2nodes, cur_sub_id2host_id, sub_id2host_ids);
	return { true, sub_id2host_ids };
}

/// <summary>
/// recursive version of is_isolated_node_matched (call by it)
/// </summary>
/// <param name="host_lable2comb">a map each host graph node lable mapping to their combination</param>
/// <param name="available_labels">current available labels</param>
/// <param name="isolated_label2nodes">isolated node lable mapping to itself</param>
/// <param name="cur_sub_id2host_id">current map sub graph node mapping to host graph node</param>
/// <param name="sub_id2host_ids">all possible maps that sub graph node mapping to host graph node</param>
void get_isolated_matched_node_maps(std::unordered_map<std::string, std::vector<std::vector<Node>>>& host_lable2comb,
									std::unordered_set<std::string>& available_labels,
									std::unordered_map<std::string, std::vector<Node>>& isolated_label2nodes,
									std::unordered_map<int, int>& cur_sub_id2host_id, 
									std::vector<std::unordered_map<int, int>>& sub_id2host_ids) {
	if (available_labels.size() == 0) {
		sub_id2host_ids.emplace_back(cur_sub_id2host_id);
		return;
	}
	// find an available lable(so just choose begin()) 
	auto cur_label = *available_labels.begin();
	// recursively map each subgraph(isolated) node lable to host(available) graph node 
	available_labels.erase(cur_label);
	for (const auto& nodes : host_lable2comb[cur_label]) {
		for (int i = 0; i < isolated_label2nodes[cur_label].size(); ++i) {
			cur_sub_id2host_id[isolated_label2nodes[cur_label][i].id] = nodes[i].id;
		}
		get_isolated_matched_node_maps(host_lable2comb, available_labels, isolated_label2nodes, cur_sub_id2host_id, sub_id2host_ids);
		for (int i = 0; i < isolated_label2nodes[cur_label].size(); ++i) {
			cur_sub_id2host_id.erase(isolated_label2nodes[cur_label][i].id);
		}
	}
	available_labels.insert(cur_label);
}

/// <summary>
/// replace the redex in host graph by sub graph
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="redex">redex</param>
/// <param name="sub_graph">sub graph</param>
/// <returns>replaced graph</returns>
Graph replace_redex(const Graph& host_graph, 
					const Graph& redex, 
					const Graph& sub_graph) {
	Graph temp(host_graph);
	delete_redex(temp, redex);
	add_subgraph(temp, sub_graph);
	return temp;
}

/// <summary>
/// delete the redex in host graph, and modify the dangle edge mark
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="redex">redex</param>
void delete_redex(Graph& host_graph, 
				  const Graph& redex) {
	delete_redex_edges(host_graph, redex);
	delete_redex_nodes(host_graph, redex);
	add_mark_on_edge(host_graph, redex);
}

/// <summary>
/// delete the redex edges in host graph edges
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="redex">redex</param>
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

/// <summary>
/// delete redex nodes in host graph nodes
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="redex">redex</param>
void delete_redex_nodes(Graph& host_graph, 
						const Graph& redex) {
	std::unordered_set<int> nodes_set;
	for (const auto& node : redex.nodes) {
		nodes_set.insert(node.id);
	}
	host_graph.nodes.erase(
		std::remove_if(host_graph.nodes.begin(), host_graph.nodes.end(),
			[nodes_set](Node node) {return nodes_set.count(node.id) != 0; }),
		host_graph.nodes.end());
}

/// <summary>
/// add mark on dangle edge(mark is assigned to the connected vertex mark), and change id on edge's node
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="redex">redex</param>
void add_mark_on_edge(Graph& host_graph, 
					  const Graph& redex) {
	std::unordered_set<int> used_vertices{0};
	for (const auto& edge : redex.edges) {
		used_vertices.insert(edge.node1.second.mark);
		used_vertices.insert(edge.node2.second.mark);
	}
	// add mark on dangle edge and change id on edge's node (mark is assigned to the connected vertex mark)
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
				host_edge.mark = it->mark; // mark this vertex for connecting
				break;
			}
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="host_graph"></param>
/// <param name="sub_graph"></param>
void add_subgraph(Graph& host_graph, 
				  const Graph& sub_graph) {
	auto new_sub_graph = modify_id_on_subgraph(host_graph, sub_graph);
	add_subgraph_nodes(host_graph, new_sub_graph);
	add_subgraph_edges(host_graph, new_sub_graph);
	connect_nodes_on_dangle_edges(host_graph, new_sub_graph);
	remove_vertex_on_edges_and_nodes(host_graph);
}

/// <summary>
/// modify the id in subgraph, such that host graph and sub graph will not have the same node/edge id
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="sub_graph">subgraph</param>
/// <returns>the new subgraph</returns>
Graph modify_id_on_subgraph(const Graph& host_graph, const Graph& sub_graph) {
	Graph temp_graph(sub_graph);
	std::unordered_map<int, int> node_map;
	// modify node id
	std::set<int> available_node_ids;
	for (int i = 0; i < host_graph.nodes.size() + sub_graph.nodes.size(); ++i) {
		available_node_ids.insert(i);
	}
	for (const auto& node : host_graph.nodes) {
		available_node_ids.erase(node.id);
	}
	for (auto& node : temp_graph.nodes) {
		auto p = available_node_ids.begin();
		node_map[node.id] = *p;
		node.id = *p;
		available_node_ids.erase(p);
	}
	// modify edge id and edge's node id
	std::set<int> available_edge_ids;
	for (int i = 0; i < host_graph.edges.size() + sub_graph.edges.size(); ++i) {
		available_edge_ids.insert(i);
	}
	for (const auto& edge : host_graph.edges) {
		available_edge_ids.erase(edge.id);
	}
	for (auto& edge : temp_graph.edges) {
		auto p = available_edge_ids.begin();
		edge.id = *p;
		edge.node1.first.id = node_map[edge.node1.first.id];
		edge.node2.first.id = node_map[edge.node2.first.id];
		available_edge_ids.erase(p);
	}
	return temp_graph;
}

/// <summary>
/// connect subgraph nodes on host graph dangle edges, by dangle edge mark
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="sub_graph">subgraph</param>
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
				host_edge.node1 = vertex_mark2node[host_edge.mark];
			}
			else if (host_edge.node2.first.id == -1) {
				host_edge.node2 = vertex_mark2node[host_edge.mark];
			}
			host_edge.mark = 0;
		}
	}
}

/// <summary>
/// add subgraph nodes to host graph nodes
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="sub_graph">subgraph</param>
inline void add_subgraph_nodes(Graph& host_graph, 
							   const Graph& sub_graph) {
	host_graph.nodes.insert(host_graph.nodes.end(), sub_graph.nodes.begin(), sub_graph.nodes.end());
}

/// <summary>
/// add subgraph edges to host graph
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="sub_graph">subgraph</param>
inline void add_subgraph_edges(Graph& host_graph, 
							   const Graph& sub_graph) {
	host_graph.edges.insert(host_graph.edges.end(), sub_graph.edges.begin(), sub_graph.edges.end());
}

/// <summary>
/// remove the vertices on edges and nodes, such that host graph is legal
/// </summary>
/// <param name="graph">graph</param>
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

/// <summary>
/// judge if the graph is initial graph by count its nodes and edges
/// </summary>
/// <param name="graph">graph</param>
/// <returns>is initial graph</returns>
inline bool is_initial_graph(const Graph& graph) {
	return graph.edges.size() == 0 && graph.nodes.size() == 0;
}

/// <summary>
/// judge if graph is available. The graph is availble if it does not exist multiple edges
/// </summary>
/// <param name="graph">graph</param>
/// <returns>is graph available</returns>
bool is_graph_available(const Graph& graph) {
	std::unordered_map<int, int> node2node;
	for (const auto& edge : graph.edges) {
		// cannot exist multiple same edges
		if ((node2node.count(edge.node1.first.id) && node2node[edge.node1.first.id] == edge.node2.first.id)
			|| (node2node.count(edge.node2.first.id) && node2node[edge.node2.first.id] == edge.node1.first.id)) {
			return false;
		}
		node2node[edge.node1.first.id] = edge.node2.first.id;
		node2node[edge.node2.first.id] = edge.node1.first.id;
	}
	return true;
}

/// <summary>
/// judge if host graph can be parsed to initial graph by using productions to replace, and return process of change
/// </summary>
/// <param name="host_graph">host graph</param>
/// <param name="productions">productions</param>
/// <returns>can be parsed, parsing process</returns>
std::pair<bool, std::vector<Graph>> parse(const Graph& host_graph, const std::vector<Production>& productions) {
	if (is_initial_graph(host_graph)) {
		Node node(0, true, "lambda", {});
		Graph initial_graph({ node }, {});
		return { true, {initial_graph} };
	}
	for (int i = 0; i < productions.size(); ++i) {
		auto production = productions[i];
		auto redexes = find_redex(host_graph, production.r_graph);
		// the initial production can be used 
		// if and only if host graph is already the right graph of initial production
		if (i == 0 && redexes.size() != 0) {
			if (redexes.size() != 1 || redexes[0].edges.size() != host_graph.edges.size() || redexes[0].nodes.size() != host_graph.nodes.size()) {
				continue;
			}
		}
		// try all redexes
		for (const auto& redex : redexes) {
			auto new_graph = replace_redex(host_graph, redex, production.l_graph);
			if (!is_graph_available(new_graph)) return { false, {} };
			// FOR DEBUGGING
			//draw_process_in_html({ host_graph, new_graph });
			//show_process();
			auto [is_ok, process] = parse(new_graph, productions);
			if (is_ok) {
				process.insert(process.begin(), host_graph);
				return{ true, process };
			}
		}
	}
	return { false, {} };
}