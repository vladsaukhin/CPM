#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<stack>
#include<map>

#include<Windows.h>

#include"Interfaces.h"

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::stack;
using std::map;

enum class TypeAnalyzer
{
	TypeA,
	TypeB
};

enum class ReservedName
{
	_none,
	_def,
	_in,
	_out,
	_while,
	_do,
	_if,
	_return,
	_then,
	_endl,
	_else,
	_AND,
	_OR,
	_NOT,
	_main,
	_int,
	_double,
	_Ind,
	_Con,
	_operator
};

struct AllLexem
{
	string val;
	int index;
	int numOfLine;
	int numOfID;
	string type;
	ReservedName alias;

	AllLexem()
		: val("")
		, index()
		, numOfLine()
		, numOfID()
		, type("")
		, alias(ReservedName::_none)
	{}

	AllLexem(const string& _val, int _index, int numLine, int numID, const string& _type, ReservedName _alias)
		: val(_val)
		, index(_index)
		, numOfLine(numLine)
		, numOfID(numID)
		, type(_type)
		, alias(_alias)
	{}

	AllLexem(const AllLexem& other)
		: val(other.val)
		, index(other.index)
		, numOfLine(other.numOfLine)
		, numOfID(other.numOfID)
		, type(other.type)
		, alias(other.alias)
	{}

	AllLexem& operator= (const AllLexem& other)
	{
		if (this != &other) // защита от неправильного самоприсваивания
		{
			val = other.val;
			index = other.index;
			numOfLine = other.numOfLine;
			numOfID = other.numOfID;
			type = other.type;
			alias = other.alias;
		}
		return *this;
	}

	AllLexem( AllLexem&& other)
		: val(std::move(other.val))
		, index(other.index)
		, numOfLine(other.numOfLine)
		, numOfID(other.numOfID)
		, type(std::move(other.type))
		, alias(other.alias)
	{}

	virtual ~AllLexem() {}

	friend std::ostream& operator<<(std::ostream& os, const AllLexem& dt)
	{
		os << std::setw(5) << dt.numOfLine
			<< std::setw(22) << dt.val
			<< std::setw(7) << dt.index
			<< std::setw(7) << dt.numOfID
			<< std::setw(13) << dt.type;
		return os;
	}
};

struct ConVal {

	bool flag;     // is con val?
	string val;		//type if yes

	// is con val?
	//type if yes
	ConVal(string v, bool i)
		:val(v), flag(i)
	{}

	ConVal(const ConVal& other)
		:val(other.val), flag(other.flag)
	{}

	ConVal& operator= (const ConVal& other)
	{
		if (this != &other)
		{
			val = other.val;
			flag = other.flag;
		}
		return *this;
	}

	ConVal(ConVal&& other)
		:val(std::move(other.val)), flag(other.flag)
	{}

	~ConVal() {}
};

struct Exept
{
	string mess;
	int line;
	string val;

	//v - value which find exept
	//l - line where find exept
	//m - message where find exept
	Exept(const string &v, int l, string m)
		: val(v)
		, line(l)
		, mess(m)
	{}

	Exept(const Exept& other)
		: mess(other.mess)
		, line(other.line)
		, val(other.val)
	{}

	Exept& operator= (const Exept& other)
	{
		if (this != &other) // защита от неправильного самоприсваивания
		{
			mess = other.mess;
			line = other.line;
			val = other.val;
		}
		return *this;
	}

	Exept(Exept&& other)
		:val(std::move(other.val)), line(other.line), mess(std::move(other.mess))
	{}

	~Exept() {}
};

struct FLAGS {

	bool unexpectedIdVal = false;
	//taken leter before dot
	bool unexpectedBehaviorBeforeDot = false;
	//taken later after dot
	bool unexpectedBehaviorAfterDot = false;
	bool numStartFromNull = false;
	bool unexpectedSignAfterDot = false;

	FLAGS()
		: unexpectedIdVal(false),
		unexpectedBehaviorBeforeDot(false),
		unexpectedBehaviorAfterDot(false),
		unexpectedSignAfterDot(false),
		numStartFromNull(false)
	{}

	void SET_FALSE_ALL()
	{
		unexpectedIdVal = false;
		unexpectedBehaviorBeforeDot = false;
		unexpectedBehaviorAfterDot = false;
		numStartFromNull = false;
		unexpectedSignAfterDot = false;
	}

	~FLAGS() {}
};

struct blockAddapter
{
	int block;
	string whichBlock;
	int line;
	int id;

	blockAddapter(int b, string wB, int l, int idd)
		: block(b)
		, whichBlock(wB)
		, line(l)
		, id(idd)
	{}

	blockAddapter(const blockAddapter& other)
		: block(other.block)
		, whichBlock(other.whichBlock)
		, line(other.line)
		, id(other.id)
	{}

	blockAddapter(blockAddapter&& other)
		: block(other.block)
		, whichBlock(std::move(other.whichBlock))
		, line(other.line)
		, id(other.id)
	{}

	~blockAddapter() {}

};

struct MPAMemento
{
	int thisState;
	int indexThis;
	int nextState;
	int finishState;
	string messError;

	MPAMemento( int a, int b, int c, int d, string e )
		: thisState(a)
		, indexThis(b)
		, nextState(c)
		, finishState(d)
		, messError(e)
	{}

	MPAMemento( const MPAMemento& other )
		: thisState( other.thisState )
		, indexThis( other.indexThis )
		, nextState( other.nextState )
		, finishState( other.finishState )
		, messError(other.messError)
	{}

	MPAMemento( MPAMemento&& other )
		: thisState( other.thisState )
		, indexThis( other.indexThis )
		, nextState( other.nextState )
		, finishState( other.finishState )
		, messError( std::move( other.messError ) )
	{}

	friend std::ostream& operator<< ( std::ostream& os, const MPAMemento& dt )
	{
		os << std::setw( 5 ) << dt.thisState
			<< std::setw( 5 ) << dt.indexThis
			<< std::setw( 5 ) << dt.nextState
			<< std::setw( 5 ) << dt.finishState
			<< std::setw( 30 ) << dt.messError;
		return os;
	}

	~MPAMemento()
	{}
};
