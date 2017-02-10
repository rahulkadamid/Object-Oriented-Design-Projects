/////////////////////////////////////////////////////////////////////////////
// Test.cpp - Testing the entire NoSqlDb with its various features         //
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

#include "NoSqlDb.h"
#include "Persistence.h"
#include "Query.h"

#ifndef TEST_NOSQLDB
#define TEST_NOSQLDB

/////////////////////////////////////////////////////////////////////////////
// Definition of main() method

int main()
{
	cout << "\n \t \t \t \t Welcome to My NoSQL Database";
	cout << "\n \t \t \t \t ===============================================\n";


	cout << "\n Working with a NoSQL<string> Database ";
	cout << "\n -------------------------------------------------------\n";

	/********************* Addition of Key/Value Pairs with "String Data" ******************************/

	cout << "\n  Addition of Key/Value Pairs of with string data";
	cout << "\n -------------------------------------------------------\n";

	NoSqlDb<string> db;

	Element<string> elem1;
	elem1.name = "elem1";
	elem1.category = "test";
	elem1.data = "elem1's string";
	elem1.description = "testdescip1";
	elem1.getVectoraccess().push_back("elem3");
	elem1.getVectoraccess().push_back("elem2");
	elem1.timeDate = elem1.getDateTime();

	db.save(elem1.name, elem1);
	cout << "\n Record #1 added " << endl;

	Element<string> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.data = "elem2's string";
	elem2.description = "testdescip2";
	elem2.getVectoraccess().push_back("elem1");
	elem2.getVectoraccess().push_back("elem3");
	elem2.timeDate = elem2.getDateTime();

	db.save(elem2.name, elem2);
	cout << "\n Record #2 added " << endl;

	Element<string> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.data = "elem3's string";
	elem3.description = "testdescip2";
	elem3.getVectoraccess().push_back("elem1");
	elem3.getVectoraccess().push_back("elem2");
	elem3.timeDate = elem3.getDateTime();

	db.save(elem3.name, elem3);
	cout << "\n Record #3 added " << endl;

	Element<string> elem4;
	elem4.name = "elem4";
	elem4.category = "test";
	elem4.data = "elem4's string";
	elem4.description = "testdescip4";
	elem4.getVectoraccess().push_back("elem2");
	elem4.getVectoraccess().push_back("elem3");
	elem4.timeDate = elem4.getDateTime();

	db.save(elem4.name, elem4);
	cout << "\n Record #4 added " << endl;

	Element<string> elem5;
	elem5.name = "elem5";
	elem5.category = "test";
	elem5.data = "elem5's string";
	elem5.description = "testdescip2";
	elem5.getVectoraccess().push_back("elem1");
	elem5.getVectoraccess().push_back("elem2");
	elem5.timeDate = elem5.getDateTime();

	db.save(elem5.name, elem5);
	cout << "\n Record #5 added " << endl;

	/************************************  Retrieve from NoSqlDb<string> ************************************/

	db.retrieveDb(db);

	/********************* Deletion of Key/Value Pairs with "String Data" ******************************/

	cout << "\n  Deleting a record from NoSqlDb<string>";
	cout << "\n ------------------------------------------\n";

	db.deleteRelationship(elem3.name);

	cout << "\n elem3 key-value pair deleted ...\n";

	/************************************  Retrieve from NoSqlDb<string> ************************************/

	db.retrieveDb(db);

	/*********************************  Replacing an existing values instance with a new instance **********************************/

	cout << "\n  Replacing an existing values instance with a new instance ";
	cout << "\n ------------------------------------------\n";
	
	cout << "Current elem4:" << endl;
	cout << "\n  " << elem4.name << ":";
	cout << elem4.show();

	Element<string> elem6;
	elem6.name = "elem6";
	elem6.category = "test6";
	elem6.data = "elem6's string now!!!";
	elem6.description = "testdescip6";
	elem6.getVectoraccess().push_back("elem3");
	elem6.getVectoraccess().push_back("elem4");
	elem6.timeDate = elem6.getDateTime();

	cout << "\n New instance elem6 created !!!" << endl;

	cout << "\n  " << elem6.name << ":";
	cout << elem6.show();

	cout << "\n  Replacing elem4's values with elem6's values" << endl;

	db.modify(elem4.name,elem6);

	cout << "\nModified elem4:" << endl;
	cout << "\n  " << elem4.name << ":";
	cout << db.value(elem4.name).show();
	

	/************************************  Retrieve from NoSqlDb<string> ************************************/

	db.retrieveDb(db);

	/************************************ NoSqlDb<string> To XML *************************************/

	Persistence<string> pdb;

	cout << "\n  Saving NoSqlDb<string> to XML ";
	cout << "\n ------------------------------------------\n";
	
	string xml = pdb.saveXml(db);

	cout << "\n  NoSqlDb<string> persisted to XmlfileStrdata.xml" << endl;

	cout << "\n ";

	/************************************  From XML to NoSqlDb<string> ************************************/

	cout << "\n  From XML to NoSqlDb<string>";
	cout << "\n ------------------------------------------\n";

	cout << "\n Loading database from XmlfileStrdata.xml file  ";

	NoSqlDb<string> db2; 
	
	db2 = pdb.restore("Xmlfile");

 	cout << "  NoSqlDb<string> loaded from XML !!! " << endl;

	/************************************  Retrieve from NoSqlDb<string> ************************************/

	db2.retrieveDb(db2);

	/********************* Time Interval after which db contents are persisted to Xml ******************************/

	cout << "\n Demo for Auto-Persisted Database ";
	cout << "\n ---------------------------------------------------------------------------------------------------------";
	cout << endl;

	Persistence<string> pdb2;
	pdb2.autoPersist(db);

	db2.retrieveDb(db2);
	/********************* Demo for NoSQL<int>  ******************************/

	cout << "\n Working with a NoSQL<int> Database ";
	cout << "\n -------------------------------------------------------\n";


	/********************* Addition of Key/Value Pairs of "Int Data" ******************************/

	cout << "\n  Addition of Key/Value Pairs of with Int Data";
	cout << "\n ----------------------------------------------------\n";

	NoSqlDb<int> idb;

	Element<int> ielem1;
	ielem1.name = "elem1";
	ielem1.category = "test";
	ielem1.data = 1;
	ielem1.description = "testdescip1";
	ielem1.getVectoraccess().push_back("elem2");
	ielem1.getVectoraccess().push_back("elem3");
	ielem1.timeDate = ielem1.getDateTime();

	idb.save(ielem1.name, ielem1);
	cout << "\n Record #1 added " << endl;

	Element<int> ielem2;
	ielem2.name = "elem2";
	ielem2.category = "test";
	ielem2.data = 2;
	ielem2.description = "testdescip2";
	ielem2.getVectoraccess().push_back("elem1");
	ielem2.getVectoraccess().push_back("elem3");
	ielem2.timeDate = ielem2.getDateTime();

	idb.save(ielem2.name, ielem2);
	cout << "\n Record #2 added " << endl;

	Element<int> ielem3;
	ielem3.name = "elem3";
	ielem3.category = "test";
	ielem3.data = 3;
	ielem3.description = "testdescip3";
	ielem3.getVectoraccess().push_back("elem1");
	ielem3.getVectoraccess().push_back("elem2");
	ielem3.timeDate = ielem3.getDateTime();

	idb.save(ielem3.name, ielem3);
	cout << "\n Record #3 added " << endl;

	Element<int> ielem4;
	ielem4.name = "elem4";
	ielem4.category = "test";
	ielem4.data = 4;
	ielem4.description = "testdescip4";
	ielem4.getVectoraccess().push_back("elem2");
	ielem4.getVectoraccess().push_back("elem3");
	ielem4.timeDate = ielem4.getDateTime();

	idb.save(ielem4.name, ielem4);
	cout << "\n Record #4 added " << endl;

	/************************************  Retrieve NoSqlDb<int> ************************************/

	idb.retrieveDb(idb);
 
	 /************************************  Query Mechanism ************************************/

	 cout << "\n Query Mechanism on NoSQL<string> Database ";
	 cout << "\n -------------------------------------------------------\n";

	 Query<string> qdb;

	 cout << "\nRequirement 7.1: value of a specified key: " << endl;
	 cout << "\n -------------------------------------------------------\n";
	 cout << "\nKey:			   " << endl;
	 cout << "       " << elem1.name << endl;
	 cout << "Value:  ";
	 cout << qdb.getValue(elem1.name, db).show();

	 cout << "\nRequirement 7.2: children of a specified key: " << endl;
	 cout << "\n -------------------------------------------------------\n";
	 cout << "\nKey:			   " << endl;
	 cout << "       " << elem1.name << endl;
	 cout << "Children:  "<<endl;
	 vector<string> children = qdb.getChildren(elem1.name,db);
	 for (auto it = children.begin(); it != children.end(); ++it)
		 cout << setw(8) << *it;
	 
	 cout << "\n\nRequirement 7.3: Set of all keys matching a specified pattern: " << endl;
	 cout << "-------------------------------------------------------\n";
	 vector<string> veckeys3 ;

	 veckeys3 = qdb.getKeySpecPatternL("(elem1)(.*)", db.keys(),db);

	 if (veckeys3.size() == 0)
	 {
		 cout << "\nSorry no matching keys found"<<endl;
	 }
	 else
	 {	
		 cout << "All Matching Keys:" << endl;
		 for (auto it = veckeys3.begin(); it != veckeys3.end(); ++it)
		 {
			 cout << setw(8) << *it;
		 }

	 }
	 cout << endl;

	 cout << "\n\nRequirement 7.4: All keys with specified pattern in their item name: " << endl;
	 cout << "-------------------------------------------------------\n";
	 vector<string> veckeys4;

	 veckeys4 = qdb.getItemSpecPatternL("(elem)(.*)", db.keys(), db);

	 if (veckeys4.size() == 0)
	 {
		 cout << "\nSorry no matching keys found" << endl;
	 }
	 else
	 {
		 cout << "All Matching Keys:" << endl;
		 for (auto it = veckeys4.begin(); it != veckeys4.end(); ++it)
		 {
			 cout << setw(8) << *it;
		 }

	 }
	 cout << endl;

	 cout << "\n\nRequirement 7.5: All keys with specified pattern in their category name: " << endl;
	 cout << "-------------------------------------------------------\n";
	 vector<string> veckeys5;

	 veckeys5 = qdb.getCategorySpecPatternL("(tes)(.*)", db.keys(),db);

	 if (veckeys5.size() == 0)
	 {
		 cout << "\nSorry no matching keys found" << endl;
	 }
	 else
	 {
		 cout << "All Matching Keys:" << endl;
		 for (auto it = veckeys5.begin(); it != veckeys5.end(); ++it)
		 {
			 cout << setw(8) << *it;
		 }

	 }
	 cout << endl;

	 cout << "\n\nRequirement 7.6: All keys with specified pattern in their data section: " << endl;
	 cout << "-------------------------------------------------------\n";
	 vector<string> veckeys6;

	 veckeys6 = qdb.getDataSpecPatternL("(ele)(.*)", db.keys(),db);

	 if (veckeys6.size() == 0)
	 {
		 cout << "\nSorry no matching keys found" << endl;
	 }
	 else
	 {
		 cout << "All Matching Keys:" << endl;
		 for (auto it = veckeys6.begin(); it != veckeys6.end(); ++it)
		 {
			 cout << setw(8) << *it;
		 }

	 }
	 cout << endl;

	 cout << "\n\nRequirement 7.7: All keys contain values written within specified time interval: " << endl;
	 cout << "-------------------------------------------------------\n";
	 vector<string> veckeys7;

	 veckeys7 = qdb.getTimeDateIntervalL("Sun Feb 5 18:30:41 2017" , db.getDateTime(), db.keys(),db);

	 if (veckeys7.size() == 0)
	 {
		 cout << "\nSorry no matching keys found" << endl;
	 }
	 else
	 {
		 cout << "All Matching Keys:" << endl;
		 for (auto it = veckeys7.begin(); it != veckeys7.end(); ++it)
		 {
			 cout << setw(8) << *it;
		 }

	 }
	 cout << endl;

	 
}

#endif