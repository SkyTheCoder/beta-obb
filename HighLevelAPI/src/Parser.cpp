//------------------------------------------------------------------------------
//
// File Name:	Parser.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Parser.h"
#include <Vector2D.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Exception class for problems encountered while reading/writing from files

// Constructor
// Params:
//   fileName = The name of the file the exception was thrown for.
//   errorDetails = A more description of what went wrong.
ParseException::ParseException(const std::string& fileName, const std::string& errorDetails) :
	std::exception(("Error reading file " + fileName + ". " + errorDetails).c_str())
{
}

// Opens a file for loading.
// Params:
//   filename = The name of the file to parse.
//   mode = The stream mode.
Parser::Parser(const std::string& filename, std::ios_base::openmode mode) :
	stream(std::fstream(filename, mode)), filename(filename), indentLevel(0)
{
}

// Closes the currently open file.
Parser::~Parser()
{
	stream.close();
}

// Begins a new scope when writing to a file.
// Outputs a curly brace and increases the tab count.
void Parser::BeginScope()
{
	// Ensure that the file is open.
	CheckFileOpen();

	// Place an opening bracket at the appropriate indentation level, and increment the indentation level.
	for (unsigned i = 0; i < indentLevel; i++)
		stream << tab;
	stream << "{" << std::endl;
	++indentLevel;
}

// Ends the current scope when writing to a file.
// Outputs an end curly brace and decreases tab count.
void Parser::EndScope()
{
	// Ensure that the file is open.
	CheckFileOpen();

	// Decrement the indentation level, and place a closing bracket at the appropriate indentation level.
	--indentLevel;
	for (unsigned i = 0; i < indentLevel; i++)
		stream << tab;
	stream << "}" << std::endl;
}

// Reads a piece of text from the currently open file
// and skips to the next word afterwards.
// Params:
//   text = The text to skip.
void Parser::ReadSkip(const std::string& text)
{
	// Ensure that the file is open.
	CheckFileOpen();

	// Read the next word in the stream.
	std::string word;
	stream >> word;

	// If the word is not what was expected, throw an exception.
	if (word != text)
		throw ParseException(filename, "The following text could not be found: " + text);
}

// Skips characters in the stream up until the next
// occurrence of the given delimiter.
// Params:
//   delimiter = The character to skip.
void Parser::ReadSkip(char delimiter)
{
	// Ensure that the file is open.
	CheckFileOpen();

	// Skip past the next instance of the delimiter.
	stream.ignore(10, delimiter);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Checks if the file was opened correctly. If not, throws an exception.
void Parser::CheckFileOpen()
{
	// If the file is not open, throw an exception.
	if (!stream.is_open())
		throw ParseException(filename, "The file could not be opened.");
}
