#pragma once
#include <vector>
#include "Edge.h"

class Node {
	Vertex* super_v;
	std::vector<Vertex*> vertices;
};

class Vertex{
	bool marked;
	std::vector<Edge*> edges;
};