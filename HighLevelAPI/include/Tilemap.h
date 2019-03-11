//------------------------------------------------------------------------------
//
// File Name:	Tilemap.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Vector2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward Declarations:
//------------------------------------------------------------------------------

class Matrix2D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
class Tilemap
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Create a tilemap from the given parameters.
	// Params:
	//   numColumns = The width of the map.
	//   numRows = The height of the map.
	//   data = The array containing the map data.
	Tilemap(unsigned numColumns, unsigned numRows, int** data);

	// Destructor.
	~Tilemap();

	// Gets the width of the map (in tiles).
	unsigned GetWidth() const;

	// Get the height of the map (in tiles).
	unsigned GetHeight() const;

	// Gets the value of the cell in the map with the given indices.
	// Params:
	//   column = The column of the cell.
	//   row = The row of the cell.
	// Returns:
	//   -1 if the indices are invalid, 0 if the cell is empty, 
	//   or a positive integer otherwise.
	int GetCellValue(unsigned column, unsigned row) const;

	// Create a tilemap from the given file.
	// Params:
	//   filename = The name of the file containing the tilemap data.
	static Tilemap* CreateTilemapFromFile(const std::string& filename);

private:
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
	static bool ReadIntegerVariable(std::ifstream& file, const std::string& name, int& variable);

	// Helper function for reading in arrays of integers.
	// Params:
	//   file = The file stream to read from.
	//   name = The text to look for before reading the array.
	//   columns = The number of columns in the array.
	//   rows = The number of rows in the array.
	// Returns:
	//   A pointer to the dynamically allocated array if the data is valid, nullptr otherwise.
	static int** ReadArrayVariable(std::ifstream& file, const std::string& name, 
		unsigned columns, unsigned rows);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Dimensions of the map
	unsigned numColumns;
	unsigned numRows;

	// The map data (a 2D array)
	int** data;

	// Relative path for tilemaps
	static std::string mapPath;
};

//------------------------------------------------------------------------------
