
#include <iostream>

using namespace std;

void R1()
{
	cout << "\n";
	cout << "  Requirement # 1" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Using Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs";
	cout << "\n\n";
}

void R2()
{	
	cout << "\n";
	cout << "  Requirement # 2" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Using the C++ standard library's streams for all I/O and new and delete for all heapbased memory management";
	cout << "\n\n";
}

void R3()
{
	cout << "\n";
	cout << "  Requirement # 3" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Provided packages as described in Purpose section enforcing \"Single Responsiblity Principle\"   ";
	cout << "\n\n";
}

void R4()
{
	cout << "\n";
	cout << "  Requirement # 4" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Provided a TypeAnalysis package that identifies all of the types defined in a specified file.";
	cout << "\n\n";
}

void R5()
{
	cout << "\n";
	cout << "  Requirement # 5" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Provided a DependencyAnalysis package that identifies all of the dependencies between files in a specified file collection.";
	cout << "\n\n";
}

void R6()
{
	cout << "\n";
	cout << "  Requirement # 6" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Finding strong components in the dependency graph defined by the relationships evaluated in the previous requirement";
	cout << "\n\n";
}

void R7()
{
	cout << "\n";
	cout << "  Requirement # 7" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Write the analysis results, in XML format, to a specified file.";
	cout << "\n\n";

}

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
	cout << "\n\n";
}

void R9()
{	
	cout << "\n";
	cout << "  Requirement # 9" << endl;
	cout << "  ===============";
	cout << "\n\n";
	cout << "\t Include an automated unit test suite that demonstrates meeting all the requirements of this project" << endl;
	cout << "\n\t We are in a Automated unit Test Suite !!!! ";
	cout << "\n\n";

}

int main(int argc, char* argv[])
{	
	using namespace CodeAnalysis;

	CodeAnalysisExecutive exec;

	cout << "\n \t \t \t \t \t Welcome to Type Based Dependency Analyzer ";
	cout << "\n \t \t \t \t \t =========================================\n";

	 R1();
	 R2();
	 R3();
	 R4();
	 R5();
	 R6();
	 R7();
	 R8();
	 R9();

	return 0;
}