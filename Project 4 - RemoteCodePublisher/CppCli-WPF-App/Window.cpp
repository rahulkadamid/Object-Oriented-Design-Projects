////////////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application i.e Client GUI	  //			
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
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Utilities/Utilities.h"
#include <string>

using namespace CppCliWindows;
using namespace System::Windows;
using namespace System::Windows::Data;
using namespace System::Globalization;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Diagnostics;

//----< Constructs GUI  >--------------------
WPFCppCliDemo::WPFCppCliDemo(array<System::String^>^ args)
{
  if (!ParseCommandLineArgs(args))
		return;
  // set up channel
  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr_ = pObjFact->createSendr();
  pRecvr_ = pObjFact->createRecvr();
  pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
  pChann_->start(toStdString(virtual_path));
  delete pObjFact;
  // set event handlers
  this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing += gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  // Execution Tab event handlers
  hExecuteViewBrowseButton_->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
  hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::upload);
  hExecuteViewPublishButton_->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::publish);
  hLAZYDownloadButton_->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::lazydownload);
  hIISDownloadButton_->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::iisdownload);
  // File List Tab event handlers
  hLazyBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::lazyBrowse);
  hLazyGetAllFilesButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::lazygetAllFiles);
  hIISServerBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::iisBrowse);
  hIISGetAllFilesButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::iisgetAllFiles);
  hOpenBrowserButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::openBrowser);
  hDeleteFileButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::deleteFile);
  hReDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::redownload);
  // set Window properties
  this->Title = "Client GUI";
  this->Width = 620;
  this->Height = 620;
  getUserSettings();
  // attach dock panel to Window
  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);
  // setup Window controls and views
  setUpTabControl();
  setUpStatusBar();
  createExecutionView();
  setUpFileListView();
  setUpFileListView2();
  hStatus->Text = "Browse to find analysis path or enter in textbox";
}

//----< Destroys GUI handles with MockChannel  >--------------------
WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
  delete pSendr_;
  delete pRecvr_;
}

//----< Parses Command Line args  >--------------------
bool WPFCppCliDemo::ParseCommandLineArgs(array<System::String^>^ args)
{
	if (args->Length == 3)
	{
		clientdetails = args[0];
		serverdetails = args[1];
		virtual_path = args[2];

		array<System::String^>^ server = serverdetails->Split(':');
		array<System::String^>^ client = clientdetails->Split(':');
		if (server->Length == 2)
		{
			serverAddress = server[0];
			serverPort = server[1];
		}
		if (client->Length == 2)
		{
			clientAddress = client[0];
			clientPort = client[1];
		}
		Console::WriteLine(L"\n Command Line args: Clientdetails:{0} Serverdetails:{1} Virtual Path:{2}\n", clientdetails , serverdetails, virtual_path);
		return true;
	}
	else
	{
		Console::WriteLine(L"\n Please provide proper command line arguments. For more details check README.docx\n");
		return false;
	}
}

//----< Makes HttpMessage style messages  >--------------------
HttpMessage WPFCppCliDemo::makeMessage(size_t n, const std::string& body)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("UPLOAD", "Message"));
		msg.addBody(body);
		break;
	case 2:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("PUBLISH", "Message"));
		msg.addBody(body);
		break; 
	case 3:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("LAZYDOWNLOAD", "Message"));
		msg.addBody(body);
		break;
	case 4:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("IISDOWNLOAD", "Message"));
		msg.addBody(body);
		break;
	case 5:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("OPENBROWSER", "Message"));
		msg.addBody(body);
		break;
	case 6:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("DELETEFILE", "Message"));
		msg.addBody(body);
		break;
	default:
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
		break;
	}
	return msg;
}

//----< after clicking the upload button  >--------------------
void WPFCppCliDemo::upload(Object^ sender, RoutedEventArgs^ args)
{	
	HttpMessage msg = makeMessage(1, toStdString(path_));
	pSendr_->postMessage(msg);
	String^ consoleMsg = "Upload to Server: " + "\"" + path_ + "\"";
	Console::WriteLine("\n  {0}", consoleMsg);
	String^ statusMsg = "Uploaded to Server!!!";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< after clicking the publish button  >--------------------
void WPFCppCliDemo::publish(Object^ sender, RoutedEventArgs^ args)
{
	HttpMessage msg = makeMessage(2,"");
	pSendr_->postMessage(msg);
	String^ consoleMsg = "Sending Publish request to Server";
	Console::WriteLine("\n  {0}", consoleMsg);
	String^ statusMsg = "Published to Server!!!";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< after clicking the lazydownload button  >--------------------
void WPFCppCliDemo::lazydownload(Object^ sender, RoutedEventArgs^ args)
{
	HttpMessage msg = makeMessage(3, "");
	pSendr_->postMessage(msg);	
	String^ consoleMsg = "Sending Lazy Download request to Server";
	Console::WriteLine("\n  {0}", consoleMsg);
	String^ statusMsg = "Downloaded to Client Repository!!!";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< after clicking the iisdownload button  >--------------------
void WPFCppCliDemo::iisdownload(Object^ sender, RoutedEventArgs^ args)
{
	HttpMessage msg = makeMessage(4, toStdString(virtual_path));
	pSendr_->postMessage(msg);
	String^ consoleMsg = "Sending IIS Download request to Server";
	Console::WriteLine("\n  {0}", consoleMsg);
	String^ statusMsg = "Downloaded to IIS Server Repository!!!";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< after clicking the lazyBrowse button  >--------------------
void WPFCppCliDemo::lazyBrowse(Object^ sender, RoutedEventArgs^ args)
{
	hListBox->Items->Clear();
	String^ path = "../Repository_Client/";
	std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
	array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
	array<String^>^ cppfiles = System::IO::Directory::GetFiles(path + "CppFiles/", L"*.htm");
	array<String^>^ hfiles = System::IO::Directory::GetFiles(path + "HeaderFiles/", L"*.htm");
	for (int i = 0; i < dirs->Length; ++i)
	{
		hListBox->Items->Add(L"<> " + dirs[i]);
		if ( i == 0)
		{ 
			for (int j = 0; j < cppfiles->Length; ++j)
				hListBox->Items->Add(cppfiles[j]);
		}
		else
		{
			for (int j = 0; j < hfiles->Length; ++j)
				hListBox->Items->Add(hfiles[j]);
		}
	}
	String^ statusMsg = "Showing all Categories with Corresponding available files in those categories";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< after clicking the lazygetAllFiles button  >--------------------
void WPFCppCliDemo::lazygetAllFiles(Object^ sender, RoutedEventArgs^ args)
{
	hListBox->Items->Clear();

	String^ path = "../Repository_Client/HeaderFiles/";
	std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
	array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.htm");
	for (int i = 0; i < files->Length; ++i)
	hListBox->Items->Add(files[i]);

	String^ path2 = "../Repository_Client/CppFiles/";
	std::cout << "\n  opening folder \"" << toStdString(path2) << "\"";
	array<String^>^ files2 = System::IO::Directory::GetFiles(path2, L"*.htm");
	for (int i = 0; i < files2->Length; ++i)
		hListBox->Items->Add(files2[i]);

	array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
	for (int i = 0; i < dirs->Length; ++i)
	hListBox->Items->Add(L"<dir> " + dirs[i]);
	String^ statusMsg = "Getting all files in Server Repository irrespective of Categories";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< after clicking the iisBrowse button  >--------------------
void WPFCppCliDemo::iisBrowse(Object^ sender, RoutedEventArgs^ args)
{
	hListBox->Items->Clear();
	std::cout << "\n  Opening folder \"" << toStdString(virtual_path) << "\"";
	array<String^>^ dirs = System::IO::Directory::GetDirectories(virtual_path);
	array<String^>^ cppfiles = System::IO::Directory::GetFiles(virtual_path + "CppFiles\\", L"*.htm");
	array<String^>^ hfiles = System::IO::Directory::GetFiles(virtual_path + "HeaderFiles\\", L"*.htm");
	for (int i = 0; i < dirs->Length; ++i)
	{
		hListBox->Items->Add(L"<> " + dirs[i]);
		if (i == 0)
		{
			for (int j = 0; j < cppfiles->Length; ++j)
				hListBox->Items->Add(cppfiles[j]);
		}
		else
		{
			for (int j = 0; j < hfiles->Length; ++j)
				hListBox->Items->Add(hfiles[j]);
		}
	}
	String^ statusMsg = "Browsing IIS Server";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< after clicking the iisgetAllFiles button  >--------------------
void WPFCppCliDemo::iisgetAllFiles(Object^ sender, RoutedEventArgs^ args)
{
	hListBox->Items->Clear();

	String^ path = virtual_path + "HeaderFiles\\";
	std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
	array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.htm");
	for (int i = 0; i < files->Length; ++i)
		hListBox->Items->Add(files[i]);

	String^ path2 = virtual_path + "CppFiles\\";
	std::cout << "\n  opening folder \"" << toStdString(path2) << "\"";
	array<String^>^ files2 = System::IO::Directory::GetFiles(path2, L"*.htm");
	for (int i = 0; i < files2->Length; ++i)
		hListBox->Items->Add(files2[i]);

	array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
	for (int i = 0; i < dirs->Length; ++i)
		hListBox->Items->Add(L"<dir> " + dirs[i]);
	String^ statusMsg = "Getting all files from IIS Server irrespective of Categories";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< after clicking the openBrowser button  >--------------------
void WPFCppCliDemo::openBrowser(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
	}
	if (count > 0)
	{
		HttpMessage msg;
		for each (String^ item in items)
		{
			msg = makeMessage(5, toStdString(item));
			pSendr_->postMessage(msg);
			String^ consoleMsg = "Opening in Browser: ";
			Console::WriteLine("\n  {0} {1}", consoleMsg, item);
			hStatus->Text = "opening htm file in Browser...";
		}
	}
}

//----< after clicking the deleteFile button  >--------------------
void WPFCppCliDemo::deleteFile(Object^ sender, RoutedEventArgs^ args)
{
	int index = 0;
	int count = hListBox->SelectedItems->Count;
	hStatus->Text = "Show Selected Items";
	array<System::String^>^ items = gcnew array<String^>(count);
	if (count > 0) {
		for each (String^ item in hListBox->SelectedItems)
		{
			items[index++] = item;
		}
	}
	if (count > 0)
	{
		HttpMessage msg;
		String^ consoleMsg = "";
		for each (String^ item in items)
		{
			msg = makeMessage(6, toStdString(item));			// DELETEFILE msg
			pSendr_->postMessage(msg);
			hListBox->Items->Remove(item);
			consoleMsg = "Deleting File: ";
			Console::WriteLine("\n  {0} {1}", consoleMsg, item);
		}
		msg = makeMessage(2, "");								// PUBLISH msg
		pSendr_->postMessage(msg);
		consoleMsg = "Sending Publish request to Server";
		Console::WriteLine("\n  {0}", consoleMsg);
	}
}

//----< after clicking the redownload button  >--------------------
void WPFCppCliDemo::redownload(Object^ sender, RoutedEventArgs^ args)
{
	HttpMessage msg = makeMessage(3, "");
	pSendr_->postMessage(msg);
	String^ consoleMsg = "Sending Lazy Download request to Server";
	Console::WriteLine("\n  {0}", consoleMsg);

	msg = makeMessage(4, toStdString(virtual_path));
	pSendr_->postMessage(msg);
	consoleMsg = "Sending IIS Download request to Server";
	Console::WriteLine("\n  {0}", consoleMsg);

	String^ statusMsg = "ReDownloaded to Client Repository and IIS Server Repository!!!";
	array<Object^>^ parms = gcnew array<Object^> {statusMsg };
	Action< String^>^ act1 = gcnew Action<String^>(this, &WPFCppCliDemo::updateStatusbar);
	Dispatcher->Invoke(act1, parms);
}

//----< Updates Status bar  >--------------------
void WPFCppCliDemo::updateStatusbar(String ^ args)
{
	hStatus->Text = args;
}

//----< set up for Status bar in GUI  >--------------------
void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  //status->FontWeight = FontWeights::Bold;
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//----< set up for Tab Control in GUI  >--------------------
void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hExecutionTab->Header = "Execution";
  hFileListTab->Header = "File List";
  hTabControl->Items->Add(hExecutionTab);
  hTabControl->Items->Add(hFileListTab);
}

//----< set up for Text Blocks in GUI  >--------------------
void WPFCppCliDemo::setTextBlockPropertiesEXT()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hExecutionGrid->RowDefinitions->Add(hRow1Def);
	hExecutionGrid->Margin = Thickness(40);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Height = 25;
	hBorder1->Child = hExecuteViewTextBox_;
	hExecuteViewTextBox_->Padding = Thickness(2);
	hExecuteViewTextBox_->Text = "";
	hExecuteViewTextBox_->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hExecuteViewTextBox_->FontWeight = FontWeights::Normal;
	hExecuteViewTextBox_->FontSize = 14;
	hExecutionGrid->SetRow(hBorder1, 0);
	hExecutionGrid->Children->Add(hBorder1);
}

//----< set up for Buttons in GUI  >--------------------
void WPFCppCliDemo::setButtonsPropertiesEXT()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75); 
	hExecutionGrid->RowDefinitions->Add(hRow2Def);
	hExecuteViewBrowseButton_->Content = "Browse";
	hExecuteViewBrowseButton_->Width = 68;
	hExecuteViewBrowseButton_->Height = 30;
	hUploadButton->Content = "Upload";
	hUploadButton->Width = 68;
	hUploadButton->Height = 30;
	hExecuteViewPublishButton_->Content = "Publish";
	hExecuteViewPublishButton_->Width = 68;
	hExecuteViewPublishButton_->Height = 30;
	hStackPanel2->Children->Add(hExecuteViewBrowseButton_);
	Label^ hHorizontalSpacer = gcnew Label();
	hHorizontalSpacer->Width = 30;	
	hStackPanel2->Children->Add(hHorizontalSpacer);
	hStackPanel2->Children->Add(hUploadButton);
	Label^ hHorizontalSpacer1 = gcnew Label();
	hHorizontalSpacer1->Width = 30;
	hStackPanel2->Children->Add(hHorizontalSpacer1);
	hStackPanel2->Children->Add(hExecuteViewPublishButton_);
	Label^ hHorizontalSpacer2 = gcnew Label();
	hHorizontalSpacer2->Width = 30;
	hStackPanel2->Children->Add(hHorizontalSpacer2);
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hExecutionGrid->SetRow(hStackPanel2, 1);
	hExecutionGrid->Children->Add(hStackPanel2);
}

//----< set up for Buttons in GUI  >--------------------
void WPFCppCliDemo::setButtonsPropertiesEXT2()
{
	RowDefinition^ hRow2Def2 = gcnew RowDefinition();
	hRow2Def2->Height = GridLength(75);
	hExecutionGrid->RowDefinitions->Add(hRow2Def2);
	hLAZYDownloadButton_->Content = "Lazy Download";
	hLAZYDownloadButton_->Width = 90;
	hLAZYDownloadButton_->Height = 30;
	hStackPanel3->Children->Add(hLAZYDownloadButton_);
	Label^ hHorizontalSpacer1 = gcnew Label();
	hHorizontalSpacer1->Width = 30;
	hStackPanel3->Children->Add(hHorizontalSpacer1);
	hIISDownloadButton_->Content = "IIS Download";
	hIISDownloadButton_->Width = 90;
	hIISDownloadButton_->Height = 30;
	hStackPanel3->Children->Add(hIISDownloadButton_);
	hStackPanel3->Orientation = Orientation::Horizontal;
	hStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hExecutionGrid->SetRow(hStackPanel3, 2);
	hExecutionGrid->Children->Add(hStackPanel3);
}

//----< set up for FileListView in GUI  >--------------------
void WPFCppCliDemo::setUpFileListView()
{
  Console::Write("\n  setting up FileList view");
  hFileListGrid->Margin = Thickness(20);
  hFileListTab->Content = hFileListGrid;
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  hRow1Def->Height = GridLength(200);
  hFileListGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hListBox->SelectionMode = SelectionMode::Multiple;
  hBorder1->Child = hListBox;
  hFileListGrid->SetRow(hBorder1, 0);
  hFileListGrid->Children->Add(hBorder1);

  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(40);
  RowDefinition^ hRow2Def2 = gcnew RowDefinition();
  hRow2Def2->Height = GridLength(40);
  RowDefinition^ hRow2Def3 = gcnew RowDefinition();
  hRow2Def3->Height = GridLength(40);
  RowDefinition^ hRow2Def4 = gcnew RowDefinition();
  hRow2Def4->Height = GridLength(40);
  RowDefinition^ hRow2Def5 = gcnew RowDefinition();
  hRow2Def5->Height = GridLength(40);
  RowDefinition^ hRow2Def6 = gcnew RowDefinition();
  hRow2Def6->Height = GridLength(40);
  RowDefinition^ hRow2Def7 = gcnew RowDefinition();
  hRow2Def7->Height = GridLength(40);

  hFileListGrid->RowDefinitions->Add(hRow2Def);
  hFileListGrid->RowDefinitions->Add(hRow2Def2);
  hFileListGrid->RowDefinitions->Add(hRow2Def3);
  hFileListGrid->RowDefinitions->Add(hRow2Def4);
  hFileListGrid->RowDefinitions->Add(hRow2Def5);
  hFileListGrid->RowDefinitions->Add(hRow2Def6);
  hFileListGrid->RowDefinitions->Add(hRow2Def7);

}

//----< set up for FileListView in GUI  >--------------------
void WPFCppCliDemo::setUpFileListView2()
{
	hLazyBrowseButton->Content = "Lazy Download Browse";
	hLazyBrowseButton->Height = 30;
	hLazyBrowseButton->Width = 150;
	hFileListGrid->SetRow(hLazyBrowseButton, 1);
	hFileListGrid->Children->Add(hLazyBrowseButton);

	hLazyGetAllFilesButton->Content = "Lazy Download Get all Files";
	hLazyGetAllFilesButton->Height = 30;
	hLazyGetAllFilesButton->Width = 150;
	hFileListGrid->SetRow(hLazyGetAllFilesButton, 2);
	hFileListGrid->Children->Add(hLazyGetAllFilesButton);

	hIISServerBrowseButton->Content = "IIS Server Browse";
	hIISServerBrowseButton->Height = 30;
	hIISServerBrowseButton->Width = 150;
	hFileListGrid->SetRow(hIISServerBrowseButton, 3);
	hFileListGrid->Children->Add(hIISServerBrowseButton);

	hIISGetAllFilesButton->Content = "IIS Server Get all Files";
	hIISGetAllFilesButton->Height = 30;
	hIISGetAllFilesButton->Width = 150;
	hFileListGrid->SetRow(hIISGetAllFilesButton, 4);
	hFileListGrid->Children->Add(hIISGetAllFilesButton);

	hOpenBrowserButton->Content = "Open in Browser";
	hOpenBrowserButton->Height = 30;
	hOpenBrowserButton->Width = 150;
	hFileListGrid->SetRow(hOpenBrowserButton, 5);
	hFileListGrid->Children->Add(hOpenBrowserButton);

	hDeleteFileButton->Content = "Delete File";
	hDeleteFileButton->Height = 30;
	hDeleteFileButton->Width = 150;
	hFileListGrid->SetRow(hDeleteFileButton, 6);
	hFileListGrid->Children->Add(hDeleteFileButton);

	hReDownloadButton->Content = "ReDownload Lazy and IIS ";
	hReDownloadButton->Height = 30;
	hReDownloadButton->Width = 150;
	hFileListGrid->SetRow(hReDownloadButton, 7);
	hFileListGrid->Children->Add(hReDownloadButton);

	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//----< set up for ExecutionView in GUI  >--------------------
void WPFCppCliDemo::createExecutionView()
{	
	Console::Write("\n  setting up Execution view");
	Border^ hExecutionBorder = gcnew Border();
	hExecutionBorder->BorderThickness = Thickness(1);
	hExecutionBorder->Margin = Thickness(15);
	hExecutionBorder->BorderBrush = Brushes::LightGray;
	hExecutionBorder->Child = hExecutionGrid;
	hExecutionTab->Content = hExecutionBorder;

	setTextBlockPropertiesEXT();
	setButtonsPropertiesEXT();
	setButtonsPropertiesEXT2();

}

//----< retrieve user settings from last execution >-----------------
/*
*  Look for user settings file in directory where GUI was started.
*/
void WPFCppCliDemo::getUserSettings()
{
	String^ settingsPath = Path::GetFullPath(Assembly::GetExecutingAssembly()->Location);
	settingsPath = Path::GetDirectoryName(settingsPath);
	if (!System::IO::Directory::Exists(settingsPath))
	{
		throw gcnew Exception("invalid settings path");
	}
	settingsPath += "\\UserSettings.txt";

	try
	{
		System::IO::StreamReader^ sr = gcnew System::IO::StreamReader(settingsPath);

		userSettings_.path = sr->ReadLine();
		sr->Close();
	}
	catch (Exception^ /*e*/)
	{
		// use default values
		userSettings_.path = Environment::CurrentDirectory;
	}
	hExecuteViewTextBox_->Text = path_ = userSettings_.path;
}

//----< persist user settings to file >------------------------------
/*
*  Look for user settings file in directory where GUI was started.
*/
void WPFCppCliDemo::saveUserSettings()
{
	String^ settingsPath = Path::GetFullPath(Assembly::GetExecutingAssembly()->Location);
	settingsPath = Path::GetDirectoryName(settingsPath);
	if (!System::IO::Directory::Exists(settingsPath))
	{
		throw gcnew Exception("invalid settings path path");
	}
	settingsPath += "\\UserSettings.txt";

	try
	{
		System::IO::StreamWriter^ sr = gcnew System::IO::StreamWriter(settingsPath);

		sr->WriteLine(path_);
		sr->Close();
	}
	catch (Exception^ e)
	{
		Console::Write("\n\n{0}\n", e->Message);
	}
}

//----< converts .net string to c++ native string  >--------------------
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}

//----< converts c++ native string to .net string >--------------------
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

//----< event handler for Browse Button >----------------------------
/*
*  - Used in both Execution and Setup Views.
*  - Uses an OpenFileDialog so we can select either files or folders.
*  - Always clears file list on browse.
*/
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	String^ defaultFileName = "FolderSelection";
	System::Windows::Forms::DialogResult result;
	hOpenFileDialog->CheckPathExists = true;
	hOpenFileDialog->CheckFileExists = false;
	hOpenFileDialog->ShowReadOnly = false;
	hOpenFileDialog->ReadOnlyChecked = true;
	hOpenFileDialog->ValidateNames = false;
	hOpenFileDialog->Multiselect = true;
	hOpenFileDialog->FileName = defaultFileName;
	hOpenFileDialog->InitialDirectory = hExecuteViewTextBox_->Text;
	result = hOpenFileDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		array<String^>^ fileNames = hOpenFileDialog->FileNames;
		String^ path = System::IO::Path::GetDirectoryName(fileNames[0]);

		for (int i = 0; i < fileNames->Length; ++i)
		{
			String^ name = System::IO::Path::GetFileName(fileNames[i]);
		}
		hExecuteViewTextBox_->Text = path;
		path_ = path;
		hExecuteViewPublishButton_->IsEnabled = true;
	}
}

//----< process on loading the GUI >----------------------------
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
  hExecuteViewTextBox_->Text = path_;
}

//----< process on unloading the GUI >----------------------------
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
  saveUserSettings();
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
  Console::Title = "Client GUI Console";
  Console::WriteLine(L"\n Starting Client GUI");

  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo(args));
  Console::WriteLine(L"\n\n");
}