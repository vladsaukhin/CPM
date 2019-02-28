#include "Program.h"

Program::~Program()
{}

Program::Program( const string & filename )
	: m_fileName( filename )
	, m_builded( false )
{}

void Program::SaveStateCode() const
{
	ofstream fout(m_fileName.c_str());
	fout << m_buff;
	fout.close();
}

void Program::InitStateCode()
{
	ifstream fin( m_fileName.c_str() );

	if ( fin.is_open() )
	{
		string reader;
		while ( !fin.eof() )
		{
			getline( fin, reader );
			m_buff += reader;
			m_buff += '\n';
		}

		fin.close();
	}
}

void Program::Start(const TypeAnalyzer& syntaxAnalyzerType)
{
	cout << "###########################################################################################################" << endl;
	cout << "### Build started: Project: " << m_fileName << endl;

	lexicalAnalyzerTypeA();
	//syntaxAnalyzerType__( syntaxAnalyzerType );

	cout << "### Done building project " << m_fileName << " -- " << (m_builded ? "":"FAILED") <<endl;
	cout << "###########################################################################################################" << endl;
}

void Program::lexicalAnalyzerTypeA()
{
	std::shared_ptr<LexicalAnalyzer> lexicalAnalyzer = std::make_shared<LexicalAnalyzer>( m_buff );

	lexicalAnalyzer->StartProcessing();

	lexicalAnalyzer->ViewLogs();
	if(exept.size() != 0)
		ShellExecute(0, 0, "C:\master\C++\CPM_\CPM_\Logs\LexicalLogsTypeA.txt", 0, 0, SW_SHOW);

	lexicalAnalyzer->WriteAllToFile();
	ShellExecute(0, 0, "C:\master\C++\CPM_\CPM_\Result\Lexems.txt", 0, 0, SW_SHOW);
	
	lexicalAnalyzer->WriteLexemToFile();
	ShellExecute(0, 0, "C:\master\C++\CPM_\CPM_\Result\Variables.txt", 0, 0, SW_SHOW); 
	
	lexicalAnalyzer->WriteConstToFile();
	ShellExecute(0, 0, "C:\master\C++\CPM_\CPM_\Result\Constants.txt", 0, 0, SW_SHOW);

	exept = lexicalAnalyzer->exept;
	m_allLexem = lexicalAnalyzer->GetLexem();
}

void Program::syntaxAnalyzerType__(const TypeAnalyzer& syntaxAnalyzerType)
{
	std::shared_ptr<IAnalyzer> syntaxAnalyzer;
	switch ( syntaxAnalyzerType )
	{
	case TypeAnalyzer::TypeA: 
		syntaxAnalyzer = std::make_shared<SyntaxAnalyzer>( m_allLexem );
		break;
	case TypeAnalyzer::TypeB:
		syntaxAnalyzer = std::make_shared<MPA>( m_allLexem );
		break;
	default:
		cerr << "Unexpected type of syntax analyzer!" << endl;
	}
	
	if (exept.empty())
	{
		if (m_allLexem.empty())
		{
			cerr << "List of lexems is empty" << endl;
			return;
		}
		else
		{
			syntaxAnalyzer->StartProcessing();
			syntaxAnalyzer->ViewLogs();
		}
	}
	else
	{
		cerr << "Can't continue. You have some mistakes after lexical analyzing" << endl;
		return;
	}
}
