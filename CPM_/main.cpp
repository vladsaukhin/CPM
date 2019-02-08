// Author: Vlad Saukhin (vladsaukhin.official@gmail.com, vsaukhin@cqg.com)
//	About:  This main functions where programm starting and finishing
// Include 2 parts:
// 5 semester: lab 1-3 lexical and syntax analythers
//	6 semester: n/a

#include"Program.h"

int main() {

	std::unique_ptr<Program> pr1 = std::make_unique<Program>("./Source/source.cpm");
	pr1->InitStateCode();

	pr1->Start(TypeAnalyzer::TypeA);
	
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
