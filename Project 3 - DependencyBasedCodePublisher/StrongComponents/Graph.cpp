/////////////////////////////////////////////////////////////////////////////
// Graph.cpp - Testing the Strong Components part of Dependency Analyzer   //
// ver 1.0                                                                 //
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

#include <iostream>
#include <fstream>
#include "Graph.h"

using namespace GraphLib;

typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;

#ifdef TEST_GRAPH

/////////////////////////////////////////////////////////////////////////////
// Definition of main() method

int main()
{
	std::cout << "\n  Testing Graph Library";
	std::cout << "\n =======================\n";
	try
	{
		std::cout << "\n  Constructing Graph instance";
		std::cout << "\n -----------------------------";

		graph g;

		vertex v0("0");
		vertex v1("1");
		vertex v2("2");
		vertex v3("3");
		vertex v4("4");
		g.addVertex(v0);
		g.addVertex(v1);
		g.addVertex(v2);
		g.addVertex(v3);
		g.addVertex(v4);
		g.addEdge("e1", v1, v0);
		g.addEdge("e2", v0, v2);
		g.addEdge("e3", v2, v1);
		g.addEdge("e4", v0, v3);
		g.addEdge("e5", v3, v4);

		display::show(g);
		std::cout << "\n";
		std::cout << "\n";
		std::cout << "   Strong Component Analysis:" << endl;
		std::cout << "\n";
		g.SCC();		// Performs Strong Components Analysis
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
	std::cout << "\n\n";
	return 0;
}

#endif

