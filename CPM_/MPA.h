#pragma once
#include"CommoType.h"
#include<list>

class MPA
{
public:
	MPA( const std::vector<allLexem>& );
	~MPA();

	void StartProcessing();

	int next_step( int next_state, int i );
	

public:
	std::vector<Exept> exept;
private:
	std::vector<allLexem> m_lexem;
	std::vector<MPAMemento> m_mpa;
	std::vector<int> StackInt;
	std::vector<string> erro;
	int next_state;
	int state;
	int present_token;
};

