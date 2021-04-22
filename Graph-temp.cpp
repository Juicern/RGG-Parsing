#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Vertex {
public:
	char label; // if key == '0' : superVertex, else : simple vertex 
	int mark; // 0-> no marked 

	Vertex() {}
	Vertex(char _label, int _mark) : label(_label), mark(_mark) {}
};

class Node {
public:
	int id;
	bool is_termimal;
	string label;
	vector<Vertex> vertices;

	Node() {}
	Node(int _id, bool _is_terminal, string _label, vector<Vertex> _vertices) : id(_id), is_termimal(_is_terminal), label(_label), vertices(_vertices) {}
};

class Edge {
public:
	int id;
	pair<int, Vertex> mark; // 不需要删除：0， 悬边：连接的被删结点的id， 需要被删的边： -1
	pair<Node, Vertex> node1;
	pair<Node, Vertex> node2;

	Edge() {}
	Edge(int _id, pair<Node, Vertex> _node1, pair<Node, Vertex> _node2) : id(_id), node1(_node1), node2(_node2) {}
};

class Graph {
public:
	vector<Node> nodes;
	vector<Edge> edges;

	Graph() {};
	Graph(vector<Node> _nodes, vector<Edge> _edges) : nodes(_nodes), edges(_edges) {}
};

class Production {
public:
	Graph l_graph;
	Graph r_graph;
};
// find the redex（需修改Edge的mark）
vector<Graph> find_redex(Graph host_graph, Graph sub_graph, vector<Production> &production);

vector<Graph> replace_redex(Graph host_graph, vector<Graph> redexes, Graph sub_graph) {
	vector<Graph> graphs;
	for (auto& redex : redexes) {
		Graph temp = host_graph;
		delete_redex(temp, redex);
		add_sub_graph(temp, sub_graph);
		graphs.push_back(temp);
	}
	return graphs;
}

void delete_redex(Graph& host_graph, Graph& redex) {
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


void add_sub_graph(Graph& deleted_graph, Graph& sub_graph) {
	// add nodes
	deleted_graph.nodes.emplace(sub_graph.nodes.begin(), sub_graph.nodes.end());

	// hanlde 悬边
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
				edge.mark = { -1, Vertex() };
			}
		}
	}
	// delete 悬边
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
	deleted_graph.edges.emplace(sub_graph.edges.begin(), sub_graph.edges.end());
}

vector<Vertex> get_vertices(vector<char> labels, vector<int> marks) {
	int n = labels.size();
	vector<Vertex> vertices;
	for (int i = 0; i < n; ++i) {
		vertices.push_back(Vertex(labels[i], marks[i]));
	}
	return vertices;
}

vector<Node> get_nodes(vector<int> ids, vector<bool> flags, vector<string> labels, vector<vector<Vertex>> vertices) {
	int n = labels.size();
	vector<Node> nodes;
	for (int i = 0; i < n; ++i) {
		nodes.push_back(Node(ids[i], flags[i], labels[i], vertices[i]));
	}
	return nodes;
}

vector<Edge> get_edges(vector<int> ids, vector<Node> node1s, vector<Vertex> vertex1s, vector<Node> node2s, vector<Vertex> vertex2s) {
	int n = ids.size();
	vector<Edge> edges;
	for (int i = 0; i < n; ++i) {
		edges.push_back(Edge(ids[i], { node1s[i], vertex1s[i] }, { node2s[i], vertex2s[i] }));
	}
	return edges;
}

vector<Edge> get_edges(vector<int> ids, vector<pair<Node, Vertex>> node1s, vector<pair<Node, Vertex>> node2s) {
	int n = ids.size();
	vector<Edge> edges;
	for (int i = 0; i < n; ++i) {
		edges.push_back(Edge(ids[i], node1s[i], node2s[i]));
	}
	return edges;
}

int main() {
	vector<char> vertex_label1s = { 'T', 'D', 'R', 'R' };
	vector<int> vertex_mark1s = { 1, 2, 3, 4 };
	auto vertices1 = get_vertices(vertex_label1s, vertex_mark1s);

	vector<char> vertex_label2s = { 'T', 'D', 'R', 'R' };
	vector<int> vertex_mark2s = { 1, 2, 3, 4 };
	auto vertices2 = get_vertices(vertex_label2s, vertex_mark2s);

	vector<vector<Vertex>> verticeses;
	verticeses.push_back(vertices1);
	verticeses.push_back(vertices2);
	vector<int> node_ids = { 1, 2 };
	vector<bool> node_flags = { false, true };
	vector<string> node_labels = { "send", "recieve" };
	auto nodes = get_nodes(node_ids, node_flags, node_labels, verticeses);

	auto edges = get_edges({ 1 }, { {nodes[0], verticeses[0][1]} }, { {nodes[1], verticeses[1][2]} });

	Graph graph(nodes, edges);
}