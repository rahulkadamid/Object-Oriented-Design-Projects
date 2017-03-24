#ifndef DEPANAL_H
#define DEPANAL_H

/////////////////////////////////////////////////////////////////////////////
// DepAnal.h - It performs dependency analysis over the TypeTable		   //
// ver 1.0 	   prepared from AST   										   //
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
/*
* Module Operations:
* ==================
*   Finds, for each file in a specified file collection, all other files from the file 
*   collection on which they depend.
*   File A depends on file B, if and only if, it uses the name of any type or
*   global function or global data defined in file B.
*   It might do that by calling a function or method of a type or by inheriting the type.
*
*   Note: This intentionally does not record dependencies of a file on files
*   outside the file set, e.g., language and platform libraries.
* 
*  Public Interface:
*  ==================
*
*  void doTypeAnal();						// Performs Type Analysis
*  void storeToTypeTable(ASTNode* pNode);	// Stores information into TypeTable
*  TypeTable& getTypeTable();				// Provides acces to TypeTable
*  Scanner::Toker& getToker();				// Provides access to Toker
*  void storeAllFilenames(ASTNode* pNode);	// stores all filenames used in analysis
*  set<string> getAllFilenames();			// Retrieves set of filenames used in analysis
*
* Required Files:
* ===============
* DepAnal.h, DepAnal.cpp
*
* Build Command:
* ==============
* devenv TypeBasedDepAnalyzer.sln /Rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 : 7 March 2017
* - My first release
*/
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cctype>
#include <string>
#include <iosfwd>
#include <sstream>
#include <fstream>
#include <set> 
#include <iomanip> 
#include "../TypeTable/TypeTable.h"
#include "../TypeAnalysis/TypeAnal.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/ActionsAndRules.h"
#include "../NoSqlDb/NoSqlDb.h"

using namespace std;

namespace CodeAnalysis
{
	class DepAnal
	{
	public:
		// Searches for Dependency by tokenizing each file with TypeTable
		bool searchDependency(string fileSpec, TypeTable& tt , Scanner::Toker& toker);
		void displayDependency();						// Displays Dependencies
		void storeToNoSqlDb(NoSqlDb<string>& db);		// Stores Dependencies to NoSQlDb
		unordered_map<string, set<string>> getDepMap();	// Provides access to dependency map
		
	private:
		unordered_map<string, set<string>> depmap;		// Dependency map to store dependencies

	};
	//---------------<  Provides access to dependency map >------------------
	inline unordered_map<string, set<string>> DepAnal::getDepMap() 
	{
		return depmap;
	}

	//---------------<  Searches for Dependency by tokenizing each file with TypeTable >------------------
	inline bool DepAnal::searchDependency(string fileSpec, TypeTable& tt, Scanner::Toker& toker)
	{
		try
		{
			ifstream in(fileSpec);
			if (!in.good())
			{
				cout << "\n  can't open " << fileSpec << "\n\n";
				return 1;
			}			
			toker.returnComments();
			toker.attach(&in);
			do
			{
				string tok = toker.getTok();
				if (tok == "\n")
					tok = "newline";

				map<string, vector<Value>> ttmap = tt.returnTypeTable();

				if (ttmap.find(tok) != ttmap.end())   // If found
				{	
						for (Value val : ttmap[tok])
						{	
							if(val.getFilename() != fileSpec)
								depmap[fileSpec].insert(val.getFilename());
						}							
				}
			} while (in.good());
		}
		catch (logic_error& ex)
		{
			cout << "\n  " << ex.what();
		}
		return true;
	}
	//---------------<  Displays Dependencies >------------------
	inline void DepAnal::displayDependency()
	{	
		cout << "\n";
		cout << right;
		cout << "\n     ---------------------------------------------------------------------------------------------------------------------";
		cout << "\n					DEPENDENCIES:";
		cout << "\n     ---------------------------------------------------------------------------------------------------------------------\n";
		cout << "\n ";
		for (auto i : depmap)
		{	
			cout << right;
			cout << endl << "\t" + i.first << ": ";
			for (auto j : i.second)
				cout << endl << "\t\t" << "->" + j;
			cout << "\n\n";
		}
	}			
	//---------------< Stores Dependencies to NoSQlDb >------------------
	inline void DepAnal::storeToNoSqlDb(NoSqlDb<string>& db)
	{
		for (auto i : depmap)
		{
			Element<string> elem;

			int found = int(i.first.rfind("\\"));
			string rfilename = i.first.substr(found + 1, i.first.length());

			elem.name = rfilename;
			if(rfilename[rfilename.length()-1] == 'p')
					elem.category = "CppFile";	
			else
					elem.category = "HeaderFile";

			elem.data = "file's string data";
			elem.description = "file description";
			elem.timeDate = elem.getDateTime();

			for (auto j : i.second)
			{
				elem.getVectoraccess().push_back(j);
			}

			db.save(i.first, elem);			
		}		
	}
}
#endif