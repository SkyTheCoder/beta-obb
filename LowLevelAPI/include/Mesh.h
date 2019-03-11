//------------------------------------------------------------------------------
//
// File Name:	Mesh.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		Beta Engine
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class ShaderManager;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

// Types of data found in vertex buffers for default mesh
enum BUFFER_TYPE
{
	// DO NOT MODIFY - used by default vertices/meshes
	POSITION,
	COLOR,
	TEXTURE_COORDINATE,
	NUM
};

// Different methods for drawing meshes
enum MeshDrawMode
{
	MDM_Points = 0,
	MDM_Lines,
	MDM_Lines_Strip,
	MDM_Triangles,

	// Keep this one last
	MDM_Num
};

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Mesh
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	Mesh(unsigned numVertices, MeshDrawMode drawMode = MDM_Triangles, unsigned numBuffers = BUFFER_TYPE::NUM);
	~Mesh();

	// Draw the mesh
	void Draw() const;

	unsigned GetNumVertices() const;
	const std::string& GetName() const;
	void SetName(const std::string& name);

	friend class MeshFactory;

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	void SetDrawMode(MeshDrawMode drawMode);

	//------------------------------------------------------------------------------
	// Private Data:
	//------------------------------------------------------------------------------

	const unsigned numVertices;
	const unsigned numBuffers;
	unsigned* bufferIDs;
	unsigned arrayObjectID;
	unsigned drawMode;
	std::string name;
};

//------------------------------------------------------------------------------
