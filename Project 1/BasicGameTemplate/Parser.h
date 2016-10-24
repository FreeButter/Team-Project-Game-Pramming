/**
Simple Json Parser
Parser.h
Purpose: Manages Json Document objects

@author Miguel Saavedra
@version 1.1 3/10/16
*/
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "document.h"
#include "filereadstream.h"
#pragma warning (disable : 4996)
using namespace std;
using namespace rapidjson;
class Parser
{
	//Member Methods:
public:
	static Parser& GetInstance();
	~Parser();
	void loadInFile(string file);

protected:

private:
	Parser();
	Parser(const Parser& parser);
	Parser& operator=(const Parser& parser);

	//Member Data:
public:
	Document document;
protected:
	static Parser* sm_pInstance;

private:

};

#endif //__PARSER_H__



string readFile(string file);