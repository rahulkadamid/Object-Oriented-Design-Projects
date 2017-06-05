#ifndef CODEPUBLISHER_H
#define CODEPUBLISHER_H
////////////////////////////////////////////////////////////////////////////////
// CodePublisher.h - Main Code Publishing Logic	defined      				  //			
// ver 1.1																	  //			
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
*  This module consists of main logic to public source code files into html files
*  with their dependent file names as hyperlinks in html.
*  The various operations supported here are as follows:
*   
*	codepublish			->	publishes a set of all files
*	readnmodifyfile		->	reads and modifies the contents of a file
*	lazydownload		->	publishes contents of a file
*	addHtmlTitle		->	adds title to html files
*	addHtmlPrologue		->	adds html prologue
*	addDependencies		->	adds dependencies to the html file
*	addHeadTag			->	adds head tag
*	addPreTag			->	adds pre tag
*	modifyString		->	Modifies filepath to be able to open in Browser
*	getPubFiles			->	Displays all published filenames
*	openPubFiles		->	Opens file given in command line args in Browser 
*	publishIndex		->	publishes Index file
*	addTableofContents	->	adds Table of Contents
*
*  Required Files:
*  ---------------
*  - CodePublisher.h ,CodePublisher.cpp, main.cpp
*
*  Maintanence History:
*  --------------------
*  ver 1.1 : 2 May 2017
*  - added Categories
*  - Header files are published in HeaderFiles Folder and
*    Cpp files are published in CppFiles Folder
*  ver 1.0 : 4 April 2017
*  - first release
*
*/
#include <string>
#include <set>
#include <unordered_map>

using namespace std;

///////////////////////////////////////////////////////////////////////
// CodePublisher class  
// - provides main logic to convert source code files into html files

class CodePublisher
{
private:
	string out;					// contents of a file in a string
	set<string> pub_files;		// set of all published files
public:
	void codepublish(set<string> allfiles, unordered_map<string, set<string>> depmap);
	string readnmodifyfile(string filename);
	string publish(string content, string filename, unordered_map<string, set<string>> depmap);
	void addHtmlTitle(string rfilename);
	void addHtmlPrologue(string rfilename);
	void addDependencies(string filename, unordered_map<string, set<string>> depmap);
	void addHeadTag();
	void addPreTag(string content);
	string modifyString(string str);
	void getPubFiles();
	void openPubFiles(string str);
	void publishIndex(unordered_map<string, set<string>> depmap);
	void addTableofContents(unordered_map<string, set<string>> depmap);
};

#endif
