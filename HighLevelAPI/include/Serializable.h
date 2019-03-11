//------------------------------------------------------------------------------
//
// File Name:	Serializable.h
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

#include "BetaObject.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References
//------------------------------------------------------------------------------

class Parser;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   modify the public interface (function prototypes) declared in the header.
class Serializable
{
public:
	// Loads object data from a file.
	// Params:
	//   parser = The parser for the file.
	virtual void Deserialize(Parser& parser);

	// Saves object data to a file.
	// Params:
	//   parser = The parser for the file.
	virtual void Serialize(Parser& parser) const;

protected:
	// These functions are protected to ensure this class is effectively abstract.
	Serializable();
	virtual ~Serializable();
	Serializable(const Serializable& other);
	Serializable& operator=(const Serializable& other);
};

//------------------------------------------------------------------------------
