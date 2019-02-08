#include "Program.h"

Program::~Program()
{
}

void Program::SaveStateCode() const
{
	ofstream fout(m_fileName.c_str());
	fout << m_buff;
	fout.close();
}

void Program::InitStateCode()
{
	ifstream fin(m_fileName.c_str());
	
	if (fin.is_open()) {
	
		string reader;
		while (!fin.eof()) {
			getline(fin, reader);
			m_buff += reader;
			m_buff += '\n';
		}

		fin.close();
	}

	
}

void Program::Start()
{
	analyzer();
	analyzer2();
}

void Program::analyzer() 
{

	LexicalAnalyzer lexicalAnalyzer(m_buff);

	lexicalAnalyzer.StartProcessing();

	lexicalAnalyzer.writeToFile();
	lexicalAnalyzer.writeToFileLexem();
	lexicalAnalyzer.writeToFileConst();

	exept = lexicalAnalyzer.exept;
	m_allLexem = lexicalAnalyzer.GetLexem();
	//lexicalAnalyzer.~LexicalAnalyzer();
	/*catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}*/

	
}

void Program::analyzer2()
{
	SyntaxAnalyzer syntaxAnalyzer(m_allLexem);
	//MPA mpaAnalyzer( m_allLexem );

	if (exept.empty())
	{
		if (m_allLexem.empty())
		{
			cerr << "after lexicalAnalyzer u did't find any lexem(token)" << endl;
			return;
		}
		else
		{

			syntaxAnalyzer.StartProcessing();
			//mpaAnalyzer.StartProcessing();
		}
	}
	else
	{
		cerr << "U have some mistake with lexicalAnalyzer" << endl;
		return;
	}
}
