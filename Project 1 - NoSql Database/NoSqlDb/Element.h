#ifndef ELEMENT_H
#define ELEMENT_H
/////////////////////////////////////////////////////////////////////////////
// Element.h - Class Element which defines the values for NoSqlDb          //
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
/*
* Module Operations:
* ==================
* This module consists of class Element which defines the values in the Key-Value 
* Pair NoSqlDb
* The various operations supported here are as follows:
*  - show					->  shows the contents of the Element class (i.e Properties ,children etc.)
*  - getDateTime			->  provides the current Date and Time
*  - getVectoraccess		->  provides access to its private vector of children
*   
* Required Files:
* ===============
* Element.h, Test.cpp
*
* Build Command:
* ==============
* devenv NoSql_Database.sln /Rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 : 7 Feb 2017
* - first release
*/
#include "../CppProperties/CppProperties.h"
#include <ctime>
#include <fstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////
// Element class

template<typename Data>
class Element
{
public:
	using Name = string;
	using Category = string;
	using Description = string;
	using TimeDate = string;

	//-------------------------<MetaData supported with Properties >------------------------------------
	Property<Name> name;								
	Property<Category> category;						
	Property<Description> description;					
	Property<TimeDate> timeDate;		
	Property<Data> data;				// Actual Templatized Data	
	
	string show();						// shows the contents of the Element class (i.e Properties ,children etc.)
	string getDateTime();				// provides the current Date and Time
	vector<string> & getVectoraccess(); // provides access to its private vector of children			

private:
	vector<string> children;			// Private vector of children						 

};

//-------------------------<provides access to its private vector of children >------------------------------------
template<typename Data>
vector<string> & Element<Data>::getVectoraccess()
{
	return children;
}

//-------------------------<provides the current Date and Time >------------------------------------
template<typename Data>
string Element<Data>::getDateTime()
{		   
	time_t tim;
	time(&tim);
	return ctime(&tim);
}

//-------------------------<shows the contents of the Element class (i.e Properties ,children etc.) >----------------
template<typename Data>
string Element<Data>::show()
{
	ostringstream out;
	out.setf(ios::adjustfield, ios::left);
	out << "\n    " << setw(8) << "name" << " : " << name;
	out << "\n    " << setw(8) << "category" << " : " << category;
	out << "\n    " << setw(8) << "descript" << " : " << description;
	out << "\n    " << setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << setw(8) << "data" << " : " << data;
	out << "\n    " << setw(8) << "children" << " : ";

	for (auto it = children.begin(); it != children.end(); ++it)
		out << setw(8) << *it << " ";

	cout << endl;

	out << "\n";
	return out.str();
}

#endif