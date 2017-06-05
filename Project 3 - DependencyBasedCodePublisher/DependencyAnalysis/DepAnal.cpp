/////////////////////////////////////////////////////////////////////////////
// DepAnal.cpp - Testing the Dependency Analyis part of Dependency Analyzer//
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

#include "DepAnal.h"
#include "../TypeTable/TypeTable.h"
#include "../NoSqlDb/NoSqlDb.h"
#include <iostream>

using namespace std;
using namespace CodeAnalysis;

#ifdef TEST_DEPANAL

/////////////////////////////////////////////////////////////////////////////
// Definition of main() method

int main()
{
	cout << "Testing DepAnal ....";

	string fileSpec = "../My Test Files/A.cpp";

	DepAnal da;

	TypeTable tt;

	Scanner::Toker toker;

	da.searchDependency(fileSpec, tt, toker);

	da.displayDependency();

	NoSqlDb<string> db;

	da.storeToNoSqlDb(db);

	da.getDepMap();

}
#endif // TEST_DEPANAL