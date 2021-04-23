#pragma once
#include "Graph.h"

class Production {
public:
	Graph left_graph;
	Graph right_graph;
	Production(Graph _left_graph, Graph _right_graph) : left_graph(_left_graph), right_graph(_right_graph) {}
};
