////////////////////////////////////////////////////////////////////////////////
// DepAnal.cpp - Provides Dependency Analysis by analyzing #include stmts  	  //			
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

#include <iostream>
#include <fstream>
#include "DepAnal.h"

//----< provides access to the dependency map  >--------------------
unordered_map<string, set<string>> DepAnal::getdepmap()
{
	return depmap;
}

//----< logic to analyze #include stmts and store dependecies in map  >--------------------
void DepAnal::processDependency(set<string> allfiles ,string path, string filename)
{
	ifstream file1(path + filename);
	string fstr;
	while (getline(file1, fstr))
	{
		if (fstr[0] == '#')
		{
			std::size_t found = fstr.find("\"");
			if(found != std::string::npos)
			{	
				std::size_t found2 = fstr.find("/");
				if (found2 != std::string::npos)
				{
					size_t start = fstr.rfind("/");
					size_t end = fstr.rfind("\"");
					string depstr = fstr.substr(start + 1, end - start - 1);
					if (filename != depstr && depstr[0] != '#')
					{
						if (allfiles.find(depstr) != allfiles.end())
						{
							depmap[filename].insert(depstr);
						}
					}
				}
				else
				{
					size_t start2 = fstr.find("\"");
					size_t end2 = fstr.rfind("\"");
					string depstr2 = fstr.substr(start2 + 1, end2 - start2 - 1);
					if (filename != depstr2 && depstr2[0] != '#')
					{
						if (allfiles.find(depstr2) != allfiles.end())
						{
							depmap[filename].insert(depstr2);
						}
					}
						
				}
			}			
		}
	}
}

//----< displays dependecies on console >--------------------
void DepAnal::displayDependency()
{	
	cout << "   -------------------------------------------------------------------------" << endl;
	cout << "   File Dependencies:" << endl;
	cout << "   -------------------------------------------------------------------------" << endl;

	for (auto a : depmap)
	{
		cout << "    " << a.first << endl;
		for (auto b : a.second)
			cout << "    " << "   ->  " << b << endl;
	}

}
