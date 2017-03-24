/////////////////////////////////////////////////////////////////////////////
// TypeAnal.cpp - Testing the TypeAnalysis part of Dependency Analyzer     //
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

#include "TypeAnal.h"

#include <iostream>

using namespace std;
using namespace CodeAnalysis;

#ifdef TEST_TYPEANAL

/////////////////////////////////////////////////////////////////////////////
// Definition of main() method

int main()
{
	cout << "Testing TypeAnal";

	TypeAnal ta;

	ta.doTypeAnal();

}
#endif
