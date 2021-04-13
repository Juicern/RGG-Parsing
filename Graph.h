#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#define DEFAULT_SIZE 4
using namespace std;

// definition of Edge
struct Edge {
	int adjVex;
	Edge* first_edge;

	Edge() : first_edge(nullptr) {}
	Edge(int _adjVex, Edge* _next_edge = nullptr) : adjVex(_adjVex), first_edge(_next_edge) {}
};

// definition of Node
struct Node {
	string data;
	Edge* first_edge;

	Node() : first_edge(nullptr) {}
	Node(string _data, Edge* _first_edge = nullptr) : data(_data), first_edge(_first_edge) {}
};

enum Visit_Status{VISITED, UNVISITED};

class Graph {
public:
	Graph();
	Graph(vector<string> _nodes);
	~Graph();
	void insert_edge(int node1, int node2);
	void delete_edge(int node1, int node2);
	void show();
	void dfs();
	void bfs();
private:
	unordered_map<int, Visit_Status> tag;
	unordered_map<int, Node> node_table;
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

void Graph::insert_edge(int node1, int node2) {
	node_table[node1].first_edge = new Edge(node2, node_table[node1].first_edge);
	node_table[node2].first_edge = new Edge(node1, node_table[node2].first_edge);
	edge_num++;
}

void Graph::delete_edge(int node1, int node2) {
	Edge* p = node_table[node1].first_edge;
	Edge* pre = new Edge();
	pre->first_edge = p;
	while (p && p->adjVex != node2) {
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
	while (p && p->adjVex != node1) {
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





