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

	void writeAllToFile() const;
	void writeLexemToFile() const;
	void writeConstToFile() const;

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
	bool isLeter(const char& sign) const;
	bool isNum(const char& sign) const;
	bool isSign(const char& sign) const;
	bool isSignDuplicate(const char& sign) const;
	checkConVal isConVal(const string& val, FLAGS& flag) const;
	string whichTypeID() const;
	int isDeclarationID(const string& val) const;
	bool stateInt();
	bool stateLetters();
	bool stateComment();
	bool stateNewLine();
	bool stateDot();
	bool stateAddLexem();
	bool stateDoubleSign(const char& sign);
	AllLexem whichID(const string& val, int& block) const;
	int countBlock() const;

	bool WasWriteToFileLexem( const std::vector<AllLexem>& out, const int& test, const int& bl) const;
	void isCorrectSigns();
	
public:
	vector<Exept> exept;

private:
	string m_buff;
	vector<ReservedLexem> m_lexem;
	vector<AllLexem> m_allLexem;
	
	const string m_alphabetEN = "etaoinshrdlcumwfgypbvkjxqz_ETAOINSHRDLCUMWFGYPBVKJXQZ";
	const string m_num = "1234567890";
	const string m_sign = "+-*/[],?:{}()";
	const string m_signDuplicate = "+-*/<>=![],?:{}()";

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

