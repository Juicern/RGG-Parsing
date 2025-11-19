/*****************************************************************************
***
Copyright 2021 by Juicern(JuicernChu@outlook.com).
All rights reserved.

Permission to use, copy, modifyand distribute this software for personaland educational use is hereby granted without fee, provided that the above copyright notice appears in all copiesand that both that copyright noticeand this permission notice appear in supporting documentation, and that the names of Addison Wesley Longman or the author are not used in advertising or publicity pertaining to distribution of the software without specific, written prior permission.Addison Wesley Longmanand the author make no representations about the suitability of this software for any purpose.It is provided "as is" without express or implied warranty.

ADDISON WESLEY LONGMAN AND THE AUTHOR DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.IN NO EVENT SHALL ADDISON WESLEY LONGMAN OR THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
***
******************************************************************************/

#pragma once
#include "read_graph.h"
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

std::string resolve_graph_asset(const std::string& file_name) {
	std::vector<fs::path> candidates;
	auto cur = fs::current_path();
	for (int i = 0; i < 5 && !cur.empty(); ++i) {
		candidates.emplace_back(cur / file_name);
		candidates.emplace_back(cur / "RGG-Backend" / file_name);
		cur = cur.parent_path();
	}
	for (const auto& candidate : candidates) {
		if (fs::exists(candidate)) {
			return candidate.string();
		}
	}
	throw std::runtime_error("Cannot locate " + file_name + ". Run from the repository root or ensure the file exists.");
}

std::string get_host_graph_path() {
	return resolve_graph_asset("host_graph.txt");
}

std::string get_productions_path() {
	return resolve_graph_asset("productions.txt");
}

/// <summary>
/// get a vertex from string
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
Vertex generate_vertex(const std::string& s) {
	/* s should be like: 
        T
        2
    */
    auto strs = split(s, "\n");
    return Vertex(strs[0][0], std::stoi(strs[1]));
}

/// <summary>
/// get a node from string
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
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

/// <summary>
/// get a edge from string
/// </summary>
/// <param name="s"></param>
/// <param name="nodes"></param>
/// <returns></returns>
Edge generate_edge(const std::string& s, const std::vector<Node>& nodes) {
    /* s should be like:
    1
    2 3
    4 5
    */
    int id;
    std::pair<Node, Vertex> point1;
    std::pair<Node, Vertex> point2;
    std::unordered_map<int, Node> id2node;
    for (const auto& node : nodes) {
        id2node[node.id] = node;
    }
    auto strs = split(s, "\n");
    id = std::stoi(strs[0]);
    auto s_node1 = split(strs[1], " ");
    auto s_node2 = split(strs[2], " ");
    if (s_node1[1] == "-1") {
        point1 = std::make_pair(id2node[std::stoi(s_node1[0])], Vertex());
    }
    else {
        for (const auto& vertex : id2node[std::stoi(s_node1[0])].vertices) {
            if (vertex.mark == std::stoi(s_node1[1])) {
                point1 = std::make_pair(id2node[std::stoi(s_node1[0])], vertex);
                break;
            }
        }
    }
    if (s_node2[1] == "-1") {
        point2 = std::make_pair(id2node[std::stoi(s_node2[0])], Vertex());
    }
    else {
        for (const auto& vertex : id2node[std::stoi(s_node2[0])].vertices) {
            if (vertex.mark == std::stoi(s_node2[1])) {
                point2 = std::make_pair(id2node[std::stoi(s_node2[0])], vertex);
                break;
            }
        }
    }
    return Edge(id, point1, point2);
}

/// <summary>
/// get a graph from string
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
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

/// <summary>
/// get a production from string
/// </summary>
/// <param name="s"></param>
/// <returns></returns>
Production generate_production(const std::string& s) {
    auto strs = split(s, GRAPH_SPLIT);
    return Production(generate_graph(strs[0]), generate_graph(strs[1]));
}

/// <summary>
/// get host graph
/// </summary>
/// <returns></returns>
Graph read_host_graph() {
    std::ifstream in(get_host_graph_path());
    std::istreambuf_iterator<char> begin(in);
    std::istreambuf_iterator<char> end;
    std::string s(begin, end);
    in.close();
    return generate_graph(s);
}

/// <summary>
/// get productions
/// </summary>
/// <returns></returns>
std::vector<Production> read_productions() {
    std::ifstream in(get_productions_path());
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

/// <summary>
/// split a string by given separator
/// </summary>
/// <param name="in">string</param>
/// <param name="delim">separator</param>
/// <returns>splited strings</returns>
std::vector<std::string> split(const std::string& in, const std::string& delim) {
    std::regex re{ delim };
    std::vector<std::string> ret {std::sregex_token_iterator(in.begin(), in.end(), re, -1), std::sregex_token_iterator()};
    ret.erase(std::remove_if(ret.begin(), ret.end(), [](std::string s) -> bool{return s == "" || s == "\n"; }), ret.end());
    return ret;
}
