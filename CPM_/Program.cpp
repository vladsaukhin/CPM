#include "Program.h"

Program::~Program()
{
}

Program::Program( const string & filename )
	:m_fileName( filename )
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

void Program::Start(const short& syntaxAnalyzerType)
{
	lexicalAnalyzerTypeA();
	syntaxAnalyzerType__( syntaxAnalyzerType );
}

void Program::lexicalAnalyzerTypeA()
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

void Program::syntaxAnalyzerType__(const short& syntaxAnalyzerType)
{

	switch ( syntaxAnalyzerType )
	{
	case 1: SyntaxAnalyzer syntaxAnalyzer( m_allLexem );  break;
	case 2: MPA mpaAnalyzer( m_allLexem ); break;
	default:
		cerr << "Unexpected type of syntax analyzer!" << endl;
	}
	

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
