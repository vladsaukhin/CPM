#include "MPA.h"

MPA::MPA( const std::vector<AllLexem>& vectorlexem )
	: m_lexem( vectorlexem )
	, next_state(1)
	, present_token(0)
{
	ifstream fin( "./Source/mpa.txt" );
	if ( fin.is_open() )
	{

		string temp;
		string arr[5];
		while ( !fin.eof() )
		{

			for ( int i = 0; i < 5; i++ )
			{
				std::getline( fin, temp, ' ' );
				arr[i] = temp;
				temp = "";
			}

			m_mpa.emplace_back( atoi( arr[0].c_str() ), atoi( arr[1].c_str() ), atoi( arr[2].c_str() ), atoi( arr[3].c_str() ), arr[4] );
		}
	}
	else
	{
		exept.emplace_back( "it from the", 0,
			"open file[mpa.txt]: can't start running without mpa.txt" );
		m_lexem.resize( 0 );
		cerr << "open file[mpa.txt]: can't start running without mpa.txt" << endl;
	}
}

MPA::~MPA()
{}

void MPA::StartProcessing()
{
	int i = 0;
	while ( true )
	{
		if ( present_token < m_lexem.size() )
		{
			next_state = next_step( next_state, present_token );
			if ( next_state == 0 && i == 0 && present_token + 1 == m_lexem.size() )
			{
				cout << "The program is completed successfully!!!" << endl; 
				break;
			}
			
		}
		else
		{
			cout << "It is not possible to continue to program the program" << endl;
			break;
		}

		if ( i <= StackInt.size() )
		{
			present_token++;
		}
		i = StackInt.size();
	}
}

void MPA::ViewLogs()
{
	try
	{
		ofstream fout;
		fout.open("./Logs/SyntaxLogsTypeB.txt");

		for ( int j = 0; j < exept.size(); j++ )
		{
			fout << j + 1 << "	In line " << exept.at(j).line
				<< " u have " << exept.at(j).mess
				<< "  but got  '" << exept.at(j).val << "'" << std::endl;
		}
		fout.close();
	}
	catch ( std::exception& e )
	{
		cerr << e.what() << endl;
	}
}

int MPA::next_step( int next_state, int i )
{

	int count = 0;
	for ( int j = 0; j < m_mpa.size(); j++ )
	{
		if ( m_mpa[j].thisState == next_state )
		{
			count = j;
			break;
		}
	}

	bool error1 = true;

	do
	{
		if ( m_lexem.at( i ).index == m_mpa[count].indexThis )
		{
			next_state = m_mpa[count].nextState;
			if ( m_mpa[count].finishState != 0 )
			{
				StackInt.push_back( m_mpa[count].finishState	 );
			}
			std::vector<string>::iterator end = erro.end();
			if ( erro.size() >= 1 )
			{
				end--;
				erro.erase( end );
			}

			error1 = false;
			break;
		}
		count += 1;

	}
	while ( m_mpa.size() > count && m_mpa[count].thisState == next_state );

	if ( StackInt.size() != 0 && next_state == StackInt[StackInt.size() - 1] )
	{
		//std::vector<int>::iterator end = StackInt.end();
		//end--;
		//StackInt.erase( end );
	}

	if ( count >= m_mpa.size() )
	{
		count = m_mpa.size() - 1;
	}

	if ( error1 )
	{
		if ( StackInt.size() != 0 && erro.size() < 1 )
		{
			next_state = StackInt[StackInt.size() - 1];
			std::vector<int>::iterator end = StackInt.end();
			end--;
			StackInt.erase( end );
			erro.push_back( m_mpa[count].messError );
		}
		else
		{
			if ( erro.size() != 0 )
			{
				cout << erro[0] << "  at row " << m_lexem.at( i ).numOfLine << endl;
				exept.emplace_back( m_lexem.at( i ).val, m_lexem.at( i ).numOfLine,  erro[0]);
			}
			else
			{
				cout << m_mpa[count - 1].messError << "  at row " << m_lexem.at( i ).numOfLine << endl;
				exept.emplace_back( m_lexem.at( i ).val, m_lexem.at( i ).numOfLine, m_mpa[count - 1].messError );
			}
		}
	}

	state = count;
	return next_state;
}
