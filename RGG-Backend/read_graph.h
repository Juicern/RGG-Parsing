#pragma once
#include "Graph.h"
constexpr auto VERTEX_SPLIT = "VERTEX";
constexpr auto NODE_SPLIT = "NODE";
constexpr auto EDGE_SPLIT = "EDGE";
constexpr auto GRAPH_SPLIT = "GRAPH";
constexpr auto PRODUCTION_SPLIT = "PRODUCTION";
inline std::string get_host_graph_path();
inline std::string get_production_path();
Vertex generate_vertex(const std::string&);
Node generate_node(const std::string&);
Edge generate_edge(const std::string&, const std::vector<Node>&);
Graph generate_graph(const std::string&);
Production generate_production(const std::string&);
Graph read_host_graph();
std::vector<Production> read_productions();
std::vector<std::string> split(const std::string& s, const std::string&);