#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
////////////////////////////////////////////////////////////////////////////////
// MockChannel.h - Mock Channel to communicate with Client GUI				  //			
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
*  Mock Channel to communicate with Client GUI. It also hosts a send queue, a receive queue, 
*  and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  Required Files:
*  ---------------
*  MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*  HttpMessage.h , HttpMessage.cpp , Utilities.h , Utilities.cpp
*  FileSystem.h , FileSystem.cpp , Logger.h , Logger.cpp
*  MsgClient.h , MsgClient.cpp , Sockets.h , Sockets.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 2 May 2017
*  - first release
*/
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include "../HttpMessage/HttpMessage.h"
#include <string>

struct ISendr
{
  virtual void postMessage(const HttpMessage& msg) = 0;
};

struct IRecvr
{
  virtual HttpMessage getMessage() = 0;
};

struct IMockChannel
{
public:
  virtual void start(const std::string& virtual_path) = 0;
  virtual void stop() = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
    DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


