#pragma once
#include<string>
#include<vector>
#include<iostream>
#include<iomanip>
#include<stack>

using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::stack;

struct mymap
{
	string val;
	int index;

	mymap(string v, int i)
		:val(v), index(i)
	{}

	mymap(mymap&& other)
		:val(std::move(other.val)), index(other.index)
	{}
	~mymap() {}

};

struct allLexem
{
	string val;
	int index;
	int numOfLine;
	int ID;
	string type;
	int block;

	allLexem()
		:val(""),
		index(),
		numOfLine(),
		ID(),
		type(""),
		block()
	{}

	allLexem(string v, int i, int numLine, int IID, string t, int b)
		:val(v),
		index(i),
		numOfLine(numLine),
		ID(IID),
		type(t),
		block(b)
	{}

	allLexem(const allLexem& other)
		:val(other.val),
		index(other.index),
		numOfLine(other.numOfLine),
		ID(other.ID),
		type(other.type),
		block(other.block)
	{}
	allLexem& operator= (const allLexem& other)
	{
		if (this != &other) // защита от неправильного самоприсваивания
		{
			val = other.val;
			index = other.index;
			numOfLine = other.numOfLine;
			ID = other.ID;
			type = other.type;
			block = other.block;
		}
		return *this;
	}

	allLexem(allLexem&& other)
		:val(std::move(other.val)), index(other.index), numOfLine(other.numOfLine), ID(other.ID),
		type(std::move(other.type)), block(other.block)
	{}

	friend std::ostream& operator<< (std::ostream& os, const allLexem& dt)
	{
		os << std::setw(5) << dt.numOfLine << std::setw(22) << dt.val << std::setw(7)
			<< dt.index << std::setw(7)
			<< dt.ID << std::setw(13) << dt.type << std::setw(13) << dt.block;
		return os;
	}

	~allLexem() {}

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
		if (this != &other) // защита от неправильного самоприсваивания
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
		:val(v), line(l), mess(m)
	{}

	Exept(const Exept& other)
		: mess(other.mess)
		, line(other.line)
		, val(other.val)
	{
	}
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

	//move
	Exept(Exept&& other)
		:val(std::move(other.val)), line(other.line), mess(std::move(other.mess))
	{}
	//Exept& operator= (Exept&& other) = default;
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
		: thisState( a )
		, indexThis( b )
		, nextState( c )
		, finishState( d )
		, messError(e)
	{
	}

	MPAMemento( const MPAMemento& other )
		: thisState( other.thisState )
		, indexThis( other.indexThis )
		, nextState( other.nextState )
		, finishState( other.finishState )
		, messError(other.messError)
	{
	}

	MPAMemento( MPAMemento&& other )
		: thisState( other.thisState )
		, indexThis( other.indexThis )
		, nextState( other.nextState )
		, finishState( other.finishState )
		, messError( std::move( other.messError ) )
	{
	}

	friend std::ostream& operator<< ( std::ostream& os, const MPAMemento& dt )
	{
		os << std::setw( 5 ) << dt.thisState << std::setw( 5 ) << dt.indexThis << std::setw( 5 )
			<< dt.nextState << std::setw( 5 ) << dt.finishState << std::setw( 30 ) << dt.messError;
		return os;
	}

	~MPAMemento() {}

};
