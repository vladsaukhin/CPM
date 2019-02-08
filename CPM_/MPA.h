#pragma once
#include"CommoType.h"
#include<list>

class MPA: public IAnalyzer
{
public:
	MPA( const std::vector<AllLexem>& );
	~MPA();

	void StartProcessing() override;
	void ViewLogs() override;

	int next_step( int next_state, int i );
	
public:
	std::vector<Exept> exept;
private:
	std::vector<AllLexem> m_lexem;
	std::vector<MPAMemento> m_mpa;
	std::vector<int> StackInt;
	std::vector<string> erro;
	int next_state;
	int state;
	int present_token;
};

