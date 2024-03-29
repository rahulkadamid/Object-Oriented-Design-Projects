/////////////////////////////////////////////////////////////////////////////
// TestExec.cpp - Testing the Type Based Dependency Analyzer Functionality //
// ver 1.4                                                                 //
// ----------------------------------------------------------------------- //
// copyright � Rahul Kadam, 2017                                           //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Hp, Core i5, Windows 10                                    //
// Application: OOD Project 2 - TypeBasedDepAnalyzer ,Spring 2017          //
// Source:      Jim Fawcett,  CST 4-187, 							       //
//              (315) 443-3948 , jfawcett@twcny.rr.com                     //
// Author:      Rahul Kadam, Syracuse University          		           //
//              (315) 751-8862, rkadam@syr.edu							   //
/////////////////////////////////////////////////////////////////////////////

#include "TestExec.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <chrono>
#include <ctime>

#include "../Parser/Parser.h"
#include "../FileSystem/FileSystem.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../AbstractSyntaxTree/AbstrSynTree.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../TypeAnalysis/TypeAnal.h"
#include "../TypeTable/TypeTable.h"
#include "../DependencyAnalysis/DepAnal.h"
#include "../FileSystem/FileSystem.h"
#include "../NoSqlDb/NoSqlDb.h"
#include "../NoSqlDb/Persistence.h"
#include "../StrongComponents/Graph.h"


using Rslt = Logging::StaticLogger<0>;  // use for application results
using Demo = Logging::StaticLogger<1>;  // use for demonstrations of processing
using Dbug = Logging::StaticLogger<2>;  // use for debug output

using namespace GraphLib;

typedef Graph<string, string> graph;
typedef Vertex<string, string> vertex;
typedef Display<string, string> display;

/////////////////////////////////////////////////////////////////////
// AnalFileMgr class
// - Derives from FileMgr to make application specific file handler
//   by overriding FileMgr::file(), FileMgr::dir(), and FileMgr::done()

using Path = string;
using File = string;
using Files = vector<File>;
using Pattern = string;
using Ext = string;
using FileMap = unordered_map<Pattern, Files>;

using namespace CodeAnalysis;
using namespace std;
//----< initialize application specific FileMgr >--------------------
/*
 * - Accepts CodeAnalysisExecutive's path and fileMap by reference
 */
AnalFileMgr::AnalFileMgr(const Path& path, FileMap& fileMap)
  : FileMgr(path), fileMap_(fileMap), numFiles_(0), numDirs_(0) {}

//----< override of FileMgr::file(...) to store found files >------

void AnalFileMgr::file(const File& f)
{
  File fqf = d_ + "\\" + f;
  Ext ext = FileSystem::Path::getExt(fqf);
  Pattern p = "*." + ext;
  fileMap_[p].push_back(fqf);
  ++numFiles_;
}
//----< override of FileMgr::dir(...) to save current dir >----------

void AnalFileMgr::dir(const Dir& d)
{
  d_ = d;
  ++numDirs_;
}
//----< override of FileMgr::done(), not currently used >------------

void AnalFileMgr::done()
{
}
//----< returns number of matched files from search >----------------

size_t AnalFileMgr::numFiles()
{
  return numFiles_;
}
//----< returns number of dirs searched >----------------------------

size_t AnalFileMgr::numDirs()
{
  return numDirs_;
}

/////////////////////////////////////////////////////////////////////
// CodeAnalysisExecutive class
// - 
using Path = string;
using Pattern = string;
using Patterns = vector<Pattern>;
using File = string;
using Files = vector<File>;
using Ext = string;
using FileMap = unordered_map<Pattern, Files>;
using ASTNodes = vector<ASTNode*>;
using FileToNodeCollection = vector<pair<File, ASTNode*>>;

//----< initialize parser, get access to repository >----------------

CodeAnalysisExecutive::CodeAnalysisExecutive()
{
  pParser_ = configure_.Build();
  if (pParser_ == nullptr)
  {
    throw exception("couldn't create parser");
  }
  pRepo_ = Repository::getInstance();
}
//----< cleanup >----------------------------------------------------

CodeAnalysisExecutive::~CodeAnalysisExecutive()
{
  /*
   *  Nothing to do:
   *  - pParser_ and pRepo_ point to objects that the configure_
   *    destructor will delete.
   *  - The AbstractSynTree object will cleanup its contents when
   *    it goes out of scope by deleting the root of the AST node
   *    tree.  
   *  - Each node deletes its children, so, again, nothing more 
   *    to do.
   *  - This is here to present these comments and to make this
   *    base destructor virtual.
   */
}
//----< usage message >----------------------------------------------

void showUsage()
{
  ostringstream out;
  out << "\n  Usage:";
  out << "\n  Command Line Arguments are:";
  out << "\n  - 1st: path to subdirectory containing files to analyze";
  out << "\n  - remaining non-option arguments are file patterns, e.g., *.h and/or *.cpp, etc.";
  out << "\n  - must have at least one file pattern to specify what to process";
  out << "\n  - option arguments have the format \"\"/x\" , where x is one of the options:";
  out << "\n    - m : display function metrics";
  out << "\n    - s : display file sizes";
  out << "\n    - a : display Abstract Syntax Tree";
  out << "\n    - r : set logger to display results";
  out << "\n    - d : set logger to display demo outputs";
  out << "\n    - b : set logger to display debug outputs";
  out << "\n    - f : write all logs to logfile.txt";
  out << "\n  A metrics summary is always shown, independent of any options used or not used";
  out << "\n\n";
  cout << out.str();
  //Rslt::write(out.str());
  //Rslt::flush();
}
//----< show command line arguments >--------------------------------

void CodeAnalysisExecutive::showCommandLineArguments(int argc, char* argv[])
{
  ostringstream out;
  out << "\n     Path: \"" << FileSystem::Path::getFullFileSpec(argv[1]) << "\"\n     Args: ";
  for (int i = 2; i < argc - 1; ++i)
    out << argv[i] << ", ";
  out << argv[argc - 1];
  Rslt::write(out.str());
  Rslt::flush();
}
//----< handle command line arguments >------------------------------
/*
* Arguments are:
* - path: possibly relative path to folder containing all analyzed code,
*   e.g., may be anywhere in the directory tree rooted at that path
* - patterns: one or more file patterns of the form *.h, *.cpp, and *.cs
* - options: /m (show metrics), /s (show file sizes), and /a (show AST)
*/
bool CodeAnalysisExecutive::ProcessCommandLine(int argc, char* argv[])
{
  if (argc < 2)
  {
    showUsage();
    return false;
  }
  try {
    path_ = FileSystem::Path::getFullFileSpec(argv[1]);
    if (!FileSystem::Directory::exists(path_))
    {
      cout << "\n\n  path \"" << path_ << "\" does not exist\n\n";
      return false;
    }
    for (int i = 2; i < argc; ++i)
    {
      if (argv[i][0] == '/')
        options_.push_back(argv[i][1]);
      else
        patterns_.push_back(argv[i]);
    }
    if (patterns_.size() == 0)
    {
      showUsage();
      return false;
    }
  }
  catch (exception& ex)
  {
    cout << "\n\n  command line argument parsing error:";
    cout << "\n  " << ex.what() << "\n\n";
    return false;
  }
  return true;
}
//----< returns path entered on command line >-------------------

string CodeAnalysisExecutive::getAnalysisPath()
{
  return path_;
}
//----< returns reference to FileMap >---------------------------
/*
 * Supports quickly finding all the files found with a give pattern
 */
FileMap& CodeAnalysisExecutive::getFileMap()
{
  return fileMap_;
}
//----< searches path for files matching specified patterns >----
/*
 * - Searches entire diretory tree rooted at path_, evaluated 
 *   from a command line argument.
 * - Saves found files in FileMap.
 */
void CodeAnalysisExecutive::getSourceFiles()
{
  AnalFileMgr fm(path_, fileMap_);
  for (auto patt : patterns_)
    fm.addPattern(patt);
  fm.search();
  numFiles_ = fm.numFiles();
  numDirs_ = fm.numDirs();
}
//----< helper: is text a substring of str? >--------------------

bool contains(const string& str, const string& text)
{
  if (str.find(text) < str.length())
    return true;
  return false;
}
//----< retrieve from fileMap all files matching *.h >-----------

vector<File>& CodeAnalysisExecutive::cppHeaderFiles()
{
  cppHeaderFiles_.clear();
  for (auto item : fileMap_)
  {
    if (contains(item.first, "*.h"))
    {
      for (auto file : item.second)
        cppHeaderFiles_.push_back(file);
    }
  }
  return cppHeaderFiles_;
}
//----< retrieve from fileMap all files matching *.cpp >---------

vector<File>& CodeAnalysisExecutive::cppImplemFiles()
{
  cppImplemFiles_.clear();
  for (auto item : fileMap_)
  {
    if (contains(item.first, "*.cpp"))
    {
      for (auto file : item.second)
        cppImplemFiles_.push_back(file);
    }
  }
  return cppImplemFiles_;
}
//----< retrieve from fileMap all files matching *.cs >----------

vector<File>& CodeAnalysisExecutive::csharpFiles()
{
  csharpFiles_.clear();
  for (auto item : fileMap_)
  {
    if (contains(item.first, "*.cs"))
    {
      for (auto file : item.second)
        csharpFiles_.push_back(file);
    }
  }
  return csharpFiles_;
}
//----< report number of Source Lines Of Code (SLOCs) >----------

CodeAnalysisExecutive::Slocs CodeAnalysisExecutive::fileSLOCs(const File& file) 
{ 
  return slocMap_[file];
}
//----< report number of files processed >-----------------------

size_t CodeAnalysisExecutive::numFiles()
{
  return numFiles_;
}
//----< report number of directories searched >------------------

size_t CodeAnalysisExecutive::numDirs()
{
  return numDirs_;
}
//----< show processing activity >-------------------------------

void CodeAnalysisExecutive::showActivity(const File& file)
{
  function<string(string, size_t)> trunc = [](string in, size_t count)
  {
    return in.substr(0, count);
  };

  if (Rslt::running())
  {
    cout << left << "\r     Processing file: " << setw(80) << trunc(file, 80);
  }
}

void CodeAnalysisExecutive::clearActivity()
{
  if (Rslt::running())
  {
    cout << left << "\r                      " << setw(80) << string(80,' ');
  }
}
//----< parses code and saves results in AbstrSynTree >--------------
/*
* - Processes C++ header files first to build AST with nodes for
*   all public classes and structs.
* - Then processes C++ implementation files.  Each member function
*   is relocated to its class scope node, not the local scope.
* - Therefore, this ordering is important.
* - C# code has all member functions inline, so we don't need to
*   do any relocation of nodes in the AST.  Therefore, that analysis
*   can be done at any time.
* - If you bore down into the analysis code in ActionsAndRules.h you
*   will find some gymnastics to handle template syntax.  That can
*   get somewhat complicated, so there may be some latent bugs there.
*   I don't know of any at this time.
*/
void CodeAnalysisExecutive::setLanguage(const File& file)
{
  string ext = FileSystem::Path::getExt(file);
  if (ext == "h" || ext == "cpp")
    pRepo_->language() = Language::Cpp;
  else if (ext == "cs")
    pRepo_->language() = Language::CSharp;
}

void CodeAnalysisExecutive::processSourceCode(bool showProc)
{
  for (auto file : cppHeaderFiles())
  {
    if (showProc)
      showActivity(file);
    pRepo_->package() = FileSystem::Path::getName(file);

    if (!configure_.Attach(file))
    {
      ostringstream out;
      out << "\n  could not open file " << file << "\n";
      Rslt::write(out.str());
      Rslt::flush();
      continue;
    }
    // parse file
    Rslt::flush();
    Demo::flush();
    Dbug::flush();

    if(!Rslt::running())
      Demo::write("\n\n  opening file \"" + pRepo_->package() + "\"");
    if(!Demo::running() && !Rslt::running())
      Dbug::write("\n\n  opening file \"" + pRepo_->package() + "\"");
    pRepo_->language() = Language::Cpp;
    pRepo_->currentPath() = file;
    while (pParser_->next())
    {
      pParser_->parse();
    }

    Slocs slocs = pRepo_->Toker()->currentLineCount();
    slocMap_[pRepo_->package()] = slocs;
  }
  for (auto file : cppImplemFiles())
  {
    if (showProc)
      showActivity(file);
    pRepo_->package() = FileSystem::Path::getName(file);

    if (!configure_.Attach(file))
    {
      ostringstream out;
      out << "\n  could not open file " << file << "\n";
      Rslt::write(out.str());
      Rslt::flush();
      continue;
    }
    // parse file

    if (!Rslt::running())
      Demo::write("\n\n  opening file \"" + pRepo_->package() + "\"");
    if (!Demo::running() && !Rslt::running())
      Dbug::write("\n\n  opening file \"" + pRepo_->package() + "\"");
    pRepo_->language() = Language::Cpp;
    pRepo_->currentPath() = file;
    while (pParser_->next())
      pParser_->parse();

    Slocs slocs = pRepo_->Toker()->currentLineCount();
    slocMap_[pRepo_->package()] = slocs;
  }
  for (auto file : csharpFiles())
  {
    if (showProc)
      showActivity(file);
    pRepo_->package() = FileSystem::Path::getName(file);

    if (!configure_.Attach(file))
    {
      ostringstream out;
      out << "\n  could not open file " << file << "\n";
      Rslt::write(out.str());
      continue;
    }
    // parse file

    if (!Rslt::running())
      Demo::write("\n\n  opening file \"" + pRepo_->package() + "\"");
    if (!Demo::running() && !Rslt::running())
      Dbug::write("\n\n  opening file \"" + pRepo_->package() + "\"");
    pRepo_->language() = Language::CSharp;
    pRepo_->currentPath() = file;
    while (pParser_->next())
      pParser_->parse();

    Slocs slocs = pRepo_->Toker()->currentLineCount();
    slocMap_[pRepo_->package()] = slocs;
  }
  if (showProc)
    clearActivity();
  ostringstream out;
  out << left << "\r  " << setw(77) << " ";
  Rslt::write(out.str());
}
//----< evaluate complexities of each AST node >---------------------

void CodeAnalysisExecutive::complexityAnalysis()
{
  ASTNode* pGlobalScope = pRepo_->getGlobalScope();
  CodeAnalysis::complexityEval(pGlobalScope);
}
//----< comparison functor for sorting FileToNodeCollection >----
/*
* - supports stable sort on extension values
* - displayMetrics(...) uses to organize metrics display
*/
struct CompExts
{
  bool operator()(const pair<File, ASTNode*>& first, const pair<File, ASTNode*>& second)
  {
    return FileSystem::Path::getExt(first.first) > FileSystem::Path::getExt(second.first);
  }
};
//----< comparison functor for sorting FileToNodeCollection >----
/*
* - supports stable sort on name values
* - displayMetrics(...) uses these functions to organize metrics display
*/
static void removeExt(string& name)
{
  size_t extStartIndex = name.find_last_of('.');
  name = name.substr(0, extStartIndex);
}

struct CompNames
{
  bool operator()(const pair<File, ASTNode*>& first, const pair<File, ASTNode*>& second)
  {
    string fnm = FileSystem::Path::getName(first.first);
    removeExt(fnm);
    string snm = FileSystem::Path::getName(second.first);
    removeExt(snm);
    return fnm < snm;
  }
};
//----< display header line for displayMmetrics() >------------------

void CodeAnalysisExecutive::displayHeader()
{
  ostringstream out;
  out << right;
  out << "\n ";
  out << setw(25) << "file name";
  out << setw(12) << "type";
  out << setw(35) << "name";
  out << setw(8) << "line";
  out << setw(8) << "size";
  out << setw(8) << "cplx";
  out << right;
  out << "\n  ";
  out << setw(25) << "-----------------------";
  out << setw(12) << "----------";
  out << setw(35) << "---------------------------------";
  out << setw(8) << "------";
  out << setw(8) << "------";
  out << setw(8) << "------";
  Rslt::write(out.str());
}
//----< display single line for displayMetrics() >-------------------

void CodeAnalysisExecutive::displayMetricsLine(const File& file, ASTNode* pNode)
{
  function<string(string, size_t)> trunc = [](string in, size_t count)
  {
    return in.substr(0, count);
  };
  ostringstream out;
  out << right;
  out << "\n ";
  out << setw(25) << trunc(file, 23);
  out << setw(12) << pNode->type_;
  out << setw(35) << trunc(pNode->name_, 33);
  out << setw(8) << pNode->startLineCount_;
  out << setw(8) << pNode->endLineCount_ - pNode->startLineCount_ + 1;
  size_t debug1 = pNode->startLineCount_;
  size_t debug2 = pNode->endLineCount_;
  out << setw(8) << pNode->complexity_;
  Rslt::write(out.str());
}
//----< display lines containing public data declaration >-----------

string CodeAnalysisExecutive::showData(const Scanner::ITokCollection* pTc)
{
  string semiExpStr;
  for (size_t i=0; i<pTc->length(); ++i)
    semiExpStr += (*pTc)[i] + " ";
  return semiExpStr;
}

void CodeAnalysisExecutive::displayDataLines(ASTNode* pNode, bool isSummary)
{
  for (auto datum : pNode->decl_)
  {
    if (pNode->type_ == "function")
      continue;
    if (datum.access_ == Access::publ && datum.declType_ == DeclType::dataDecl)
    {
      ostringstream out;
      out << right;
      out << "\n ";
      out << setw(25) << "public data:" << " ";
      if (isSummary)
      {
        out << datum.package_ << " : " << datum.line_ << " - " 
          << pNode->type_ << " " << pNode->name_ << "\n " << setw(15) << " ";
      }
      out << showData(datum.pTc);
      Rslt::write(out.str());
    }
  }
}
//----<  helper for displayMetrics() >-------------------------------
/*
* - Breaking this out as a separate function allows application to
*   display metrics for a subset of the Abstract Syntax Tree
*/
void CodeAnalysisExecutive::displayMetrics(ASTNode* root)
{
  flushLogger();
  ostringstream out;
  out << "Code Metrics - Start Line, Size (lines/code), and Complexity (number of scopes)";
  Utils::sTitle(out.str(), 3, 92, out, '=');
  out << "\n";
  Rslt::write(out.str());

  function<void(ASTNode* pNode)> co = [&](ASTNode* pNode) {
    if (
      pNode->type_ == "namespace" ||
      pNode->type_ == "function" ||
      pNode->type_ == "class" ||
      pNode->type_ == "interface" ||
      pNode->type_ == "struct" ||
      pNode->type_ == "lambda"
      )
      fileNodes_.push_back(pair<File, ASTNode*>(pNode->package_, pNode));
  };
  ASTWalkNoIndent(root, co);
  stable_sort(fileNodes_.begin(), fileNodes_.end(), CompExts());
  stable_sort(fileNodes_.begin(), fileNodes_.end(), CompNames());

  displayHeader();

  string prevFile;
  for (auto item : fileNodes_)
  {
    if (item.first != prevFile)
    {
      Rslt::write("\n");
      displayHeader();
    }
    displayMetricsLine(item.first, item.second);
    displayDataLines(item.second);
    prevFile = item.first;
  }
  Rslt::write("\n");
}
//----< display metrics results of code analysis >---------------

void CodeAnalysisExecutive::displayMetrics()
{
  ASTNode* pGlobalScope = pRepo_->getGlobalScope();
  displayMetrics(pGlobalScope);
}
//----< walk tree of element nodes >---------------------------------

template<typename element>
void TreeWalk(element* pItem, bool details = false)
{
  static string path;
  if (path != pItem->path_ && details == true)
  {
    path = pItem->path_;
    Rslt::write("\n" + path);
  }
  static size_t indentLevel = 0;
  ostringstream out;
  out << "\n  " << string(2 * indentLevel, ' ') << pItem->show();
  Rslt::write(out.str());
  auto iter = pItem->children_.begin();
  ++indentLevel;
  while (iter != pItem->children_.end())
  {
    TreeWalk(*iter);
    ++iter;
  }
  --indentLevel;
}
//----< display the AbstrSynTree build in processSourceCode() >------

void CodeAnalysisExecutive::displayAST()
{
  flushLogger();
  ASTNode* pGlobalScope = pRepo_->getGlobalScope();
  Utils::title("Abstract Syntax Tree");
  TreeWalk(pGlobalScope);
  Rslt::write("\n");
}
//----< show functions with metrics exceeding specified limits >-----

void CodeAnalysisExecutive::displayMetricSummary(size_t sMax, size_t cMax)
{
  flushLogger();
  ostringstream out;
  Utils::sTitle("Functions Exceeding Metric Limits and Public Data", 3, 92, out, '=');
  Rslt::write(out.str());
  displayHeader();

  if (fileNodes_.size() == 0)  // only build fileNodes_ if displayMetrics hasn't been called
  {
    function<void(ASTNode* pNode)> co = [&](ASTNode* pNode) {
      fileNodes_.push_back(pair<File, ASTNode*>(pNode->package_, pNode));
    };
    ASTNode* pGlobalNamespace = pRepo_->getGlobalScope();
    ASTWalkNoIndent(pGlobalNamespace, co);
    stable_sort(fileNodes_.begin(), fileNodes_.end(), CompExts());
    stable_sort(fileNodes_.begin(), fileNodes_.end(), CompNames());
  }
  for (auto item : fileNodes_)
  {
    if (item.second->type_ == "function")
    {
      size_t size = item.second->endLineCount_ - item.second->startLineCount_ + 1;
      size_t cmpl = item.second->complexity_;
      if (size > sMax || cmpl > cMax)
        displayMetricsLine(item.first, item.second);
    }
  }
  Rslt::write("\n");
  for (auto item : fileNodes_)
  {
    displayDataLines(item.second, true);
  }
  Rslt::write("\n");
}
//----< comparison functor for sorting SLOC display >----------------

struct compFiles
{
private:
  string ChangeFirstCharOfExt(const string& fileName) const
  {
    string temp = fileName;
    size_t pos = temp.find_last_of('.');
    if (pos < temp.size() - 1)
      if (temp[pos + 1] == 'h')
        temp[pos + 1] = 'a';
    return temp;
  }
public:
  bool operator()(const string& fileName1, const string& fileName2) const
  {
    return ChangeFirstCharOfExt(fileName1) < ChangeFirstCharOfExt(fileName2);
  }
};
//----< show sizes of all the files processed >----------------------

void CodeAnalysisExecutive::displaySlocs()
{
  flushLogger();
  Utils::sTitle("File Size - Source Lines of Code", 3, 92);
  size_t slocCount = 0;
  map<string, size_t, compFiles> fileColl;
  for (auto item : fileMap_)
  {
    for (auto file : item.second)
    {
      File fileName = FileSystem::Path::getName(file);
      fileColl[file] = slocMap_[fileName];
    }
  }
  for (auto fitem : fileColl)
  {
    ostringstream out;
    out << "\n  " << setw(8) << fitem.second << " : " << fitem.first;
    Rslt::write(out.str());
    slocCount += fitem.second;
  }
  ostringstream out;
  out << "\n\n      Total line count = " << slocCount << "\n";
  Rslt::write(out.str());
  Rslt::write("\n");
}
//----< display analysis info based on command line options >--------

void CodeAnalysisExecutive::dispatchOptionalDisplays()
{
  for (auto opt : options_)
  {
    switch (opt)
    {
    case 'm':
      displayMetrics();
      Rslt::start();
      break;
    case 'a':
      displayAST();
      Rslt::start();
      break;
    case 's':
      displaySlocs();
      Rslt::start();
      break;
    default:
      break;
    }
  }
}
//----< display analysis info based on command line options >--------

void CodeAnalysisExecutive::setDisplayModes()
{
  for (auto opt : options_)
  {
    switch (opt)
    {
    case 'r':
      Rslt::start();
      break;
    case 'd':
      Demo::start();
      break;
    case 'b':
      Dbug::start();
      break;
    case 'f':
      setLogFile("logFile.txt");
      break;
    default:
      if (opt != 'a' && opt != 'b' && opt != 'd' && opt != 'f' && opt != 'm' && opt != 'r' && opt != 's')
      {
        cout << "\n\n  unknown option " << opt << "\n\n";
      }
    }
  }
}
//----< helper functions for managing application's logging >--------

void CodeAnalysisExecutive::startLogger(ostream& out)
{
  Rslt::attach(&out);
  Demo::attach(&out);
  Dbug::attach(&out);

  // will start Demo and Dbug if  have options /d and /b

  setDisplayModes();
}

void CodeAnalysisExecutive::flushLogger()
{
  Rslt::flush();
  Demo::flush();
  Dbug::flush();
}

void CodeAnalysisExecutive::stopLogger()
{
  Rslt::flush();
  Demo::flush();
  Dbug::flush();
  Rslt::stop();
  Demo::stop();
  Dbug::stop();
}
//----< open file stream for logging >-------------------------------
/*
*  - must come after CodeAnalysisExecutive::processCommandLine()
*  - must come before starting any of the loggers
*/
void CodeAnalysisExecutive::setLogFile(const File& file)
{
  string path = getAnalysisPath();
  path += "\\" + file;
  pLogStrm_ = new ofstream(path);
  if (pLogStrm_->good())
  {
    Rslt::attach(pLogStrm_);
    Demo::attach(pLogStrm_);
    Dbug::attach(pLogStrm_);
  }
  else
    Rslt::write("\n  couldn't open logFile.txt for writing");
}

string CodeAnalysisExecutive::systemTime()
{ 
  time_t sysTime = time(&sysTime);
  char buffer[27];
  ctime_s(buffer, 27, &sysTime);
  buffer[24] = '\0';
  string temp(buffer);
  return temp;
}

//----< Demonstrating Requirement 1 >--------
void R1()
{
	cout << "\n";
	cout << "  Requirement # 1" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Using Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs";
	cout << "\n\n";
}
//----< Demonstrating Requirement 2 >--------
void R2()
{
	cout << "\n";
	cout << "  Requirement # 2" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Using the C++ standard library's streams for all I/O and new and delete for all heapbased memory management";
	cout << "\n\n";
}
//----< Demonstrating Requirement 3 >--------
void R3()
{
	cout << "\n";
	cout << "  Requirement # 3" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Provided packages as described in Purpose section enforcing \"Single Responsiblity Principle\"   ";
	cout << "\n\n";
}
//----< Demonstrating Requirement 4 >--------
void R4(TypeAnal& ta)
{
	cout << "\n";
	cout << "  Requirement # 4" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Provided a TypeAnalysis package that identifies all of the types defined in a specified file.";
	cout << "\n\n";
	cout << "   -------------------------------------------------------------------------" << endl;
	cout << "   Type analysis started ...." << endl;
	cout << "   -------------------------------------------------------------------------" << endl;
	ta.doTypeAnal();
}
//----< Demonstrating Requirement 5 >--------
void R5(DepAnal& da , set<string> allfilenames , TypeAnal& ta)
{
	cout << "\n";
	cout << "  Requirement # 5" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Provided a DependencyAnalysis package that identifies all of the dependencies between files in a specified file collection.";
	cout << "\n\n";
	for (string str : allfilenames)
	{
		da.searchDependency(str, ta.getTypeTable(), ta.getToker());
	}
	cout << "   -------------------------------------------------------------------------" << endl;
	cout << "   Dependency analysis started ...." << endl;
	cout << "   -------------------------------------------------------------------------" << endl;
	da.displayDependency();
}
//----< Demonstrating Requirement 6 >--------
void R6(graph& g, set<string> allfilenames, unordered_map<string, set<string>> depmap)
{
	cout << "\n";
	cout << "  Requirement # 6" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Finding strong components in the dependency graph defined by the relationships evaluated in the previous requirement";
	cout << "\n\n";
	for (auto str : allfilenames)
	{
		vertex v1(str);
		g.addVertex(v1);
	}
	vector< Vertex<string, string> > vertvector = g.getVertexVector();
	set<string> sset;
	cout << "   -------------------------------------------------------------------------" << endl;
	cout << "   Constructing Graph from Dependency Analysis results ..." << endl;
	cout << "   -------------------------------------------------------------------------" << endl;
	int edgevalue = 1;
	for (Vertex<string, string> vert : vertvector)
	{
		if (depmap.find(vert.value()) != depmap.end())
		{
			sset = depmap[vert.value()];

			for (string sset_string : sset)
			{
				for (Vertex<string, string> vert2 : vertvector)
				{

					if (vert2.value() == sset_string)
						g.addEdge(to_string(edgevalue++), vert, vert2);
				}
			}
		}
	}
	display::show(g);
	cout << "\n\n";
	cout << "   -------------------------------------------------------------------------" << endl;
	cout << "   Strong Component Analysis started ..." << endl;
	cout << "   -------------------------------------------------------------------------" << endl;
	g.SCC();
	cout << "\n";
	cout << "   Please Check ../DependencyAndStrongComponents.png";
	cout << "\n";
}
//----< Demonstrating Requirement 7 >--------
void R7(DepAnal& da)
{
	cout << "\n";
	cout << "  Requirement # 7" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Write the analysis results, in XML format, to a specified file.";
	cout << "\n\n";

	NoSqlDb<string> db;
	cout << "   Storing Dependencies in NoSqlDb" << endl;
	cout << "" << endl;
	da.storeToNoSqlDb(db);

	Persistence<string> pa;

	cout << "   Writing analysis results in default XML file.." << endl;
	cout << "" << endl;
	pa.saveXml(db);

	cout << "   Please check \"../Xmlfile.xml\" for Results " << endl;
	cout << "" << endl;
}
//----< Demonstrating Requirement 8 >--------
void R8()
{
	cout << "\n";
	cout << "  Requirement # 8" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Processing the command line, accepting:" << endl;
	cout << "\t -> Path to the directory tree containing files to analyze" << endl;
	cout << "\t -> List of file patterns to match for selection of files to analyze" << endl;
	cout << "\t -> Specification of the XML results file, supplying a default file" << endl;
	cout << "\n";
	cout << "   This Test Executive processes command line arguments with file patterns to match for selection of files" << endl; 
	cout << "\n";
	cout << "   The XML results are stored in default file \"../Xmlfile.xml\" " << endl;
	cout << "\n\n";
}
//----< Demonstrating Requirement 9 >--------
void R9()
{
	cout << "\n";
	cout << "  Requirement # 9" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Include an automated unit test suite that demonstrates meeting all the requirements of this project" << endl;
	cout << "   We are in a Automated unit Test Suite !!!! ";
	cout << "\n\n";

}
//----< header to Type Based Dependency Analyzer >--------
void header()
{
	cout << "\n \t \t \t \t \t Welcome to My Type Based Dependency Analyzer ";
	cout << "\n \t \t \t \t \t ============================================\n";
	cout << "\n\n";
}
//----< conduct code analysis >--------------------------------------

#include <fstream>

int main(int argc, char* argv[])
{	
  using namespace CodeAnalysis;
  header();
  CodeAnalysisExecutive exec;
  bool succeeded = exec.ProcessCommandLine(argc, argv);
  if (!succeeded)  {	  return 1;   }
  exec.setDisplayModes();
  exec.startLogger(cout);
  ostringstream tOut("CodeAnalysis");
  Utils::sTitle(tOut.str(), 3, 92, tOut, '=');
  Rslt::write(tOut.str());
  Rslt::write("\n     " + exec.systemTime());
  Rslt::flush();
  exec.showCommandLineArguments(argc, argv);
  Rslt::write("\n");
  exec.getSourceFiles();
  exec.processSourceCode(true);
  exec.complexityAnalysis();
  exec.dispatchOptionalDisplays();
  exec.flushLogger();
  //exec.displayMetricSummary(50, 10);
  exec.flushLogger();
  Rslt::write("\n");
  ostringstream out;
  out << "\n  " << setw(10) << "searched" << setw(6) << exec.numDirs() << " dirs";
  out << "\n  " << setw(10) << "processed" << setw(6) << exec.numFiles() << " files";
  Rslt::write(out.str());
  Rslt::write("\n");
  exec.stopLogger();

  R1();
  R2();
  R3();
  TypeAnal ta;
  R4(ta);
  set<string> allfilenames = ta.getAllFilenames();
  DepAnal da;
  R5(da, allfilenames, ta);
  unordered_map<string, set<string>> depmap = da.getDepMap();
  graph g;
  R6(g, allfilenames, depmap);
  R7(da);
  R8();
  R9();

  return 0;
}