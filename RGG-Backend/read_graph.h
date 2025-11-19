/*****************************************************************************
***
Copyright 2021 by Juicern(JuicernChu@outlook.com).
All rights reserved.

Permission to use, copy, modifyand distribute this software for personaland educational use is hereby granted without fee, provided that the above copyright notice appears in all copiesand that both that copyright noticeand this permission notice appear in supporting documentation, and that the names of Addison Wesley Longman or the author are not used in advertising or publicity pertaining to distribution of the software without specific, written prior permission.Addison Wesley Longmanand the author make no representations about the suitability of this software for any purpose.It is provided "as is" without express or implied warranty.

ADDISON WESLEY LONGMAN AND THE AUTHOR DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.IN NO EVENT SHALL ADDISON WESLEY LONGMAN OR THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
***
******************************************************************************/

#pragma once
#include "Graph.h"
constexpr auto VERTEX_SPLIT = "VERTEX";
constexpr auto NODE_SPLIT = "NODE";
constexpr auto EDGE_SPLIT = "EDGE";
constexpr auto GRAPH_SPLIT = "GRAPH";
constexpr auto PRODUCTION_SPLIT = "PRODUCTION";
std::string resolve_graph_asset(const std::string&);
std::string get_host_graph_path();
std::string get_productions_path();
Vertex generate_vertex(const std::string&);
Node generate_node(const std::string&);
Edge generate_edge(const std::string&, const std::vector<Node>&);
Graph generate_graph(const std::string&);
Production generate_production(const std::string&);
Graph read_host_graph();
std::vector<Production> read_productions();
std::vector<std::string> split(const std::string& s, const std::string&);
