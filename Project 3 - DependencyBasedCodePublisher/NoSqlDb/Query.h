#ifndef QUERY_H
#define QUERY_H
/////////////////////////////////////////////////////////////////////////////
// Query.h - Class Query which defines queries supported over NoSqlDb      //
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
* This module consists of class Query which defines different queries in separate methods.
* The various Queries supported here are as follows:
*  - getValue ,  getChildren ,  
*  - getKeySpecPatternL , getItemSpecPatternL , getCategorySpecPatternL , 
*  - getDataSpecPatternL , getTimeDateIntervalL
*  - getIntersection , getUnion
* Required Files:
* ===============
* Query.h, Test.cpp
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
#include "NoSqlDb.h"
#include <regex>
#include <functional>

///////////////////////////////////////////////////////////////////////////
// Query class

template<typename Data>
class Query:public NoSqlDb<Data>
{
public:
	Element<Data> getValue(Key key, NoSqlDb<Data>& db);
	vector<string> getChildren(Key key, NoSqlDb<Data>& db);
	vector<string> getKeySpecPatternL(string pattern, vector<string> input_keys ,NoSqlDb<Data>& db);
	vector<string> getItemSpecPatternL(string pattern, vector<string> input_keys, NoSqlDb<Data>& db);
	vector<string> getCategorySpecPatternL(string pattern, vector<string> input_keys, NoSqlDb<Data>& db);
	vector<string> getDataSpecPatternL(string pattern, vector<string> input_keys, NoSqlDb<Data>& db);
	vector<string> getTimeDateIntervalL(string time1, string time2, vector<string> input_keys, NoSqlDb<Data>& db);
	vector<string> getIntersection(vector<string> keys1, vector<string> keys2, NoSqlDb<Data>& db);
	vector<string> getUnion(vector<string> keys1, vector<string> keys2, NoSqlDb<Data>& db);
};

//-------------------------< getValue of a Key >------------------------------------
template<typename Data>
Element<Data> Query<Data>::getValue(Key key, NoSqlDb<Data>& db)
{		
	unordered_map<Key, Element<Data>> store = db.getMapAccess();

	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();	
}

//-------------------------< getChildren of a Key >------------------------------------
template<typename Data>
vector<string> Query<Data>::getChildren(Key key, NoSqlDb<Data>& db)
{
	unordered_map<Key, Element<Data>> store = db.getMapAccess();

	Element<Data> elem;

	if (store.find(key) != store.end())
	{
		elem = store[key];
	}
	
	return elem.getVectoraccess();
}

//-------------------------< get all keys with specific pattern in their key >------------------------------------
template<typename Data>
vector<string> Query<Data>::getKeySpecPatternL(string pattern, vector<string> input_keys, NoSqlDb<Data>& db)
{
	vector<string> output_keys;

	regex e(pattern);

	for_each(input_keys.begin(), input_keys.end(), [&](string val)
	{
		if (regex_match(val, e))
			output_keys.push_back(val);
	});

	return output_keys;  // default no keys
}

//-------------------------< get all keys with specific pattern in their item name >------------------------------------
template<typename Data>
vector<string>  Query<Data>::getItemSpecPatternL(string pattern, vector<string> input_keys, NoSqlDb<Data>& db)
{
	vector<string> output_keys;

	regex e(pattern);

	Element<Data> elem;

	for_each(input_keys.begin(), input_keys.end(), [&](string val)
	{	
		elem = getValue(val, db);
		string str = elem.name;
		if (regex_match(str, e))
			output_keys.push_back(val);
	});

	return output_keys;  // default no keys

}

//-------------------------< get all keys with specific pattern in their category name >------------------------------------
template<typename Data>
vector<string>  Query<Data>::getCategorySpecPatternL(string pattern, vector<string> input_keys, NoSqlDb<Data>& db)
{
	vector<string> output_keys;

	regex e(pattern);

	Element<Data> elem;

	for_each(input_keys.begin(), input_keys.end(), [&](string val)
	{
		elem = getValue(val, db);
		string str = elem.category;
		if (regex_match(str, e))
			output_keys.push_back(val);
	});

	return output_keys;  // default no keys

}

//-------------------------< get all keys with specific pattern in their data >------------------------------------
template<typename Data>
vector<string>  Query<Data>::getDataSpecPatternL(string pattern, vector<string> input_keys, NoSqlDb<Data>& db)
{
	vector<string> output_keys;

	regex e(pattern);

	Element<Data> elem;

	for_each(input_keys.begin(), input_keys.end(), [&](string val)
	{
		elem = getValue(val, db);
		string str = elem.data;
		if (regex_match(str, e))
			output_keys.push_back(val);
	});

	return output_keys;  // default no keys
}

//-------------------------< get all keys created within specified time interval >------------------------------------
template<typename Data>
vector<string>  Query<Data>::getTimeDateIntervalL(string time1, string time2, vector<string> input_keys, NoSqlDb<Data>& db)
{

	vector<string> output_keys;

	Element<Data> elem;

	for_each(input_keys.begin(), input_keys.end(), [&](string val)
	{
		elem = getValue(val, db);

		string elem_time = elem.timeDate;

		if (elem_time > time1 && elem_time < time2)
			output_keys.push_back(val);
	});

	return output_keys;  // default no keys
}

//-------------------------< get all keys for compound queries - Intersection >------------------------------------
template<typename Data>
vector<string>  Query<Data>::getIntersection(vector<string> keys1, vector<string> keys2, NoSqlDb<Data>& db)
{
	vector<string> keys3;

	sort(keys1.begin(), keys1.end());
	sort(keys2.begin(), keys2.end());

	set_intersection(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), back_inserter(keys3));

	return keys3;
}

//-------------------------< get all keys for compound queries - Union >------------------------------------
template<typename Data>
vector<string> Query<Data>::getUnion(vector<string> keys1, vector<string> keys2, NoSqlDb<Data>& db)
{
	vector<string> keys4;

	sort(keys1.begin(), keys1.end());
	sort(keys2.begin(), keys2.end());

	set_union(keys1.begin(), keys1.end(), keys2.begin(), keys2.end(), back_inserter(keys4));

	return keys4;

}

#endif