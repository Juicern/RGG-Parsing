#include "show_graph.h"

void show_process() {
	system("start process.html");
}
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
std::string draw_graph_in_div(const Graph& graph) {
	std::string div;
	div += "<div class = \"mermaid\">\n";
	div += "graph TD\n";
	std::unordered_map<int, Node> isolated_nodes;
	for (const auto& node : graph.nodes) {
		isolated_nodes[node.id] = node;
	}
	for (const auto& edge : graph.edges) {
		if (isolated_nodes.count(edge.node1.first.id)) {
			isolated_nodes.erase(edge.node1.first.id);
		}
		if (isolated_nodes.count(edge.node2.first.id)) {
			isolated_nodes.erase(edge.node2.first.id);
		}
		div += std::to_string(edge.node1.first.id) + "[" + edge.node1.first.label + "]";
		div += " --- ";
		div += std::to_string(edge.node2.first.id) + "[" + edge.node2.first.label + "]";
		div += "\n";
	}
	for (const auto& [id, node] : isolated_nodes) {
		div += std::to_string(id) + "[" + node.label + "]";
		div += "\n";
	}
	div += "</div>\n";
	return div;
}