#include "assembler.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;
const int FLAG_EQUAL_TO = 1;
const int FLAG_GREATER_THAN = 2;
const int FLAG_LESS_THAN = 3;


int GetRegIndex(string reg)
{
	int intres = reg[1] - '0';
	return intres;
}



int GetConstant(string constant)
{
	int len = constant.size();
	return stoi(constant.substr(1, len));
}


bool IsReg(string term)
{
	bool check = true;
	if (term[0] == 'R')
	{
		check = true;
	}
	else
	{
		check = false;
	}
	return check;
}


int ExtractValue(string source, int* registers)
{
	if (IsReg(source) == true)
	{
		return registers[GetRegIndex(source)];

	}
	else
	{
		return GetConstant(source);
	}
}

// Exposed assembler

bool BuildCommandList(vector<vector<string>>& commandList, string filepath)
{
	commandList.clear();
	string line;
	string newline;
	ifstream file(filepath);

	vector<string> labelNames;
	vector<string> labelRows;
	
	
	if (file.is_open())
	{
		while (getline(file, line))
		{
			line.erase(remove(line.begin(), line.end(), '\n'));
			if (line.length() != 0)
			{
				vector<string> lineParts;

				int startPosition = 0;
				int endPosition = line.find(" ");
				
				while (endPosition != string::npos)
				{
					lineParts.push_back(line.substr(startPosition, endPosition - startPosition));
					startPosition = endPosition + 1;
					endPosition = line.find(" ", startPosition);

				}
				lineParts.push_back(line.substr(startPosition));
				if (lineParts[0].back() == ':')
				{
					lineParts[0] = lineParts[0].substr(0, lineParts[0].size() - 1);
					labelNames.push_back(lineParts[0]);
					labelRows.push_back(std::to_string(commandList.size()));
				}
				else
				{
					commandList.push_back(lineParts);
				}
			}
			 
		}
		file.close();
		int index;
		for(int i=0; i < commandList.size(); i++)
		{
			if (commandList[i][0] == "JEQ" || commandList[i][0] == "JGT" || commandList[i][0] == "JLT")
			{
				index = -1;
				for (int a = 0; a < labelNames.size(); a++)
				{
					if (labelNames[a] == commandList[i][1])
					{
						index = a;
					}
				}
				if (index == -1)
				{
					commandList[i][1] = labelRows.back();
				}
				else
				{
					commandList[i][1] = labelRows[index];
				}
			}
			
			


		}
		return true;
		
	}
	else
	{
		return false;
	}
}

int PeekNextCommandIndex(int* registers)
{
	return registers[REGISTER_SIZE - 1];
}

int GetAndStepCommandIndex(int* registers)
{
	int index = registers[REGISTER_SIZE - 1];
	registers[REGISTER_SIZE - 1] += 1;
	return index;
}

//Assignment instructions 

void MOV(vector<string>& command, int* registers)
{
	int destIndex = GetRegIndex(command[1]);
	int value = ExtractValue(command[2], registers);
	registers[destIndex] = value;
}

//Arithmetic instructions

void ADD(vector<string>& command, int* registers)
{
	int destIndex = GetRegIndex(command[1]);
	int term1 = ExtractValue(command[2], registers);
	int term2 = ExtractValue(command[3], registers);
	registers[destIndex] = term1 + term2;
}

void SUB(vector<string>& command, int* registers)
{
	int destIndex = GetRegIndex(command[1]);
	int term1 = ExtractValue(command[2], registers);
	int term2 = ExtractValue(command[3], registers);
	registers[destIndex] = term1 - term2;
}

//Jump instructions 

void CMP(vector<string>& command, int* registers)
{
	int value1 = ExtractValue(command[1], registers);
	int value2 = ExtractValue(command[2], registers);
	if (value1 == value2)
	{
		registers[REGISTER_SIZE - 2] = FLAG_EQUAL_TO;
	}
	else if (value1 > value2)
	{
		registers[REGISTER_SIZE - 2] = FLAG_GREATER_THAN;
	}
	else
	{
		registers[REGISTER_SIZE - 2] = FLAG_LESS_THAN;
	}
}

void JEQ(vector<string>& command, int* registers)
{
	if (registers[REGISTER_SIZE - 2] == FLAG_EQUAL_TO)
	{
		registers[REGISTER_SIZE - 1] = stoi(command[1]);
	}
}

void JGT(vector<string>& command, int* registers)
{
	if (registers[REGISTER_SIZE - 2] == FLAG_GREATER_THAN)
	{
		registers[REGISTER_SIZE - 1] = stoi(command[1]);
	}
}

void JLT(vector<string>& command, int* registers)
{
	if (registers[REGISTER_SIZE - 2] == FLAG_LESS_THAN)
	{
		registers[REGISTER_SIZE - 1] = stoi(command[1]);
	}
}
