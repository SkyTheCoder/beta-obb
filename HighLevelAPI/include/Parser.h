//------------------------------------------------------------------------------
//
// File Name:	Parser.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		CS230
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <exception>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Vector2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Exception class for problems encountered while reading/writing from files
class ParseException : public std::exception
{
public:
	// Constructor
	// Params:
	//   fileName = The name of the file the exception was thrown for.
	//   errorDetails = A more description of what went wrong.
	ParseException(const std::string& fileName, const std::string& errorDetails);
};

// Parser class - reads/writes data object data to/from files
// You are free to change the contents of this structure as long as you do not
// modify the public interface (function prototypes) declared in the header.
class Parser
{
public:
	// Opens a file for loading.
	// Params:
	//   filename = The name of the file to parse.
	//   mode = The stream mode.
	Parser(const std::string& filename, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out);

	// Closes the currently open file.
	~Parser();

	// Begins a new scope when writing to a file.
	// Outputs a curly brace and increases the tab count.
	void BeginScope();

	// Ends the current scope when writing to a file.
	// Outputs an end curly brace and decreases tab count.
	void EndScope();

	// Writes a variable name and value to the currently open file.
	// Params:
	//   name = The name of the variable.
	//   variable = The value of the variable.
	template<typename T>
	void WriteVariable(const std::string& name, const T& variable)
	{
		// Ensure that the file is open.
		CheckFileOpen();

		// Write the variable to the stream at the appropriate indentation level.
		for (unsigned i = 0; i < indentLevel; i++)
			stream << tab;

		stream << name << " : " << variable << std::endl;
	}

	// Writes a value to the currently open file.
	// Params:
	//   value = The value to write.
	template<typename T>
	void WriteValue(const T& value)
	{
		// Ensure that the file is open.
		CheckFileOpen();

		// Write the value to the stream at the appropriate indentation level.
		for (unsigned i = 0; i < indentLevel; i++)
			stream << tab;

		stream << value << std::endl;
	}

	// Reads the value of a variable with the given name from the currently open file.

	// Params:
	//   name = The name of the variable.
	//   variable = The variable to read to.
	template<typename T>
	void ReadVariable(const std::string& name, T& variable)
	{
		// Ensure that the file is open.
		CheckFileOpen();

		// Read the next word in the stream.
		std::string word;
		stream >> word;

		// If the word is not what was expected, throw an exception.
		if (word != name)
			throw ParseException(filename, "A variable with the given name could not be found: " + name);

		// Skip the : between the variable name and its value.
		ReadSkip(':');

		// Read the next value in the stream.
		stream >> variable;
	}

	// Reads the next value from the currently open file.
	// Params:
	//   value = The value to read to.
	template<typename T>
	void ReadValue(T& value)
	{
		// Ensure that the file is open.
		CheckFileOpen();

		// Read the next value in the stream.
		stream >> value;
	}

	// Reads a piece of text from the currently open file
	// and skips to the next word afterwards.
	// Params:
	//   text = The text to skip.
	void ReadSkip(const std::string& text);

	// Skips characters in the stream up until the next
	// occurrence of the given delimiter.
	// Params:
	//   delimiter = The character to skip.
	void ReadSkip(char delimiter);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Checks if the file was opened correctly. If not, throws an exception.
	void CheckFileOpen();

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	std::fstream stream;
	std::string filename;
	unsigned indentLevel;
	const char* tab = "  ";
};
