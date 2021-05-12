#pragma once
#include "read_graph.h"

inline std::string get_host_graph_path() {
	return "../host_graph.txt";
}

inline std::string get_production_path() {
	return "../productions.txt";
}
Vertex generate_vertex(const std::string& s) {
	/* s should be like: 
        T
        2
    */
    auto strs = split(s, "\n");
    return Vertex(strs[0][0], std::stoi(strs[1]));
}
Node generate_node(const std::string& s) {
    /* s should be like:
        2
        1
        state
        VERTEX
        T
        2
        VERTEX
        D
        3
    */
	int id;
	bool is_terminal;
	std::string label;
	std::vector<Vertex> vertices;
    auto strs = split(s, VERTEX_SPLIT);
    auto paras = split(strs[0], "\n");
    id = std::stoi(paras[0]);
    is_terminal = paras[1] == "1";
    label = paras[2];
    for (int i = 1; i < strs.size(); ++i) {
        vertices.push_back(generate_vertex(strs[i]));
    }
    bool has_super_vertex = false;
    for (const auto& vertex : vertices) {
        if (vertex.label == '-') {
            has_super_vertex = true;
            break;
        }
    }
    if (!has_super_vertex) {
        vertices.emplace_back(Vertex('-', 0));
    }
    return Node(id, is_terminal, label, vertices);
}
Edge generate_edge(const std::string& s, const std::vector<Node>& nodes) {
    /* s should be like:
    1
    2 3
    4 5
    */
    int id;
    std::pair<Node, Vertex> node1;
    std::pair<Node, Vertex> node2;
    std::unordered_map<int, Node> id2node;
    for (const auto& node : nodes) {
        id2node[node.id] = node;
    }
    auto strs = split(s, "\n");
    id = std::stoi(strs[0]);
    auto s_node1 = split(strs[1], " ");
    auto s_node2 = split(strs[2], " ");
    if (s_node1[1] == "-1") {
        node1 = std::make_pair(id2node[std::stoi(s_node1[0])], Vertex());
    }
    else {
        for (const auto& vertex : id2node[std::stoi(s_node1[0])].vertices) {
            if (vertex.mark == std::stoi(s_node1[1])) {
                node1 = std::make_pair(id2node[std::stoi(s_node1[0])], vertex);
                break;
            }
        }
    }
    if (s_node2[1] == "-1") {
        node2 = std::make_pair(id2node[std::stoi(s_node2[0])], Vertex());
    }
    else {
        for (const auto& vertex : id2node[std::stoi(s_node2[0])].vertices) {
            if (vertex.mark == std::stoi(s_node2[1])) {
                node2 = std::make_pair(id2node[std::stoi(s_node2[0])], vertex);
                break;
            }
        }
    }
    return Edge(id, node1, node2);
}
Graph generate_graph(const std::string& s) {
    /* s should be like:
        NODE
        ...
        NODE
        ...
        EDGE
        ...
        EDGE
        ...
    */
    auto strs = split(s, EDGE_SPLIT);
    auto node_strs = split(strs[0], NODE_SPLIT);
    std::vector<Node> nodes;
    for (const auto node_str : node_strs) {
        nodes.emplace_back(generate_node(node_str));
    }
    std::vector<Edge> edges;
    for (int i = 1; i < strs.size(); ++i) {
        edges.emplace_back(generate_edge(strs[i], nodes));
    }
    return Graph(nodes, edges);
}
Production generate_production(const std::string& s) {
    auto strs = split(s, GRAPH_SPLIT);
    return Production(generate_graph(strs[0]), generate_graph(strs[1]));
}
Graph read_host_graph() {
    std::ifstream in(get_host_graph_path());
    std::istreambuf_iterator<char> begin(in);
    std::istreambuf_iterator<char> end;
    std::string s(begin, end);
    in.close();
    return generate_graph(s);
}
std::vector<Production> read_productions() {
    std::ifstream in(get_production_path());
    std::istreambuf_iterator<char> begin(in);
    std::istreambuf_iterator<char> end;
    std::string s(begin, end);
    auto strs = split(s, PRODUCTION_SPLIT);
    std::vector<Production> productions;
    for (const auto& str : strs) {
        productions.emplace_back(generate_production(str));
    }
    return productions;
}

std::vector<std::string> split(const std::string& in, const std::string& delim) {
    std::regex re{ delim };
    std::vector<std::string> ret {std::sregex_token_iterator(in.begin(), in.end(), re, -1), std::sregex_token_iterator()};
    ret.erase(std::remove_if(ret.begin(), ret.end(), [](std::string s) -> bool{return s == "" || s == "\n"; }), ret.end());
    return ret;
}