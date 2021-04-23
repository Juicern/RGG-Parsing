#pragma
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <string>
#include <list>
#include <fstream>
#include <algorithm>
#include <exception>

#define make_check 1

using namespace std;

class Graph
{
public:
	Graph(char* name);
	Graph(unsigned vertex_number);
	Graph(Graph& other);
	~Graph();

	void show();
	void show_isomorphism();

	void set_graph(unsigned n);
	bool check_isomorphism(Graph other);
	bool check_vertex_number(unsigned n);
	bool check_degrees(vector < vector <unsigned> > matrx);
	bool check_hard_case(Graph& other);

	bool match(vector <int>& b_table, Graph& other, unsigned match_this, unsigned match_on);
	int bijection(vector <int> b_table, unsigned match_this);
	bool is_connected(Graph& other, unsigned match_this, unsigned match_on);
	bool already_matched(vector <int> b_table, unsigned to_match);
	void unmatch(vector <int>& b_table, unsigned unmatch_this);
	bool tree_check(vector <int> b_table, Graph& other, unsigned match_this, unsigned match_on);

	list <unsigned> give_list_of_degrees(vector<vector<unsigned>> matrx);
	void count(vector < vector <unsigned> >& matrx);

private:
	vector < vector <unsigned> > matrx;
	unsigned vertex_number;
	bool iso_flag;
	vector<int> isomorphism_table;
};

inline Graph::Graph(unsigned vertex_number = 0) : vertex_number(vertex_number), matrx(), iso_flag(0)
{
	set_graph(vertex_number);
	count(matrx);
}

inline Graph::Graph(Graph& other) : matrx(other.matrx), vertex_number(other.vertex_number), isomorphism_table(other.isomorphism_table), iso_flag(iso_flag)
{}

Graph::Graph(char* name) : Graph()
{
	ifstream file_stream;
	int buffer;

	int x, y;

	file_stream.open(name, ifstream::in);
	//if (!file_stream.good()) throw std::out_of_range("No file\n");
	if (!file_stream.good())
	{
		cout << "file not found";
		//file_stream.close();
		exit(0);
	}
	file_stream >> vertex_number;

	set_graph(vertex_number);

	if (file_stream.is_open())
	{
		while (!file_stream.eof())
		{
			file_stream >> buffer;
			x = buffer;
			//			buffer.erase(buffer.begin(), buffer.end());

			file_stream >> buffer;
			y = buffer;
			//			buffer.erase(buffer.begin(), buffer.end());

			matrx[x][y] = 1;
			matrx[y][x] = 1;
		}
	}
	file_stream.close();
	count(matrx);
}

Graph::~Graph()
{

}

inline void Graph::show()
{
	cout << "\t";
	for (unsigned i = 0; i < vertex_number; i++)
		cout << i << "\t";

	cout << endl;
	for (unsigned i = 0; i < vertex_number; i++)
	{
		cout << i << "\t";
		for (unsigned j = 0; j <= vertex_number; j++)
			cout << matrx[i][j] << "\t";
		cout << "\n";
	}
	cout << "\n";
}

inline void Graph::show_isomorphism()
{
	if (iso_flag)
	{
		cout << "grafy izomorficzne\n";
		for (unsigned i = 0; i < isomorphism_table.size(); i++)
			cout << i << " --> " << isomorphism_table[i] << "\n";
	}
	else cout << "grafy nieizomorficzne";
	cout << endl;

}

inline void Graph::set_graph(unsigned n)
{
	unsigned count = 0;
	for (unsigned i = 0; i < vertex_number; i++)
	{
		vector <unsigned> vec;

		for (unsigned i = 0; i < vertex_number + 1; i++)
			vec.push_back(0);

		for (unsigned j = 0; j < vertex_number; j++)
			matrx.push_back(vec);
	}
}

bool Graph::check_isomorphism(Graph other)
{
	return (check_vertex_number(other.vertex_number)
		&& check_degrees(other.matrx)
		&& check_hard_case(other));
}

inline bool Graph::check_vertex_number(unsigned n)
{
	return vertex_number == n;
}

bool Graph::check_degrees(vector < vector <unsigned> > m)
{
	return give_list_of_degrees(matrx) == give_list_of_degrees(m);
}

bool Graph::check_hard_case(Graph& other)
{
	vector <int> b_table;
	for (int i = 0; i < vertex_number; i++)
	{
		b_table.push_back(-1);
	}

	return tree_check(b_table, other, 0, 0);
}

bool Graph::match(vector <int>& b_table, Graph& other, unsigned match_this, unsigned match_on)
{
	bool is_one, is_zero;
	if (matrx[match_this][vertex_number] != other.matrx[match_on][vertex_number]) return false;
	if (b_table[match_this] == int(match_on)) return true;
	if (b_table[match_this] != -1) return false;

	for (unsigned i = 0; i < match_this; i++)
	{
		if (matrx[match_this][i] == 1)
		{
			if (is_one = !is_connected(other, bijection(b_table, i), match_on))
				return false;
		}
		if (matrx[match_this][i] == 0)
		{
			if (is_zero = is_connected(other, bijection(b_table, i), match_on))
				return false;
		}
	}
	b_table[match_this] = int(match_on);

	return true;
}

inline int Graph::bijection(vector <int> b_table, unsigned i)
{
	return b_table[i];
}

inline bool Graph::is_connected(Graph& other, unsigned match_this, unsigned match_on)
{
	if (other.matrx[match_this][match_on] == 1) return 1;
	return 0;
}

inline bool Graph::already_matched(vector<int> b_table, unsigned to_check)
{
	for (int i = 0; i < b_table.size(); i++)
	{
		if (b_table[i] == to_check) return true;
	}

	return false;
}

inline void Graph::unmatch(vector<int>& b_table, unsigned unmatch_this)
{
	b_table[unmatch_this] = -1;
}

bool Graph::tree_check(vector<int> b_table, Graph& other, unsigned from, unsigned matches_number)
{
	if (matches_number == vertex_number)
	{
		isomorphism_table = b_table;
		iso_flag = 1;
		return true;
	}

	if (bijection(b_table, from) != -1) return false;
	for (int i = 0; i < vertex_number; i++)
	{
		if (already_matched(b_table, i)) continue;
		if (match(b_table, other, from, i))
		{
			matches_number++;
			if (tree_check(b_table, other, from + 1, matches_number)) return true;
			unmatch(b_table, from);
			matches_number--;
		}


	}
	return false;
}

inline list<unsigned> Graph::give_list_of_degrees(vector< vector<unsigned> > matrx)
{
	list <unsigned> lista;
	for (unsigned i = 0; i < vertex_number; i++)
	{
		lista.push_back(matrx[i][vertex_number]);
	}
	lista.sort();
	return lista;
}

void Graph::count(vector<vector<unsigned>>& matrx)
{
	for (unsigned i = 0; i < vertex_number; i++)
	{
		unsigned count = 0;
		for (unsigned j = 0; j < vertex_number; j++)
		{
			if (matrx[i][j] == 1) count++;
		}
		matrx[i][vertex_number] = count;
	}

}