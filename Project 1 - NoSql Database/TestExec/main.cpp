/////////////////////////////////////////////////////////////////////////////
// main.cpp - Main function for Test Executive                             //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Rahul Kadam, 2017                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Hp, Core i5, Windows 10                                    //
// Application: OOD Project 1 - NoSql Database ,Spring 2017                //
// Author:      Rahul Kadam, Syracuse University            		       //
//              (315) 751-8862, rkadam@syr.edu		                       //
/////////////////////////////////////////////////////////////////////////////

#include "TestExec.h"

using namespace std;

#ifndef TEST_MAIN
#define TEST_MAIN

/////////////////////////////////////////////////////////////////////////////
// Definition of main() method

int main()
{
	cout << "\n \t \t \t \t \t Welcome to My NoSQL Database";
	cout << "\n \t \t \t \t \t =============================\n";

	// Creating an Instance of TestExec class
	TestExec test;

	// Calling all the requirements in the Project
	test.requirement1();
	test.requirement2a();	
	test.requirement2b();
	test.requirement3();	
	test.requirement4();	
	test.requirement5();	
	test.requirement6();	
	test.requirement7a();
	test.requirement7b();
	test.requirement7c();
	test.requirement7d();
	test.requirement7e();
	test.requirement7f();
	test.requirement7g();
	test.requirement8();	
	test.requirement9();	
	test.requirement10();	 
	test.requirement11();	 
	test.requirement12();
	
	cout << "\n\n";
	cout << "\t \t \t Thank You !!!!!";
	cout << "\n\n";

	return 0;
}

#endif
