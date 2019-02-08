#pragma once
#include"CommoType.h"

class LexicalAnalyzer
{
public:
	LexicalAnalyzer(const string& buff);
	~LexicalAnalyzer();

public:

	void StartProcessing();
	void writeToFile() const;
	void writeToFileLexem() const;
	void writeToFileConst() const;

	vector<allLexem> GetLexem() const
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
	void stateInt(size_t& i, const size_t& l);
	void stateLetters(size_t& i, const size_t& l, FLAGS& flag);
	void stateCommit(size_t& i, const size_t & l);
	void stateAddLexem(const int& numOfLine, FLAGS& flag, int& numOfConstVal, int& numOfIdentifier);
	void stateDoubleSign(const char& sign, size_t& iter, const size_t & length, FLAGS& flag, const int& numOfLine,
		int& numOfConstVal, int& numOfIdentifier);
	allLexem whichID(const string& val, int& block) const;
	int countBlock() const;

	bool WasWriteToFileLexem( const std::vector<allLexem>& out, const int& test, const int& bl) const;
	void isCorrectSigns();
	
public:
	vector<Exept> exept;

private:
	string m_buff;
	vector<mymap> m_lexem;
	vector<allLexem> m_allLexem;
	
	const string m_alphabetEN = "etaoinshrdlcumwfgypbvkjxqz_ETAOINSHRDLCUMWFGYPBVKJXQZ";
	const string m_num = "1234567890";
	const string m_sign = "+-*/[],?:{}()";
	const string m_signDuplicate = "+-*/<>=![],?:{}()";

private:
	string storage;
	int corentBlock;
};

