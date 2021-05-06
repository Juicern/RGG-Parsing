#include <iostream>
#include "show_graph.h"
#include "read_graph.h"
int main() {
	Vertex v1('R', 1);
	Vertex v2('T', 2);
	std::vector<Vertex> vs1{ v1, v2 };
	Node n1(1, false, "send", vs1);

	Vertex v3('A', 4);
	Vertex v4('Y', 5);
	std::vector<Vertex> vs2{ v3, v4 };
	Node n2(2, true, "receive", vs2);
	
	Node n3(4, false, "if", {});

	Edge e1(1, { n1, v1 }, { n2, v4 });
	Edge e2(1, { n1, v2 }, { n2, v3 });

	std::vector<Node> ns{ n1, n2, n3};
	std::vector<Edge> es{ e1, e2 };
	Graph g(ns, es);

	std::vector<Graph> gs{ g };

	auto gs1 = read_host_graph();

	draw_process_in_html({gs1});
	show_process();
}
