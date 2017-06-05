#ifndef GRAPH_H
#define GRAPH_H
/////////////////////////////////////////////////////////////////////////////
// Graph.h - Graph Library + Strong Componenets							   //
// ver 1.0 	   														       //
// ----------------------------------------------------------------------- //
// copyright © Rahul Kadam, 2017                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Hp, Core i5, Windows 10                                    //
// Application: OOD Project 2 - TypeBasedDepAnalyzer ,Spring 2017          //
// Source:      Jim Fawcett,  CST 4-187, 							       //
//              (315) 443-3948 , jfawcett@twcny.rr.com                     //
// Author:      Rahul Kadam, Syracuse University          		           //
//              (315) 751-8862, rkadam@syr.edu							   //
/////////////////////////////////////////////////////////////////////////////
/*
Package Operations:
===================
A graph is a collection of vertices, connected by edges.  Each
vertex holds some kind of information held in an instance of
a type V and an integer id, which should be unique with a given
graph.  Each edge connects a parent vertex with a child
vertex and it too holds information in an instance of a type E.

This package provides two classes that support creation of
instances of a graph: Vertex<V,E> and Graph<V,E>.  These are
template-based with parameters V and E, as described above.

The graph holds an aggregated collection of vertices.  Each
vertex may hold one or more edges.  The edge is implemented
as an instance of pair<int,E> where the first parameter
is the id of the child vertex and the second parameter is
an instance of the edge information type E.

Note:
It is important that vertex ids be unique.  If you set any of
them with the second argument of the Vertex constructor or with
the id() function it is up to you to ensure uniqueness.  If you
don't explicitly set any ids then the constructor insures that
vertices get sequential ids in the order constructed.

Added Strong Components:
A strong component is the largest set of files that are all mutually dependent.
That is, all the files which can be reached from any other file in the set by following
direct or transitive dependency links.
The term "Strong Component" comes from the theory of directed graphs.
There are a number of algorithms for finding strong components in graphs.
Below is the Implementation of Tarjan Algorithm.

Tarjan Algorithm Functions :
//  The function to do DFS traversal. It uses SCCUtil()
->	void SCC();
// A recursive function that finds and prints strongly connected
// components using DFS traversal
// u --> The vertex to be visited next
// disc[] --> Stores discovery times of visited vertices
// low[] -- >> earliest visited vertex (the vertex with minimum
//             discovery time) that can be reached from subtree
//             rooted with current vertex
// *st -- >> To store all the connected ancestors (could be part
//           of SCC)
// stackMember[] --> bit/index array for faster check whether
//                  a node is in stack
->	void SCCUtil(Vertex<V, E> u, int disc[], int low[], stack<Vertex<V, E>> *st, bool stackMember[]);


Maintanence Information:
========================
Required files:
---------------
Graph.h, Graph.cpp,

Build Process:
--------------
Using Visual Studio Command Prompt:
devenv Graph.sln /rebuild debug

Revision History:
-----------------
ver 1.6 : 7 March 17
-  Now Supports Strong Components
ver 1.5 : 16 Feb 13
- now supports dfs with both function pointers and functors
ver 1.4 : 11 Feb 13
- removed indent function, now part of XmlWriter,
replaced ctor with default param with two ctors,
one supplies sequential ids, the other accepts user defined id
ver 1.3 : 28 Jan 13
- added GraphToXmlString and GraphFromXmlString functions
ver 1.2 : 25 Jan 13
- changed id() return to reference so can be changed
ver 1.1 : 21 Jan 13
- added prologue comments, compile test directive
ver 1.0 : 14 Jan 13
- first release

*/
/*
* To Do:
* - move GraphToXml and GraphFromXml to separate package
* - Strong Components, Topological Sorting
* - Find disconnected graph partitions
*/

#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <algorithm>
#include <iostream>
#include <sstream>

#define NIL -1

using namespace std;

namespace GraphLib
{
	///////////////////////////////////////////////////////////////
	// Vertex class

	template<typename V, typename E>
	class Vertex
	{
	public:
		typedef pair<int, E> Edge;  // graph index of target vertex, edge type
		typename typedef vector<Edge>::iterator iterator;
		iterator begin();
		iterator end();
		Vertex(V v, size_t id);
		Vertex(V v);
		void add(Edge& edge);
		// Vertex(const Vertex<V,E>& v);                  compiler generated is correct
		// Vertex<V,E>& operator=(const Vertex<V,E>& v);  compiler generated is correct
		Edge& operator[](size_t i);
		Edge operator[](size_t i) const;
		V& value();
		size_t& id();
		size_t size();
		bool& mark();

	private:
		vector<Edge> _edges;
		V _v;
		size_t _id;
		static size_t count;
		bool _mark;
	};
	//----< reserve memory for, and initialize, static count >-----
	template<typename V, typename E>
	size_t Vertex<V, E>::count = 0;

	//----< set and return boolean mark, used for traversal >------
	template<typename V, typename E>
	bool& Vertex<V, E>::mark() { return _mark; }

	//----< return iterator pointing to first edge >---------------
	template<typename V, typename E>
	typename Vertex<V, E>::iterator Vertex<V, E>::begin() { return _edges.begin(); }

	//----< return iterator pointing to one past last edge >-------
	template<typename V, typename E>
	typename Vertex<V, E>::iterator Vertex<V, E>::end() { return _edges.end(); }

	//----< construct instance, specifying id - must be unique >---
	template<typename V, typename E>
	Vertex<V, E>::Vertex(V v, size_t id) : _v(v), _id(id), _mark(false) {}

	//----< construct instance - creates id sequentially >-------
	template<typename V, typename E>
	Vertex<V, E>::Vertex(V v) : _v(v), _id(count++), _mark(false) {}

	//----< add edge to vertex edge collection >-------------------
	template<typename V, typename E>
	void Vertex<V, E>::add(Edge& edge) { _edges.push_back(edge); }

	//----< index non-const vertex's edges >-----------------------
	template<typename V, typename E>
	typename Vertex<V, E>::Edge& Vertex<V, E>::operator[](size_t i) { return _edges[i]; }

	//----< index const vertex's edges >---------------------------
	template<typename V, typename E>
	typename Vertex<V, E>::Edge Vertex<V, E>::operator[](size_t i) const { return _edges[i]; }

	//----< set and read value of vertex's held type, V >----------
	template<typename V, typename E>
	V& Vertex<V, E>::value() { return _v; }

	//----< return vertex's id >-----------------------------------
	template<typename V, typename E>
	size_t& Vertex<V, E>::id() { return _id; }

	//----< return number of edges >-------------------------------
	template<typename V, typename E>
	size_t Vertex<V, E>::size() { return _edges.size(); }

	///////////////////////////////////////////////////////////////
	// Graph class

	template<typename V, typename E>
	class Graph
	{
	public:
		typename typedef vector< Vertex<V, E> >::iterator iterator;
		iterator begin();
		iterator end();
		// Graph(const Graph<V,E>& g);                  compiler generated is correct
		// Graph<V,E>& operator=(const Graph<V,E>& g);  compiler generated is correct
		Vertex<V, E>& operator[](size_t i);
		Vertex<V, E> operator[](size_t i) const;
		void addVertex(Vertex<V, E> v);
		void addEdge(E eval, Vertex<V, E>& parent, Vertex<V, E>& child);
		size_t findVertexIndexById(size_t id);
		size_t size();
		template<typename F>
		void dfs(Vertex<V, E>& v, F f);
		vector< Vertex<V, E> >  getVertexVector();
		void SCC();
		void SCCUtil(Vertex<V, E> u, int disc[], int low[], stack<Vertex<V, E>> *st, bool stackMember[]);

	private:
		vector< Vertex<V, E> > adj;
		unordered_map<size_t, size_t> idMap; // id maps to graph index
		template<typename F>
		void dfsCore(Vertex<V, E>& v, F f);
	};

	//----< get Vertex Vector>-------------
	template<typename V, typename E>
	vector< Vertex<V, E> > Graph<V, E>::getVertexVector()
	{
		return adj;
	}

	//----< return iterator pointing to first vertex >-------------
	template<typename V, typename E>
	typename Graph<V, E>::iterator Graph<V, E>::begin() { return adj.begin(); }

	//----< return iterator pointing one past last vertex >--------
	template<typename V, typename E>
	typename Graph<V, E>::iterator Graph<V, E>::end() { return adj.end(); }

	//----< index non-const graph's vertex collection >------------
	template<typename V, typename E>
	typename Vertex<V, E>& Graph<V, E>::operator[](size_t i) { return adj[i]; }

	//----< index const graph's vertex collection >----------------
	template<typename V, typename E>
	typename Vertex<V, E> Graph<V, E>::operator[](size_t i) const { return adj[i]; }

	//----< add vertex to graph's vertex collection >--------------
	template<typename V, typename E>
	void Graph<V, E>::addVertex(Vertex<V, E> v)
	{
		adj.push_back(v);
		idMap[v.id()] = adj.size() - 1;
	}
	//----< return number of vertices in graph's collection >------
	template<typename V, typename E>
	size_t Graph<V, E>::size() { return adj.size(); }

	//----< return index of vertex with specified id >-------------
	template<typename V, typename E>
	size_t Graph<V, E>::findVertexIndexById(size_t id)
	{
		return idMap[id];
	}
	//----< add edge from specified parent to child vertices >-----
	template<typename V, typename E>
	void Graph<V, E>::addEdge(E eVal, Vertex<V, E>& parent, Vertex<V, E>& child)
	{
		size_t childIndex = findVertexIndexById(child.id());
		if (childIndex == adj.size())
			throw exception("no edge child");
		size_t parentIndex = findVertexIndexById(parent.id());
		if (parentIndex == adj.size())
			throw exception("no edge parent");
		Vertex<V, E>::Edge e;
		e.first = (int)childIndex;
		e.second = eVal;
		adj[parentIndex].add(e);
	}
	//----< recursive depth first search with action f >-----------
	template<typename V, typename E>
	template<typename F>
	void Graph<V, E>::dfsCore(Vertex<V, E>& v, F f)
	{
		f(v);
		v.mark() = true;
		for (auto edge : v)
		{
			if (adj[edge.first].mark() == false)
				dfsCore(adj[edge.first], f);
		}
		for (auto& vert : adj)
		{
			if (vert.mark() == false)
				dfsCore(vert, f);
		}
	}
	//----< depth first search, clears marks for next search >-----
	template<typename V, typename E>
	template<typename F>
	void Graph<V, E>::dfs(Vertex<V, E>& v, F f)
	{
		dfsCore(v, f);
		for (auto& vert : adj)
			vert.mark() = false;
	}

	template<typename V, typename E>
	void Graph<V, E>::SCC()
	{
		int no_of_vertex = (int)adj.size();
		int *disc = new int[no_of_vertex];
		int *low = new int[no_of_vertex];
		bool *stackMember = new bool[no_of_vertex];
		stack<Vertex<V, E>> *st = new stack<Vertex<V, E>>();

		// Initialize disc and low, and stackMember arrays
		for (int i = 0; i < no_of_vertex; i++)
		{
			disc[i] = NIL;
			low[i] = NIL;
			stackMember[i] = false;
		}

		// Call the recursive helper function to find strongly
		// connected components in DFS tree with vertex 'i'
		for (Vertex<V, E> vert : adj)
		{
			if (disc[vert.id()] == NIL)
				SCCUtil(vert, disc, low, st, stackMember);
		}

	}

	template<typename V, typename E>
	void Graph<V, E>::SCCUtil(Vertex<V, E> u, int disc[], int low[], stack<Vertex<V, E>> *st, bool stackMember[])
	{
		// A static variable is used for simplicity, we can avoid use
		// of static variable by passing a pointer.
		static int time = 0;
		// Initialize discovery time and low value
		disc[u.id()] = low[u.id()] = ++time;
		st->push(u);
		stackMember[u.id()] = true;
		// Go through all vertices adjacent to this
		for (auto i = u.begin(); i != u.end(); ++i)
		{
			Vertex<V, E>::Edge v = *i;  // v is current adjacent of 'u'															
										// If v is not visited yet, then recur for it
			if (disc[v.first] == -1)
			{
				SCCUtil(adj[v.first], disc, low, st, stackMember);

				// Check if the subtree rooted with 'v' has a
				// connection to one of the ancestors of 'u'
				// Case 1 (per above discussion on Disc and Low value)
				low[u.id()] = min(low[u.id()], low[v.first]);
			}
			// Update low value of 'u' only of 'v' is still in stack
			// (i.e. it's a back edge, not cross edge).
			// Case 2 (per above discussion on Disc and Low value)
			else if (stackMember[v.first] == true)
				low[u.id()] = min(low[u.id()], disc[v.first]);
		}
		// head node found, pop the stack and print an SCC

		string out = "";
		if (low[u.id()] == disc[u.id()])
		{
			while (st->top().id() != u.id())
			{
				out = st->top().value();
				cout << "    " << out << "\n";
				stackMember[st->top().id()] = false;
				st->pop();
			}
			out = st->top().value();
			cout << "    " << out << "\n";
			cout << "    -------------------------------------------" << "\n";
			stackMember[st->top().id()] = false;
			st->pop();
		}
	}

	///////////////////////////////////////////////////////////////
	// Display class

	template<typename V, typename E>
	class Display
	{
	public:
		typedef Graph<V, E> graph;
		typedef Vertex<V, E> vertex;
		typedef pair<int, E> edge;

		static vector< Vertex<V, E> > vertsWithNoParents(graph& g)
		{
			vector<size_t> parentCount;
			graph::iterator iter = g.begin();
			while (iter != g.end())
			{
				parentCount.push_back(0);
				++iter;
			}
			iter = g.begin();
			while (iter != g.end())
			{
				vertex v = *iter;
				for (size_t i = 0; i<v.size(); ++i)
				{
					edge e = v[i];
					parentCount[e.first]++;
				}
				++iter;
			}
			vector<vertex> noParents;
			for (size_t j = 0; j<g.size(); ++j)
				if (parentCount[j] == 0)
					noParents.push_back(g[j]);
			return noParents;
		}

		static void show(graph& g)
		{
			graph::iterator iter = g.begin();
			while (iter != g.end())
			{
				vertex v = *iter;
				cout << "\n  vertex id = " << v.id() << ", value = " << (v.value()).c_str();
				for (size_t i = 0; i<v.size(); ++i)
				{
					vertex::Edge edge = v[i];
					cout << "\n    edge points to vertex with id = " << g[edge.first].id();
					cout << " and value = " << g[edge.first].value().c_str();
					cout << ", edge value = " << (edge.second).c_str();
				}
				++iter;
			}
		}
	};
	//----< convert integer to string >--------------------------
	string ToString(int i)
	{
		ostringstream in;
		in << i;
		return in.str();
	}
	//----< convert string to integer >--------------------------
	int ToInt(const string& str)
	{
		istringstream in(str);
		int retn;
		in >> retn;
		return retn;
	}
}
#endif
