/*****************************************************************************
***
Copyright 2021 by Juicern(JuicernChu@outlook.com).
All rights reserved.

Permission to use, copy, modifyand distribute this software for personaland educational use is hereby granted without fee, provided that the above copyright notice appears in all copiesand that both that copyright noticeand this permission notice appear in supporting documentation, and that the names of Addison Wesley Longman or the author are not used in advertising or publicity pertaining to distribution of the software without specific, written prior permission.Addison Wesley Longmanand the author make no representations about the suitability of this software for any purpose.It is provided "as is" without express or implied warranty.

ADDISON WESLEY LONGMAN AND THE AUTHOR DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.IN NO EVENT SHALL ADDISON WESLEY LONGMAN OR THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
***
******************************************************************************/

#pragma once
#include "show_graph.h"

/// <summary>
/// open process.html file in browsera
/// </summary>
void show_process() {
	system("start process.html");
}

/// <summary>
/// draw mermaid to show process 
/// </summary>
/// <param name="graphs"></param>
void draw_process_in_html(const std::vector<Graph>& graphs) {
	std::ofstream file("process.html", std::ios::out);
	file << "<!doctype html>\n";
	file << "<html>\n";
	file << "<head>\n";
	file << "<script src=\"mermaid.min.js\"></script>\n";
	file << "<script>mermaid.initialize({startOnLoad:true});</script>\n";
	file << "</head>\n";
	file << "<body>\n";
	for (const auto& graph : graphs) {
		file << draw_graph_in_div(graph);
		file << "<hr/>\n";
	}
	file << "</body>\n";
	file << "</html>\n";
	file.close();
}

/// <summary>
/// draw a graph in mermaid
/// </summary>
/// <param name="graph"></param>
/// <returns></returns>
std::string draw_graph_in_div(const Graph& graph) {
	std::string div;
	div += "<div class = \"mermaid\">\n";
	div += "graph TD\n";
	std::unordered_map<int, Node> isolated_nodes;
	for (const auto& node : graph.nodes) {
		isolated_nodes[node.id] = node;
	}
	for (const auto& edge : graph.edges) {
		if (isolated_nodes.count(edge.point1.first.id)) {
			isolated_nodes.erase(edge.point1.first.id);
		}
		if (isolated_nodes.count(edge.point2.first.id)) {
			isolated_nodes.erase(edge.point2.first.id);
		}
		div += std::to_string(edge.point1.first.id) + "[" + edge.point1.first.label + "]";
		div += " --- ";
		div += std::to_string(edge.point2.first.id) + "[" + edge.point2.first.label + "]";
		div += "\n";
	}
	for (const auto& [id, node] : isolated_nodes) {
		div += std::to_string(id) + "[" + node.label + "]";
		div += "\n";
	}
	div += "</div>\n";
	return div;
}