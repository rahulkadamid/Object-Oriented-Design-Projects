////////////////////////////////////////////////////////////////////////////////
// main.cpp - Testing DepAnal functionality 	                              //			
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
#include <string>
#include <vector>

#include "DepAnal.h"
#include "../FileSystem/FileSystem.h"

using namespace std;

#ifdef TEST_DEPENDENCY_ANALYSIS

//----< Testing Dependency Analysis >--------------------
int main()
{
	
	cout << "\n\n" << endl;
	cout << "    Processing all Files please wait ..." << endl;
	cout << "\n\n" << endl;

	std::set<std::string> allfilesinServerRepo;

	std::string rpath = "../Repository_Server/";
	//  send all *.h files from Repository_Client folder
	std::vector<std::string> headerfiles = FileSystem::Directory::getFiles(rpath, "*.h");
	for (size_t i = 0; i < headerfiles.size(); ++i)
	{
		allfilesinServerRepo.insert(headerfiles[i]);
	}

	//  send all *.cpp files from Repository_Client folder
	std::vector<std::string> cppfiles = FileSystem::Directory::getFiles(rpath, "*.cpp");
	for (size_t i = 0; i < cppfiles.size(); ++i)
	{
		allfilesinServerRepo.insert(cppfiles[i]);
	}	

	DepAnal da;

	for (string str : allfilesinServerRepo)
	{
		da.processDependency(allfilesinServerRepo,rpath,str);
	}

	da.displayDependency();
	
	return 0;
}

#endif