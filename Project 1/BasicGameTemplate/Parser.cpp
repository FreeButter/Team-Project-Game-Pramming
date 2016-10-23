/**
Simple Json Parser
Parser.cpp
Purpose: Manages Json Document objects

@author Miguel Saavedra
@version 1.1 3/10/16
*/
// This include:
#include "pch.h"
#include "Parser.h"

// Library includes:
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;
// Static Members:
Parser* Parser::sm_pInstance = 0;

Parser&
Parser::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Parser();
	}

	return (*sm_pInstance);
}

Parser::Parser()
{

}

Parser::~Parser()
{

}

/*
This Loads in a file through a stream
and loads the json body into a json Document object
*/
void
Parser::loadInFile(string file)
{
	// Open the file for reading
	FILE* fp = fopen(file.c_str(), "rb"); // non-Windows use "r"
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	// Set the document with the data in the input stream
	document.ParseStream(is);
	fclose(fp);
}


