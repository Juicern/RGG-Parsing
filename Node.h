#pragma once
#include <vector>
#include "Edge.h"

struct Node {
	Vertex* super_v;
	std::vector<Vertex*> vertices;
};

struct Vertex {
	bool marked;
	std::vector<Edge*> edges;
};