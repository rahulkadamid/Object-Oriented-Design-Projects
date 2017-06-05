#ifndef MSGSERVER_H
#define MSGSERVER_H
////////////////////////////////////////////////////////////////////////////////
// MsgServer.h  - Provides Server's functionality						      //			
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
*  This package implements a server that recieves HTTP style messages in its Blocking queue and 
*  stores recieves files from sockets in its own repository. 
*  It also publishes the source code files into html files with their dependencies.
*
*  The various operations supported here are as follows:
*
*	downloadServer			->   processes LAZYDOWNLOAD request from client
*	iisdownloadServer		->   processes IISDOWNLOAD request from client
*	publishServer			->   processes PUBLISH request from client
*	deleteFile				->   processes DELETEFILE request from client
*	storecpphFilesinRepo	->   stores all .h and .cpp files in Server Repository in a set
*	readMessage				->   reads messages from client
*	readFile				->   reads files from socket and saves files in its own Repository
*
* Required Files:
*   MsgServer.h, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*   CodePublisher.h , CodePublisher.cpp
*	DepAnal.h , DepAnal.cpp
*
*  Maintenance History :
*  --------------------
*    ver 1.0 : 2 May 2017
*     -first release
*/

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"

#include <set>
#include <string>
#include <iostream>

using namespace Logging;
using namespace Utilities;
using namespace Async;
using namespace std;

using Show = StaticLogger<1>;

/////////////////////////////////////////////////////////////////////
// MsgServer class
/////////////////////////////////////////////////////////////////////

class MsgServer
{
public:
	MsgServer(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	void downloadServer(Socket& socket);
	void iisdownloadServer(const string& path, Socket& socket);
	void publishServer();
	set<string> storecpphFilesinRepo();
	void deleteFile(const string& filename);
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
};
#endif