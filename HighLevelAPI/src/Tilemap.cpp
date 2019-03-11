//------------------------------------------------------------------------------
//
// File Name:	Tilemap.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Tilemap.h"

#include "Matrix2DStudent.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a tilemap from the given parameters.
// Params:
//   numColumns = The width of the map.
//   numRows = The height of the map.
//   data = The array containing the map data.
Tilemap::Tilemap(unsigned numColumns, unsigned numRows, int** data) : numColumns(numColumns), numRows(numRows), data(data)
{
}

// Destructor.
Tilemap::~Tilemap()
{
	// Delete all allocated rows.
	for (unsigned column = 0; column < numColumns; column++)
	{
		delete[] data[column];
	}

	// Delete the container column array.
	delete[] data;
}

// Gets the width of the map (in tiles).
unsigned Tilemap::GetWidth() const
{
	return numColumns;
}

// Get the height of the map (in tiles).
unsigned Tilemap::GetHeight() const
{
	return numRows;
}

// Gets the value of the cell in the map with the given indices.
// Params:
//   column = The column of the cell.
//   row = The row of the cell.
// Returns:
//   -1 if the indices are invalid, 0 if the cell is empty, 
//   or a positive integer otherwise.
int Tilemap::GetCellValue(unsigned column, unsigned row) const
{
	// Verify that the column and row are valid.
	if (column >= numColumns || row >= numRows)
		return -1;

	return data[column][row];
}

// Create a tilemap from the given file.
// Params:
//   filename = The name of the file containing the tilemap data.
Tilemap* Tilemap::CreateTilemapFromFile(const std::string& filename)
{
	std::ifstream file(filename);

	// Verify that the file is open.
	if (!file.is_open())
		return nullptr;
	
	// Read the width of the map.
	int numColumns;
	if (!ReadIntegerVariable(file, "width", numColumns))
		return nullptr;

	// Read the height of the map.
	int numRows;
	if (!ReadIntegerVariable(file, "height", numRows))
		return nullptr;

	// Read the map array.
	int** data = ReadArrayVariable(file, "data", numColumns, numRows);
	if (data == nullptr)
		return nullptr;
	
	// Create the tilemap.
	return new Tilemap(numColumns, numRows, data);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Helper function for reading in values of integers.
// Params:
//   file = The file stream to read from.
//   name = The text to look for before reading the value.
//   variable = If the name is valid, the variable to put the value in.
// Returns:
//   True if a value with the given name was found, false otherwise.
bool Tilemap::ReadIntegerVariable(std::ifstream& file, const std::string& name, int& variable)
{
	// Read the property name.
	std::string word = "";
	file >> word;

	// If the property name is not what was expected, exit.
	if (word != name)
		return false;
	
	// Read the value.
	file >> variable;

	return true;
}

// Helper function for reading in arrays of integers.
// Params:
//   file = The file stream to read from.
//   name = The text to look for before reading the array.
//   columns = The number of columns in the array.
//   rows = The number of rows in the array.
// Returns:
//   A pointer to the dynamically allocated array if the data is valid, nullptr otherwise.
int** Tilemap::ReadArrayVariable(std::ifstream& file, const std::string& name,
	unsigned columns, unsigned rows)
{
	// Read the property name.
	std::string word = "";
	file >> word;

	// If the property name is not what was expected, exit.
	if (word != name)
		return nullptr;

	// Allocate the new 2D array.
	int** arr = new int*[columns];
	for (unsigned column = 0; column < columns; column++)
	{
		arr[column] = new int[rows];
	}

	// Read the values.
	for (unsigned row = 0; row < rows; row++)
	{
		for (unsigned column = 0; column < columns; column++)
		{
			file >> arr[column][row];
		}
	}

	return arr;
}

//------------------------------------------------------------------------------
