////////////////////////////////////////////////////////////////////////////////
// TestExec.cpp - Test Executive for Remote Code Publisher  	              //			
// ver 1.0																	  //			
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
/*
*  Package Operations:
*  ===================
*  This module is the Test Executive for Remote Code Publisher 
*
*	header			->  Header for TestExec
*	r1				->  Demo for Requirement 1
*	r2				->  Demo for Requirement 2
*	r3				->  Demo for Requirement 3
*	r4				->  Demo for Requirement 4
*	r5				->  Demo for Requirement 5
*	r6				->  Demo for Requirement 6
*	r7				->  Demo for Requirement 7
*	r8				->  Demo for Requirement 8
*	r9				->  Demo for Requirement 9
*	r10				->  Demo for Requirement 10
*	r11				->  Demo for Requirement 11
*	demo			->  Demo with File Transfer
*	
*  Required Files:
*  ---------------
*  - TestExec.cpp
*
*  Maintanence History:
*  --------------------
*  ver 1.0 :2 May 2017
*  - first release
*
*/

#include <iostream>

using namespace std;

//----< header for TestExec  >--------------------
void header()
{
	cout << "\n \t \t \t \t \t Welcome to My Remote Code Publisher ";
	cout << "\n \t \t \t \t \t ====================================\n";
	cout << "\n";
}

//----< Demo for Requirement 1  >--------------------
void r1()
{
	cout << "\n\n";
	cout << "  Requirement # 1" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Using Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs" << endl;
	cout << "\t Also using Windows Presentation Foundation (WPF) to provide a required client Graphical User Interface (GUI).";
	cout << "\n\n";
}

//----< Demo for Requirement 2  >--------------------
void r2()
{
	cout << "\n\n";
	cout << "  Requirement # 2" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Using the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management";
	cout << "\n\n";

}

//----< Demo for Requirement 3  >--------------------
void r3()
{
	cout << "\n\n";
	cout << "  Requirement # 3" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Providing a Publisher program that provides functionality to publish, as linked web pages, the contents of a set of C++ source code files." << endl;
	cout << "\n\n";

}

//----< Demo for Requirement 4  >--------------------
void r4()
{
	cout << "\n\n";
	cout << "  Requirement # 4" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t For the publishing process all the requirements of CodePublisher developed in Project #3 are satisfied" << endl;
	cout << "\n\n";
}

//----< Demo for Requirement 5  >--------------------
void r5()
{
	cout << "\n\n";
	cout << "  Requirement # 5" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Providing a Client program that can upload files, and view Repository contents, as described in the Purpose section," << endl;
	cout << "\t Project - \"CppCli-WPF-APP\" is the Client GUI which has following features: " << endl;
	cout << "\t 1. It can upload source code text files to the Remote Publisher by using \"Upload\" Button and Publish them to htm using \"Publish\" Button " << endl;
	cout << "\t 2. It can display file categories by using: " << endl;
	cout << "\t    -> \"Lazy Download Browse\" Button for \"Lazy Download\" " << endl;
	cout << "\t    -> \"IIS Server Browse\" Button for \"IIS Download\" " << endl;
	cout << "\t 3. It can display all available files in any category by also using:" << endl;
	cout << "\t    -> \"Lazy Download Browse\" Button for \"Lazy Download\" " << endl;
	cout << "\t    -> \"IIS Server Browse\" Button for \"IIS Download\" " << endl;
	cout << "\t 4. It can display all of the files in any category that have no parents by using: " << endl;
	cout << "\t    -> \"Lazy Download Get all Files\" Button for \"Lazy Download\" " << endl;
	cout << "\t    -> \"IIS Server Get all Files\" Button for \"IIS Download\" " << endl;
	cout << "\t 5. It can open multiple htm files displayed on GUI by selecting them and then using \"Open in Browser\" Button " << endl;
	cout << "\t 6. It can delete multiple htm files displayed on GUI by selecting them and then using \"Delete File\" Button " << endl;
	cout << "\t    After Deleting Files we can also RePublish and ReDownload files by using \"ReDownload Lazy and IIS\" Button " << endl;
	cout << "\t 7. On starting, will download style sheet and JavaScript files from the Server Repository to IIS Server virtual path " << endl;
	cout << "\n\n";

}

//----< Demo for Requirement 6  >--------------------
void r6()
{
	cout << "\n\n";
	cout << "  Requirement # 6" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t It does provide a message-passing communication system, based on Sockets, used to access the Repository's functionality from another process or machine." << endl;
	cout << "\n\n";

}

//----< Demo for Requirement 7  >--------------------
void r7()
{
	cout << "\n\n";
	cout << "  Requirement # 7" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t The communication system provides support for passing HTTP style messages using asynchronous one-way messaging." << endl;
	cout << "\n\n";
}

//----< Demo for Requirement 8  >--------------------
void r8()
{
	cout << "\n\n";
	cout << "  Requirement # 8" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t The communication system also supports sending and receiving streams of bytes. Streams are established with an initial exchange of messages." << endl;
	cout << "\n\n";
}

//----< Demo for Requirement 9  >--------------------
void r9()
{
	cout << "\n\n";
	cout << "  Requirement # 9" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Also included an automated unit test suite demonstrating all the requirements of this project including the transmission of files." << endl;
	cout << "\t For in detail Demo with GUI screenshots please refer Readme.docx" << endl;
	cout << "\n\n";
}

//----< Demo for Requirement 10  >--------------------
void r10()
{
	cout << "\n\n";
	cout << "  Requirement # 10 (BONUS)" << endl;
	cout << "  ========================";
	cout << "\n\n";
	cout << "\t Using a lazy lazydownload strategy" << endl;
	cout << "\t \"Lazy Download\" button is used for this requirement " << endl;
	cout << "\t Following buttons also help fulfill these requirements: " << endl;
	cout << "\t 1. Lazy Download Browse     2. Lazy Download Get all Files" << endl;
	cout << "\t 3. Open in Browser          4. Delete File" << endl;
	cout << "\t 5. Redownload Lazy and IIS" << endl;
	cout << "\n\n";
}

//----< Demo for Requirement 11  >--------------------
void r11()
{
	cout << "\n\n";
	cout << "  Requirement # 11 (BONUS)" << endl;
	cout << "  ========================";
	cout << "\n\n";
	cout << "\t Using IIS Server" << endl;
	cout << "\t \"IIS Download\" button is used for this requirement " << endl;
	cout << "\t Following buttons also help fulfill these requirements: " << endl;
	cout << "\t 1. IIS Download Browse     2. IIS Server Get all Files" << endl;
	cout << "\t 3. Open in Browser         4. Delete File" << endl;
	cout << "\t 5. Redownload Lazy and IIS" << endl;	
	cout << "\n\n";
}

//----< Demo >--------------------
void demo()
{
	cout << "\n\n";
	cout << "  Demonstration" << endl;
	cout << "  =============";
	cout << "\n\n";
	cout << "\t From Command Line:" << endl;
	cout << "\t Client Details: localhost:8081" << endl;
	cout << "\t Server Details: localhost:8080" << endl;
	cout << "\t Virtual Path: C:\\CodePublisher\\" << endl;
	cout << "\n";
	cout << "\t On loading the GUI, the javascript and css files will be downloaded in the virtual path folder -> C:\\CodePublisher\\" << endl;
	cout << "\n";
	cout << "\t Execution Tab:" << endl;
	cout << "\t 1. \"Browse\" button selects a certain 'Folder' for analysis" << endl;
	cout << "\t 2. \"Upload\" button Uploads all the .h and .cpp files present in folder selected above to path ../Repository_Server/" << endl;
	cout << "\t    Please check *.h and *.cpp files present in 'Folder' in ../Repository_Server/" << endl;
	cout << "\t 3. \"Publish\" button Publishes all the .h and .cpp files present in folder ../Repository_Server/ into folder ../Repository_Code_Publish/" << endl;
	cout << "\t    with the corresponding folders as Categories:" << endl;
	cout << "\t    Please check ../Repository_Code_Publish/CppFiles for *.cpp and ../Repository_Code_Publish/HeaderFiles for *.h" << endl;
	cout << "\t 4. \"Lazy Download\" button Downloads Published html files from folder ./Repository_Code_Publish/ into folder ../Repository_Client/" << endl;
	cout << "\t    with the corresponding folders as Categories:" << endl;
	cout << "\t    Please check ../Repository_Client/CppFiles for *.cpp and ../Repository_Client/HeaderFiles for *.h" << endl;
	cout << "\t 5. \"IIS Download\" button Downloads Published html files from folder ./Repository_Code_Publish/ into folder C:\\CodePublisher\\" << endl; 
	cout << "\t    with the corresponding folders as Categories:" << endl;
	cout << "\t    Please check C:\\CodePublisher\\CppFiles\\ for *.cpp and C:\\CodePublisher\\HeaderFiles\\ for *.h" << endl;
	cout << "\t File List Tab:" << endl;
	cout << "\t   Lazy Download Buttons:" << endl;
	cout << "\t    1. \"Lazy Download Browse\" button Displays Categories plus the files present in each category " << endl;
	cout << "\t    2. \"Lazy Download Get all Files \" button Display all Published Files in the repository plus with no parents" << endl;
	cout << "\t   IIS Download Buttons:" << endl;
	cout << "\t    1. \"IIS Download Browse\" button Displays Categories plus the files present in each category " << endl;
	cout << "\t    2. \"IIS Download Get all Files\" button Display all Published Files in the repository plus with no parents" << endl;
	cout << "\t   General Use Buttons:" << endl;
	cout << "\t    1. \"Open in Browser \" button Opens selected files in Browser. Can be used for both Lazy Download and IIS Download Repositories." << endl;
	cout << "\t    2. \"Delete File \" button Deletes selected published files, plus also republishes the undeleted (remaining) files." << endl;
	cout << "\t        Can be used for both Lazy Download and IIS Download Repositories." << endl;
	cout << "\t    3. \"ReDownload Lazy and IIS \" button After Deleting and republishing above files we can Redownload current files "<< endl;
	cout << "\t    in our Lazy and IIS Repositories using this button." << endl;
	cout << "\n\n";
}


#ifdef TEST_EXEC

//----< Test Executive for Remote Code Publisher >--------------------
int main()
{	
	header();
	r1();
	r2();
	r3();
	r4();
	r5();
	r6();
	r7();
	r8();
	r9();
	r10();
	r11();
	demo();
	system("pause");
	return 0;
}

#endif