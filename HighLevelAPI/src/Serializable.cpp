//------------------------------------------------------------------------------
//
// File Name:	Serializable.cpp
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

#include "Serializable.h"
#include "Parser.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// Loads object data from a file.
// Params:
//   parser = The parser for the file.
void Serializable::Deserialize(Parser& parser)
{
	UNREFERENCED_PARAMETER(parser);
}

// Saves object data to a file.
// Params:
//   parser = The parser for the file.
void Serializable::Serialize(Parser& parser) const
{
	UNREFERENCED_PARAMETER(parser);
}

// These functions are protected to ensure this class is effectively abstract.
Serializable::Serializable()
{
}

Serializable::~Serializable()
{
}

Serializable::Serializable(const Serializable& other)
{
	UNREFERENCED_PARAMETER(other);
}

Serializable& Serializable::operator=(const Serializable& other)
{
	UNREFERENCED_PARAMETER(other);
	return *this;
}

//------------------------------------------------------------------------------
