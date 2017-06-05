////////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Mock Channel to communicate with Client GUI			  //			
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

#define IN_DLL
#include "MockChannel.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
	
#include "../MsgClient/MsgClient.h"
#include <string>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using BQueue = Async::BlockingQueue < HttpMessage >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
  void postMessage(const HttpMessage& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

//----< posts Message to Sender's queue >--------------------------------------------------
void Sendr::postMessage(const HttpMessage& msg)
{
  sendQ_.enQ(msg);
}

//----< returns Sender's queue >--------------------------------------------------
BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
  HttpMessage getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

//----< gets Message from Reciever's queue >--------------------------------------------------
HttpMessage Recvr::getMessage()
{
  return recvQ_.deQ();
}

//----< returns Reciever's queue >--------------------------------------------------
BQueue& Recvr::queue()
{
  return recvQ_;
}

/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr);
  void start(const std::string& virtual_path);
  void stop();
  void loadUtils(const std::string& virtual_path);
private:
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  bool stop_ = false;
  MsgClient client;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< Loads Utilities in virtual directory >-------------------------------

void MockChannel::loadUtils(const std::string& virtual_path)
{
	// Create HeaderFiles directory
	if (!FileSystem::Directory::exists(virtual_path + "HeaderFiles\\"))
		FileSystem::Directory::create(virtual_path + "HeaderFiles\\");
	// Create CppFiles directory
	if (!FileSystem::Directory::exists(virtual_path + "CppFiles\\"))
		FileSystem::Directory::create(virtual_path + "CppFiles\\");

	string dpath = "../Repository_Code_Publish/";
	vector<string> htmlfiles = FileSystem::Directory::getFiles(dpath, "*");

	ofstream fp_out;
	ifstream ifs;

	for (string filename : htmlfiles)
	{
		ifs.open(dpath + filename);
		string rfilename;
		rfilename = virtual_path + filename;
		fp_out.open(rfilename);
		fp_out << ifs.rdbuf() << endl;
		ifs.close();
		fp_out.close();
	}
}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start(const std::string& virtual_path)
{
  std::cout << "\n  MockChannel starting up";
  loadUtils(virtual_path);
  thread_ = std::thread(
    [this] {
    Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
    if (pSendr == nullptr || pRecvr == nullptr)
    {
      std::cout << "\n  failed to start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BQueue& recvQ = pRecvr->queue();
    while (!stop_)
    {
	  HttpMessage msg = sendQ.deQ(); 
	  
	  if (msg.attributes()[0].first == "UPLOAD")
		  client.upload(msg.bodyString());
	  else if (msg.attributes()[0].first == "PUBLISH")
		  client.publish();
	  else if (msg.attributes()[0].first == "LAZYDOWNLOAD")
		  client.download();
	  else if (msg.attributes()[0].first == "IISDOWNLOAD")
		  client.iisdownload(msg.bodyString());
	  else if (msg.attributes()[0].first == "OPENBROWSER")
		  client.openBrowser(msg.bodyString());
	  else if (msg.attributes()[0].first == "DELETEFILE")
		  client.deleteFile(msg.bodyString());
    }
    std::cout << "\n  Server stopping\n\n";
  });

}

//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
  return new MockChannel(pISendr, pIRecvr); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif
