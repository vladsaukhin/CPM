#pragma once

#include"LexicalAnalyzer.h"
#include"SyntaxAnalyzer.h"
#include"MPA.h"

class Program 
{
public:

	Program(string filename)
		:m_fileName(filename)
	{}
	~Program();

	void SaveStateCode() const;
	void InitStateCode();

	void Start();

	inline string& GetBuff()
	{
		return this->m_buff;
	}
	inline void SetFile(const string& newFileName)
	{
		m_fileName = newFileName;
	}

private:
	Program() = delete;
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;
	Program(Program&&) = delete;
	Program& operator=(Program&&) = delete;

	void analyzer();
	void analyzer2();

public:
	vector<Exept> exept;

private:
	string m_buff;
	string m_fileName;
	vector<allLexem> m_allLexem;
};

