#include "pch.h"
#include "CppUnitTest.h"
#include "../RGG-Backend/Graph.cpp"
#include "../RGG-Backend/read_graph.cpp"
#include "../RGG-Backend/show_graph.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace graph_test {
	TEST_CLASS(test_for_vertex) {
		TEST_METHOD(test_for_default_constructor) {
			auto v = make_shared<Vertex>();
			Assert::IsNotNull(v.get());
		}
		TEST_METHOD(test_for_2nd_constructor) {
			auto v = make_shared<Vertex>('T', 2);
			Assert::AreEqual(v->label, 'T');
			Assert::AreEqual(v->mark, 2);
		}
		TEST_METHOD(test_for_copy_constructor) {
			auto v1 = make_shared<Vertex>('T', 2);
			auto v2 = make_shared<Vertex>(*v1.get());
			Assert::AreEqual(v1->label, v2->label);
			Assert::AreEqual(v1->mark, v2->mark);
			Assert::IsFalse(v1.get() == v2.get());
		}
		TEST_METHOD(test_for_operator_equals) {
			Vertex v1('T', 2);
			Vertex v2('T', 3);
			Assert::IsTrue(v1 == v2);
		}
	};
	TEST_CLASS(test_for_node) {
		TEST_METHOD(test_for_default_constructor) {
			auto n = make_shared<Node>();
			Assert::IsNotNull(n.get());
		}
		TEST_METHOD(test_for_2nd_constructor) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs{ v1, v2 };
			auto n = make_shared<Node>(1, false, "send", vs);
			Assert::AreEqual(n->id, 1);
			Assert::AreEqual(n->is_terminal, false);
			Assert::IsTrue(n->label == "send");
			Assert::AreEqual(n->vertices.size(), vs.size());
			for (auto i = 0; i < n->vertices.size(); ++i) {
				Assert::IsTrue(n->vertices[i] == vs[i]);
			}
		}
		TEST_METHOD(test_for_copy_constructor) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs{ v1, v2 };
			auto n1 = make_shared<Node>(1, false, "send", vs);
			auto n2 = make_shared<Node>(*n1);
			Assert::AreEqual(n1->id, n2->id);
			Assert::AreEqual(n1->is_terminal, n2->is_terminal);
			Assert::IsTrue(n1->label == n2->label);
			Assert::AreEqual(n1->vertices.size(), n2->vertices.size());
			for (auto i = 0; i < n1->vertices.size(); ++i) {
				Assert::IsTrue(n1->vertices[i] == n2->vertices[i]);
			}
			Assert::IsFalse(n1 == n2);
		}
		TEST_METHOD(test_for_operator_equals) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs{ v1, v2 };
			Node n1(1, false, "send", vs);
			Node n2(1, true, "receive", {});
			Assert::IsTrue(n1 == n2);
		}
	};
	TEST_CLASS(test_for_edge) {
		TEST_METHOD(test_for_default_constructor) {
			auto e = make_shared<Edge>();
			Assert::IsNotNull(e.get());
		}
		TEST_METHOD(test_for_2nd_constructor) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);
			Edge e(1, { n1, v1 }, { n2, v4 });

			Assert::AreEqual(e.id, 1);
			Assert::IsTrue(e.node1.first == n1);
			Assert::IsTrue(e.node1.second == v1);
			Assert::IsTrue(e.node2.first == n2);
			Assert::IsTrue(e.node2.second == v4);
		}
		TEST_METHOD(test_for_copy_constructor) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);
			pair<Node, Vertex> p1(n1, v2);
			pair<Node, Vertex> p2(n2, v4);
			auto e1 = make_shared<Edge>(1, p1, p2);
			auto e2 = make_shared<Edge>(*e1);

			Assert::AreEqual(e1->id, 1);
			Assert::IsTrue(e1->node1.first == e2->node1.first);
			Assert::IsTrue(e1->node1.second == e2->node1.second);
			Assert::IsTrue(e1->node2.first == e2->node2.first);
			Assert::IsTrue(e1->node2.second == e2->node2.second);
			Assert::IsFalse(e1 == e2);
		}
		TEST_METHOD(test_for_operator_equals) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);
			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(1, { n1, v2 }, { n2, v3 });
			Assert::IsTrue(e1 == e2);
		}
	};
	TEST_CLASS(test_for_graph) {
		TEST_METHOD(test_for_default_constructor) {
			auto g = make_shared<Graph>();
			Assert::IsNotNull(g.get());
		}
		TEST_METHOD(test_for_2nd_constructor) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);

			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(1, { n1, v2 }, { n2, v3 });

			vector<Node> ns{ n1, n2 };
			vector<Edge> es{ e1, e2 };
			Graph g(ns,es);

			Assert::AreEqual(g.edges.size(), es.size());
			Assert::AreEqual(g.nodes.size(), ns.size());
			for (auto i = 0; i < g.edges.size(); ++i) {
				Assert::IsTrue(g.edges[i] == es[i]);
			}
			for (auto i = 0; i < g.nodes.size(); ++i) {
				Assert::IsTrue(g.nodes[i] == ns[i]);
			}
		}
		TEST_METHOD(test_for_copy_constructor) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);
			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(1, { n1, v2 }, { n2, v3 });

			vector<Node> ns{ n1, n2 };
			vector<Edge> es{ e1, e2 };
			auto g1 = make_shared<Graph>(ns, es);
			auto g2 = make_shared<Graph>(*g1);

			Assert::AreEqual(g1->edges.size(), g2->edges.size());
			Assert::AreEqual(g1->nodes.size(), g2->nodes.size());
			for (auto i = 0; i < g1->edges.size(); ++i) {
				Assert::IsTrue(g1->edges[i] == g2->edges[i]);
			}
			for (auto i = 0; i < g1->nodes.size(); ++i) {
				Assert::IsTrue(g1->nodes[i] == g2->nodes[i]);
			}
			Assert::IsFalse(g1 == g2);
		}
	};
	TEST_CLASS(test_for_production) {
		TEST_METHOD(test_for_default_constructor) {
			auto p = make_shared<Production>();
			Assert::IsNotNull(p.get());
		}
		TEST_METHOD(test_for_2nd_constructor) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);
			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(6, { n1, v2 }, { n2, v3 });

			vector<Node> ns1{ n1, n2 };
			vector<Edge> es1{ e1, e2 };
			Graph g1(ns1, es1);
			
			Vertex v5('I', 1);
			Vertex v6('H', 2);
			vector<Vertex> vs3{ v5, v6 };
			Node n3(6, false, "if", vs3);

			Vertex v7('A', 4);
			Vertex v8('Y', 5);
			vector<Vertex> vs4{ v3, v4 };
			Node n4(7, true, "else", vs4);
			Edge e3(7, { n3, v5 }, { n3, v8 });
			Edge e4(5, { n3, v6 }, { n3, v7 });

			vector<Node> ns2{ n1, n2 };
			vector<Edge> es2{ e3, e4 };
			Graph g2(ns2, es2);

			Production p(g1, g2);

			Assert::AreEqual(p.l_graph.edges.size(), g1.edges.size());
			Assert::AreEqual(p.r_graph.edges.size(), g2.edges.size());
			Assert::AreEqual(p.l_graph.nodes.size(), g1.nodes.size());
			Assert::AreEqual(p.r_graph.nodes.size(), g2.nodes.size());

			for (auto i = 0; i < p.l_graph.edges.size(); ++i) {
				Assert::IsTrue(p.l_graph.edges[i] == g1.edges[i]);
			}
			for (auto i = 0; i < p.r_graph.edges.size(); ++i) {
				Assert::IsTrue(p.r_graph.edges[i] == g2.edges[i]);
			}
			for (auto i = 0; i < p.l_graph.nodes.size(); ++i) {
				Assert::IsTrue(p.l_graph.nodes[i] == g1.nodes[i]);
			}
			for (auto i = 0; i < p.r_graph.nodes.size(); ++i) {
				Assert::IsTrue(p.l_graph.nodes[i] == g1.nodes[i]);
			}	
		}
	};
	TEST_CLASS(test_for_delete_redex) {
		TEST_METHOD(test_for_no_dangle_edge) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);

			//isolated node
			Node n3(8, true, "state", {});

			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(6, { n1, v2 }, { n2, v3 });

			vector<Node> ns1{ n1, n2, n3 };
			vector<Edge> es1{ e1, e2 };

			Graph host_g(ns1, es1);
			Graph redex({ n3 }, {});

			delete_redex(host_g, redex);

			Assert::IsTrue(host_g.edges.size() == 2);
			Assert::IsTrue(host_g.nodes.size() == 2);
			Assert::IsTrue(host_g.nodes[0] == n1);
			Assert::IsTrue(host_g.nodes[1] == n2);
			Assert::IsTrue(host_g.edges[0] == e1);
			Assert::IsTrue(host_g.edges[1] == e2);
		}
		TEST_METHOD(test_for_dangle_edge) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);

			Vertex v5('C', 7);
			Vertex v6('D', 8);
			vector<Vertex> vs3{ v5, v6 };
			Node n3(8, true, "state", vs3);

			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(6, { n1, v2 }, { n2, v3 });
			Edge e3(4, { n1, v1 }, { n3, v5 });
			vector<Node> ns1{ n1, n2, n3 };
			vector<Edge> es1{ e1, e2, e3 };

			Graph host_g(ns1, es1);
			Graph redex({ n3 }, {});

			delete_redex(host_g, redex);

			Assert::IsTrue(host_g.edges.size() == 3);
			Assert::IsTrue(host_g.nodes.size() == 2);
			Assert::IsTrue(host_g.nodes[0] == n1);
			Assert::IsTrue(host_g.nodes[1] == n2);
			Assert::IsTrue(host_g.edges[0] == e1);
			Assert::IsTrue(host_g.edges[1] == e2);
			Assert::IsTrue(host_g.edges[2] == e3);
			Assert::IsTrue(host_g.edges[2].mark == v5.mark);
		}
		TEST_METHOD(test_for_deleted_edge_with_no_nodes) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);

			Vertex v5('C', 7);
			Vertex v6('D', 8);
			vector<Vertex> vs3{ v5, v6 };
			Node n3(8, true, "state", vs3);

			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(6, { n1, v2 }, { n2, v3 });
			Edge e3(4, { n1, v1 }, { n3, v5 });
			vector<Node> ns1{ n1, n2, n3 };
			vector<Edge> es1{ e1, e2, e3 };

			Graph host_g(ns1, es1);
			Graph redex({ n3, n1 }, {e3});

			delete_redex(host_g, redex);

			Assert::IsTrue(host_g.edges.size() == 2);
			Assert::IsTrue(host_g.nodes.size() == 1);

			Assert::IsTrue(host_g.edges[0] == e1);
			Assert::IsTrue(host_g.edges[1] == e2);
			Assert::IsTrue(host_g.edges[0].node1.first.id == -1);
			Assert::IsTrue(host_g.edges[1].node1.first.id == -1);

			Assert::IsTrue(host_g.nodes[0] == n2);
		}
		TEST_METHOD(test_for_deleted_edge_in_redex) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);

			Vertex v5('C', 7);
			Vertex v6('D', 8);
			vector<Vertex> vs3{ v5, v6 };
			Node n3(8, true, "state", vs3);

			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(6, { n1, v2 }, { n2, v3 });
			Edge e3(4, { n1, v1 }, { n3, v5 });
			vector<Node> ns1{ n1, n2, n3 };
			vector<Edge> es1{ e1, e2, e3 };

			Graph host_g(ns1, es1);
			Graph redex({ n3}, {e2});

			delete_redex(host_g, redex);

			Assert::IsTrue(host_g.edges.size() == 2);
			Assert::IsTrue(host_g.nodes.size() == 2);

			Assert::IsTrue(host_g.edges[0] == e1);
			Assert::IsTrue(host_g.edges[1] == e3);
			Assert::IsTrue(host_g.edges[1].node2.first.id == -1);

			Assert::IsTrue(host_g.nodes[0] == n1);
			Assert::IsTrue(host_g.nodes[1] == n2);
		}
	};
	TEST_CLASS(test_for_add_sub_graph) {
		TEST_METHOD(test_for_no_dangle_edge) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);

			//isolated node
			Node n3(8, true, "state", {});

			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(6, { n1, v2 }, { n2, v3 });

			vector<Node> ns1{ n1, n2 };
			vector<Edge> es1{ e1, e2 };

			Graph host_g(ns1, es1);
			Graph sub_g({ n3 }, {});

			add_subgraph(host_g, sub_g);

			Assert::IsTrue(host_g.edges.size() == 2);
			Assert::IsTrue(host_g.nodes.size() == 3);
		}
		TEST_METHOD(test_for_dangle_edge_need_to_be_connected) {
			Vertex v1('R', 1);
			Vertex v2('T', 2);
			vector<Vertex> vs1{ v1, v2 };
			Node n1(1, false, "send", vs1);

			Vertex v3('A', 4);
			Vertex v4('Y', 5);
			vector<Vertex> vs2{ v3, v4 };
			Node n2(2, true, "receive", vs2);

			//isolated node
			Vertex v5('A', 7);
			Node n3(8, true, "state", {v5});

			Edge e1(1, { n1, v1 }, { n2, v4 });
			Edge e2(6, { n1, v2 }, { n2, v3 });
			Edge e3(3, { n1, v2 }, { n3, {} });

			vector<Node> ns1{ n1, n2 };
			vector<Edge> es1{ e1, e2, e3 };

			Graph host_g(ns1, es1);
			// make host_g.edges[2] be a dangle edge
			host_g.edges[2].mark = 7;
			Graph sub_g({n3}, {});

			add_subgraph(host_g, sub_g);

			Assert::IsTrue(host_g.edges.size() == 3);
			Assert::IsTrue(host_g.nodes.size() == 3);
		}
	};
	TEST_CLASS(test_for_reading) {
		TEST_METHOD(test_for_generate_vertex) {
			string s = "T\n2";
			auto v = generate_vertex(s);
			Assert::IsTrue(v.label == 'T');
			Assert::IsTrue(v.mark == 2);
		}
		TEST_METHOD(test_for_generate_node_with_no_vertex) {
			std::string s = "2\n1\nstate";
			auto node = generate_node(s);
			Assert::IsTrue(node.id == 2);
			Assert::IsTrue(node.is_terminal = true);
			Assert::IsTrue(node.label == "state");
			Assert::IsTrue(node.vertices.size() == 1);
		}
		TEST_METHOD(test_for_generate_node_with_vertices) {
			std::string s = "2\n1\nstate\nVERTEX\nT\n2\nVERTEX\nD\n3";
			auto node = generate_node(s);
			Assert::IsTrue(node.id == 2);
			Assert::IsTrue(node.is_terminal = true);
			Assert::IsTrue(node.label =="state");
			Assert::IsTrue(node.vertices[0].label == 'T');
			Assert::IsTrue(node.vertices[0].mark == 2);
			Assert::IsTrue(node.vertices[1].label == 'D');
			Assert::IsTrue(node.vertices[1].mark == 3);
		}
		TEST_METHOD(test_for_generate_edge_with_no_vertices) {
			Node n1(2, false, "state", {});
			Node n2(4, true, "if", {});
			string s = "1\n2 -1\n4 -1";
			auto edge = generate_edge(s, { n1, n2 });
			Assert::IsTrue(edge.id == 1);
			Assert::IsTrue(edge.mark == 0);
			Assert::IsTrue(edge.node1.first == n1);
			Assert::IsTrue(edge.node2.first == n2);
			Assert::IsTrue(edge.node1.second.mark == NOMARK);
			Assert::IsTrue(edge.node2.second.mark == NOMARK);
		}
		TEST_METHOD(test_for_generate_edge_with_Vertives) {
			Node n1(2, false, "state", {Vertex('T', 2)});
			Node n2(4, true, "if", {Vertex('M', 0)});
			string s = "1\n2 2\n4 0";
			auto edge = generate_edge(s, { n1, n2 });
			Assert::IsTrue(edge.id == 1);
			Assert::IsTrue(edge.mark == 0);
			Assert::IsTrue(edge.node1.first == n1);
			Assert::IsTrue(edge.node2.first == n2);
			Assert::IsTrue(edge.node1.second.mark == 2);
			Assert::IsTrue(edge.node2.second.mark == 0);
		}
		TEST_METHOD(test_for_generate_graph) {
			string s = "NODE\n";
			s += "2\n1\nstate\nVERTEX\nT\n2\nVERTEX\nD\n3\n";
			s += "NODE\n";
			s += "3\n0\nif\nVERTEX\nT\n4\nVERTEX\nD\n5\n";
			s += "EDGE\n";
			s += "1\n2 2\n3 5\n";
			auto graph = generate_graph(s);
			Assert::IsTrue(graph.nodes.size() == 2);
			Assert::IsTrue(graph.edges.size() == 1);
			Assert::IsTrue(graph.nodes[0].id == 2);
			Assert::IsTrue(graph.nodes[1].id == 3);
			Assert::IsTrue(graph.edges[0].node1.first.id = 2);
			Assert::IsTrue(graph.edges[0].node2.first.id = 3);
			Assert::IsTrue(graph.edges[0].node1.second.mark = 2);
			Assert::IsTrue(graph.edges[0].node2.second.mark = 5);
		}
		TEST_METHOD(test_for_generate_production) {
			string s;
			s += "GRAPH\n";
			s += "NODE\n";
			s += "2\n1\nstate\nVERTEX\nT\n2\nVERTEX\nD\n3\n";
			s += "NODE\n";
			s += "3\n0\nif\nVERTEX\nT\n4\nVERTEX\nD\n5\n";
			s += "EDGE\n";
			s += "1\n2 2\n3 5\n";

			s += "GRAPH\n";
			s += "NODE\n";
			s += "2\n1\nstate\nVERTEX\nT\n2\nVERTEX\nD\n3\n";
			s += "NODE\n";
			s += "3\n0\nif\nVERTEX\nT\n4\nVERTEX\nD\n5\n";
			s += "EDGE\n";
			s += "1\n2 2\n3 5\n";

			auto production = generate_production(s);
			auto graph1 = production.l_graph;
			auto graph2 = production.r_graph;

			Assert::IsTrue(graph1.nodes.size() == 2);
			Assert::IsTrue(graph1.edges.size() == 1);
			Assert::IsTrue(graph1.nodes[0].id == 2);
			Assert::IsTrue(graph1.nodes[1].id == 3);
			Assert::IsTrue(graph1.edges[0].node1.first.id = 2);
			Assert::IsTrue(graph1.edges[0].node2.first.id = 3);
			Assert::IsTrue(graph1.edges[0].node1.second.mark = 2);
			Assert::IsTrue(graph1.edges[0].node2.second.mark = 5);

			Assert::IsTrue(graph2.nodes.size() == 2);
			Assert::IsTrue(graph2.edges.size() == 1);
			Assert::IsTrue(graph2.nodes[0].id == 2);
			Assert::IsTrue(graph2.nodes[1].id == 3);
			Assert::IsTrue(graph2.edges[0].node1.first.id = 2);
			Assert::IsTrue(graph2.edges[0].node2.first.id = 3);
			Assert::IsTrue(graph2.edges[0].node1.second.mark = 2);
			Assert::IsTrue(graph2.edges[0].node2.second.mark = 5);
		}
		TEST_METHOD(test_for_read_host_graph) {
			//auto graph = read_host_graph();
			//draw_process_in_html({ graph });
			//show_process();
		}
		TEST_METHOD(test_for_read_productions) {
			//read_productions();
		}
	};
	TEST_CLASS(test_for_is_one_to_one) {
		TEST_METHOD(test_with_same_node_order_matched) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Node n3(3, true, "state", {});
			Edge e1(1, { n1, {} }, { n2, {} });
			Edge e2(2, { n1, {} }, { n3, {} });

			Node n4(4, true, "state", {});
			Node n5(5, true, "if", {});
			Node n6(6, true, "state", {});
			Edge e3(4, { n4, {} }, { n5, {} });
			Edge e4(4, { n4, {} }, { n6, {} });

			auto [is_ok, node_map] = is_one_to_one({ e1, e2 }, { e3, e4 });
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_map[4] == 1);
			Assert::IsTrue(node_map[5] == 2);
			Assert::IsTrue(node_map[6] == 3);
		}
		TEST_METHOD(test_with_not_same_node_order_matched) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Node n3(3, true, "state", {});
			Edge e1(1, { n1, {} }, { n2, {} });
			Edge e2(2, { n1, {} }, { n3, {} });

			Node n4(4, true, "state", {});
			Node n5(5, true, "if", {});
			Node n6(6, true, "state", {});
			Edge e3(4, { n5, {} }, { n4, {} });
			Edge e4(4, { n4, {} }, { n6, {} });

			auto [is_ok, node_map] = is_one_to_one({ e1, e2 }, { e3, e4 });
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_map[4] == 1);
			Assert::IsTrue(node_map[5] == 2);
			Assert::IsTrue(node_map[6] == 3);
		}
		TEST_METHOD(test_with_label_not_matched) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Edge e1(1, { n1, {} }, { n2, {} });
			Node n3(5, true, "state", {});
			Node n4(4, true, "else", {});
			Edge e2(4, { n3, {} }, { n4, {} });
			auto [is_ok, node_map] = is_one_to_one({ e1 }, { e2 });
			Assert::IsTrue(!is_ok);
		}
		TEST_METHOD(test_with_duplicate_label) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "state", {});
			Node n3(3, true, "state", {});
			Node n4(4, true, "if", {});
			Node n5(5, true, "if", {});
			Edge e1(1, { n1, {} }, { n4, {} });
			Edge e2(2, { n2, {} }, { n4, {} });
			Edge e3(2, { n2, {} }, { n5, {} });
			Edge e4(2, { n3, {} }, { n4, {} });

			Node n6(6, true, "state", {});
			Node n7(7, true, "state", {});
			Node n8(8, true, "state", {});
			Node n9(9, true, "if", {});
			Node n10(10, true, "if", {});
			Edge e5(4, { n6, {} }, { n9, {} });
			Edge e6(4, { n7, {} }, { n10, {} });
			Edge e7(4, { n6, {} }, { n9, {} });
			Edge e8(4, { n8, {} }, { n10, {} });

			auto [is_ok, node_map] = is_one_to_one({ e1, e2, e3, e4 }, { e5, e6, e7, e8 });
			Assert::IsTrue(!is_ok);
		}
	};
	TEST_CLASS(test_for_is_node_matched) {
		TEST_METHOD(test_for_get_lable_count) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "state", {});
			Node n3(3, true, "state", {});
			Node n4(4, true, "if", {});
			Node n5(5, true, "if", {});
			Edge e1(1, { n1, {} }, { n4, {} });
			Edge e2(2, { n2, {} }, { n4, {} });
			Edge e3(2, { n2, {} }, { n5, {} });
			Edge e4(2, { n3, {} }, { n4, {} });

			auto lable_count = get_lable_count({ e1, e2, e3, e4 });
			Assert::IsTrue(lable_count["state"] == 3);
			Assert::IsTrue(lable_count["if"] == 2);
		}
		TEST_METHOD(test_for_nodes_count_not_same) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "state", {});
			Node n3(3, true, "state", {});
			Node n4(4, true, "if", {});
			Node n5(5, true, "if", {});
			Edge e1(1, { n1, {} }, { n4, {} });
			Edge e2(2, { n2, {} }, { n4, {} });
			Edge e3(2, { n2, {} }, { n5, {} });
			Edge e4(2, { n3, {} }, { n4, {} });

			Node n6(6, true, "state", {});
			Node n7(7, true, "state", {});
			Node n8(8, true, "state", {});
			Node n9(9, true, "if", {});
			Edge e5(4, { n6, {} }, { n9, {} });
			Edge e6(4, { n7, {} }, { n8, {} });
			Edge e7(4, { n6, {} }, { n9, {} });
			Edge e8(4, { n6, {} }, { n7, {} });

			auto [is_ok, edge_map, node_map] = is_node_matched({ e1, e2, e3, e4 }, { e5, e6, e7, e8 });
			Assert::IsTrue(!is_ok);
		}
		TEST_METHOD(test_for_duplicate_label) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "state", {});
			Node n3(3, true, "state", {});
			Node n4(4, true, "if", {});
			Node n5(5, true, "if", {});
			Edge e1(1, { n1, {} }, { n4, {} });
			Edge e2(2, { n2, {} }, { n4, {} });
			Edge e3(2, { n2, {} }, { n5, {} });
			Edge e4(2, { n3, {} }, { n4, {} });

			Node n6(6, true, "state", {});
			Node n7(7, true, "state", {});
			Node n8(8, true, "state", {});
			Node n9(9, true, "if", {});
			Node n10(10, true, "if", {});
			Edge e5(4, { n6, {} }, { n9, {} });
			Edge e6(4, { n7, {} }, { n10, {} });
			Edge e7(4, { n6, {} }, { n9, {} });
			Edge e8(4, { n8, {} }, { n10, {} });

			auto [is_ok, edge_map, node_map] = is_node_matched({ e1, e2, e3, e4 }, { e5, e6, e7, e8 });
			Assert::IsTrue(!is_ok);
		}
		TEST_METHOD(test_for_matched_by_order) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Node n3(3, true, "state", {});
			Edge e1(1, { n1, {} }, { n2, {} });
			Edge e2(2, { n1, {} }, { n3, {} });

			Node n4(4, true, "state", {});
			Node n5(5, true, "if", {});
			Node n6(6, true, "state", {});
			Edge e3(4, { n4, {} }, { n5, {} });
			Edge e4(4, { n4, {} }, { n6, {} });

			auto [is_ok, edge_map, node_map] = is_node_matched({ e1, e2 }, { e3, e4 });
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_map[4] == 1);
			Assert::IsTrue(node_map[5] == 2);
			Assert::IsTrue(node_map[6] == 3);
		}
		TEST_METHOD(test_for_matched_not_by_order) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Node n3(3, true, "state", {});
			Edge e1(1, { n1, {} }, { n2, {} });
			Edge e2(2, { n1, {} }, { n3, {} });

			Node n4(4, true, "state", {});
			Node n5(5, true, "if", {});
			Node n6(6, true, "state", {});
			Edge e3(4, { n4, {} }, { n5, {} });
			Edge e4(4, { n4, {} }, { n6, {} });

			auto [is_ok, edge_map, node_map] = is_node_matched({ e1, e2 }, { e4, e3 });
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_map[4] == 1);
			Assert::IsTrue(node_map[5] == 2);
			Assert::IsTrue(node_map[6] == 3);
		}
	};
	TEST_CLASS(test_for_is_isolated_node_matched) {
		TEST_METHOD(test_for_available_labels_not_enough) {
			Node n1(1, true, "state", {}); 
			Node n2(2, true, "state", {});
			Node n3(3, true, "state", {});
			auto [is_ok, node_map] = is_isolated_node_matched({ n1 }, { n2, n3 });
			Assert::IsTrue(!is_ok);
		}
		TEST_METHOD(test_for_only_one_lable_with_one_node) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "state", {});
			auto [is_ok, node_maps] = is_isolated_node_matched({ n1 }, { n2 });
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_maps.size() == 1);
			Assert::IsTrue(node_maps[0][2] == 1);
		}
		TEST_METHOD(test_for_one_lable_with_multiple_nodes) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "state", {});
			Node n3(3, true, "state", {});
			auto [is_ok, node_maps] = is_isolated_node_matched({ n1, n3 }, { n2 });
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_maps.size() == 2);
			Assert::IsTrue(node_maps[0][2] == 1);
			Assert::IsTrue(node_maps[1][2] == 3);
		}
		TEST_METHOD(test_for_mutiple_labels_with_one_node) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Node n3(3, true, "state", {});
			Node n4(4, true, "if", {});
			auto [is_ok, node_maps] = is_isolated_node_matched({ n1, n2 }, { n3, n4 });
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_maps.size() == 1);
			Assert::IsTrue(node_maps[0][3] == 1);
			Assert::IsTrue(node_maps[0][4] == 2);

		}
		TEST_METHOD(test_for_mutiple_labels_with_mutiple_nodes) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Node n3(3, true, "if", {});
			Node n4(4, true, "state", {});
			Node n5(5, true, "if", {});
			auto [is_ok, node_maps] = is_isolated_node_matched({ n1, n2, n3 }, { n4, n5 });
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_maps.size() == 2);
		}
	};
	TEST_CLASS(test_for_handle_isolated_nodes) {
		TEST_METHOD(test_for_simple_condition) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "state", {});
			Node n3(3, true, "state", {});
			std::unordered_map<int, int> m{ {5, 2} };
			auto [is_ok, node_map] = handle_isolated_nodes({ n1, n2 }, { n3 }, m);
			Assert::IsTrue(is_ok);
			Assert::IsTrue(node_map.size() == 1);
		}
	};
	TEST_CLASS(test_for_find_redex) {
		TEST_METHOD(test_for_comb_elem) {
			Edge edge1(1, {}, {});
			Edge edge2(2, {}, {});
			Edge edge3(3, {}, {});
			Edge edge4(4, {}, {});
			Edge edge5(5, {}, {});
			Edge edge6(6, {}, {});
			std::vector<Edge> edges{ edge1, edge2, edge3, edge4, edge5, edge6 };

			auto comb_edges1 = comb_elem(edges, 6);
			Assert::IsTrue(comb_edges1.size() == 1);

			auto comb_edges2 = comb_elem(edges, 5);
			Assert::IsTrue(comb_edges2.size() == 6);
		}
		TEST_METHOD(test_for_get_redex_by_matched) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Edge e1(1, { n1, {} }, { n2, {} });
			Graph sub_graph({ n1, n2 }, { e1 });
			std::unordered_map<int, int> edge_map{ {1, 2} };
			std::unordered_map<int, int> node_map{ {1, 3}, {2, 4} };
			auto redex = get_redex_by_matched(sub_graph, edge_map, node_map);
			Assert::IsTrue(redex.edges[0].id == 2);
			Assert::IsTrue(redex.edges[0].node1.first.id == 3);
			Assert::IsTrue(redex.edges[0].node2.first.id == 4);
			Assert::IsTrue(redex.nodes[0].id == 3);
			Assert::IsTrue(redex.nodes[1].id == 4);
		}
		TEST_METHOD(test_for_with_no_edge) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "state", {});
			Node n3(3, true, "state", {});

			Graph g1({ n1, n2 }, {});
			Graph g2({ n3 }, {});

			auto redexes = find_redex(g1, g2);
			Assert::IsTrue(redexes.size() == 2);
			Assert::IsTrue(redexes[0].nodes[0].id == 1);
			Assert::IsTrue(redexes[1].nodes[0].id == 2);
		}
		TEST_METHOD(test_for_with_no_isolated_node) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Node n3(3, true, "else", {});
			Node n1231(121, true, "dfafds", {});
			Edge e1(1, { n1, {} }, { n2, {} });
			Edge e2(2, { n1, {} }, { n3, {} });
			Edge e3(3, { n2, {} }, { n3, {} });
			Edge e10(2131, { n1, {} }, { n1231, {} });
			Graph g1({ n1, n2, n3, n1231 }, { e1, e2, e3, e10 });

			Node n4(4, true, "state", {});
			Node n5(5, true, "if", {});
			Node n6(6, true, "else", {});
			Edge e4(1, { n4, {} }, { n5, {} });
			Edge e5(2, { n4, {} }, { n6, {} });
			Edge e6(3, { n5, {} }, { n6, {} });
			Graph g2({ n4, n5, n6 }, { e4, e5, e6 });
			
			auto redexes = find_redex(g1, g2);
			
			Assert::IsTrue(redexes.size() == 1);
		}
		TEST_METHOD(test_for_with_isolated_node) {
			Node n1(1, true, "state", {});
			Node n2(2, true, "if", {});
			Node n3(3, true, "else", {});
			Edge e1(1, { n1, {} }, { n2, {} });
			Edge e2(2, { n1, {} }, { n3, {} });
			Edge e3(3, { n2, {} }, { n3, {} });
			Graph g1({ n1, n2, n3 }, { e1, e2, e3 });

			Node n4(4, true, "state", {});
			Node n5(5, true, "if", {});
			Node n6(6, true, "else", {});
			Edge e4(1, { n4, {} }, { n5, {} });
			Graph g2({ n4, n5, n6 }, { e4});

			auto redexes = find_redex(g1, g2);

			Assert::IsTrue(redexes.size() == 1);
		}
	};
}