#ifndef DEPANAL_H
#define DEPANAL_H
////////////////////////////////////////////////////////////////////////////////
// DepAnal.h - Provides Dependency Analysis by analyzing #include stmts  	  //			
// ver 1.0																	  //			
// -----------------------------------------------------------------------	  //			
// copyright © Rahul Kadam, 2017											  //			
// All rights granted provided that this notice is retained					  //			
// -----------------------------------------------------------------------	  //			
// Language:    Visual C++, Visual Studio 2015								  //			
// Platform:    Hp, Core i5, Windows 10										  //			
// Application: OOD Project 4 - RemoteCodePublisher ,Spring 2017	          //			
// Source:      Jim Fawcett,  CST 4-187, 									  //			
//              (315) 443-3948 , jfawcett@twcny.rr.com						  //			
// Author:      Rahul Kadam, Syracuse University							  //			
//              (315) 751-8862, rkadam@syr.edu								  //			
////////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  This module provides Dependency Analysis by analyzing #include stmts.
*  The various operations supported here are as follows:
*
*	getdepmap			->	provides access to the dependency map
*	processDependency	->	logic to analyze #include stmts and store dependecies in map
*	displayDependency	->	displays dependecies on console
* 
*  Required Files:
*  ---------------
*  - DepAnal.h ,DepAnal.cpp, main.cpp
*  - FileSystem.h , FileSystem.cpp
* 
*  Maintanence History:
*  --------------------
*  ver 1.0 : 2 May 2017
*  - first release
*
*/

#include <string>
#include <unordered_map>
#include <set>

using namespace std;

///////////////////////////////////////////////////////////////////////
// DepAnal class  
// - provides dependency analysis by analyzing #include statements

class DepAnal
{
private:
	unordered_map<string, set<string>> depmap;			// dependency map
public:
	unordered_map<string, set<string>> getdepmap();
	void processDependency(set<string> allfiles, string path,string filename);
	void displayDependency();
};



#endif
