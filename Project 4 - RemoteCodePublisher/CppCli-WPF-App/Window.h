#ifndef WINDOW_H
#define WINDOW_H
////////////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application i.e Client GUI		  //			
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
*  This package is the Client GUI build using a C++\CLI WPF application.  It 
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with two views:
*  Execution View and File List View. It loads the DLL holding MockChannel.  
*  MockChannel hosts a send queue, a receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*  HttpMessage.h , HttpMessage.cpp , Utilities.h , Utilities.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 2 May 2017
*  - first release
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>
#include <string>

/////////////////////////////////////////////////////////////////////
// Settings struct:
//   - holds settings that will be retrieved at startup 
//     and persisted at shutdown

using UserSettings = ref struct Settings {
	String^ path;                  // analysis path
};

namespace CppCliWindows
{
  ref class WPFCppCliDemo : Window
  {
    // MockChannel references
    ISendr* pSendr_;
    IRecvr* pRecvr_;
    IMockChannel* pChann_;

    // Controls for Window
    DockPanel^ hDockPanel = gcnew DockPanel();				// support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
    TabItem^ hSendMessageTab = gcnew TabItem();
    TabItem^ hFileListTab = gcnew TabItem();
    TabItem^ hConnectTab = gcnew TabItem();
    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();
	TabItem^ hExecutionTab = gcnew TabItem();

	// Controls for Execution View
	Grid^ hExecutionGrid = gcnew Grid();
	Button^ hExecuteViewBrowseButton_ = gcnew Button();		// Browse Button
	Button^ hUploadButton = gcnew Button();					// Upload Button
	Button^ hExecuteViewPublishButton_ = gcnew Button();	// Publish Button
	Button^ hLAZYDownloadButton_ = gcnew Button();			// LAZY Download Button
	Button^ hIISDownloadButton_ = gcnew Button();			// IIS Download Button
	TextBox^ hExecuteViewTextBox_ = gcnew TextBox();
	Forms::OpenFileDialog^ hOpenFileDialog = gcnew Forms::OpenFileDialog();
	StackPanel^ hStackPanel2 = gcnew StackPanel();
	StackPanel^ hStackPanel3 = gcnew StackPanel();

    // Controls for FileListView View
    Grid^ hFileListGrid = gcnew Grid();
    Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
    ListBox^ hListBox = gcnew ListBox();
	Button^ hLazyBrowseButton = gcnew Button();			// Lazy Browse Button
	Button^ hLazyGetAllFilesButton = gcnew Button();	// Lazy Get All Files Button
	Button^ hIISServerBrowseButton = gcnew Button();	// IIS Server Browse Button
	Button^ hIISGetAllFilesButton = gcnew Button();		// IIS Get All Files Button
	Button^ hOpenBrowserButton = gcnew Button();		// Open in Browser Button
	Button^ hDeleteFileButton = gcnew Button();			// Delete File Button
	Button^ hReDownloadButton = gcnew Button();			// ReDownload Button

  public:
	WPFCppCliDemo(array<System::String^>^ args);
	~WPFCppCliDemo();
	//Store and Display user Settings
	void getUserSettings();
	void saveUserSettings();
	//Setup Status Bar and Tab Control bar
    void setUpStatusBar();
    void setUpTabControl();
	//Create Execution View and File List View
	void createExecutionView();
    void setUpFileListView();
	void setUpFileListView2();
	//set TextBlock and Button Properties of entire GUI
	void setTextBlockPropertiesEXT();
	void setButtonsPropertiesEXT();
	void setButtonsPropertiesEXT2();
	//makes HttpMessage style messages
	HttpMessage makeMessage(size_t n, const std::string& msgBody);  
	// Execution Tab buttons functions
	void browseForFolder(Object^ sender, RoutedEventArgs^ args);
	void upload(Object^ sender, RoutedEventArgs^ args);
	void publish(Object^ sender, RoutedEventArgs^ args);
	void lazydownload(Object^ sender, RoutedEventArgs^ args);
	void iisdownload(Object^ sender, RoutedEventArgs^ args);
	// File List Tab buttons functions
	void lazyBrowse(Object^ sender, RoutedEventArgs^ args);
	void lazygetAllFiles(Object^ sender, RoutedEventArgs^ args);
	void iisBrowse(Object^ sender, RoutedEventArgs^ args);
	void iisgetAllFiles(Object^ sender, RoutedEventArgs^ args);
	void openBrowser(Object^ sender, RoutedEventArgs^ args);
	void deleteFile(Object^ sender, RoutedEventArgs^ args);
	void redownload(Object^ sender, RoutedEventArgs^ args);
	// Loading GUI, Unloading GUI and Updating Status Bar functions
	void updateStatusbar(String ^ args);
    void OnLoaded(Object^ sender, RoutedEventArgs^ args);
    void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
  private:
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
	bool ParseCommandLineArgs(array<System::String^>^ args);
	// server and client related details.
	String^ clientdetails;
	String^ serverdetails;
	String^ serverAddress;
	String^ serverPort;
	String^ clientAddress;
	String^ clientPort;
	String^ virtual_path;
	String^ path_;
	String^ cmdLine_;
	UserSettings userSettings_;
  };
}


#endif
