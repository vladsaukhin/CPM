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
	string whichTypeID() const;
	int isDeclarationID(const string& val) const;	
	AllLexem whichID(const string& val, int& block) const;
	int countBlock() const;
	///AddLexem

	bool wasWriteToFileLexem(const std::vector<AllLexem>& out, const int& test, const int& bl) const;
	ReservedName whichAlias(const int&);
public:
	vector<Exept> exept;

private:
	string m_buff;
	vector<ReservedLexem> m_lexem;
	vector<AllLexem> m_allLexem;
	
	const string m_alphabetEN = "etaoinshrdlcumwfgypbvkjxqz_ETAOINSHRDLCUMWFGYPBVKJXQZ";
	const string m_num = "1234567890";
	const string m_sign = "+-*/,?:{}()[]";

private:
	string storage;
	int corentBlock;
	FLAGS flag;
	const size_t length;
	size_t i;
	int numOfConstVal;
	int numOfIdentifier;
	int numOfLine;
};

