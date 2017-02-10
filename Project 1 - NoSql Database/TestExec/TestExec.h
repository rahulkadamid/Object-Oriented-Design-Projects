#ifndef TESTEXEC_H
#define TESTEXEC_H
/////////////////////////////////////////////////////////////////////////////
// TestExec.h - Class TestExec with each requirement as each method        //
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
* This module consists of class TestExec which descibes each requirement to 
* satisfy the project in separate methods
* This package begins the demonstration of meeting requirements.
* Every requirement from 1 to 12 have been successfully implemented
* and demonstrated here. Any notes, if any, to be taken care of are
* mentioned as per the requirement.
*
* Required Files:
* ===============
* TestExec.h, main.cpp
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
#include "../NoSqlDb/NoSqlDb.h"
#include "../NoSqlDb/Persistence.h"
#include "../NoSqlDb/Query.h"

using namespace std;

class TestExec
{
public:
	void requirement1();
	void requirement2a();
	void requirement2b();
	void requirement3();
	void requirement4();
	void requirement5();
	void requirement6();
	void requirement7a();
	void requirement7b();
	void requirement7c();
	void requirement7d();
	void requirement7e();
	void requirement7f();
	void requirement7g();
	void requirement8();
	void requirement9();
	void requirement10();
	void requirement11();
	void requirement12();

private:
	NoSqlDb<string> db_string;
	NoSqlDb<int> db_int;
};

//-------------< Following is the demonstration of each requirement, for each database type >----------

//------ NoSqlDb implemented in C++ using standard C++ Libraries and Visual Studio 2015--------------
void TestExec::requirement1()
{	
	//------ Demonstrating NoSqlDb implemented in C++ using standard C++ Libraries and Visual Studio 2015--------
	title("Requirement # 1");
	cout << "\n\n";
	title("Implemented in C++");
	cout << "\n\n";
	cout << "\t NoSqlDb implemented in C++ using standard C++ Libraries and Visual Studio 2015";
	cout << "\n\n";
}
 
//-------Implementing a template (here as a string) class-----------------------
void TestExec::requirement2a()
{
	//-------Demonstrating that the NoSqlDb is a template class that can store String as well as int data-------
	title("Requirement # 2");
	cout << "\n\n";
	title("Implementing a template class");
	cout << "\n\n";

	cout << "\n  Addition of Key/Value Pairs of with string data";
	cout << "\n -------------------------------------------------------\n";

	Element<string> elem1;
	elem1.name = "elem1";
	elem1.category = "category1";
	elem1.data = "elem1's string";
	elem1.description = "testdescip1";
	elem1.getVectoraccess().push_back("elem3");
	elem1.getVectoraccess().push_back("elem2");
	elem1.timeDate = elem1.getDateTime();

	db_string.save(elem1.name, elem1);
	cout << "\n Record #1 added " << endl;

	Element<string> elem2;
	elem2.name = "elem2";
	elem2.category = "category2";
	elem2.data = "elem2's string";
	elem2.description = "testdescip2";
	elem2.getVectoraccess().push_back("elem1");
	elem2.getVectoraccess().push_back("elem3");
	elem2.timeDate = elem2.getDateTime();

	db_string.save(elem2.name, elem2);
	cout << "\n Record #2 added " << endl;

	db_string.retrieveDb(db_string);

}

//-------Implementing a template (here as a int) class-----------------------
void TestExec::requirement2b()
{
	//-------Demonstrating that the NoSqlDb is a template class that can store String as well as int data-------
	cout << "\n  Addition of Key/Value Pairs of with int data";
	cout << "\n -------------------------------------------------------\n";

	Element<int> ielem1;
	ielem1.name = "ielem1";
	ielem1.category = "icategory1";
	ielem1.data = 1;
	ielem1.description = "itestdescip1";
	ielem1.getVectoraccess().push_back("ielem2");
	ielem1.getVectoraccess().push_back("ielem3");
	ielem1.timeDate = ielem1.getDateTime();

	db_int.save(ielem1.name, ielem1);
	cout << "\n Record #1 added " << endl;

	Element<int> ielem2;
	ielem2.name = "ielem2";
	ielem2.category = "icategory2";
	ielem2.data = 2;
	ielem2.description = "itestdescip2";
	ielem2.getVectoraccess().push_back("ielem1");
	ielem2.getVectoraccess().push_back("ielem3");
	ielem2.timeDate = ielem2.getDateTime();

	db_int.save(ielem2.name, ielem2);
	cout << "\n Record #2 added " << endl;

	db_int.retrieveDb(db_int);
	cout << "-----------------------------------------------------------------         ";
	title("Demo of Template class taking in string and int data finished");
	cout << "\n\n";
}

//-------Addition and Deletion of Key/value pairs-----------------------
void TestExec::requirement3()
{	
	//-------Demonstrating that for the NoSqlDb we can Add and Delete Key/Value pairs------------
	title("Requirement # 3");
	cout << "\n\n";
	title(" Addition and Deletion of Key/value pairs");
	cout << "\n\n";

	cout << "\n  Addition of Key/Value Pairs of with string data";
	cout << "\n -------------------------------------------------------\n";

	Element<string> elem3;
	elem3.name = "elem3";
	elem3.category = "category3";
	elem3.data = "elem3's string";
	elem3.description = "testdescip3";
	elem3.getVectoraccess().push_back("elem1");
	elem3.getVectoraccess().push_back("elem2");
	elem3.timeDate = elem3.getDateTime();

	db_string.save(elem3.name, elem3);
	cout << "\n Record #3 added " << endl;

	Element<string> elem4;
	elem4.name = "elem4";
	elem4.category = "category4";
	elem4.data = "elem4's string";
	elem4.description = "testdescip4";
	elem4.getVectoraccess().push_back("elem2");
	elem4.getVectoraccess().push_back("elem3");
	elem4.timeDate = elem4.getDateTime();

	db_string.save(elem4.name, elem4);
	cout << "\n Record #4 added " << endl;

	cout << "\n  Deletion of key/value pair from NoSqlDb<string>";
	cout << "\n ------------------------------------------\n";

	db_string.deleteRelationship(elem3.name);

	cout << "\n elem3 key-value pair deleted ...\n";

	db_string.retrieveDb(db_string);
}

//-----Addition and/or deletion of relationships , editing text metadata----------
//-----Replacing existing value's instance with new instance----------
void TestExec::requirement4()
{
	//----------Demonstrating that for the NoSqlDb we can add/delete relationships and change value's of instances
	title("Requirement # 4");
	cout << "\n\n";
	title(" Addition and/or deletion of relationships , editing text metadata Replacing existing value's instance with new instance");
	cout << "\n\n";

	db_string.retrieveDb(db_string);

	cout << "\n  Creating a new instance";
	cout << "\n ------------------------------------------\n";

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

	cout << "\n  Changing relationships , editing text metadat and replacing elem1's values with elem6's values" << endl;

	db_string.modify("elem1", elem6);

	cout << "\nModified elem1:" << endl;
	cout << "\n  " << "elem1" << ":";
	cout << db_string.value("elem1").show();

}

//-----Save / Restore database contents to / from xml file------------
void TestExec::requirement5()
{	
	//----------Demonstrating that for the NoSqlDb we can Save and Restore the contents to and from xml file
	title("Requirement # 5");
	cout << "\n\n";
	title(" Save/Restore database contents to/from xml file");
	cout << "\n\n";

	db_string.retrieveDb(db_string);

	//----------Creating a Persistence class object to call saveXml and restore methods------------
	Persistence<string> pdb;

	cout << "\n  Saving NoSqlDb<string> to XML ";
	cout << "\n ------------------------------------------\n";

	//---------Saving database to XML--------------
	string xml = pdb.saveXml(db_string);

	cout << "\n  NoSqlDb<string> persisted to Xmlfile.xml" << endl;

	cout << "\n ";

	cout << "\n  From XML to NoSqlDb<string>";
	cout << "\n ------------------------------------------\n";

	cout << "\n Loading database from Xmlfile.xml file  ";
	
	//---------Restoring database back from XML--------------
	db_string = pdb.restore("Xmlfile.xml");

	cout << "  NoSqlDb<string> loaded from XML !!! " << endl;

	db_string.retrieveDb(db_string);
}

//-----Auto Persist database to xml in positive time intervals-------------
void TestExec::requirement6()
{	
	//---------Demonstrating that for the NoSqlDb we can Auto Persist database to xml----------
	title("Requirement # 6");
	cout << "\n\n";
	title(" Auto Persist database to xml in positive time intervals");
	cout << "\n\n";

	cout << "\n Demo for Auto-Persisted Database ";
	cout << "\n ---------------------------------------------------------------------------------------------------------";
	cout << endl;

	Persistence<string> pdb;
	pdb.autoPersist(db_string);

	cout << "\n Please check Xmlfile.xml" << endl;
	cout << "\n";

}

//------Static Querying Mechanism-----------------
void TestExec::requirement7a()
{	
	//---------Demonstrating that we can execute static queries over the NoSqlDb--------
	title("Requirement # 7");
	cout << "\n\n";
	title(" Static Querying Mechanism");
	cout << "\n\n";

	cout << "\n  Retrieving elements from NoSqlDb<string>";
	cout << "\n ------------------------------------------\n";

	db_string.retrieveDb(db_string);

	cout << "\n Query Mechanism on NoSQL<string> Database ";
	cout << "\n -------------------------------------------------------\n";

	Query<string> qdb;

	//---------Demonstrating Query->value of a specified key--------------
	cout << "\nRequirement 7.1: value of a specified key: " << endl;
	cout << "\n -------------------------------------------------------\n";
	cout << "\nKey:			   " << endl;
	cout << "       " << "elem1" << endl;
	cout << "Value:  ";
	cout << qdb.getValue("elem1", db_string).show();
}

//------Static Querying Mechanism-----------------
void TestExec::requirement7b() 
{	
	//---------Demonstrating that we can execute static queries over the NoSqlDb--------
	Query<string> qdb;

	//---------Demonstrating Query->children of a specified key--------------
	cout << "\nRequirement 7.2: children of a specified key: " << endl;
	cout << "\n -------------------------------------------------------\n";
	cout << "\nKey:			   " << endl;
	cout << "       " << "elem1" << endl;
	cout << "Children:  " << endl;
	vector<string> children = qdb.getChildren("elem1", db_string);
	for (auto it = children.begin(); it != children.end(); ++it)
		cout << setw(8) << *it;
}

//------Static Querying Mechanism-----------------
void TestExec::requirement7c() 
{	
	//---------Demonstrating that we can execute static queries over the NoSqlDb--------
	Query<string> qdb;

	//---------Demonstrating Query->Set of all keys matching a specified pattern--------------
	cout << "\n\nRequirement 7.3: Set of all keys matching a specified pattern: " << endl;
	cout << "-------------------------------------------------------\n";
	vector<string> veckeys3;

	veckeys3 = qdb.getKeySpecPatternL("(elem)(.*)", db_string.keys(), db_string);

	if (veckeys3.size() == 0)
	{
		cout << "\nSorry no matching keys found" << endl;
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


}

//------Static Querying Mechanism-----------------
void TestExec::requirement7d() 
{	
	//---------Demonstrating that we can execute static queries over the NoSqlDb--------
	Query<string> qdb;

	//---------Demonstrating Query->All keys with specified pattern in their item name--------------
	cout << "\n\nRequirement 7.4: All keys with specified pattern in their item name: " << endl;
	cout << "-------------------------------------------------------\n";
	vector<string> veckeys4;

	veckeys4 = qdb.getItemSpecPatternL("(elem)(.*)", db_string.keys(), db_string);

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


}

//------Static Querying Mechanism-----------------
void TestExec::requirement7e() 
{	
	//---------Demonstrating that we can execute static queries over the NoSqlDb--------
	Query<string> qdb;

	//---------Demonstrating Query->All keys with specified pattern in their category name--------------
	cout << "\n\nRequirement 7.5: All keys with specified pattern in their category name: " << endl;
	cout << "-------------------------------------------------------\n";
	vector<string> veckeys5;

	veckeys5 = qdb.getCategorySpecPatternL("(cat)(.*)", db_string.keys(), db_string);

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



}

//------Static Querying Mechanism-----------------
void TestExec::requirement7f() 
{
	//---------Demonstrating that we can execute static queries over the NoSqlDb--------
	Query<string> qdb;

	//---------Demonstrating Query->All keys with specified pattern in their data name--------------

	cout << "\n\nRequirement 7.6: All keys with specified pattern in their data section: " << endl;
	cout << "-------------------------------------------------------\n";
	vector<string> veckeys6;

	veckeys6 = qdb.getDataSpecPatternL("(ele)(.*)", db_string.keys(), db_string);

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

}

//------Static Querying Mechanism-----------------
void TestExec::requirement7g() 
{
	//---------Demonstrating that we can execute static queries over the NoSqlDb--------
	Query<string> qdb;

	//---------Demonstrating Query->All keys contain values written within specified time interval--------------

	cout << "\n\nRequirement 7.7: All keys contain values written within specified time interval: " << endl;
	cout << "-------------------------------------------------------\n";
	vector<string> veckeys7;

	veckeys7 = qdb.getTimeDateIntervalL("Sun Feb 5 18:30:41 2017", db_string.getDateTime(), db_string.keys(), db_string);

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

//------Compound Querying Mechanism - Intersection-----------------
void TestExec::requirement8()
{	
	//---------Demonstrating that we can execute Compound queries over the NoSqlDb--------
	title("Requirement # 8");
	cout << "\n\n";
	title(" Compound Querying Mechanism - Intersection ");
	cout << "\n\n";

	Query<string> qdb;

	vector<string> veckeys4;

	veckeys4 = qdb.getItemSpecPatternL("(elem)(.*)", db_string.keys(), db_string);

	vector<string> veckeys5;

	veckeys5 = qdb.getCategorySpecPatternL("(cat)(.*)", db_string.keys(), db_string);

	vector<string> veckeys6;

	veckeys6 = qdb.getIntersection(veckeys4, veckeys5, db_string);

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

}

//------Compound Querying Mechanism - Union-----------------
void TestExec::requirement9()
{	
	//---------Demonstrating that we can execute Compound queries over the NoSqlDb--------
	title("Requirement # 9");
	cout << "\n\n";
	title(" Compound Querying Mechanism - Union ");
	cout << "\n\n";

	Query<string> qdb;

	vector<string> veckeys4;

	veckeys4 = qdb.getItemSpecPatternL("(elem)(.*)", db_string.keys(), db_string);

	vector<string> veckeys5;

	veckeys5 = qdb.getCategorySpecPatternL("(cat)(.*)", db_string.keys(), db_string);

	vector<string> veckeys6;

	veckeys6 = qdb.getUnion(veckeys4, veckeys5, db_string);

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

}

//------Submit Projects Package structure and dependency relationships
void TestExec::requirement10()
{	
	//-----------Demonstrating that Package structure and dependency relationships have been shared
	title("Requirement # 10");
	cout << "\n\n";
	title(" Submit Projects Package structure and dependency relationships ");
	cout << "\n\n";

	cout << " Please check package_structure.xml " << endl;
	cout << "\n\n";
}

//------Accompanied by a Test Executive-------------
void TestExec::requirement11()
{	
	//-----------Demonstrating that Test Executive was created
	title("Requirement # 11");
	cout << "\n\n";
	title(" Accompanied by a Test Executive");
	cout << "\n\n";

	cout << " We are in a Test Executive !!! :) ";

	cout << "\n\n";
}

//------Support regular expression matching for queries---------
void TestExec::requirement12()
{	
	//-------Demonstrating that regular expression were used for matching queries
	title("Requirement # 12");
	cout << "\n\n";
	title(" Support regular expression matching for queries");
	cout << "\n\n";

	cout << " Did use regular expression for pattern matching queries...Check /NoSqlDb/Query.h ";
}


#endif