#ifndef MSGCLIENT_H
#define MSGCLIENT_H
////////////////////////////////////////////////////////////////////////////////
// MsgClient.h - Native Client to Support GUI Client functionality			  //			
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
*  -------------------
* This package implements a client that sends HTTP style messages and
* files to a server that simply displays messages and stores files.
*
* The various operations supported here are as follows:
*
*	upload				->  Sends UPLOAD request to server
*	download			->  Sends LAZYDOWNLOAD request to server
*	iisdownload			->  Sends IISDOWNLOAD request to server
*	publish				->  Sends PUBLISH request to server
*	openBrowser			->  Opens give file in Browser
*	deleteFile			->  Sends DELETEFILE request to server
*	makeMessage			->  Makes Http style messages to send to server
*	sendMessage			->  Sends Http style messages to server
*	sendFile			->  Sends Files to server
*
* Required Files:
*   MsgClient.h, MsgClient.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*
*  Maintenance History :
*  --------------------
*    ver 1.0 : 2 May 2017
*     -first release
*/

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
#include <thread>

using namespace Logging;
using namespace Utilities;
using namespace std;
using Show = StaticLogger<1>;
using Utils = StringHelper;

/////////////////////////////////////////////////////////////////////
// MsgClient class
/////////////////////////////////////////////////////////////////////

class MsgClient
{
public:
	using EndPoint = string;
	void upload(const string& path);
	void download();
	void iisdownload(const string& path);
	void publish();
	void openBrowser(const string& filename);
	void deleteFile(const string& filename);
private:
	HttpMessage makeMessage(size_t n, const string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const string& path, const string& fqname, Socket& socket);
};

#endif