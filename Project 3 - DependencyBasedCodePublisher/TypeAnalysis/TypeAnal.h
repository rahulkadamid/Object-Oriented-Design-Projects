#ifndef TYPEANAL_H
#define TYPEANAL_H

/////////////////////////////////////////////////////////////////////////////
// TypeAnal.h -  It performs TypeAnalysis over the AST and saves analysis  //
// ver 1.0 		 in a TypeTable											   //
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
* It finds all the types and global functions defined in each of a collection 
* of C++ source files. It does this by building rules to detect:
* -> type definitions: classes, structs, enums, typedefs, and aliases.
* -> global function definitions
* -> global data definitions
* and capture their fully qualified names and the files where they are defined. 
* It uses the TypeTable package to store that information.
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
* TypeAnal.h, TypeAnal.cpp
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
#include "../Parser/ActionsAndRules.h"
#include "../TypeTable/TypeTable.h"

#include <iostream>
#include <string>
#include <set>

using namespace std;

namespace CodeAnalysis
{
	class TypeAnal
	{
	public:
		using SPtr = shared_ptr<ASTNode*>;

		TypeAnal();
		void doTypeAnal();						// Performs Type Analysis
		void storeToTypeTable(ASTNode* pNode);	// Stores information into TypeTable
		TypeTable& getTypeTable();				// Provides acces to TypeTable
		Scanner::Toker& getToker();				// Provides access to Toker
		void storeAllFilenames(ASTNode* pNode);	// stores all filenames used in analysis
		set<string> getAllFilenames();			// Retrieves set of filenames used in analysis

	private:
		void DFS(ASTNode* pNode);				// Performs Depth First Search over the AST
		AbstrSynTree& ASTref_;					// AST's Instance
		ScopeStack<ASTNode*> scopeStack_;		// ScopeStacks's Instances
		Scanner::Toker& toker_;					// Toker's Instance
		TypeTable tt;							// TypeTable's Instance
		set<string> allFilenames;				// Set of all filenames used in Analysis
	};

	//---------------< Retrieves set of filenames used in analysis>------------------
	inline set<string> TypeAnal::getAllFilenames()
	{	
		return allFilenames;
	}
	//---------------< Provides acces to TypeTable>------------------
	inline TypeTable& TypeAnal::getTypeTable()
	{
		return tt;
	}
	//---------------< Provides access to Toker>------------------
	inline Scanner::Toker& TypeAnal::getToker()
	{
		return toker_;
	}
	//---------------< Retrieves set of filenames used in analysis>------------------
	inline TypeAnal::TypeAnal() :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker()))
	{
	}
	//---------------< Performs Type Analysis>------------------
	inline void TypeAnal::doTypeAnal()
	{	
		ASTNode* pRoot = ASTref_.root();
		//DFS(pRoot);
		storeToTypeTable(pRoot);
		storeAllFilenames(pRoot);
		//tt.display();
	}
	//---------------< Performs Depth First Search over the AST >------------------
	inline void TypeAnal::DFS(ASTNode* pNode)
	{
		static string path = "";
		if (pNode->path_ != path)
		{
			//cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
			cout << "\n    -- " << pNode->path_;

			cout << "\n    -- "  << pNode->package_;
			path = pNode->path_;
		}
		cout << "\n      " << pNode->name_;
		cout << ", " << pNode->type_;
		for (auto pChild : pNode->children_)
			DFS(pChild);
	}
	//---------------< stores all filenames used in analysis  >------------------
	inline void TypeAnal::storeAllFilenames(ASTNode* pNode)
	{	
		static string path = "";

		if (pNode->path_ != path)
		{
			allFilenames.insert(pNode->path_);
		}
		for (auto pChild : pNode->children_)
			storeAllFilenames(pChild);
	}
	//---------------< Stores information into TypeTable >------------------
	inline void TypeAnal::storeToTypeTable(ASTNode* pNode)
	{	
		static string path = "";

		string which_type = pNode->type_;

		if (which_type == "namespace")
			scopeStack_.push(pNode);

		if (pNode->path_ != path)
		{
			Value val;			

			if (which_type == "class" || which_type == "struct" || which_type == "enum" ||
				which_type == "typedef" || which_type == "using" || which_type == "namespace" ||
				which_type == "function")
			{
				val.setTypeName(pNode->name_);

				val.setType(pNode->type_);

				val.setNamespace(scopeStack_.top()->name_);

				val.setFilename(pNode->path_);		

				if(pNode->name_ != "main")
					tt.save(val.getTypeName(), val);						
			}
		}
		for (auto pChild : pNode->children_)
			storeToTypeTable(pChild);
		if (pNode->type_ == "namespace")
			scopeStack_.pop();
	}
}

#endif