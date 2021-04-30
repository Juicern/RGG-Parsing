#include <iostream>
#include "show_graph.h"
int main() {
	Vertex v1('R', 1);
	Vertex v2('T', 2);
	vector<Vertex> vs1{ v1, v2 };
	Node n1(1, false, "send", vs1);

	Vertex v3('A', 4);
	Vertex v4('Y', 5);
	vector<Vertex> vs2{ v3, v4 };
	Node n2(2, true, "receive", vs2);
	
	Node n3(4, false, "if", {});

	Edge e1(1, { n1, v1 }, { n2, v4 });
	Edge e2(1, { n1, v2 }, { n2, v3 });

	vector<Node> ns{ n1, n2, n3};
	vector<Edge> es{ e1, e2 };
	Graph g(ns, es);

	vector<Graph> gs{ g };
	draw_process_in_html(gs);
	show_process();
}
