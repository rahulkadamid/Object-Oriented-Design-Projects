////////////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Native Client to Support GUI Client functionality		  //			
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

#include "MsgClient.h"
#include <ShellAPI.h>
#include <cstdio>

using namespace Logging;
using namespace Utilities;
using namespace std;

using Show = StaticLogger<1>;
using Utils = StringHelper;

//----<  Sends UPLOAD request to server  >--------------------
void MsgClient::upload(const string& path)
{
	Show::attach(&cout);
	Show::start();
	string rpath = path;
	replace(rpath.begin(), rpath.end(), '\\', '/');
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
		//  send all *.h files from Repository_Client folder
		vector<string> headerfiles = FileSystem::Directory::getFiles(rpath, "*.h");
		for (size_t i = 0; i < headerfiles.size(); ++i)
			sendFile(rpath, headerfiles[i], si);
		
		//  send all *.cpp files from Repository_Client folder
		vector<string> cppfiles = FileSystem::Directory::getFiles(rpath, "*.cpp");
		for (size_t i = 0; i < cppfiles.size(); ++i)
			sendFile(rpath, cppfiles[i], si);

		// shut down server's client handler
		HttpMessage msg;
		msg = makeMessage(1, "quit", "localhost:8080");
		sendMessage(msg, si);
	}
	catch (exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		string exMsg = "\n  " + string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//----<  Sends LAZYDOWNLOAD request to server  >--------------------
void MsgClient::download()
{
	Show::attach(&cout);
	Show::start();
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
		HttpMessage msg;
		// send lazydownload message to server
		msg = makeMessage(2, "", "localhost:8080");
		sendMessage(msg, si);

		// shut down server's client handler
		msg = makeMessage(1, "quit", "localhost:8080");
		sendMessage(msg, si);
	}
	catch (exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		string exMsg = "\n  " + string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//----<  Sends IISDOWNLOAD request to server  >--------------------
void MsgClient::iisdownload(const string& path)
{
	Show::attach(&cout);
	Show::start();
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
		HttpMessage msg;
		// send IIS lazydownload message to server
		msg = makeMessage(5, path, "localhost:8080");
		sendMessage(msg, si);

		// shut down server's client handler
		msg = makeMessage(1, "quit", "localhost:8080");
		sendMessage(msg, si);
	}
	catch (exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		string exMsg = "\n  " + string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//----<  Sends PUBLISH request to server  >--------------------
void MsgClient::publish()
{
	Show::attach(&cout);
	Show::start();
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
		HttpMessage msg;
		// send publish message to server
		msg = makeMessage(3, "", "localhost:8080");
		sendMessage(msg, si);
		// shut down server's client handler
		msg = makeMessage(1, "quit", "localhost:8080");
		sendMessage(msg, si);
	}
	catch (exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		string exMsg = "\n  " + string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//----<  Opens give file in Browser  >--------------------
void MsgClient::openBrowser(const string& filename)
{	
	string astr = "\\\\";
	string str = filename;
	for (int i = 0; i < int(str.length()); i++)
	{
		if (str[i] == '/')
		{
			str.insert(i + 1, " ");
			str.replace(i, 2, astr);
		}
	}
	wstring stemp = wstring(str.begin(), str.end());
	LPCWSTR sw = stemp.c_str();
	ShellExecute(NULL, L"open", sw, NULL, NULL, SW_SHOWDEFAULT);
}

//----<  Sends DELETEFILE request to server >--------------------
void MsgClient::deleteFile(const string& filename)
{
	try
	{
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
		HttpMessage msg;
		// Make and Send "DELETEFILE" HttpMessage
		msg = makeMessage(4, filename, "localhost:8080");
		sendMessage(msg, si);		
		// shut down server's client handler
		msg = makeMessage(1, "quit", "localhost:8080");
		sendMessage(msg, si);
		// Deleting from Client's Repository 
		remove(filename.c_str());
	}
	catch (exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		string exMsg = "\n  " + string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 * - To do that the first argument is 1, e.g., index for the type of message to create.
 * - The body may be an empty string.
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */
HttpMessage MsgClient::makeMessage(size_t n, const string& body, const EndPoint& ep)
{
  HttpMessage msg;
  HttpMessage::Attribute attrib;
  EndPoint myEndPoint = "localhost:8081";  
   switch (n)
  {
  case 1:
    msg.clear();
    msg.addAttribute(HttpMessage::attribute("UPLOAD", "Message"));
    break;
  case 2:
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("LAZYDOWNLOAD", "Message"));
	  break;
  case 3:
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("PUBLISH", "Message"));
	  break;
  case 4:
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("DELETEFILE", "Message"));
	  break;
  case 5:
	  msg.clear();
	  msg.addAttribute(HttpMessage::attribute("IISDOWNLOAD", "Message"));
	  break;
  default:
    msg.clear();
    msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
  }
   msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
   msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
   msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

   msg.addBody(body);
   if (body.size() > 0)
   {
	   attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
	   msg.addAttribute(attrib);
   }
  return msg;
}

//----< send message using socket >----------------------------------
void MsgClient::sendMessage(HttpMessage& msg, Socket& socket)
{
  string msgString = msg.toString();
  socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}

//----< send file using socket >-------------------------------------
/*
* - Sends a message to tell receiver a file is coming.
* - Then sends a stream of bytes until the entire file
*   has been sent.
* - Sends in binary mode which works for either text or binary.
*/
bool MsgClient::sendFile(const string& path, const string& filename, Socket& socket)
{
	string fqname = path + "/" + filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
	{
		cout << "File not good" << endl;
		return false;
	}

	HttpMessage msg = makeMessage(1, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg, socket);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}

//----< entry point - runs two clients each on its own thread >------

#ifdef TEST_MSGCLIENT

int main()
{
	  ::SetConsoleTitle(L"Clients Running on Threads");

	  Show::title("Testing MsgClient Class");

	  MsgClient c1;
	  thread t1(
		[&]() 
	  {   
		  c1.upload("../RemoteCodePublisher/CodePublisher/");
		  c1.publish();
	  }  
	  );

	  t1.join();
 }
#endif