#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
constexpr auto NULLSTR = "-";
using namespace std;

// definition of Edge
struct Edge {
	int adjVex;
	Edge* first_edge;
	string vertex_data;

	Edge() : first_edge(nullptr) {}
	Edge(int _adjVex, Edge* _next_edge = nullptr, string v_data = NULLSTR) : adjVex(_adjVex), first_edge(_next_edge), vertex_data(v_data) {}
	~Edge() {
		delete[] first_edge;
	}
};

// definition of Vertex
struct Vertex {
	string data;
	bool is_marked;
	Vertex* first_vertex;

	Vertex() {}
	Vertex(string _data, bool _is_marked = false, Vertex* _first_vertex = nullptr) : data(_data), is_marked(_is_marked), first_vertex(_first_vertex) {}
	~Vertex() {
		delete[] first_vertex;
	}
};

// definition of Node
struct Node {
	string data; 
	Edge* first_edge;
	Vertex* first_vertex;
	bool is_terminal;
	bool is_marked;

	Node() : first_edge(nullptr) {}
	Node(string _data, bool _is_terminal = true, Edge* _first_edge = nullptr) : data(_data), is_terminal(_is_terminal), first_edge(_first_edge) {}
	~Node() {
		delete[] first_edge;
		delete[] first_vertex;
	}
};

enum Visit_Status{VISITED, UNVISITED};

class Graph {
public:
	Graph();
	Graph(vector<string> _nodes);
	~Graph();
	void insert_edge(int node1, int node2, string v_data1 = NULLSTR, string v_data2 = NULLSTR);
	void delete_edge(int node1, int node2, string v_data1 = NULLSTR, string v_data2 = NULLSTR);
	int insert_node(string data);
	void delete_node(int node1);
	void insert_vertex(int node, string data, bool is_marked);
	void delete_vertex(int node, string data);
	void show();
	void dfs();
	void bfs();
private:
	unordered_map<int, Visit_Status> tag;
	unordered_map<int, Node> node_table;
	unordered_set<int> unused_keys;
	int node_num;
	int edge_num;

	void dfs(int start_node);
	void bfs(int start_node);
};

Graph:: Graph() {
	node_num = 0;
	edge_num = 0;
}

Graph::Graph(vector<string> _nodes) {
	node_num = _nodes.size();
	edge_num = 0;
	for (int i = 0; i < node_num; i++) {
		tag[i] = UNVISITED;
		node_table[i] = Node(_nodes[i]);
	}
}

Graph::~Graph() {
	tag.clear();
	node_table.clear();
}

void Graph::insert_edge(int node1, int node2, string v_data1 = NULLSTR, string v_data2 = NULLSTR) {
	node_table[node1].first_edge = new Edge(node2, node_table[node1].first_edge, v_data2);
	node_table[node2].first_edge = new Edge(node1, node_table[node2].first_edge, v_data1);
	edge_num++;
}

void Graph::delete_edge(int node1, int node2, string v_data1 = NULLSTR, string v_data2 = NULLSTR) {
	Edge* p = node_table[node1].first_edge;
	Edge* pre = new Edge();
	pre->first_edge = p;
	while (p && p->adjVex != node2 && p->vertex_data == v_data2) {
		pre = p;
		p = p->first_edge;
	}
	if (p) {
		pre->first_edge = p->first_edge;
		delete(p);
		node_table[node1].first_edge = pre->first_edge;
	}
	
	p = node_table[node2].first_edge;
	pre->first_edge = p;
	while (p && p->adjVex != node1 && p->vertex_data == v_data1) {
		pre = p;
		p = p->first_edge;
	}
	if (p) {
		pre->first_edge = p->first_edge;
		delete(p);
		node_table[node2].first_edge = pre->first_edge;
	}
	delete(pre);	
}

int Graph::insert_node(string data) {
	++node_num;
	if (unused_keys.size() == 0) {
		node_table[node_num - 1] = Node(data);
		tag[node_num - 1] = UNVISITED;
		return node_num - 1;
	}
	else {
		int key = *unused_keys.begin();
		unused_keys.erase(key);
		node_table[key] = Node(data);
		tag[key] = UNVISITED;
		unused_keys.insert(node_num - 1);
		return key;
	}
}

void Graph::delete_node(int node) {
	Edge* p = node_table[node].first_edge;
	while (p) {
		delete_edge(node, p->adjVex);
		p = p->first_edge;
		--edge_num;
	}
	node_table.erase(node);
	tag.erase(node);
	unused_keys.insert(node);
	--node_num;
}

void Graph::insert_vertex(int node, string data, bool is_marked = false) {
	node_table[node].first_vertex = new Vertex(data, is_marked, node_table[node].first_vertex);
}
void Graph::delete_vertex(int node, string _data) {
	Vertex* pre = new Vertex();
	Vertex* p = node_table[node].first_vertex;
	pre->first_vertex = p;
	while (p && p->data != _data) {
		pre = p;
		p = p->first_vertex;
	}
	if (p) {
		pre->first_vertex = p->first_vertex;
		delete(p);
	}
	delete(pre);
}

void Graph::show() {
	for (const auto& [_, node] : node_table) {
		cout << node.data << ":";
		Edge* p = node.first_edge;
		while (p) {
			cout << p->adjVex << " ";
			p = p->first_edge;
		}
		cout << endl;
	}
}

void Graph::dfs(int start_node) {
	if (tag[start_node] == UNVISITED) {
		cout << node_table[start_node].data << " ";
		tag[start_node] = VISITED;
		Edge* p = node_table[start_node].first_edge;
		while (p) {
			dfs(p->adjVex);
			p = p->first_edge;
		}
	}
}

void Graph::dfs() {
	for (const auto& [i, _] : node_table) {
		if (tag[i] == UNVISITED) {
			dfs(i);
		}
		cout << endl;
	}
	for (auto& [i, _] : tag) {
		tag[i] = UNVISITED;
	}
}

void Graph::bfs(int start_node) {
	if (tag[start_node] != VISITED) {
		queue<int> q;
		q.push(start_node);
		tag[start_node] = VISITED;
		cout << node_table[start_node].data << " ";
		Edge* p;
		while (!q.empty()) {
			int i = q.front();
			q.pop();
			p = node_table[i].first_edge;
			while (p) {
				if (tag[p->adjVex] == UNVISITED) {
					tag[p->adjVex] = VISITED;
					cout << node_table[p->adjVex].data;
					q.push(p->adjVex);
				}
				p = p->first_edge;
			}
		}
	}
}
void Graph::bfs() {
	for (const auto& [i, _] : node_table) {
		if (tag[i] == UNVISITED) {
			bfs(i);
			cout << endl;
		}
	}
	for (const auto& [i, _] : tag) {
		tag[i] = UNVISITED;
	}
 }





