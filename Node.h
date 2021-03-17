#include <vector>

struct Node {
	SuperVertex* super_v;
	std::vector<Vertex*> vertices;
};

struct SuperVertex {
	bool marked;
	std::vector<Node*> adjacents;
};

struct Vertex {
	bool marked;
};