#ifndef TYPETABLE_H
#define TYPETABLE_H

/////////////////////////////////////////////////////////////////////////////
// TypeTable.h - TypeAnalysis stores analysis from the AST in a	TypeTable  //
// ver 1.0 		  														   //
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
* It Provides a container class that stores type information needed for dependency analysis.
* Class Value which acts as a Value Part of Key-Value Relationship of the 
* map defined in TypeTable container class. 
* Class TypeTable acts as a map container storing Key-Value pairs where:
*			Key		-> typename
*			Value	-> vector of class Value(i.e Typename,Type,Namespace,Filepath)
*
*  Public Interface:
*  ==================
*
*		bool save(string type_name, Value val);			// Save into TypeTable
*		void display();									// Display TypeTable
*		void displayHeader();							// Display TypeTable Header
*		map<string, vector<Value>>& returnTypeTable();	// returns TypeTable
*
* Required Files:
* ===============
* TypeTable.h, TypeTable.cpp
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

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip> 

using namespace std;

namespace CodeAnalysis
{
	class Value
	{
	private:
		string type_name;			// Name of the Type Defined		
		string value_type;			// Type of above name defined 
		string value_namespace;		// Namespace the type defined in 
		string value_filename;		// Filepath of type defined in

	public:
		void setTypeName(string t_name)				// Sets Typename
		{
			type_name = t_name;
		}
		string getTypeName()						// Gets Typename
		{
			return type_name;
		}
		void setType(string val_type)				// Sets Type 
		{
			value_type = val_type;
		}
		string getType()							// Gets Type 
		{
			return value_type;
		}
		void setNamespace(string val_namespace)		// Sets Namespace 
		{
			value_namespace = val_namespace;
		}	
		string getNamespace()						// Gets Namespace 
		{
			return value_namespace;
		}
		void setFilename(string val_filename)		// Sets Filename
		{
			value_filename = val_filename;
		}
		string getFilename()						// Gets Filename 
		{
			return value_filename;
		}
	};

	class TypeTable
	{
	private:
		map<string, vector<Value>> ttmap;				// TypeTable map
	public:
		bool save(string type_name, Value val);			// Save into TypeTable
		void display();									// Display TypeTable
		void displayHeader();							// Display TypeTable Header
		map<string, vector<Value>>& returnTypeTable();	// returns TypeTable
	};

	//---------------<  returns TypeTable >------------------
	inline map<string, vector<Value>>& TypeTable::returnTypeTable()
	{
		return ttmap;
	}
	//---------------<  Save into TypeTable >------------------
	inline bool TypeTable::save(string type_name, Value val)
	{
		ttmap[type_name].push_back(val);

		return true;
	}
	//---------------<  Display TypeTable Header >------------------
	inline void TypeTable::displayHeader()
	{	
		cout << "\n";
		cout << right;
		cout << "\n     ---------------------------------------------------------------------------------------------------------------------";
		cout << "\n					TYPE TABLE:";
		cout << "\n     ---------------------------------------------------------------------------------------------------------------------\n";
		cout << "\n ";
		cout << setw(25) << "Type name";
		cout << setw(25) << "Type";
		cout << setw(25) << "Namespace";
		cout << setw(25) << "Filename";
		cout << right;
		cout << "\n  ";
		cout << setw(25) << "-------------------";
		cout << setw(25) << "-------------------";
		cout << setw(25) << "-------------------";
		cout << setw(25) << "-------------------";
	}
	//---------------<  Display TypeTable >------------------
	inline void TypeTable::display()
	{	
		TypeTable::displayHeader();
		cout << "\n ";
		map<string, vector<Value>>::iterator it = ttmap.begin();
		for (it; it != ttmap.end(); ++it)
		{	
			cout << right;			
			for (Value val : it->second)
			{
				cout << setw(25) << it->first;
				cout << setw(25) << val.getType();
				cout << setw(25) << val.getNamespace();
				cout << "\t\t" << setw(50) << val.getFilename();
				cout << "\n ";
			}			
		}
	}
}
#endif