#pragma once
#include"CommoType.h"

class LexicalAnalyzer : public IAnalyzer
{
public:
	LexicalAnalyzer(const string& buff);
	~LexicalAnalyzer();

public:
	void StartProcessing() override;
	void ViewLogs() override;

	void WriteAllToFile() const;
	void WriteLexemToFile() const;
	void WriteConstToFile() const;

	vector<AllLexem> GetLexem() const
	{
		return m_allLexem;
	}

private:
	LexicalAnalyzer() = delete;
	LexicalAnalyzer(const LexicalAnalyzer&) = delete;
	LexicalAnalyzer& operator=(const LexicalAnalyzer&) = delete;
	LexicalAnalyzer(LexicalAnalyzer&&) = delete;
	LexicalAnalyzer& operator=(LexicalAnalyzer&&) = delete;

private:

	///Check
	bool isLeter(const char& sign) const;
	bool isNum(const char& sign) const;
	bool isSign(const char& sign) const;
	void isCorrectSigns();
	///Check

	///States
	bool stateInt();
	bool stateLetters();
	bool stateComment();
	bool stateNewLine();
	//include '.' and '..'
	bool stateDot();
	//add sign and count brackets
	bool stateSign(int&, int&, int&);
	//using only for < > = !
	void stateDoubleSign(const char& sign);
	///States

	///AddLexem
	void stateAddLexem();
	ConVal isConVal();

	///AddLexem

	ReservedName whichAlias(const int&);
public:
	vector<Exept> exept;

private:
	string m_buff;
	map<string, int> m_reserveLexem;
	vector<AllLexem> m_allLexem;
	
	const string m_alphabetEN = "etaoinshrdlcumwfgypbvkjxqz_ETAOINSHRDLCUMWFGYPBVKJXQZ";
	const string m_num = "1234567890";
	const string m_sign = "+-*/,?:{}()[]";

private:
	string storage;
	FLAGS flag;
	const size_t length;
	size_t i_class;
	int numOfConstVal;
	int numOfIdentifier;
	int numOfLine;
};

