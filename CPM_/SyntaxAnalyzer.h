#pragma once
#include"CommoType.h"

class SyntaxAnalyzer : public IAnalyzer
{
public:
	SyntaxAnalyzer(const std::vector<allLexem>&);
	~SyntaxAnalyzer();

	void StartProcessing() override;

private:

	void declarationLexem(int& index);
	void assignmentState(int& index);
	void expretionState(int& index, bool ifst);
	void inputState(int& index);
	void outputState(int& index);
	void ifState(int& index);
	void whileState(int& index);
	void callState(int& index);

	
	void state—omma(int & index);
	string whichState(int index, int& line);
	void exeptAndSkip(int& index, const string& mess);
public:
	std::vector<Exept> exept;
private:
	std::vector<allLexem> m_lexem;

};

