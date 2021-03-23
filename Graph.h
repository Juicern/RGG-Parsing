#pragma once
#include <vector>
#include <string>
#include <unordered_set>

class Edge {
public:
	int adjvex;
	std::pair<Node, Node> nodes;
	std::unordered_set<Edge> next;
	// Constructor
	Edge(int a) :adjvex(a) {}
	Edge(int a, std::pair<Node, Node> ns) : adjvex(a), nodes(ns) {}
	Edge(int a, std::unordered_set<Edge>& n) : adjvex(a), next(n) {}
};

// one Node includes a super vertex and some vertices;
class Node {
public:
	std::string data;
	Vertex super_v;
	std::unordered_set<Vertex> vertices;
	std::unordered_set<Edge> edges; // The edges of Node include all edges of its super vertex and vertices
	// Constructor
	Node() = default;
	Node(std::string d) : data(d) {}
	Node(std::string d, Vertex sv) : data(d) {}
	Node(std::string d, Vertex sv, std::unordered_set<Vertex> vs) : data(d), super_v(sv), vertices(vs) {}
};

class Vertex {
public:
	Node parent;
	bool marked;
	std::unordered_set<Edge> edges;
	// Constructor
	Vertex() = default;
	Vertex(Node p, bool m = false) : parent(p), marked(m) {}
};

class Graph {
public:
	std::vector<Node> nodes;
	int vex_num;
	int edge_num;
};

std::vector<Vertex> edge_get_adjacent_vertices(const Edge& edge) {
	std::vector<Vertex> vertices;
	auto s_edge1 = edge.nodes.first.super_v.edges;
	auto s_edge2 = edge.nodes.second.super_v.edges;
	if (s_edge1.find(edge) != s_edge1.end()) {
		vertices.push_back(s_edge1);
	}
	if (s_edge2.find(edge) != s_edge1.end()) {
		vertices.push_back(s_edge1);
	}
	auto vs1 = edge.nodes.first.vertices;
	auto vs2 = edge.nodes.second.vertices;
	for (const auto& vertex : vs1) {
		if (vertex.edges.find(edge) != vertex.edges.end()) {
			vertices.push_back(vertex);
		}
	}
	for (const auto& vertex : vs2) {
		if (vertex.edges.find(edge) != vertex.edges.end()) {
			vertices.push_back(vertex);
		}
	}
	return vertices;
}

std::vector<Vertex> vertex_get_adjacent_vertices(const Vertex& vertex) {
	std::vector<Vertex> vertices;
	for (const auto& edge : vertex.edges) {
		for (const auto& v : edge_get_adjacent_vertices(edge)) {
			if (&v != &vertex) {
				vertices.push_back(v);
			}
		}
	}
	return vertices;
}