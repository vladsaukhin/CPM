#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<stack>

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

enum class TypeAnalyzer
{
	TypeA,
	TypeB
};

enum class ReservedName
{
	_none,
	_int,
	_double,
	_uint,
	_udouble,
	_in,
	_out,
	_while,
	_do,
	_if,
	_return,
	_endl,
	_main,
	_Ind,
	_Con,
	_operator
};

struct ReservedLexem
{
	string val;
	int index;

	ReservedLexem()
		: val("")
		, index()
	{}

	ReservedLexem(const string& _val, int _index)
		: val(_val)
		, index(_index)
	{}

	ReservedLexem(const ReservedLexem& other)
		: val(other.val)
		, index(other.index)
	{}

	ReservedLexem(ReservedLexem&& other)
		: val(std::move(other.val))
		, index(other.index)
	{}

	virtual ~ReservedLexem()
	{}

};

struct AllLexem: public ReservedLexem
{
	int numOfLine;
	int numOfID;
	string type;
	int block;
	ReservedName alias;

	AllLexem()
		: ReservedLexem()
		, numOfLine()
		, numOfID()
		, type("")
		, block()
		, alias(ReservedName::_none)
	{}

	AllLexem(const string& _val, int _index, int numLine, int numID, const string& _type, int _block)
		: ReservedLexem(_val, _index)
		, numOfLine(numLine)
		, numOfID(numID)
		, type(_type)
		, block(_block)
	{}

	AllLexem(const string& _val, int _index, int numLine, int numID, const string& _type, int _block, ReservedName _alias)
		: ReservedLexem(_val, _index)
		, numOfLine(numLine)
		, numOfID(numID)
		, type(_type)
		, block(_block)
		, alias(_alias)
	{}

	AllLexem(const AllLexem& other)
		: ReservedLexem(other)
		, numOfLine(other.numOfLine)
		, numOfID(other.numOfID)
		, type(other.type)
		, block(other.block)
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
			block = other.block;
			alias = other.alias;
		}
		return *this;
	}

	AllLexem( AllLexem&& other)
		: ReservedLexem(other)
		, numOfLine(other.numOfLine)
		, numOfID(other.numOfID)
		, type(std::move(other.type))
		, block(other.block)
		, alias(other.alias)
	{}

	virtual ~AllLexem() {}

	friend std::ostream& operator<<(std::ostream& os, const AllLexem& dt)
	{
		os << std::setw(5) << dt.numOfLine
			<< std::setw(22) << dt.val
			<< std::setw(7) << dt.index
			<< std::setw(7) << dt.numOfID
			<< std::setw(13) << dt.type
			<< std::setw(13) << dt.block;
		return os;
	}

};

struct checkConVal {

	bool flag;
	string val;

	checkConVal(string v, bool i)
		:val(v), flag(i)
	{}

	checkConVal(const checkConVal& other)
		:val(other.val), flag(other.flag)
	{}

	checkConVal& operator= (const checkConVal& other)
	{
		if (this != &other)
		{
			val = other.val;
			flag = other.flag;
		}
		return *this;
	}

	checkConVal(checkConVal&& other)
		:val(std::move(other.val)), flag(other.flag)
	{}

	~checkConVal() {}
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
	bool unexpectedBehaviorBeforeDot = false;
	bool unexpectedBehaviorAfterDot = false;
	bool unexpectedBehaviorBeforeDoubleSign = false;
	bool numStartFromNull = false;
	bool unexpectedSignAfterDot = false;
	bool unexpectedSignAfterSign = false;

	FLAGS()
		: unexpectedIdVal(false),
		unexpectedBehaviorBeforeDot(false),
		unexpectedBehaviorAfterDot(false),
		unexpectedSignAfterDot(false),
		unexpectedBehaviorBeforeDoubleSign(false),
		numStartFromNull(false),
		unexpectedSignAfterSign(false)
	{}

	void SET_FALSE_ALL()
	{
		unexpectedIdVal = false;
		unexpectedBehaviorBeforeDot = false;
		unexpectedBehaviorAfterDot = false;
		unexpectedBehaviorBeforeDoubleSign = false;
		numStartFromNull = false;
		unexpectedSignAfterSign = false;
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
