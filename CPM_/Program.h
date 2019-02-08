#pragma once

#include"LexicalAnalyzer.h"
#include"SyntaxAnalyzer.h"
#include"MPA.h"

class Program 
{
public:
	Program(const string& filename);
	~Program();

	void SaveStateCode() const;
	void InitStateCode();

	void Start(const TypeAnalyzer&);

	inline string& GetBuff()
	{
		return m_buff;
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

	void lexicalAnalyzerTypeA();
	void syntaxAnalyzerType__(const TypeAnalyzer&);

public:
	vector<Exept> exept;

private:
	string m_buff;								//source code storage
	string m_fileName;						//name of source file
	vector<allLexem> m_allLexem;			//all found tokens
};

