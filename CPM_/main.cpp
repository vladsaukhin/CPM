// Author: Vlad Saukhin (vladsaukhin.official@gmail.com, vsaukhin@cqg.com)
//	About:  This main functions where programm starting and finishing
// !!!  Windows OS ONLY  !!!
// Include 2 parts:
// 5 semester: lab 1-3 lexical and syntax analyzers
//	6 semester: n/a

#include"Program.h"

int main() {

	std::unique_ptr<Program> pr1 = std::make_unique<Program>("./Source/source.cpm");
	pr1->InitStateCode();

	pr1->Start(TypeAnalyzer::TypeB);
	
	system("pause");
	return 0;
}