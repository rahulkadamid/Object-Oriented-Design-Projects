////////////////////////////////////////////////////////////////////////////////
// MsgServer.cpp  - Provides Server's functionality						      //			
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

#include "MsgServer.h"
#include "../DependencyAnalysisInclude/DepAnal.h"
#include "../CodePublisher/CodePublisher.h"

#include <unordered_map>
#include <fstream>
#include <cstdio>

using namespace Logging;
using namespace Utilities;
using namespace Async;
using namespace std;

using Show = StaticLogger<1>;

//----< receiver functionality is defined by this function >---------
void MsgServer::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			break;
		}
		if (msg.attributes()[0].first == "LAZYDOWNLOAD")
		{
			Show::write("\n\n  Processing LAZYDOWNLOAD request");
			downloadServer(socket);
		}
		if (msg.attributes()[0].first == "IISDOWNLOAD")
		{
			Show::write("\n\n  Processing IISDOWNLOAD request");
			iisdownloadServer(msg.bodyString(), socket);
		}
		if (msg.attributes()[0].first == "PUBLISH")
		{
			Show::write("\n\n  Processing PUBLISH request");
			publishServer();
		}
		if (msg.attributes()[0].first == "DELETEFILE")
		{
			Show::write("\n\n  Processing DELETEFILE request");
			deleteFile(msg.bodyString());
		}
		msgQ_.enQ(msg);
	}
}

//----< processes LAZYDOWNLOAD request from client >---------
void MsgServer::downloadServer(Socket& socket)
{		
	// Download Header Files
	string rpath = "../Repository_Code_Publish/HeaderFiles/";
	vector<string> htmlfiles = FileSystem::Directory::getFiles(rpath, "*.htm");

	ofstream fp_out;
	ifstream ifs;

	for (string filename : htmlfiles)
	{
		ifs.open(rpath + filename);
		string rfilename;
		rfilename = "../Repository_Client/HeaderFiles/" + filename;
		fp_out.open(rfilename);
		fp_out << ifs.rdbuf() << endl;
		ifs.close();
		fp_out.close();
	}

	// Download Cpp Files
	string rpath2 = "../Repository_Code_Publish/CppFiles/";
	vector<string> htmlfiles2 = FileSystem::Directory::getFiles(rpath2, "*.htm");

	ofstream fp_out2;
	ifstream ifs2;

	for (string filename2 : htmlfiles2)
	{
		ifs2.open(rpath2 + filename2);
		string rfilename2;
		rfilename2 = "../Repository_Client/CppFiles/" + filename2;
		fp_out2.open(rfilename2);
		fp_out2 << ifs2.rdbuf() << endl;
		ifs2.close();
		fp_out2.close();
	}
}

//----< processes IISDOWNLOAD request from client >---------
void MsgServer::iisdownloadServer(const std::string& path, Socket& socket)
{	
	// Create HeaderFiles directory
	if (!FileSystem::Directory::exists(path + "HeaderFiles\\"))
		FileSystem::Directory::create(path + "HeaderFiles\\");
	// Create CppFiles directory
	if (!FileSystem::Directory::exists(path + "CppFiles\\"))
		FileSystem::Directory::create(path + "CppFiles\\");
	// Download Header Files
	string rpath = "../Repository_Code_Publish/HeaderFiles/";
	vector<string> htmlfiles = FileSystem::Directory::getFiles(rpath, "*.htm");

	ofstream fp_out;
	ifstream ifs;

	for (string filename : htmlfiles)
	{
		ifs.open(rpath + filename);
		string rfilename;
		rfilename = path + "HeaderFiles\\" + filename;
		fp_out.open(rfilename);
		fp_out << ifs.rdbuf() << endl;
		ifs.close();
		fp_out.close();
	}
	// Download Cpp Files
	string rpath2 = "../Repository_Code_Publish/CppFiles/";
	vector<string> htmlfiles2 = FileSystem::Directory::getFiles(rpath2, "*.htm");
	ofstream fp_out2;
	ifstream ifs2;
	for (string filename2 : htmlfiles2)
	{
		ifs2.open(rpath2 + filename2);
		string rfilename2;
		rfilename2 = path + "CppFiles\\" + filename2;
		fp_out2.open(rfilename2);
		fp_out2 << ifs2.rdbuf() << endl;
		ifs2.close();
		fp_out2.close();
	}
}

//----< processes PUBLISH request from client >---------
void MsgServer::publishServer()
{	
	string rpath = "../Repository_Server/";

	set<string> cpphfilesinServerRepo = storecpphFilesinRepo();

	DepAnal da;

	for (string str : cpphfilesinServerRepo)
	{
		da.processDependency(cpphfilesinServerRepo, rpath, str);
	}

	unordered_map<string, set<string>> depmap = da.getdepmap();

	CodePublisher cp;
	cp.codepublish(cpphfilesinServerRepo, depmap);
	cout << "\n  Code Published !!!!";

}

//----< processes DELETEFILE request from client >---------
void MsgServer::deleteFile(const string& filename)
{	
	int found = int(filename.rfind("/"));
	string rfilename = filename.substr(found + 1, filename.length());

	// Removing from Published Server Repository
	if (rfilename[int(rfilename.rfind(".")) - 1] == 'h') 
	{	// Header file
		string rpath = "../Repository_Code_Publish/HeaderFiles/";
		string dfilename = rpath + rfilename;
		remove(dfilename.c_str());
	}
	else  
	{	// Cpp file
		string rpath2 = "../Repository_Code_Publish/CppFiles/";
		string dfilename2 = rpath2 + rfilename;
		remove(dfilename2.c_str());
	}

	// Removing from UnPublished Server Repository
	int found3 = int(rfilename.rfind("."));
	string rfilename3 = rfilename.substr(0, found3);
	string rpath3 = "../Repository_Server/";
	string dfilename3 = rpath3 + rfilename3;
	remove(dfilename3.c_str());

}

//----< stores all .h and .cpp files in Server Repository in a set >---------
set<string> MsgServer::storecpphFilesinRepo()
{	
	set<string> cpphfilesinServerRepo;

	string rpath = "../Repository_Server/";
	vector<string> headerfiles = FileSystem::Directory::getFiles(rpath, "*.h");
	for (size_t i = 0; i < headerfiles.size(); ++i)
	{
		cpphfilesinServerRepo.insert(headerfiles[i]);
	}

	vector<string> cppfiles = FileSystem::Directory::getFiles(rpath, "*.cpp");
	for (size_t i = 0; i < cppfiles.size(); ++i)
	{
		cpphfilesinServerRepo.insert(cppfiles[i]);
	}
	return cpphfilesinServerRepo;
}

//----< reads messages from client >---------
HttpMessage MsgServer::readMessage(Socket& socket)
{
  connectionClosed_ = false;
  HttpMessage msg;
  while (true){
    string attribString = socket.recvString('\n');
    if (attribString.size() > 1){ 
	   HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
       msg.addAttribute(attrib);     
	}
    else{      	break;   	}
  }
  if (msg.attributes().size() == 0){		
	  connectionClosed_ = true;    return msg;  
  }
  string filename = msg.findValue("file");
  if (filename != ""){
	  size_t contentSize;
	  string sizeString = msg.findValue("content-length");
	  if (sizeString != "")
		  contentSize = Converter<size_t>::toValue(sizeString);
	  else
		  return msg;
	  readFile(filename, contentSize, socket);
  }
  if (filename != ""){
	  // construct message body
	  msg.removeAttribute("content-length");
	  string bodyString = "<file>" + filename + "</file>";
	  string sizeString = Converter<size_t>::toString(bodyString.size());
	  msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	  msg.addBody(bodyString);
  }
  else{
	  // read message body
	  size_t numBytes = 0;
	  size_t pos = msg.findAttribute("content-length");
	  if (pos < msg.attributes().size()){
		  numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
		  Socket::byte* buffer = new Socket::byte[numBytes + 1];
		  socket.recv(numBytes, buffer);
		  buffer[numBytes] = '\0';
		  string msgBody(buffer);
		  msg.addBody(msgBody);
		  delete[] buffer;
	  }
  }
  return msg;
}

//----< reads files from socket and saves files in its own Repository >--------------------
/*
 * This function expects the sender to have already send a file message, 
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */
bool MsgServer::readFile(const string& filename, size_t fileSize, Socket& socket)
{
  string fqname = "../Repository_Server/" + filename;
  FileSystem::File file(fqname);
  file.open(FileSystem::File::out, FileSystem::File::binary);
  if (!file.isGood())
  {
    /*
     * This error handling is incomplete.  The client will continue
     * to send bytes, but if the file can't be opened, then the server
     * doesn't gracefully collect and dump them as it should.  That's
     * an exercise left for students.
     */
    Show::write("\n\n  can't open file " + fqname);
    return false;
  }

  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];

  size_t bytesToRead;
  while (true)
  {
    if (fileSize > BlockSize)
      bytesToRead = BlockSize;
    else
      bytesToRead = fileSize;

    socket.recv(bytesToRead, buffer);

    FileSystem::Block blk;
    for (size_t i = 0; i < bytesToRead; ++i)
      blk.push_back(buffer[i]);

    file.putBlock(blk);
    if (fileSize < BlockSize)
      break;
    fileSize -= BlockSize;
  }
  file.close();
  return true;
}

//----< test stub >--------------------------------------------------

#ifdef TEST_MSGSERVER

int main()
{
  ::SetConsoleTitle(L"HttpMessage Server - Runs Forever");

  Show::attach(&cout);
  Show::start();
  Show::title("\n  HttpMessage Server started");

  BlockingQueue<HttpMessage> msgQ;

  try
  {
    SocketSystem ss;
    SocketListener sl(8080, Socket::IP6);
    MsgServer cp(msgQ);
    sl.start(cp);
    /*
     * Since this is a server the loop below never terminates.
     * We could easily change that by sending a distinguished 
     * message for shutdown.
     */
    while (true)
    {
      HttpMessage msg = msgQ.deQ();
    }
  }
  catch (exception& exc)
  {
    Show::write("\n  Exeception caught: ");
    string exMsg = "\n  " + string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}

#endif