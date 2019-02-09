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
	int numOfLine = 1;

	int countBracket1 = 0; // like ()   |   0 is true else false
	int countBracket2 = 0; // like []   |   0 is true else false
	int countBracket3 = 0; // like {}   |   0 is true else false

	for (i = 0; i < length; i++)
	{
		stateComment();

		if ( m_buff.at(i) == '\n' )
		{
			numOfLine++;
			continue;
		}

		stateLetters();
		stateInt();

		//unexpected behavior after and before
		if (m_buff.at(i) == '.' && i + 1 != length && m_buff.at(i + 1) == '.')
		{
			if (storage != "")
				stateAddLexem();
			storage = "..";
			stateAddLexem();
			i++;
			continue;
		}
		else
		{
			if (m_buff.at(i) == '.' && i + 1 != length && m_buff.at(i + 1) != '.')
			{
				for (const char& item : storage)
				{
					if (!isNum(item))
					{
						flag.unexpectedBehaviorBeforeDot = true;
						break;
					}
				}
				if (storage.length() == 0) storage += '0';
				storage += m_buff.at(i);
				i++;
				if (i != length && isNum(m_buff.at(i)))
				{
					stateInt();
					stateAddLexem();
					storage = "";
				}
				else 
				{
					if (i != length && !isLeter(m_buff.at(i)))
					{
						storage += '0';
						stateAddLexem();
						storage = "";
					}
					else
					{
						flag.unexpectedBehaviorAfterDot = true;
						continue;
					}
				}
			}
			
		}

		if (m_buff.at(i) == '=')
		{
			stateDoubleSign('=');
			continue;
		}
		if (m_buff.at(i) == '<')
		{
			stateDoubleSign('<');
			continue;
		}
		if (m_buff.at(i) == '>')
		{
			stateDoubleSign('>');
			continue;
		}
		if (m_buff.at(i) == '!')
		{
			stateDoubleSign('!');
			continue;
		}
		if (isSign(m_buff.at(i)) && i + 1 != length 
			&& (m_buff.at(i + 1) != '(' || m_buff.at(i + 1) != ')' || m_buff.at(i + 1) != '{'
				||m_buff.at(i + 1) != '}' || !isSignDuplicate(m_buff.at(i + 1)))) {
			const char sign = m_buff.at(i);
			if (storage != "")
				stateAddLexem();
			storage = sign;
			stateAddLexem();
			if (sign == '(')
				countBracket1++;
			if (sign == ')')
				countBracket1--;
			if (sign == '[')
				countBracket2++;
			if (sign == ']')
				countBracket2--;
			if (sign == '{')
				countBracket3++;
			if (sign == '}')
				countBracket3--;

			storage = "";
			continue;
		}
		else
		{
			if (isSign(m_buff.at(i))  && i + 1 != length)
			{
				if (storage != "")
					stateAddLexem();
				storage = m_buff.at(i);
				i++;
				while (i != length && !isNum(m_buff.at(i)) && !isLeter(m_buff.at(i)) && m_buff.at(i) != ' ' && m_buff.at(i) != '.')
				{
					storage += m_buff.at(i);
					i++;
				}
				i--;
				flag.unexpectedSignAfterSign = true;
				stateAddLexem();
				storage = "";
				continue;
			}
		}
		if (m_buff.at(i) == ';') {
			if (storage != "")
				stateAddLexem();
			storage = ";";
			stateAddLexem();
			if (countBracket1 < 0)
			{
				exept.emplace_back("", numOfLine, "unexpected amount of (");
				countBracket1 = 0;
			}
			if (countBracket1 > 0)
			{
				exept.emplace_back("", numOfLine, "unexpected amount of )");
				countBracket1 = 0;
			}
			if (countBracket2 < 0)
			{
				exept.emplace_back("", numOfLine, "unexpected amount of [");
				countBracket2 = 0;
			}
			if (countBracket2 > 0)
			{
				exept.emplace_back("", numOfLine, "unexpected amount of ]");
				countBracket2 = 0;
			}

			storage = "";
			continue;
		}
		if (m_buff.at(i) == ' ') {
			i++;
			while (i != length && ( m_buff.at(i) == ' ' || m_buff.at(i) == '\t'))
			{
				i++;
			}
			i--;
			if(storage != "")
				stateAddLexem();
			storage = "";
			continue;
		}

	}//for


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

bool LexicalAnalyzer::isLeter(const char & sign) const
{
	for (const char& item : m_alphabetEN)
		if (item == sign) return true;
	return false;
}

bool LexicalAnalyzer::isNum(const char & sign) const
{
	for (const char& item : m_num)
		if (item == sign) return true;
	return false;
}

bool LexicalAnalyzer::isSign(const char & sign) const
{
	for (const char& item : m_sign)
		if (item == sign) return true;
	return false;
}

bool LexicalAnalyzer::isSignDuplicate(const char & sign) const
{
	for (const char& item : m_signDuplicate)
		if (item == sign) return true;
	return false;
}

checkConVal LexicalAnalyzer::isConVal(const string & val, FLAGS& flag) const
{
	checkConVal checkObj("", false);
	bool fl1 = false; // dot
	const size_t len = val.length();
	
	for (size_t i = 0; i < len; i++)
	{
		if (val.at(i) == '.')
		{
			checkObj.val += "double";
			fl1 = true;
			continue;
		}
		if (!isNum(val.at(i)))
		{
			checkObj.flag = false;
			checkObj.val = "";
			return checkObj;
		}
	}
		
	if (fl1 == false) checkObj.val += "int";
	if (val.at(0) == '0' && fl1 == false && val.length() != 1)
	{
		flag.numStartFromNull = true;
	}
	checkObj.flag = true;

	return checkObj;
}

string LexicalAnalyzer::whichTypeID() const
{
	for (auto it = m_allLexem.rbegin(); it != m_allLexem.rend(); ++it)
	{
		if (it->index == 17 || it->index == 18 || it->index == 19 || it->index == 20 || it->index == 21) break;
		if (it->index == 40 || it->index == 39 || it->index == 27 || it->index == 28)
		{
			continue;
		}
		if(it->index == 1 || it->index == 2 || it->index == 3 || it->index == 4 )
			return it->val;
		else return "";
	}
	return "";
}

int LexicalAnalyzer::isDeclarationID(const string & val) const
{
	for (const auto& item : m_allLexem)
	{
		if (item.val == val) return item.numOfID;
	}
	
	return -1;
}

void LexicalAnalyzer::stateInt()
{
	bool flagqwerty = false;
	while (i != length && isNum(m_buff.at(i)))
	{
		flagqwerty = true;
		storage += m_buff.at(i);
		i++;
	}
	if (flagqwerty) i--;
}

void LexicalAnalyzer::stateLetters()
{
	bool flagqwerty = false;
	if (i != 0 && isNum(m_buff.at(i - 1)) && isLeter(m_buff.at(i)))
		flag.unexpectedIdVal = true;
	
	while (i != length && isLeter(m_buff.at(i)))
	{
		flagqwerty = true;
		storage += m_buff.at(i);
		++i;
	}
	if(flagqwerty) --i;
}

void LexicalAnalyzer::stateComment()
{
	if (m_buff.at(i) == '/' &&  i + 1 != length && m_buff.at(i + 1) == '/')
	{
		i += 2;
		while (i != length && m_buff.at(i) != '\n') i++;
	}
}

void LexicalAnalyzer::stateAddLexem()
{
	
	if (flag.unexpectedIdVal == true)
	{
		exept.emplace_back(storage, numOfLine, "unexpected id val");
		m_allLexem.emplace_back(storage, 39, numOfLine, 0, "invalid", 0);
		storage = "";
		flag.SET_FALSE_ALL();
		return;
	}

	if (flag.unexpectedBehaviorBeforeDoubleSign == true)
	{
		exept.emplace_back(storage, numOfLine, "unexpected behavior in double sign");
		m_allLexem.emplace_back(storage, 0, numOfLine, 0, "invalid", 0);
		storage = "";
		flag.SET_FALSE_ALL();
		return;
	}
	
	if (flag.unexpectedSignAfterSign == true)
	{
		exept.emplace_back(storage, numOfLine, "unexpected behavior after sign");
		m_allLexem.emplace_back(storage, 0, numOfLine, 0, "invalid", 0);
		storage = "";
		flag.SET_FALSE_ALL();
		return;
	}

	for (size_t j = 0; j < m_lexem.size(); ++j)
	{
		if (storage == m_lexem.at(j).val)
		{
			m_allLexem.emplace_back(storage, m_lexem.at(j).index, numOfLine, 0, "", 0);
			storage = "";
			flag.SET_FALSE_ALL();
			return;
		}
	}

	if (flag.unexpectedBehaviorAfterDot == true)
	{
		exept.emplace_back(storage, numOfLine - 1, "unexpected behavior after dot");
	}
	if (flag.unexpectedBehaviorBeforeDot == true)
	{
		exept.emplace_back(storage, numOfLine, "unexpected behavior before dot");
	}

	checkConVal temp = isConVal(storage, flag);
	if (temp.flag == true && flag.numStartFromNull == true) {
		m_allLexem.emplace_back(storage, 40, numOfLine, 0, "invalid", 0);
		exept.emplace_back(storage, numOfLine, "number start from 0");
		flag.SET_FALSE_ALL();
		storage = "";
		return;
	}
	if (temp.flag == true && flag.numStartFromNull == false) {
		numOfConstVal++;
		m_allLexem.emplace_back(storage, 40, numOfLine, numOfConstVal, temp.val, 0);
		storage = "";
		flag.SET_FALSE_ALL();
		return;
	}

	AllLexem test = whichID(storage, corentBlock);
	const string check = whichTypeID();
	int block = countBlock();
	if (test.numOfID == -1 ) {
		if (check == "uint" || check == "int" || check == "udouble" || check == "double")
		{
			numOfIdentifier++;
			if (corentBlock <= block) corentBlock = block;
			m_allLexem.emplace_back(storage, 39, numOfLine, numOfIdentifier, check, block);
			flag.SET_FALSE_ALL();
			storage = "";
			return;
		}
		else
		{
			if (check == "")
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
		if (check == "uint" || check == "int" || check == "udouble" || check == "double")
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
}

void LexicalAnalyzer::stateDoubleSign(const char & sign)
{

	if (m_buff.at(i) == sign && i + 2 != length && m_buff.at(i + 1) == '=' && (m_buff.at(i + 2) == '-' || !isSignDuplicate(m_buff.at(i + 2))))
	{
		if (storage != "")
			stateAddLexem();
		storage = sign;
		storage += "=";
		stateAddLexem();
		storage = "";
		i++;
		return;
	}
	else
	{
		if (m_buff.at(i) == sign && i < length)
		{
			if (storage != "")
				stateAddLexem();
			storage = sign;
			stateAddLexem();
			storage = "";
			return;
		}
		if (m_buff.at(i) == sign && i + 1 != length
			&& (
				isNum(m_buff.at(i + 1)) || m_buff.at(i + 1) == '.' || isLeter(m_buff.at(i + 1)) || m_buff.at(i + 1) == ' '
				)
			)
		{
			if (storage != "")
				stateAddLexem();
			storage = sign;
			stateAddLexem();
			storage = "";
			return;
		}
		else
		{
			if (m_buff.at(i) == sign && i + 1 != length)
			{
				flag.unexpectedBehaviorBeforeDoubleSign = true;
				if (storage != "")
					stateAddLexem();
				storage = sign;
				i++;
				while (i != length && !isNum(m_buff.at(i)) && !isLeter(m_buff.at(i)) && m_buff.at(i) != ' ' && m_buff.at(i) != '.')
				{
					storage += m_buff.at(i);
					i++;
				}
				i--;
				
				stateAddLexem();
				storage = "";
				return;
			}
		}
	}
}

AllLexem LexicalAnalyzer::whichID(const string & val, int& block) const
{
	AllLexem temp;
	int countBlock = 1;
	
	for (auto it = m_allLexem.begin(); it != m_allLexem.end(); ++it)
	{
		if (it->index == 20)
		{
			countBlock++;
			continue;
		}
		if (it->val == val && it->index == 39 && it->block == block)
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
	for (auto it = m_allLexem.begin(); it != m_allLexem.end(); ++it)
	{
		if (it->index == 20)
		{
			countBlock++;
		}
	}
	return countBlock;
}

bool LexicalAnalyzer::WasWriteToFileLexem(const std::vector<AllLexem> &out, const int& test, const int& bl) const
{
	for (const auto& item : out)
	{
		if (item.numOfID == test && item.block == bl) return true;
	}
	return false;
}

void LexicalAnalyzer::isCorrectSigns()
{

	int numOfLine = 1;
	string unexpectedSign;
	for (const char & sign : m_buff)
	{
		if (sign == '\n')
		{
			numOfLine+=1;
			continue;
		}
		if (sign == ' ' || sign == ';' || sign == '\t'
			|| isSign(sign) || isNum(sign) || isLeter(sign)
			|| sign == '!' || sign == '<' || sign == '>' || sign == '=' || sign == '.')
		{
			continue;
		}
		else
		{
			unexpectedSign += sign;
			exept.emplace_back( unexpectedSign, numOfLine, "unexpected sign");
			unexpectedSign = "";
		}
	}
	
}

void LexicalAnalyzer::writeAllToFile() const
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

void LexicalAnalyzer::writeLexemToFile() const
{
	//remove("out.txt");
	std::ofstream fout("./Result/Variables.txt");
	std::vector<AllLexem> out;
	fout << "  Num Line" << "          Lexem     " << "  Index" << "   Id/Con" << "      Type      " << " Block" << std::endl;
	for (size_t i = 0; i < m_allLexem.size(); i++)
	{

		if (m_allLexem.at(i).index == 39 && !WasWriteToFileLexem(out, m_allLexem.at(i).numOfID, m_allLexem.at(i).block))
		{
			out.push_back(m_allLexem.at(i));
			fout << m_allLexem.at(i) << std::endl;
		}
	}

	fout.close();
}

void LexicalAnalyzer::writeConstToFile() const
{
	std::ofstream fout("./Result/Constants.txt");
	fout << "  Num Line" << "          Lexem     " << "  Index" << "   Id/Con" << "      Type      " << " Block" << std::endl;
	for (size_t i = 0; i < m_allLexem.size(); i++)
	{
		if (m_allLexem.at(i).index == 40)
		{
			fout << m_allLexem.at(i) << std::endl;
		}
	}

	fout.close();
}
