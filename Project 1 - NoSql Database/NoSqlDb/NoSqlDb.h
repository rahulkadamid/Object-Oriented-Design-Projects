#ifndef NOSQLDB_H
#define NOSQLDB_H
/////////////////////////////////////////////////////////////////////////////
// NoSqlDb.h - Class NoSqlDb defines structure of the entire key-value db  //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Rahul Kadam, 2017                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Hp, Core i5, Windows 10                                    //
// Application: OOD Project 1 - NoSql Database ,Spring 2017                //
// Co-Author:      Rahul Kadam, Syracuse University            		       //
//              (315) 751-8862, rkadam@syr.edu		                       //
// Author:         Jim Fawcett,  CST 4-187,             		           //
//              (315) 443-3948 , jfawcett@twcny.rr.com   		           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module consists of class Element which defines the entire structure of the 
* Key-Value Pair in memory NoSql Database
* The various operations supported here are as follows:
* keys 					->  Get all keys in the database
* save 					->  Save the Key-Value Pair in the database
* value					->  For a certain key provide its value
* count					->  Provide the size of Database(unordered_map)
* deleteRelationship	->  Delete the relationship between Key-Value Pair
* modify				->  Modify the values of one instance with another
* getMapAccess			->  Provide acces to the Unordered_map
* getDateTime			->  Provide current date and time
* retrieveDb			->  Retrieve all Key-Value pairs from database
*
* Required Files:
* ===============
* NoSqlDb.h, Test.cpp
*
* Build Command:
* ==============
* devenv NoSql_Database.sln /Rebuild debug
*
* Maintenance History:
* ====================
* ver 1.2 : 7 Feb 2017
* - My first release
* ver 1.1 :  Jan 27 2017
* - fixed bug in NoSqlDb::count() by replacing
*   unordered_map<key,Value>::count() with
*   unordered_map<key,Value>::size();
* ver 1.0 :  Jan 20 2017
* - Professor's first release
*/

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <thread>          
#include <chrono> 
#include "../CppProperties/CppProperties.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../Convert/Convert.h"
#include "Element.h"
#include "StrHelper.h"

using namespace std;
using namespace XmlProcessing;
using SPtr = shared_ptr<AbstractXmlElement>;

///////////////////////////////////////////////////////////////////////////
// NoSqlDb class

template<typename Data>
class NoSqlDb
{
public:
	using Key = string;
	using Keys = vector<Key>;

	Keys keys();										// Get all keys in the database 
	bool save(Key key, Element<Data> elem);				// Save the Key-Value Pair in the database
	Element<Data> value(Key key);						// For a certain key provide its value
	size_t count();										// Provide the size of Database(unordered_map)	 
	Element<Data> deleteRelationship(Key key);			// Delete the relationship between Key-Value Pair
	bool modify(Key key1, Element<Data> elem);			// Modify the values of one instance with another
	unordered_map<Key, Element<Data>> & getMapAccess(); // Provide acces to the Unordered_map
	string getDateTime();								// Provide current date and time
	void retrieveDb(NoSqlDb<Data>& db);					// Retrieve all Key-Value pairs from database

private:
	using Item = pair<Key, Element<Data>>;		 
	unordered_map<Key, Element<Data>> store;
};

//-------------------------<Retrieve all Key-Value pairs from database >------------------------------------
template<typename Data>
void NoSqlDb<Data>::retrieveDb(NoSqlDb<Data>& db)
{	
	cout << "\n  Retrieving elements from NoSqlDb";
	cout << "\n ------------------------------------------\n";

	cout << "\n  size of database = " << db.count() << "\n";

	Keys keys = db.keys();					 
	for (Key key : keys)					 
	{
		cout << "\n  " << key << ":";		 
		cout << db.value(key).show();		 
	}
	cout << "\n";

}

//-------------------------<Provide current date and time>------------------------------------
template<typename Data>
string NoSqlDb<Data>::getDateTime()
{
	time_t tim;
	time(&tim);
	return ctime(&tim);
}

//-------------------------<Provide acces to the Unordered_map >------------------------------------
template<typename Data>
unordered_map<string, Element<Data>> & NoSqlDb<Data>::getMapAccess()
{
	return store;

}

//-------------------------<Get all keys in the database >------------------------------------
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()			//retrieves keys from db in vector<strings>
{
	Keys keys;				
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}

//-------------------------<Save the Key-Value Pair in the database >------------------------------------
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)       
{
	if (store.find(key) != store.end())						 
		return false;		

	store[key] = elem;		

	return true;
}

//-------------------------<For a certain key provide its value >------------------------------------
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())						 
		return store[key];									  
	return Element<Data>();									 
}

template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}

//-------------------------<Delete the relationship between Key-Value Pair >------------------------------------
template<typename Data>
Element<Data> NoSqlDb<Data>::deleteRelationship(Key key)
{
	if (store.find(key) != store.end())						 
		  store.erase(key);				
	// when delete key ...remove every occurence of this key as a child
	return Element<Data>();									 
}

//-------------------------<Modify the values of one instance with another>------------------------------------
template<typename Data>
bool NoSqlDb<Data>::modify(Key key, Element<Data> elem)       // saves Key and Value(Element) in the db
{
	if (store.find(key) != store.end())
	{
		store[key] = elem;
		return true;
	}						 													 
	return false;
}



#endif