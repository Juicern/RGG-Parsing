#include <iostream>
#include "show_graph.h"
#include "read_graph.h"

int main() {
	auto host_graph = read_host_graph();
	auto productions = read_productions();
	//std::vector<Graph> tests;
	//for (const auto& production : productions) {
	//	tests.emplace_back(production.l_graph);
	//	tests.emplace_back(production.r_graph);
	//}
	//draw_process_in_html(tests);
	//show_process();
	auto [is_ok, processes] = parse(host_graph, productions);
	if (!is_ok) {
		std::cout << "This graph is not this grammar language" << std::endl;
	}
	else {
		draw_process_in_html(processes);
		std::cout << "This graph is a language of this grammar." << std::endl;
		std::cout << "And the process is write in the process.html." << std::endl;
		show_process();
	}
}

