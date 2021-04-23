#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
constexpr auto NOLABEL_CHAR = '-';
constexpr auto NOLABEL_STR = "-";
constexpr auto NOMARK = -1;
constexpr auto NOID = -1;

using namespace std;
class Vertex;
class Node;
class Edge;
class Graph;
class Production;
vector<Graph> find_redex(const Graph&, const Graph&, const vector<Production>&);
vector<Graph> replace_redex(const Graph&, const vector<Graph>&, const Graph&);
void delete_redex(Graph&, const Graph&);
void add_sub_graph(Graph&, const Graph&);
vector<Vertex> get_vertices(const vector<char>&, const vector<int>&);
vector<Node> get_nodes(const vector<int>&, const vector<bool>&, const vector<string>&, const vector<vector<Vertex>>&);
vector<Edge> get_edges(const vector<int>&, const vector<Node>&, const vector<Vertex>&, const vector<Node>&, const vector<Vertex>&);
vector<Edge> get_edges(const vector<int>&, const vector<pair<Node, const Vertex>>&, const vector<pair<Node, Vertex>>&);

class Vertex {
public:
	char label; // if key == '0' : superVertex, else : simple vertex 
	int mark; // 0-> not marked 

	Vertex() noexcept: label(NOLABEL_CHAR), mark(NOMARK){}
	Vertex(const char& _label, const int& _mark) : label(_label), mark(_mark) {}
	Vertex(const Vertex& _vertex) : label(_vertex.label), mark(_vertex.mark) {}
	// Vertex(Vertex&& _vertex) noexcept : label(_vertex.label), mark(_vertex.mark){}



	bool operator==(const Vertex& v)
	{
		return this->mark == v.mark;
	}
};

class Node {
public:
	int id;
	bool is_terminal;
	string label;
	vector<Vertex> vertices;

	Node() noexcept : id(NOID), is_terminal(false), label(NOLABEL_STR){}
	Node(const int& _id, const bool& _is_terminal, const string& _label, const vector<Vertex>& _vertices) : id(_id), is_terminal(_is_terminal), label(_label) {
		vertices.assign(_vertices.begin(), _vertices.end());
	}
	Node(const Node& _node) : id(_node.id), is_terminal(_node.is_terminal), label(_node.label) {
		vertices.assign(_node.vertices.begin(), _node.vertices.end());
	}
	/*Node(Node&& _node) noexcept : id(_node.id), is_terminal(_node.is_terminal), label(_node.label) {
		vertices.assign(_node.vertices.begin(), _node.vertices.end());
		_node.vertices.clear();
	}*/

	bool operator==(const Node& n) {
		return this->id == n.id;
	}
};

class Edge {
public:
	int id;
	pair<int, Vertex> mark; // ����Ҫɾ����0�� ���ߣ����ӵı�ɾ����id�� ��Ҫ��ɾ�ıߣ� -1
	pair<Node, Vertex> node1;
	pair<Node, Vertex> node2;

	Edge() noexcept: id(NOID) {}
	Edge(const int& _id, const pair<Node, Vertex>& _node1, const pair<Node, Vertex>& _node2) : id(_id) {
		mark = make_pair(0, Vertex());
		node1.first = _node1.first;
		node1.second = _node1.second;
		node2.first = _node2.first;
		node2.second = _node2.second;
	}
	Edge(const Edge& _edge) : id(_edge.id) {
		mark.first = _edge.mark.first;
		mark.second = _edge.mark.second;
		node1.first = _edge.node1.first;
		node1.second = _edge.node1.second;
		node2.first = _edge.node2.first;
		node2.second = _edge.node2.second;
	}
	/*Edge(Edge&& _edge) noexcept : id(_edge.id) {
		mark = make_pair(_edge.mark.first, _edge.mark.second);
		node1 = make_pair(_edge.node1.first, _edge.node1.second);
		node2 = make_pair(_edge.node2.first, _edge.node2.second);
	}*/

	bool operator==(const Edge& e) {
		return this->id == e.id;
	}
};

class Graph {
public:
	vector<Node> nodes;
	vector<Edge> edges;

	Graph() = default;
	Graph(const Graph& _graph) {
		nodes.assign(_graph.nodes.begin(), _graph.nodes.end());
		edges.assign(_graph.edges.begin(), _graph.edges.end());
	}
	Graph(const vector<Node>& _nodes, const vector<Edge>& _edges) : nodes(_nodes), edges(_edges) {}
	/*Graph(Graph&& _graph) noexcept {
		nodes.assign(_graph.nodes.begin(), _graph.nodes.end());
		edges.assign(_graph.edges.begin(), _graph.edges.end());
		_graph.edges.clear();
		_graph.nodes.clear();
	}*/
};

class Production {
public:
	Graph l_graph;
	Graph r_graph;
	Production() = default;
	Production(const Graph& _l_graph, const Graph& _r_graph) : l_graph(_l_graph), r_graph(_r_graph) {}
};
// find the redex
vector<Graph> find_redex(Graph& host_graph, Graph& sub_graph, vector<Production>& production) {
	throw -1;
}

vector<Graph> replace_redex(const Graph& host_graph, const vector<Graph>& redexes, const Graph& sub_graph) {
	vector<Graph> graphs;
	for (auto& redex : redexes) {
		Graph temp = host_graph;
		delete_redex(temp, redex);
		add_sub_graph(temp, sub_graph);
		graphs.push_back(temp);
	}
	return graphs;
}

void delete_redex(Graph& host_graph, const Graph& redex) {
	int mark = 0;
	// add mark on edge
	for (auto& host_edge : host_graph.edges) {
		for (auto& redex_node : redex.nodes) {
			if (host_edge.node1.first.id == redex_node.id) {
				if (host_edge.id != 0) {
					host_edge.id = -1;
				}
				else {
					host_edge.mark.first = host_edge.node1.first.id;
					host_edge.mark.second = host_edge.node1.second;
					host_edge.node1.first.id = -1; // node1 should be removed
				}
			}
			if (host_edge.node2.first.id == redex_node.id) {
				if (host_edge.id != 0) {
					host_edge.id = -1;
				}
				else {
					host_edge.mark.first = host_edge.node2.first.id;
					host_edge.mark.second = host_edge.node2.second;
					host_edge.node2.first.id = -1; // node2 should be removed;
				}
			}
		}
	}
	// delete edges (mark == -1)
	for (auto p = host_graph.edges.begin(); p != host_graph.edges.end();) {
		if (p->mark.first == -1) {
			p = host_graph.edges.erase(p);
		}
		else {
			++p;
		}
	}
	// delete nodes
	unordered_set<int> ids;
	for (const auto& node : redex.nodes) {
		ids.insert(node.id);
	}
	for (auto p = host_graph.nodes.begin(); p != host_graph.nodes.end();) {
		if (ids.find(p->id) != ids.end()) {
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

	// hanlde ����
	unordered_map<int, Node> nodes;
	for (const auto& node : sub_graph.nodes) {
		nodes[node.id] = node;
	}
	for (auto edge : deleted_graph.edges) {
		if (edge.mark.first != 0) {
			if (nodes.find(edge.mark.first) != nodes.end()) {
				if (edge.node1.first.id == -1) {
					edge.node1.first = nodes[edge.mark.first];
					edge.node1.second = edge.mark.second;
				}
			}
			else {
				edge.mark = make_pair(-1, Vertex());
			}
		}
	}
	// delete ����
	for (auto p = deleted_graph.edges.begin(); p != deleted_graph.edges.end(); ++p) {
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
		edges.push_back(Edge(ids[i], make_pair(node1s[i], vertex1s[i]), make_pair(node2s[i], vertex2s[i])));
	}
	return edges;
}

vector<Edge> get_edges(const vector<int>& ids, const vector<pair<Node, Vertex>>& node1s, const vector<pair<Node, Vertex>>& node2s) {
	auto n = ids.size();
	vector<Edge> edges;
	for (int i = 0; i < n; ++i) {
		edges.push_back(Edge(ids[i], make_pair(node1s[i].first, node2s[i].second), make_pair(node2s[i].first, node2s[i].second)));
	}
	return edges;
}
