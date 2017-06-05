////////////////////////////////////////////////////////////////////////////////
// CodePublisher.cpp - Main Code Publishing Logic defined            		  //			
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

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <windows.h>

#include "CodePublisher.h"

using namespace std;

//----< publishes a set of all files  >--------------------
void CodePublisher::codepublish(set<string> allfiles, unordered_map<string, set<string>> depmap)
{	
	string content;
	string out;
	ofstream fp_out;

	for (string filename : allfiles)
	{	
		content = readnmodifyfile(filename);
		out = publish(content, filename,depmap);
		int found = int(filename.rfind("\\"));
		string rfilename = filename.substr(found + 1, filename.length());
		if(rfilename[int(rfilename.rfind(".")) + 1] == 'h')
			rfilename = "../Repository_Code_Publish/HeaderFiles/" + rfilename + ".htm";
		else
			rfilename = "../Repository_Code_Publish/CppFiles/" + rfilename + ".htm";
		fp_out.open(rfilename);
		fp_out << out << endl;
		pub_files.insert(rfilename);
		fp_out.close();
	}	
}
//----< reads and modifies the contents of a file >--------------------
string CodePublisher::readnmodifyfile(string filename)
{	
	static long count = 0;
	string str = "";
	std::string rpath = "../Repository_Server/";
	ifstream ifs(rpath + filename);
	string content((istreambuf_iterator<char>(ifs)),
		(istreambuf_iterator<char>()));

	for (int i = 0; i < int(content.length()); i++)
	{
		if (content[i] == '<')
		{
			content[i] = '&';
			content.insert(i + 1, "lt;");
		}
		else if (content[i] == '>')
		{
			content[i] = '&';
			content.insert(i + 1, "gt;");
		}
	}
	for (int i = 0; i < int(content.length()); i++)
	{
		if (content[i] == '{' && content[i + 1] != '\'')
		{
			count++;
			str = "<img alt = \"\" src=\"../plusik.gif\" onclick=\"myFunction('myDIV" + std::to_string(count) + "')\" /><div id = \"myDIV" + std::to_string(count) + "\">";
			content.insert(i + 1, str);
		}
		else if (content[i] == '}' && content[i + 1] == ';')
		{
			count++;
			str = "</div>";
			content.insert(i + 2, str);
		}
		else if (content[i] == '}' && content[i + 1] != '\'')
		{
			count++;
			str = "</div>";
			content.insert(i + 1, str);
		}
	}

	return content;
}

//----< publishes contents of a file  >--------------------
string CodePublisher::publish(string content, string filename, unordered_map<string, set<string>> depmap)
{	
	int found = int(filename.rfind("\\"));
	string rfilename = filename.substr(found + 1, filename.length());

	addHtmlPrologue(rfilename);				//  1. Add Html Prologue

	out += "<html>\n";						//  2. Start "html" tag

	addHeadTag();							//  3. Add "head" tag
	
	out += "<body>\n";		
	
	addHtmlTitle(rfilename);				//  4. Add title of file

	addDependencies(filename ,depmap);		//  5. Add dependecies

	addPreTag(content);						//  6. Add "pre" tag with file contents
	
	out += "</body>\n";
	out += "</html>";						//  7. Close "html" tag

	return out;
}
//----< adds title to html files >--------------------
void CodePublisher::addHtmlTitle(string rfilename)
{
	out += "<h3>";
	out += rfilename;						
	out += "</h3>";
}
//----< adds html prologue >--------------------
void CodePublisher::addHtmlPrologue(string rfilename)
{
	
	out = "<!----------------------------------------------------------------------------\n";
	out += "     " + rfilename + ".htm\n";
	out += "     Rahul Kadam, CSE687 - Object Oriented Design, Spring 2017\n";
	out += "----------------------------------------------------------------------------->\n";
}
//----< adds head tag >--------------------
void CodePublisher::addHeadTag()
{
	out += "<head>\n";
	out += "<link rel=\"stylesheet\" type=\"text/css\" href=\"../theme.css\">\n";
	out += "<script src=\"../myscripts.js\"></script>\n";
	out += "</head>\n";
}
//----< adds pre tag >--------------------
void CodePublisher::addPreTag(string content)
{
	out += "<pre>\n";
	out += content;
	out += "</pre>\n";
}

//----< adds dependencies to the html file >--------------------
void CodePublisher::addDependencies(string filename , unordered_map<string, set<string>> depmap)
{
	out += "<hr/>\n";
	out += "<div class=\"indent\">\n";
	out += "<h4>";
	out += "Dependencies:";
	out += "</h4>\n";
	if (depmap.find(filename) != depmap.end())		// If dependency found
	{	
		set<string> sset = depmap[filename];

		for ( string str : sset)
		{	
			int found = int(str.rfind("\\"));
			string rfilename = str.substr(found + 1, str.length());
			string hfilename = "../HeaderFiles/" + rfilename;
			string cppfilename = "../CppFiles/" + rfilename;
			if (rfilename[int(rfilename.rfind(".")) + 1] == 'h')
				out += "<a href=\"" + hfilename + ".htm" + "\">";
			else
				out += "<a href=\"" + cppfilename + ".htm" + "\">";
			out += rfilename;
			out += "</a>\n";
		}	
	}	
	out += "</div>\n";
	out += "<hr/>\n";
}
//----< Displays all published filenames >--------------------
void CodePublisher::getPubFiles()
{
	for (string str : pub_files)
	{
		cout << "    -> " << str << endl;
	}	
}

//----< Opens file given in command line args in Browser >--------------------
void CodePublisher::openPubFiles(string str)
{	
	string s = modifyString(str);
	std::wstring stemp = std::wstring(s.begin(), s.end());
	LPCWSTR sw = stemp.c_str();
	ShellExecute(NULL, L"open", sw, NULL, NULL, SW_SHOWDEFAULT);
}

//----< Modifies filepath to be able to open in Browser	>--------------------
string CodePublisher::modifyString(string str)
{
	string astr = "\\\\";

	for (int i = 0; i < int(str.length()); i++)
	{
		if (str[i] == '/')
		{
			str.insert(i + 1, " ");
			str.replace(i, 2, astr);
		}
	}

	return str;
}
//----< publishes Index file >--------------------
void CodePublisher::publishIndex(unordered_map<string, set<string>> depmap)
{
	ofstream fp_out;

	string rfilename = "index";

	addHtmlPrologue(rfilename);				//  Add Html Prologue

	out += "<html>\n";						//  Start "html" tag

	addHeadTag();							//  Add "head" tag

	out += "<body>\n";

	addHtmlTitle(rfilename);				//  Add title of file

	addTableofContents(depmap); ;			//  Add table of contents

	out += "</body>\n";
	out += "</html>";						//  Close "html" tag

	rfilename = "../Repository/" + rfilename + ".htm";
	fp_out.open(rfilename);
	fp_out << out << endl;
	fp_out.close();
}
//----< add Table of Contents>--------------------
void CodePublisher::addTableofContents(unordered_map<string, set<string>> depmap)
{	
	for (string str : pub_files)
	{	
		int found = int(str.rfind("/"));
		string rfilename = str.substr(found + 1, str.length());
		out += "<a href=\"" + rfilename + "\">";
		out += rfilename;
		out += "</a>\n";
		out += "<br/><br/>";
	}
}