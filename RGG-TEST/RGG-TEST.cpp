#include "pch.h"
#include "CppUnitTest.h"
#include "../RGG-Backend/Graph.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace RGGTEST {
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
			Vertex v2('D', 2);
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
			Graph g1(ns, es);
			Graph g2(g1);

			Assert::AreEqual(g1.edges.size(), g2.edges.size());
			Assert::AreEqual(g1.nodes.size(), g2.nodes.size());
			for (auto i = 0; i < g1.edges.size(); ++i) {
				Assert::IsTrue(g1.edges[i] == g2.edges[i]);
			}
			for (auto i = 0; i < g1.nodes.size(); ++i) {
				Assert::IsTrue(g1.nodes[i] == g2.nodes[i]);
			}
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
}