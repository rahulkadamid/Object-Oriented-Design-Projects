//////////////////////////////////////////////////////////////////////////////
// main.cpp - Testing Code Publishing Logic					                //    
// ver 1.0																	//	
// -----------------------------------------------------------------------	//			
// copyright © Rahul Kadam, 2017											//			
// All rights granted provided that this notice is retained					//			
// -----------------------------------------------------------------------	//			
// Language:    Visual C++, Visual Studio 2015								//				
// Platform:    Hp, Core i5, Windows 10										//			
// Application: OOD Project 3 - DependencyBasedCodePublisher ,Spring 2017	//			
// Source:      Jim Fawcett,  CST 4-187, 									//			
//              (315) 443-3948 , jfawcett@twcny.rr.com						//			
// Author:      Rahul Kadam, Syracuse University          				    //				
//              (315) 751-8862, rkadam@syr.edu								//			
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "CodePublisher.h"

using namespace std;

#ifdef TEST_CODEPUBLISHER

//----< Testing Code Publisher >--------------------
int main()
{	
	cout << "\nTesting Code Publisher" << endl;

	CodePublisher cp;

	string filename = "CodePublisher.cpp";

	string content = cp.readnmodifyfile(filename);

	unordered_map<string, set<string>> depmap;

	depmap["CodePublisher.cpp"].insert("CodePublisher.h");

	string out = cp.publish(content, filename, depmap);

	ofstream fp_out;

	filename += ".htm";
	fp_out.open(filename);

	fp_out << out << endl;

	cout << "    Published files:" << endl;
	cout << "       -> " << filename << endl;
	cout << "\n" << endl;
	cout << "NOTE:-Check files in CodePublisher folder ...." << endl;
	
	return 0;
}

#endif