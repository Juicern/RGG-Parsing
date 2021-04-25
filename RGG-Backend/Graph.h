#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
constexpr auto NOLABEL_CHAR = '-';
constexpr auto NOLABEL_STR = "-";
constexpr auto NOMARK = -1;
constexpr auto NOID = -1;

using namespace std;

class Vertex {
public:
	Vertex() noexcept;
	Vertex(const char&, const int&);
	Vertex(const Vertex&);
	bool operator==(const Vertex&);
public:
	char label; // if label == '0' : superVertex, else : simple vertex 
	int mark; // 0 -> not marked 
};

class Node {
public:
	Node() noexcept;
	Node(const int&, const bool&, const string&, const vector<Vertex>&);
	Node(const Node&);
	bool operator==(const Node&);
public:
	int id;
	bool is_terminal;
	string label;
	vector<Vertex> vertices;
};

class Edge {
public:
	Edge() noexcept;
	Edge(const int&, const pair<Node, Vertex>&, const pair<Node, Vertex>&);
	Edge(const Edge&);
	bool operator==(const Edge&);
public:
	int id;
	pair<int, Vertex> mark; // 不需要删除：0， 悬边：连接的被删结点的id， 需要被删的边： -1
	pair<Node, Vertex> node1;
	pair<Node, Vertex> node2;
};

class Graph {
public:
	Graph() = default;
	Graph(const Graph&);
	Graph(const vector<Node>&, const vector<Edge>&);
public:
	vector<Node> nodes;
	vector<Edge> edges;
};

class Production {
public:
	Production() = default;
	Production(const Graph&, const Graph&);
public:
	Graph l_graph;
	Graph r_graph;
};

vector<Graph> find_redex(const Graph&, const Graph&, const vector<Production>&);
void delete_redex(Graph&, const Graph&);
void add_sub_graph(Graph&, const Graph&);
vector<Graph> replace_redex(const Graph&, const vector<Graph>&, const Graph&);
Graph replace_redex(const Graph&, const Graph&, const Graph&);
vector<Vertex> get_vertices(const vector<char>&, const vector<int>&);
vector<Node> get_nodes(const vector<int>&, const vector<bool>&, const vector<string>&, const vector<vector<Vertex>>&);
vector<Edge> get_edges(const vector<int>&, const vector<Node>&, const vector<Vertex>&, const vector<Node>&, const vector<Vertex>&);
vector<Edge> get_edges(const vector<int>&, const vector<pair<Node, const Vertex>>&, const vector<pair<Node, Vertex>>&);

