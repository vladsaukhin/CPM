#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<AllLexem>& vectorlexem)
	: m_lexem(vectorlexem)
{}

SyntaxAnalyzer::~SyntaxAnalyzer()
{}

void SyntaxAnalyzer::StartProcessing()
{

	int getIndex = 0;
	int index = -1;
	bool exitBool = false;
	vector<blockAddapter> myStack;
	stack<int> stk;
	int countLeftBracket = 1;
	string temp;

	bool flagBlockBeforeMain = false;
	bool flagMainBlock = false;
	for (const auto& i : m_lexem)
	{
		if (i.index == 20 || i.index == 21)
		{
			flagBlockBeforeMain = true;
			continue;
		}
		if (i.index == 16 && flagBlockBeforeMain)
		{
			exept.emplace_back("it in Source.pcm", 0,
				"unexpected value[declaration functions]: before the main unit you can only declare, but not realize");
			cerr << "unexpected value[declaration functions]: before the main unit you can only declare, but not realize" << endl;
			return;
		}
		if (i.index == 16) {
			flagMainBlock = true;
			break;
		}
	}

	if (!flagMainBlock)
	{
		exept.emplace_back("it in Source.pcm", 0,
			"unexpected value[main]: can't start running without main block !!!");
		cerr << "unexpected value[main]: can't start running without main block !!!" << endl;
		return;
	}

	while (true)
	{
		index += 1;
		if (index < m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			cout << index << "index ending  " << endl;
			break;
		}

		switch (getIndex)
		{
		case 1: // 'int'
		case 2: // 'double'
		case 3: // 'uint'
		case 4: // 'udouble'
		{
			int s = exept.size();
			declarationLexem(index);
			if(s == exept.size())
				index -= 1;
			break;
		}
		case 20: // '{'
		{
			int li = 0;
			temp = whichState(index, li);
			myStack.emplace_back(countLeftBracket, temp, li, 20);
			//stk.push(20);
			countLeftBracket += 1;
			break;
		}
		case 21: // '}'
		{
			int i = myStack.size() - 1;
			while (myStack.size() > 2 && myStack.at(i).id == 21)
			{	
				i -= 1;
			}
			//if (myStack.size() > 2 && myStack.at(i).block == myStack.at(i + 1).block)
				//i -= 1;
			myStack.emplace_back(myStack.at(i).block,
				myStack.at(i).whichBlock,
				myStack.at(i).line, 21);
			break;
		}
		case 16: // 'main'
		{

			index += 1;
			if (index != m_lexem.size())
				getIndex = m_lexem.at(index).index;
			if (getIndex == 20)
			{
				index -= 1;
				continue;
			}
			else
			{
				exeptAndSkip(index,"unexpected value [declaration main]: expected '{'");
				break;
			}
		
			break;
		}
		case 39: // 'ID'
		{
			index += 1;
			if (index != m_lexem.size())
				getIndex = m_lexem.at(index).index;
			else
			{
				exeptAndSkip(index, "unexpected value [work with identifier]: expected ';'");
				break;
			}
			if (getIndex == 28 || getIndex == 17 || getIndex == 18)
			{
				if (getIndex == 28)
				{
					index += 1;
					if (index != m_lexem.size())
						getIndex = m_lexem.at(index).index;
					else
					{
						exeptAndSkip(index, "unexpected value [choese bihavior after =]: expected identifier");
						break;
					}
					if (getIndex == 39)
					{
						index += 1;
						if (index != m_lexem.size())
							getIndex = m_lexem.at(index).index;
						else
						{
							exeptAndSkip(index, "unexpected value [choese bihavior after =]: expected '('");
							break;
						}
						if (getIndex == 18)
						{
							callState(index);
							break;
						}
						else
							index -= 2;
					}
					else
						index -= 1;
					assignmentState(index);
				}
				if (getIndex == 18)
					callState(index);
				break;
			}
			else
			{
				exeptAndSkip(index, "unexpected value [work with identifier]: expected ';'");
				break;
			}
			break;
		}
		case 12: // 'return'
		{
			index += 1;
			if (index != m_lexem.size())
				getIndex = m_lexem.at(index).index;
			else
			{
				exeptAndSkip(index, "unexpected value [return val]: expected identifier");
				break;
			}
			if (getIndex == 39 || getIndex == 40 || getIndex == 18)
			{
				expretionState(index, false);
			}
			else
			{
				exeptAndSkip(index,"unexpected value [return val]: expected identifier");
				break;
			}
			break;
		}
		case 17: // ';'
		{
			break;
		}
		case 5: // 'in'
		{
			inputState(index);
			break;
		}
		case 7: // 'out'
		{
			outputState(index);
			break;
		}
		case 11: // 'if'
		{
			ifState(index);
			break;
		}
		case 23: // ':' -> 'else'
		{
			index += 1;
			if (index != m_lexem.size())
				getIndex = m_lexem.at(index).index;
			else
			{
				exeptAndSkip(index, "unexpected value [else state]: expected '{'");
				break;
			}
			if (getIndex == 20)
			{
				index -= 1;
			}
			else
			{
				exeptAndSkip(index, "unexpected value [else state]: expected '{'");
				break;
			}
			break;
		}
		case 9: // 'while'
		{
			whileState(index);
			break;
		}
		default: // other...
		{
			cout << "this " << endl;
			cout << index << endl;
			exitBool = true;
			break;
		}
		}

		if (exitBool)
			break;
	}

	for (const auto& item : myStack)
	{
		cout << item.block << "    " <<   item.whichBlock << "     line:" << item.line << endl;
	}

}

void SyntaxAnalyzer::ViewLogs()
{
	try
	{
		ofstream fout;
		fout.open("./Logs/SyntaxLogsTypeA.txt");
		for ( int j = 0; j < exept.size(); j++ )
		{
			fout << j + 1 << "	In line " << exept.at(j).line
				<< " u have " << exept.at(j).mess
				<< "  but got  '" << exept.at(j).val << "'" << std::endl;
		}
		fout.close();
	}
	catch ( std::exception& e )
	{
		cerr << e.what() << endl;
	}
}

void SyntaxAnalyzer::declarationLexem(int & index)
{
	
	int getIndex = m_lexem.at(index).index;
	int getIndexPrev;
	if (getIndex == 1 || getIndex == 2 || getIndex == 3 || getIndex == 4)
	{
		index += 1;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [declaration variables]: expected identifier");
			return;
		}
		if (getIndex == 39)
		{
			index += 1;
			if (index != m_lexem.size())
				getIndex = m_lexem.at(index).index;
			else
			{
				exeptAndSkip(index, "unexpected value [declaration variables]: expected ';'");
				return; 
			}
			switch (getIndex)
			{
				case 17: // ';'
				{
					index += 1;
					return;
					break;
				}
				case 27: // ','
				{
					stateÑomma(index);
					break;
				}
				case 28: // '='
				{
					int len = exept.size();
					assignmentState(index);
					if (len < exept.size())
					{
						while (index != m_lexem.size()
							&& m_lexem.at(index).index != 17 && m_lexem.at(index).index != 20)
							index += 1;
						index -= 1;

						break;
					}
					if (m_lexem.at(index).index == 27)
						stateÑomma(index);
					break;
				}
				case 24: // '[' ---script declaration array type---
				{

					index += 1;
					if (index != m_lexem.size())				// next
						getIndex = m_lexem.at(index).index;
					else
					{
						exeptAndSkip(index, "unexpected value [declaration array]: expected expression");
						return;
					}
					int len = exept.size();						//save amount of exept
																//expretionState
					if (getIndex == 39 || getIndex == 40 || getIndex == 18)
					{
						expretionState(index, false);
					}
					else
					{
						exeptAndSkip(index, "unexpected value [declaration array]: expected identifier/val");
						break;
					}
																//exept processing
					if (len < exept.size())
					{
						while (index != m_lexem.size()
							&& m_lexem.at(index).index != 17 && m_lexem.at(index).index != 20)
							index += 1;
						index -= 1;
						break;
					}

					if (m_lexem.at(index).index == 38)
					{
						index += 1;
						if (index != m_lexem.size())
							getIndex = m_lexem.at(index).index;
						else
						{
							exeptAndSkip(index, "unexpected value [declaration array]: expected expression");
							break;
						}
					}
					else
					{
						exeptAndSkip(index, "unexpected value [declaration array->(range)]: expected '..'");
						break;
					}
																//expretionState
					if (getIndex == 39 || getIndex == 40 || getIndex == 18)
					{
						expretionState(index, false);
					}
					else
					{
						exeptAndSkip(index, "unexpected value [declaration array]: expected identifier/val");
						break;
					}
																//chack index
					
					if (index != m_lexem.size())
						getIndex = m_lexem.at(index).index;
					else
					{
						exeptAndSkip(index, "unexpected value [declaration variables]: expected ']'");
						break; //?
					}

																//exected ]; ann go next, else exept
					if (getIndex == 25)
					{
						index += 1;
						if (index != m_lexem.size())
							getIndex = m_lexem.at(index).index;
						else
						{
							exeptAndSkip(index, "unexpected value [declaration array]: expected ';'");
							break;
						}
						if (getIndex == 17)
						{
							//
						}
						else
						{
							exeptAndSkip(index, "unexpected value [declaration array]: expected ';'");
							break;
						}
					}
					else
					{
						exeptAndSkip(index, "unexpected value [declaration variables]: expected ']'");
						break;
					}
					break;
				}
				case 18: // '('
				{

					vector<string> nameID;
					bool check = false;

					index += 1;
					if (index != m_lexem.size())
					{
						getIndex = m_lexem.at(index).index;
						if (getIndex < 1 && getIndex >4)
						{
							exeptAndSkip(index, "unexpected value [declaration function]: expected ')'");
							return;
						}
							
					}
					else
					{
						exeptAndSkip(index, "unexpected value [declaration function]: expected ')'");
						return; 
					}
					
					while (true)
					{

						index += 1;
						getIndexPrev = getIndex;
						if (index != m_lexem.size())
							getIndex = m_lexem.at(index).index;
						else
						{
							exeptAndSkip(index, "unexpected value [declaration function]: expected identifier");
							return; 
						}

						if (getIndexPrev == 1 || getIndexPrev == 2 || getIndexPrev == 3 || getIndexPrev == 4)
						{
							if (getIndex == 39)
							{
								for (const string& item : nameID)
								{
									if (m_lexem.at(index).val != item)
										continue;
									else check = true;
								}
								if(!check)
									nameID.push_back(m_lexem.at(index).val);
								else
								{
									exeptAndSkip(index, "unexpected value [declaration function]: can't used identifier with similarly name");
									return;
								}
								continue;
							}
							else
							{
								exeptAndSkip(index, "unexpected value [declaration variables]: expected identifier");
								return;
							}
						}

						if (getIndexPrev == 39)
						{
							if (getIndex == 19 || getIndex == 27)
							{
								continue;
							}
							else
							{
								exeptAndSkip(index, "unexpected value [declaration function]: expected ')'");
								return;
							}
						}

						if (getIndexPrev == 27)
						{
							if (getIndex == 1 || getIndex == 2 || getIndex == 3 || getIndex == 4)
							{
								continue;
							}
							else
							{
								exeptAndSkip(index, "unexpected value [declaration variables]: expected type");
								return;
							}
						}

						if (getIndexPrev == 19)
						{
							if (getIndex == 17 || getIndex == 20)
							{
								if (getIndex == 20) break;
								else
									index += 1;
								break;
							}
							else
							{
								exeptAndSkip(index, "unexpected value [declaration function]: expected ';' or '{'");
								return;
							}
						}
					}

					break;
				}
				default:
				{
					exeptAndSkip(index, "unexpected value [declaration variables]: expected ';'");
					break;
				}

			}
		}
		else
		{
			exeptAndSkip(index, "unexpected value [declaration variables]: expected identifier");
			return;
		}
	}
	else
	{
		exeptAndSkip(index, "unexpected value [declaration variables]: expected type");
		return;
	}
}

// = ..
void SyntaxAnalyzer::assignmentState(int & index)
{
	if (m_lexem.at(index).index == 28)
	{
		index += 1;
		expretionState(index, false);
	}
	else
	{
		exept.emplace_back(m_lexem.at(index).val, m_lexem.at(index).numOfLine,
			"unexpected value [assignment]: expected '='");
	}
}

// 5*(s-9)-k/3
void SyntaxAnalyzer::expretionState(int& index, bool ifst)
{

	vector<AllLexem> temp;
	int bracket = 0;
	if (ifst)
		bracket = 1;
	int getIndex;
	int getIndexPrev;
	bool skipOne = false;
	index -= 1;

	while (true)
	{
		index += 1;

		if(index != int(m_lexem.size()))
			getIndex = m_lexem.at(index).index;
		else
		{
			break;
		}
		if (index != int(m_lexem.size()) 
			&& !skipOne && (getIndex == 40 || getIndex == 39 || getIndex == 18 || getIndex == 19
			|| getIndex == 31 || getIndex == 26 || getIndex == 32 || getIndex == 33))
		{
			temp.push_back(m_lexem.at(index));
		}
		else { 
			if (!skipOne) {
				skipOne = true;
				temp.push_back(m_lexem.at(index));
				continue;
			}
			else {
				index -= 1;
				break;
			}
		}
	}

	getIndex = temp.at(0).index;
	switch (getIndex)
	{
	case 18:
		bracket += 1;
		if (temp.size() == 1)
		{
			exept.emplace_back(temp.at(0).val, temp.at(0).numOfLine,
				"unexpected value [expretion]: expected identifier/val");
			exept.emplace_back(temp.at(0).val, temp.at(0).numOfLine,
				"unexpected value [expretion]: expected identifier/val");
			return;
		}
		break;
	case 39:
	case 40:
		break;
	default:
		exept.emplace_back(temp.at(0).val, temp.at(0).numOfLine,
			"unexpected value [expretion]: expected identifier");
		return;
	}
	
	for (size_t i = 1; i < temp.size(); i++)
	{
		getIndexPrev = getIndex;
		getIndex = temp.at(i).index;
		
		if (getIndexPrev == 18)
		{
			if (getIndex == 39 || getIndex == 40 || getIndex == 18)
			{
				if (getIndex == 18)
					bracket += 1;
				continue;
			}
			else
			{
				exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
					"unexpected value [expretion]: expected identifier/val");
				return;
			}
		}
		
		if (getIndexPrev == 39 || getIndexPrev == 40)
		{
			if (getIndex == 26 || getIndex == 31 || getIndex == 32 || getIndex == 33 || getIndex == 19)
			{
				if (getIndex == 19)
					bracket -= 1;
				if (bracket < 0)
				{
					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [expretion]: can't write ')' without a leading opening bracket ");
					return;
				}
				continue;
			}
			else {
				
				if ((getIndex == 27 || getIndex == 17 || getIndex == 30 || getIndex == 25
					|| getIndex == 34 || getIndex == 35 || getIndex == 36 || getIndex == 37
					|| getIndex == 38 || getIndex == 8 || getIndex == 6 || getIndex == 22 
					|| getIndex == 10 || getIndex == 29) && i == (temp.size() - 1) )
				{
					continue;
				}

				if (getIndex == 28 && getIndexPrev == 40 && i == (temp.size() - 1))
				{
					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [expretion]: can't assignment for l-value reference");
					return;
				}
				if ((getIndex == 39 || getIndex == 40) && (getIndexPrev == 39 || getIndexPrev == 40))
				{
					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [expretion]: expected ';'");
					return;
				}
				if (getIndex == 28 && getIndexPrev == 39 && i == (temp.size() - 1))
				{
					assignmentState(index);
					return;
				}
				if (bracket > 0)
				{
					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [expretion]: expected ')'");
					return;

				}
				if (i == (temp.size() - 1))
				{
					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [expretion]: expected ';'");
					return;
				}
				else continue;
				
			}
		}
		
		if (getIndexPrev == 26 || getIndexPrev == 31 || getIndexPrev == 32 || getIndexPrev == 33)
		{
			if (i == (temp.size() - 1) || getIndex == 39 || getIndex == 40 || getIndex == 18)
			{
				if (i == (temp.size() - 1))
				{
					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [expretion]: expected identifier/val");
					return;
				}
				if (getIndex == 18)
					bracket += 1;
				if (bracket > 0 && i == temp.size() - 1)
				{
					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [expretion]: expected ')'");
					return;
				}
				continue;
			}
			else {
				exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
					"unexpected value [expretion]: expected identifier/val");
				return;
			}
		}
		
		if (getIndexPrev == 19)
		{
			if (i == (temp.size() - 1) && (getIndex == 26 || getIndex == 31 || getIndex == 32 || getIndex == 33))
			{
				exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
					"unexpected value [expretion]: expected identifier/val");
				return;
			}
			if ( getIndex == 26 || getIndex == 31 || getIndex == 32 || getIndex == 33 || getIndex == 27 || getIndex == 17)
				continue;
			else {

				if ((getIndex == 27 || getIndex == 17 || getIndex == 30 || getIndex == 25
					|| getIndex == 34 || getIndex == 35 || getIndex == 36 || getIndex == 37
					|| getIndex == 38 || getIndex == 8 || getIndex == 6 || getIndex == 22
					|| getIndex == 10 || getIndex == 29) && i == (temp.size() - 1))
				{
					continue;
				}

				if (getIndex == 28 && getIndexPrev == 40 && i == (temp.size() - 1))
				{

					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [expresion]: can't assignment for l-value reference");
					return;
				}

				if (getIndex == 28 && getIndexPrev == 39 && i == (temp.size() - 1))
				{
					assignmentState(index);
					return;
				}
				if (ifst && getIndex != 22 && getIndex != 10)
				{
					exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
						"unexpected value [if/while state]: expected '?' or 'do'");
					return;
				}
				exept.emplace_back(temp.at(i).val, temp.at(i).numOfLine,
					"unexpected value [expretion]: expected continue");
				return;
			}
		}
	}
}

void SyntaxAnalyzer::inputState(int& index)
{

	int getIndex = m_lexem.at(index).index;
	int getIndexPrev;

	index += 1;
	getIndexPrev = getIndex;
	if (index != m_lexem.size())
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [input state]: expected SET");
		return;
	}
	if (getIndexPrev == 5)
	{
		if (getIndex == 6);
		else
		{
			exeptAndSkip(index, "unexpected value [input state]: expected SET");
			return;
		}
	}

	while (true)
	{
		index += 1;
		getIndexPrev = getIndex;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [input state]: expected SET");
			return;
		}
		if (getIndexPrev == 6)
		{
			if (getIndex == 39)
				continue;
			else
			{
				exeptAndSkip(index, "unexpected value [input state]: expected identifier");
				return;
			}
		}

		if (getIndexPrev == 39)
		{
			if (getIndex == 6 || getIndex == 17)
			{
				if (getIndex == 6)
					continue;
				if (getIndex == 17)
					return;
			}
			else
			{
				exeptAndSkip(index, "unexpected value [input state]: expected ';'");
				return;
			}
		}
	}
}

void SyntaxAnalyzer::outputState(int & index)
{
	int getIndex = m_lexem.at(index).index;
	int getIndexPrev;

	index += 1;
	getIndexPrev = getIndex;
	if (index != m_lexem.size())
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [output state]: expected GET");
		return;
	}
	if (getIndexPrev == 7)
	{
		if (getIndex == 8);
		else
		{
			exeptAndSkip(index, "unexpected value [output state]: expected GET");
			return;
		}
	}

	while (true)
	{
		index += 1;
		getIndexPrev = getIndex;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [output state]: expected GET");
			return;
		}
		if (getIndexPrev == 8)
		{
			if (getIndex == 39 || getIndex == 40 || getIndex == 18 || getIndex == 14)
			{
				if (getIndex == 14)
					continue;
				else
				{
					expretionState(index, false);
					index -= 1;
					continue;
				}
			}
			else
			{
				exeptAndSkip(index, "unexpected value [output state]: expected identifier/val");
				return;
			}
		}

		if (getIndexPrev == 39 || getIndexPrev == 40 || getIndexPrev == 18 || getIndexPrev == 14)
		{
			if (getIndex == 8 || getIndex == 17)
			{
				if (getIndex == 8)
					continue;
				if (getIndex == 17)
					return;
			}
			else
			{
				exeptAndSkip(index, "unexpected value [output state]: expected ';'");
				return;
			}
		}
	}
}

void SyntaxAnalyzer::ifState(int & index)
{

	int getIndex = m_lexem.at(index).index;
	int getIndexPrev;

	index += 1;
	getIndexPrev = getIndex;
	if (index != m_lexem.size())
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected '('");
		return;
	}
	if (getIndexPrev == 11)
	{
		if (getIndex == 18);
		else
		{
			exeptAndSkip(index, "unexpected value [if state]: expected '('");
			return;
		}
	}

	index += 1;
	if (index != m_lexem.size())				// next
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected expression");
		return;
	}
	int len = exept.size();						//save amount of exept
												//expretionState
	if (getIndex == 39 || getIndex == 40 || getIndex == 18)
	{
		expretionState(index, true);
	}
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected identifier/val");
		return;
	}
	//exept processing
	if (len < exept.size())
	{
		while (index != m_lexem.size()
			&& m_lexem.at(index).index != 17 && m_lexem.at(index).index != 20)
			index += 1;
		index -= 1;
		
		return;
	}

	if (index != m_lexem.size())				// check
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected ')'");
		return;
	}
	switch (getIndex)
	{
	case 22:
	{
		index += 1;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [if state]: expected '{'");
			return;
		}
		index -= 1;
		return;
		break;
	}
	case 29:
	case 30:
	case 34:
	case 35:
	case 36:
	case 37:
	{
		index += 1;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [if state]: expected expression");
			return;
		}
		break;
	}
	default:
	{
		exeptAndSkip(index, "unexpected value [if state]: expected '?'");
		return;
		break; 
	}
	}
	/*
	if (getIndex == 19)
	{
		index += 1;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [declaration array]: expected expression");
			return;
		}
	}
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected ')'");
		return;
	}
	*/

	len = exept.size();
	if (getIndex == 39 || getIndex == 40 || getIndex == 18)
	{
		expretionState(index, true);
	}
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected identifier/val");
		return;
	}
	//exept processing
	if (len < exept.size())
	{
		while (index != m_lexem.size()
			&& m_lexem.at(index).index != 17 && m_lexem.at(index).index != 20)
			index += 1;
		index -= 1;

		return;
	}

	if (index != m_lexem.size())				// check
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected '?'");
		return;
	}

	//'?'
	if (getIndex == 22)
	{
		index += 1;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [if state]: expected '{'");
			return;
		}
	}
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected '?'");
		return;
	}

	if (getIndex == 20);
	else
	{
		exeptAndSkip(index, "unexpected value [if state]: expected '{'");
		return;
	}
	
	index -= 1;
}

void SyntaxAnalyzer::whileState(int & index)
{

	int getIndex = m_lexem.at(index).index;
	int getIndexPrev;

	index += 1;
	getIndexPrev = getIndex;
	if (index != m_lexem.size())
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [while state: step 1]: expected '('");
		return;
	}
	if (getIndexPrev == 9)
	{
		if (getIndex == 18);
		else
		{
			exeptAndSkip(index, "unexpected value [while state: step 1]: expected '('");
			return;
		}
	}

	index += 1;
	if (index != m_lexem.size())				// next
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [while state: step 2]: expected expression");
		return;
	}
	int len = exept.size();						//save amount of exept
												//expretionState
	if (getIndex == 39 || getIndex == 40 || getIndex == 18)
	{
		expretionState(index, true);
	}
	else
	{
		exeptAndSkip(index, "unexpected value [while state: step 2]: expected identifier/val");
		return;
	}
	//exept processing
	if (len < exept.size())
	{
		exeptAndSkip(index, "unexpected value [while state: step 5]: expected '{'");

		/*
		while (index != m_lexem.size()
			&& m_lexem.at(index).index != 17 && m_lexem.at(index).index != 20)
			index += 1;
		index -= 1;
		*/
		return;
	}

	if (index != m_lexem.size())				// check
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [while state: step 3]: expected ')'");
		return;
	}
	switch (getIndex)
	{
	case 10:
	{
		index += 1;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [while state: step 4]: expected '{'");
			return;
		}
		index -= 1;
		return;
		break;
	}
	case 29:
	case 30:
	case 34:
	case 35:
	case 36:
	case 37:
	{
		index += 1;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [while state: step 4]: expected expression");
			return;
		}
		break;
	}
	default:
	{
		exeptAndSkip(index, "unexpected value [while state: step 4]: expected 'do'");
		return;
		break;
	}
	}
	
	len = exept.size();
	if (getIndex == 39 || getIndex == 40 || getIndex == 18)
	{
		expretionState(index, true);
	}
	else
	{
		exeptAndSkip(index, "unexpected value [while state: step 5]: expected identifier/val");
		return;
	}
	//exept processing
	if (len < exept.size())
	{
		exeptAndSkip(index, "unexpected value [while state: step 5]: expected '{'");
		/*
		while (index != m_lexem.size()
			&& m_lexem.at(index).index != 17 && m_lexem.at(index).index != 20)
			index += 1;
		index -= 1;
		*/
		return;
	}

	if (index != m_lexem.size())				// check
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [while state: step 6]: expected 'do'");
		return;
	}

	//'do'
	if (getIndex == 10)
	{
		index += 1;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [while state: step 7]: expected '{'");
			return;
		}
	}
	else
	{
		exeptAndSkip(index, "unexpected value [while state: step 6]: expected 'do'");
		return;
	}

	if (getIndex == 20);
	else
	{
		exeptAndSkip(index, "unexpected value [while state: step 7]: expected '{'");
		return; 
	}

	index -= 1;
}

void SyntaxAnalyzer::callState(int & index)
{
	int getIndex = m_lexem.at(index).index;
	int getIndexPrev;

	index += 1;
	getIndexPrev = getIndex;
	if (index != m_lexem.size())
		getIndex = m_lexem.at(index).index;
	else
	{
		exeptAndSkip(index, "unexpected value [call state]: expected identifier/val");
		return;
	}

	if (getIndexPrev == 18)
	{
		if (getIndex == 19 || getIndex == 39)
		{
			if (getIndex == 19) {
				index += 1;
				if (index != m_lexem.size())
					getIndex = m_lexem.at(index).index;
				else
				{
					exeptAndSkip(index, "unexpected value [call state]: expected ';'");
					return;
				}
				if (getIndex == 17)
					return;
				else
				{
					exeptAndSkip(index, "unexpected value [call state]: expected ';'");
					return;
				}
			}
		}
		else
		{
			exeptAndSkip(index, "unexpected value [call state]: expected identifier");
			return;
		}

	}

	while (true)
	{
		index += 1;
		getIndexPrev = getIndex;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [call state]: expected )");
			return;
		}

		if (getIndexPrev == 39)
		{
			if (getIndex == 27 || getIndex == 19)
			{
				continue;
			}
			else
			{
				exeptAndSkip(index, "unexpected value [call state]: expected ')'");
				return;
			}
		}

		if (getIndexPrev == 27)
		{
			if (getIndex == 39)
			{
				continue;
			}
			else
			{
				exeptAndSkip(index, "unexpected value [call state]: expected identifier");
				return;
			}
		}

		if (getIndexPrev == 19)
		{
			if (getIndex == 17)
			{
				return;
			}
			else
			{
				exeptAndSkip(index, "unexpected value [call state]: expected ';'");
				return;
			}
		}

	}

}

//circle after comma for declaration
void SyntaxAnalyzer::stateÑomma(int & index)
{
	int getIndex = m_lexem.at(index).index;
	int getIndexPrev;

	while (true)
	{
		index += 1;
		getIndexPrev = getIndex;
		if (index != m_lexem.size())
			getIndex = m_lexem.at(index).index;
		else
		{
			exeptAndSkip(index, "unexpected value [declaration variables]: expected identifier");
			return;
		}
		if (getIndexPrev == 27)
		{
			if (getIndex == 39)
				continue;
			else
			{
				exeptAndSkip(index, "unexpected value[declaration variables]: expected identifier");
				return;
			}
		}

		if (getIndexPrev == 39)
		{
			if (getIndex == 28 || getIndex == 27 || getIndex == 17)
			{
				if (getIndex == 28)
					assignmentState(index);
				if (getIndex == 17)
				{
					index += 1;
					return;
				}
				continue;
			}
			else
			{
				exeptAndSkip(index, "unexpected value [declaration variables]: expected ';'");
				return;
			}
		}
	}
}

//search which ID start a new block {}
string SyntaxAnalyzer::whichState(int index, int& line)
{
	int ind;
	for (int i = index; i >= 0; --i)
	{
		ind = m_lexem.at(i).index;
		if (ind != 17 && ind != 10 && ind != 11 && ind != 23 && ind != 16)
			continue;
		else
		{
			if (ind == 17)
			{
				line = m_lexem.at(i + 2).numOfLine;
				return m_lexem.at(i + 2).val;
			}
			line = m_lexem.at(i).numOfLine;
			return m_lexem.at(i).val;
		}
	}
}

void SyntaxAnalyzer::exeptAndSkip(int & index, const  string & mess)
{
	const size_t size = m_lexem.size();

	exept.emplace_back(m_lexem.at(index).val, m_lexem.at(index).numOfLine, mess);
	while (index != size && m_lexem.at(index).index != 17 && m_lexem.at(index).index != 20)
		index += 1;

	index -= 1;
	
}
