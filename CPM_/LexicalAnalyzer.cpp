#include "LexicalAnalyzer.h"

LexicalAnalyzer::LexicalAnalyzer(const string& buff)
	: storage("")
	, m_buff(buff)
	, corentBlock(1)
	, length(buff.length())
	, i(0)
	, numOfIdentifier(0)
	, numOfConstVal(0)
	, numOfLine(1)
{
	try
	{
		std::ifstream fin("./Source/ReserveLexem.txt");
		if (fin.is_open())
		{
			string reader;
			int i = 1;
			while ( !fin.eof() )
			{
				getline(fin, reader);
				if(reader.size() != 0)
					m_lexem.emplace_back(reader, i);
				i+=1;
			}

			isCorrectSigns();

			if ( exept.size() > 0 )
				m_buff.~basic_string();

			fin.close();
		}
		else
		{
			exept.emplace_back("", 0, "invalid behavior: cant find or/and open file with reserve lexems: name file must be 'ReserveLexem.txt' in './Source/'");
			m_buff.~basic_string();
		}
	}
	catch ( std::exception& e )
	{
		cerr << e.what() << endl;
	}
}

LexicalAnalyzer::~LexicalAnalyzer()
{}

void LexicalAnalyzer::ViewLogs()
{
	try
	{
		int ii = 0;
		std::ofstream fout("./Logs/LexicalLogsTypeA.txt");
		for ( const Exept& item : exept )
		{
			fout << ++ii << "	In line " << item.line << " you have " << item.mess << "   '" << item.val << "'" << std::endl;
		}
		fout.close();
	}
	catch ( std::exception& e )
	{
		cerr << e.what() << endl;
	}
}

void LexicalAnalyzer::StartProcessing() 
{
	int countBracket1 = 0; // like ()   |   0 is true else false
	int countBracket2 = 0; // like []   |   0 is true else false
	int countBracket3 = 0; // like {}   |   0 is true else false
	
	for (i = 0; i < length; i++)
	{
		if ( stateComment() ) continue;
		if ( stateNewLine() ) continue;
		if ( stateLetters() ) continue;
		if ( stateInt() )     continue;
		if ( stateDot() )     continue;
		if ( stateSign(countBracket1, countBracket2, countBracket3) ) continue;
	}

	//update!!!!!!!!!!!!!!!
	if (countBracket3 < 0)
	{
		exept.emplace_back("", 0, "unexpected amount of {");
		countBracket3 = 0;
	}
	if (countBracket3 > 0)
	{
		exept.emplace_back("", 0, "unexpected amount of }");
		countBracket3 = 0;
	}

}


///------------------------------< Check >------------------------------------------------------
bool LexicalAnalyzer::isLeter(const char & sign) const
{
	for ( const char& item : m_alphabetEN )
		if ( item == sign ) return true;
	return false;
}

bool LexicalAnalyzer::isNum(const char & sign) const
{
	for ( const char& item : m_num )
		if ( item == sign ) return true;
	return false;
}

bool LexicalAnalyzer::isSign(const char & sign) const
{
	for ( const char& item : m_sign )
		if ( item == sign ) return true;
	return false;
}

void LexicalAnalyzer::isCorrectSigns()
{
	int numOfLine__ = 1;
	string unexpectedSign;

	for ( const char & sign : m_buff )
	{
		if ( sign == '\n' )
		{
			numOfLine__ += 1;
			continue;
		}
		if ( sign == ' ' || sign == ';' || sign == '\t'
			|| isSign(sign) || isNum(sign) || isLeter(sign)
			|| sign == '!' || sign == '<' || sign == '>' || sign == '=' || sign == '.' )
		{
			continue;
		}
		else
		{
			unexpectedSign += sign;
			exept.emplace_back(unexpectedSign, numOfLine__, "unexpected sign");
			unexpectedSign = "";
		}
	}

}
///-----------------------------</ Check >------------------------------------------------------


///------------------------------< States >-----------------------------------------------------
bool LexicalAnalyzer::stateInt()
{
	bool exitCode = false;
	while ( i != length && isNum(m_buff.at(i)) )
	{
		exitCode = true;
		storage += m_buff.at(i);
		i += 1;
	}
	return exitCode;
}

bool LexicalAnalyzer::stateLetters()
{
	bool exitCode = false;
	if ( i != 0 && isNum(m_buff.at(i - 1)) && isLeter(m_buff.at(i)) )
		flag.unexpectedIdVal = true;

	while ( i != length && isLeter(m_buff.at(i)) )
	{
		exitCode = true;
		storage += m_buff.at(i);
		i += 1;
	}

	return exitCode;
}

bool LexicalAnalyzer::stateComment()
{
	if ( m_buff.at(i) == '/' &&  i + 1 != length && m_buff.at(i + 1) == '/' )
	{
		i += 2;
		while ( i != length && m_buff.at(i) != '\n' ) i++;
		return true;
	}
	return false;
}

bool LexicalAnalyzer::stateNewLine()
{
	if ( m_buff.at(i) == '\n' )
	{
		numOfLine++;
		return true;
	}
	return false;
}

bool LexicalAnalyzer::stateDot()
{
	bool exitCode = false;

	if ( m_buff.at(i) == '.' && i + 1 != length && m_buff.at(i + 1) == '.' )
	{
		if ( storage != "" )
			stateAddLexem();
		storage = "..";
		stateAddLexem();
		i++;
		exitCode = true;
	}
	else
	{
		if ( m_buff.at(i) == '.' && i + 1 != length && m_buff.at(i + 1) != '.' )
		{
			exitCode = true;
			for ( const char& item : storage )
			{
				if ( !isNum(item) )
				{
					flag.unexpectedBehaviorBeforeDot = true;
					break;
				}
			}
			if ( storage.length() == 0 ) storage += '0';
			storage += m_buff.at(i);
			i++;
			if ( i != length && isNum(m_buff.at(i)) )
			{
				stateInt();
			}
			else
			{
				if ( !isLeter(m_buff.at(i)) )
				{
					storage += '0';
					stateAddLexem();
				}
				else
				{
					flag.unexpectedBehaviorAfterDot = true;
				}
			}
		}

	}
	return exitCode;
}

bool LexicalAnalyzer::stateSign(int& countBracket1, int& countBracket2, int& countBracket3)
{
	bool exitCode = false;
	const char sign = m_buff.at(i);

	switch ( sign )
	{
	case ' ':
	case '\t':
	{
		exitCode = true;
		if ( storage != "" )
			stateAddLexem();
		i++;
		while ( i != length && ( m_buff.at(i) == ' ' || m_buff.at(i) == '\t' ) )
		{
			i++;
		}
		i--;
		break;
	}
	case ';':
	{
		exitCode = true;
		if ( storage != "" )
			stateAddLexem();
		storage = ";";
		stateAddLexem();

		if ( countBracket1 < 0 )
		{
			exept.emplace_back("", numOfLine, "unexpected amount of (");
			countBracket1 = 0;
		}
		else if ( countBracket1 > 0 )
		{
			exept.emplace_back("", numOfLine, "unexpected amount of )");
			countBracket1 = 0;
		}
		if ( countBracket2 < 0 )
		{
			exept.emplace_back("", numOfLine, "unexpected amount of [");
			countBracket2 = 0;
		}
		else if ( countBracket2 > 0 )
		{
			exept.emplace_back("", numOfLine, "unexpected amount of ]");
			countBracket2 = 0;
		}
		break;
	}
	case '=': exitCode = true; stateDoubleSign('='); break;
	case '<': exitCode = true; stateDoubleSign('<'); break;
	case '>': exitCode = true; stateDoubleSign('>'); break;
	case '!': exitCode = true; stateDoubleSign('!'); break;
	default:
	{
		if ( isSign(sign) )
		{
			exitCode = true;
			if ( storage != "" )
				stateAddLexem();
			storage = sign;
			stateAddLexem();
			switch ( sign )
			{
			case '(': countBracket1++; break;
			case ')': countBracket1--; break;
			case '[': countBracket2++; break;
			case ']': countBracket2--; break;
			case '{': countBracket3++; break;
			case '}': countBracket3--; break;
			}
		}
	}
	}

	return exitCode;
}

void LexicalAnalyzer::stateDoubleSign(const char & sign)
{
	if ( storage != "" )
		stateAddLexem();

	storage = sign;
	if ( i + 1 != length && m_buff.at(i + 1) == '=' )
	{
		storage += "=";
		++i;
	}
	stateAddLexem();
}
///----------------------------</ States >------------------------------------------------------


///---------------------------< AddLExem >------------------------------------------------------
void LexicalAnalyzer::stateAddLexem()
{
	if ( flag.unexpectedIdVal == true )
	{
		exept.emplace_back(storage, numOfLine, "unexpected id val");
		m_allLexem.emplace_back(storage, 39, numOfLine, 0, "invalid", 0, ReservedName::_Ind);
		storage = "";
		flag.SET_FALSE_ALL();
		return;
	}

	if ( flag.unexpectedBehaviorAfterDot == true )
	{
		exept.emplace_back(storage, numOfLine, "unexpected behavior after dot, expected ';'");
	}

	if ( flag.unexpectedBehaviorBeforeDot == true )
	{
		exept.emplace_back(storage, numOfLine, "unexpected behavior before dot, expected ';'");
	}

	for(const auto& item: m_lexem )
	{
		if ( storage == item.val )
		{
			m_allLexem.emplace_back(storage, item.index, numOfLine, 0, "", 0, whichAlias(item.index));
			storage = "";
			flag.SET_FALSE_ALL();
			return;
		}
	}

	///----------------------------------------const value --------------------------------------------
	ConVal isConstVal = isConVal();

	if ( isConstVal.flag == true && flag.numStartFromNull == true )
	{
		m_allLexem.emplace_back(storage, 40, numOfLine, 0, "invalid", 0, ReservedName::_Con);
		exept.emplace_back(storage, numOfLine, "unexpected const value, cannot start from 0");
		flag.SET_FALSE_ALL();
		storage = "";
		return;
	}
	if ( isConstVal.flag == true && flag.numStartFromNull == false )
	{
		numOfConstVal++;
		m_allLexem.emplace_back(storage, 40, numOfLine, numOfConstVal, isConstVal.val, 0, ReservedName::_Con);
		storage = "";
		flag.SET_FALSE_ALL();
		return;
	}
	///----------------------------------------const value --------------------------------------------


	///------------------------------------------value-------------------------------------------------
	AllLexem test = whichID(storage, corentBlock);
	const string check = whichTypeID();
	int block = countBlock();
	if ( test.numOfID == -1 )
	{
		if ( check == "uint" || check == "int" || check == "udouble" || check == "double" )
		{
			numOfIdentifier++;
			if ( corentBlock <= block ) corentBlock = block;
			m_allLexem.emplace_back(storage, 39, numOfLine, numOfIdentifier, check, block);
			flag.SET_FALSE_ALL();
			storage = "";
			return;
		}
		else
		{
			if ( check == "" )
			{
				numOfIdentifier++;
				m_allLexem.emplace_back(storage, 39, numOfLine, numOfIdentifier, "", test.block);
				storage = "";
				flag.SET_FALSE_ALL();
				return;
			}
		}
	}
	else
	{
		if ( check == "uint" || check == "int" || check == "udouble" || check == "double" )
		{
			numOfIdentifier++;
			m_allLexem.emplace_back(storage, 39, numOfLine, numOfIdentifier, check, block);
			flag.SET_FALSE_ALL();
			storage = "";
			return;
		}
		else
		{
			m_allLexem.emplace_back(storage, 39, numOfLine, test.numOfID, test.type, test.block);
			flag.SET_FALSE_ALL();
			storage = "";
			return;
		}
	}

	///------------------------------------------value-------------------------------------------------
}

ConVal LexicalAnalyzer::isConVal()
{
	ConVal checkObj("", false);
	bool fl1 = false; // dot
	const size_t len = storage.length();

	for ( size_t i = 0; i < len; i++ )
	{
		if ( storage.at(i) == '.' )
		{
			checkObj.val += "double";
			fl1 = true;
			continue;
		}
		if ( !isNum(storage.at(i)) )
		{
			checkObj.flag = false;
			checkObj.val = "";
			return checkObj;
		}
	}

	if ( fl1 == false ) checkObj.val += "int";
	if ( storage.at(0) == '0' && fl1 == false && len != 1 )
	{
		flag.numStartFromNull = true;
	}
	checkObj.flag = true;

	return checkObj;
}

string LexicalAnalyzer::whichTypeID() const
{
	for ( auto it = m_allLexem.rbegin(); it != m_allLexem.rend(); ++it )
	{
		if ( it->index == 17 || it->index == 18 || it->index == 19 || it->index == 20 || it->index == 21 ) break;
		if ( it->index == 40 || it->index == 39 || it->index == 27 || it->index == 28 )
		{
			continue;
		}
		if ( it->index == 1 || it->index == 2 || it->index == 3 || it->index == 4 )
			return it->val;
		else return "";
	}
	return "";
}

int LexicalAnalyzer::isDeclarationID(const string & val) const
{
	for ( const auto& item : m_allLexem )
	{
		if ( item.val == val ) return item.numOfID;
	}

	return -1;
}

AllLexem LexicalAnalyzer::whichID(const string & val, int& block) const
{
	AllLexem temp;
	int countBlock = 1;

	for ( auto it = m_allLexem.begin(); it != m_allLexem.end(); ++it )
	{
		if ( it->index == 20 )
		{
			countBlock++;
			continue;
		}
		if ( it->val == val && it->index == 39 && it->block == block )
		{
			temp.numOfID = it->numOfID;
			temp.index = it->index;
			temp.numOfLine = it->numOfLine;
			temp.type = it->type;
			temp.val = it->val;
			temp.block = it->block;
			return temp;
		}
	}

	temp.numOfID = -1;
	temp.block = countBlock;
	return temp;
}

int LexicalAnalyzer::countBlock() const
{
	int countBlock = 1;
	for ( auto it = m_allLexem.begin(); it != m_allLexem.end(); ++it )
	{
		if ( it->index == 20 )
		{
			countBlock++;
		}
	}
	return countBlock;
}
///---------------------------</ AddLExem >-----------------------------------------------------


bool LexicalAnalyzer::wasWriteToFileLexem(const std::vector<AllLexem>& out, const int& test, const int& bl) const
{
	for (const auto& item : out)
	{
		if (item.numOfID == test && item.block == bl) return true;
	}
	return false;
}

ReservedName LexicalAnalyzer::whichAlias(const int& ali)
{
	switch (ali)
	{
	case 1: return ReservedName::_int; break;
	case 2: return ReservedName::_double; break;
	case 3: return ReservedName::_uint; break;
	case 4: return ReservedName::_udouble; break;
	case 5: return ReservedName::_in; break;
	case 7: return ReservedName::_out; break;
	case 9: return ReservedName::_while; break;
	case 10: return ReservedName::_do; break;
	case 11: return ReservedName::_if; break;
	case 12: return ReservedName::_return; break;
	case 14: return ReservedName::_endl; break;
	case 16: return ReservedName::_main; break;
	case 6:
	case 8:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38: return ReservedName::_operator; break;
	default:
		return ReservedName::_none; break;
	}
}

void LexicalAnalyzer::WriteAllToFile() const
{
	//remove("__out__.txt");
	std::ofstream fout("./Result/Lexems.txt");
	fout << "Iter |" << "  Num Line" << "          Lexem     " << "  Index" << "   Id/Con" << "      Type      " << " Block" <<  std::endl;
	for (size_t i = 0; i < m_allLexem.size(); i++)
	{
		fout << i + 1 << ((i + 1) >= 1000 ? " |  " : ((i + 1) >= 100 ? "  |  " : ((i + 1) >= 10 ? "   |  " : "    |  ")));
		fout << m_allLexem.at(i) << std::endl;
	}
	
	fout.close();
}

void LexicalAnalyzer::WriteLexemToFile() const
{
	//remove("out.txt");
	std::ofstream fout("./Result/Variables.txt");
	std::vector<AllLexem> out;
	fout << "  Num Line" << "          Lexem     " << "  Index" << "   Id/Con" << "      Type      " << " Block" << std::endl;
	for (size_t i = 0; i < m_allLexem.size(); i++)
	{

		if (m_allLexem.at(i).index == 39 && !wasWriteToFileLexem(out, m_allLexem.at(i).numOfID, m_allLexem.at(i).block))
		{
			out.push_back(m_allLexem.at(i));
			fout << m_allLexem.at(i) << std::endl;
		}
	}

	fout.close();
}

void LexicalAnalyzer::WriteConstToFile() const
{
	std::ofstream fout("./Result/Constants.txt");
	fout << "  Num Line" << "          Lexem     " << "  Index" << "   Id/Con" << "      Type      " << " Block" << std::endl;
	for ( const AllLexem& item : m_allLexem )
	{
		if ( item.index == 40 )
		{
			fout << item << std::endl;
		}
	}

	fout.close();
}
