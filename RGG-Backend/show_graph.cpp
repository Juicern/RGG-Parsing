#include "show_graph.h"

void show_process() {
	system("start process.html");
}
void draw_process_in_html(const vector<Graph>& graphs) {
	ofstream file("process.html", ios::out);
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
string draw_graph_in_div(const Graph& graph) {
	string div;
	div += "<div class = \"mermaid\">\n";
	div += "graph TD\n";
	unordered_map<int, Node> nodes;
	for (const auto& node : graph.nodes) {
		nodes[node.id] = node;
	}
	for (const auto& edge : graph.edges) {
		if (nodes.find(edge.node1.first.id) != nodes.end()) {
			nodes.erase(edge.node1.first.id);
		}
		if (nodes.find(edge.node2.first.id) != nodes.end()) {
			nodes.erase(edge.node2.first.id);
		}
		div += to_string(edge.node1.first.id) + "[" + edge.node1.first.label + "]";
		div += " --- ";
		div += to_string(edge.node2.first.id) + "[" + edge.node2.first.label + "]";
		div += "\n";
	}
	for (const auto& [_, node] : nodes) {
		div += to_string(node.id) + "[" + node.label + "]";
		div += "\n";
	}
	div += "</div>\n";
	return div;
}