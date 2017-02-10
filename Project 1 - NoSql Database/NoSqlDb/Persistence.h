#ifndef PERSISTENCE_H
#define PERSISTENCE_H
/////////////////////////////////////////////////////////////////////////////
// Persistence.h - Class Persistence which defines to Xml methods          //
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
* This module consists of class Persistence which defines methods to store in-memory 
* NoSqlDb into an XML file
* The various operations supported here are as follows:
*  - saveXml		->  It converts Database contents to a string of specific format
*  - saveXmlFile	->  It saves the above received string into an Xml File
*  - autoPersist    ->  Auto Persists the Database into Xml files within certain time intervals
*  - restore		->  Restores the database stored in Xml file into the in-memory database
*
* Required Files:
* ===============
* Persistence.h, Test.cpp
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
#include <unordered_map> 

using namespace std;
using namespace XmlProcessing;
using SPtr = shared_ptr<AbstractXmlElement>;

///////////////////////////////////////////////////////////////////////////
// Persistence class

template<typename Data>
class Persistence:public NoSqlDb<Data>
{
public:
	string saveXml(NoSqlDb<Data>& db);
	void saveXmlFile(const string& xml);
	void autoPersist(NoSqlDb<Data>& db);
	NoSqlDb<Data> restore(string path);
};

//-------------------------< Convert Database contents into an string >------------------------------------
template<typename Data>
string Persistence<Data>::saveXml(NoSqlDb<Data>& db)
{
	string xml;
	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("db");	doc.docElement() = pRoot;
	
	vector<string>::const_iterator vec_itr;
	unordered_map<Key, Element<Data>>::iterator map_itr;
	
	for (map_itr = db.getMapAccess().begin(); map_itr != db.getMapAccess().end(); ++map_itr)
	{
		int count_child_keys = 0;
		SPtr pRecord = makeTaggedElement("record");						pRoot->addChild(pRecord);
		SPtr pKeyElem = makeTaggedElement("key");						pRecord->addChild(pKeyElem);
		SPtr pTextElem = makeTextElement(map_itr->first);				pKeyElem->addChild(pTextElem);
		SPtr pNameElem = makeTaggedElement("name");						pRecord->addChild(pNameElem);
		pTextElem = makeTextElement(map_itr->second.name);				pNameElem->addChild(pTextElem);
		SPtr pCategoryElem = makeTaggedElement("category");				pRecord->addChild(pCategoryElem);
		pTextElem = makeTextElement(map_itr->second.category);			pCategoryElem->addChild(pTextElem);
		SPtr pDescriptionElem = makeTaggedElement("description");		pRecord->addChild(pDescriptionElem);
		pTextElem = makeTextElement(map_itr->second.description);		pDescriptionElem->addChild(pTextElem);
		SPtr pTimeDateElem = makeTaggedElement("timeDate");				pRecord->addChild(pTimeDateElem);
		pTextElem = makeTextElement(map_itr->second.timeDate);			pTimeDateElem->addChild(pTextElem);
		SPtr pChildElem = makeTaggedElement("children");				pRecord->addChild(pChildElem);
		for (vec_itr = map_itr->second.getVectoraccess().begin(); vec_itr != map_itr->second.getVectoraccess().end(); ++vec_itr)
		{
			SPtr pChildKeyElem = makeTaggedElement("childkey");
			pChildElem->addChild(pChildKeyElem);
			pTextElem = makeTextElement(*vec_itr);
			pChildKeyElem->addChild(pTextElem);
			count_child_keys++;
		}
		SPtr pChildKeyNumElem = makeTaggedElement("numchildkeys");		pRecord->addChild(pChildKeyNumElem);
		string ChildKeyNumString = Convert<int>::toString(count_child_keys);
		pTextElem = makeTextElement(ChildKeyNumString);					pChildKeyNumElem->addChild(pTextElem);
		SPtr pDataElem = makeTaggedElement("data");						pRecord->addChild(pDataElem);
		pTextElem = makeTextElement(map_itr->second.data);				pDataElem->addChild(pTextElem);
	}
	xml = doc.toString();
	saveXmlFile(xml);
	return xml;
}

//-------------------------<  Pass above string into an Xml File >------------------------------------
template<typename Data>
void Persistence<Data>::saveXmlFile(const string& xml)
{
	ofstream fp_out;

	fp_out.open("Xmlfile.xml");

	fp_out << xml << endl;

}

//-------------------------<Auto-Persist contents of the Database into an xml File >------------------------------------
template<typename Data>
void Persistence<Data>::autoPersist(NoSqlDb<Data>& db)
{
	cout << "\n Turning ON Auto Persisting Feature" << endl;
	this_thread::sleep_for(chrono::milliseconds(0050));
	int count = 0;
	cout << "\n					PLEASE WAIT FOR 1 MIN                    " << endl;
	while (true)
	{
		count++;
		cout << "\n Persisting DataBase";
		cout << ".";
		this_thread::sleep_for(chrono::seconds(1));
		cout << ".";
		this_thread::sleep_for(chrono::seconds(1));
		cout << ".";
		this_thread::sleep_for(chrono::seconds(1));
		cout << ".";
		this_thread::sleep_for(chrono::seconds(1));
		saveXml(db);
		if (count == 2)
			break;
		cout << "\n";
	}
	cout << "\n";
	cout << "\n AutoPersistance demo to XML done !!!" << endl;
	cout << "\n Turning Off AutoPersistance" << endl;

}

//-------------------------<Restore the Database from Xml File>------------------------------------
template<typename Data>
NoSqlDb<Data> Persistence<Data>::restore(string path)
{
	NoSqlDb<Data> db;
	Element<Data> elem;
	string key_value, name_value, category_value, description_value, timeDate_value;
	string children_value, childkey_value, numchildkeys_value, data_value;

	XmlDocument doc(path, XmlDocument::file);

	vector<SPtr> record_desc = doc.descendents("record").select();
	vector<SPtr> key_desc = doc.descendents("key").select();
	vector<SPtr> name_desc = doc.descendents("name").select();
	vector<SPtr> category_desc = doc.descendents("category").select();
	vector<SPtr> description_desc = doc.descendents("description").select();
	vector<SPtr> timeDate_desc = doc.descendents("timeDate").select();
	vector<SPtr> children_desc = doc.descendents("children").select();
	vector<SPtr> childkey_desc = doc.descendents("childkey").select();
	vector<SPtr> numchildkeys_desc = doc.descendents("numchildkeys").select();
	vector<SPtr> data_desc = doc.descendents("data").select();

	int record_size = record_desc.size();  							static int j = 0;
	for (int i = 0; i < record_size; i++)
	{
		key_value = key_desc[i]->children()[0]->value();			key_value = trim(key_value);
		name_value = name_desc[i]->children()[0]->value();			name_value = trim(name_value);								
		elem.name = name_value;
		category_value = category_desc[i]->children()[0]->value();	category_value = trim(category_value);						
		elem.category = category_value;
		description_value = description_desc[i]->children()[0]->value();
		description_value = trim(description_value);				elem.description = description_value;
		timeDate_value = timeDate_desc[i]->children()[0]->value();	timeDate_value = trim(timeDate_value);						
		elem.timeDate = timeDate_value;
		numchildkeys_value = numchildkeys_desc[i]->children()[0]->value();
		int numchildkeys = Convert<int>::fromString(numchildkeys_value);
		for (int k = 0; k < numchildkeys; k++)
		{
			childkey_value = childkey_desc[j]->children()[0]->value(); 	
			childkey_value = trim(childkey_value);
			elem.getVectoraccess().push_back(childkey_value);
			j++;
		}
		data_value = data_desc[i]->children()[0]->value();			data_value = trim(data_value);
		elem.data = data_value;
		db.save(key_value, elem);
		elem.getVectoraccess().clear();
	}
	cout << "\n\n";
	return db;
}

#endif