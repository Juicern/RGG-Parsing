#include <iostream>
#include "Graph.h"
int main()
{
    vector<string> nodes = {
        "receive", "receive", "open", "send", "statement", "fork", "if", "if", "endif", "join", "assign", "end"
    };
    Graph graph(nodes);
    graph.insert_edge(0, 1);
    graph.insert_edge(0, 4);
    graph.insert_edge(0, 3);
    graph.insert_edge(1, 2);
    graph.insert_edge(1, 4);
    graph.insert_edge(4, 5);
    graph.insert_edge(5, 8);
    graph.insert_edge(11, 12);
    graph.insert_edge(11, 4);
    graph.insert_edge(3, 2);
    graph.insert_edge(7, 2);
    graph.insert_edge(2, 1);
    graph.insert_edge(1, 3); 
    graph.insert_edge(2, 8);
    graph.insert_edge(2, 5);
    graph.show();
    graph.dfs();
    return 0;
}
