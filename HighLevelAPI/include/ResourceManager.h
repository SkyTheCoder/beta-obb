//------------------------------------------------------------------------------
//
// File Name:	ResourceManager.h
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

#include <map>
#include <Vector2D.h>
#include <Color.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Mesh;
class SpriteSource;
class Texture;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class ResourceManager
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Constructor(s)
	ResourceManager();

	// Destructor
	~ResourceManager();

	// Retrieve an existing mesh required by a game object.
	// Params:
	//   objectName = The name of the mesh.
	//   createIfNotFound = Whether to create a default quad mesh if no mesh is found.
	// Returns:
	//   The mesh with the specified name.
	Mesh* GetMesh(const std::string& objectName, bool createIfNotFound = true);

	// Retrieve an existing mesh required by a game object, or create it if no mesh is found.
	// Params:
	//   objectName = The name of the mesh.
	//   textureSize = The UV size of the mesh to create if the mesh was not found.
	//   extents = The extents of the mesh to create if the mesh was not found.
	// Returns:
	//   The mesh with the specified name.
	Mesh* GetMesh(const std::string& objectName, Vector2D textureSize, Vector2D extents = Vector2D(0.5f, 0.5f));

	// Retrieve an existing mesh required by a game object, or create it if no mesh is found.
	// Params:
	//   objectName = The name of the mesh.
	//   rows = The number of rows in the UV of the mesh to create if the mesh was not found.
	//   cols = The number of columns in the UV of the mesh to create if the mesh was not found.
	//   extents = The extents of the mesh to create if the mesh was not found.
	// Returns:
	//   The mesh with the specified name.
	Mesh* GetMesh(const std::string& objectName, int numCols, int numRows, Vector2D extents = Vector2D(0.5f, 0.5f));

	// Retrieve an existing mesh required by a game object, or create it if no mesh is found.
	// Params:
	//   objectName = The name of the mesh.
	//   spriteSource = The sprite source to base UV size off of. This is used when creating the mesh if the mesh was not found.
	//   extents = The extents of the mesh to create if the mesh was not found.
	// Returns:
	//   The mesh with the specified name.
	Mesh* GetMesh(const std::string& objectName, SpriteSource* spriteSource, Vector2D extents = Vector2D(0.5f, 0.5f));

	// Retrieve an existing mesh required by a game object, or create it if no mesh is found.
	// Params:
	//   objectName = The name of the mesh.
	//   color0 = The color of the first vertex in the mesh to create if the mesh was not found.
	//   color1 = The color of the second vertex in the mesh to create if the mesh was not found.
	//   color2 = The color of the third vertex in the mesh to create if the mesh was not found.
	// Returns:
	//   The mesh with the specified name.
	Mesh* GetMesh(const std::string& objectName, const Color& color0, const Color& color1, const Color& color2);

	// Retrieve a sprite source that uses a given texture, create it if not found.
	// Params:
	//	 textureName = Filename of the texture used by the sprite source.
	//	 numCols = The number of columns in the sprite sheet.
	//	 numRows = The number of rows in the sprite sheet.
	//   createIfNotFound = Whether to create a sprite source if no sprite source is found.
	// Returns:
	//   The sprite source with the specified name.
	SpriteSource* GetSpriteSource(const std::string& textureName, int numCols = 1, int numRows = 1, bool createIfNotFound = true);
  
	// Retrieve a sprite source that uses a given texture, create it if not found.
	// Params:
	//	 textureName = Filename of the texture used by the sprite source.
	//   createIfNotFound = Whether to create a sprite source if no sprite source is found.
	// Returns:
	//   The texture with the specified name.
	Texture* GetTexture(const std::string& textureName, bool createIfNotFound = true);

	// Adds a pre-made mesh required by a game object.
	// Params:
	//   objectName = The name of the mesh.
	//   mesh = The mesh to add.
	// Returns:
	//   The mesh that was added.
	Mesh* AddMesh(const std::string& objectName, Mesh* mesh);

	// Adds a pre-made sprite source required by a game object.
	// Params:
	//   objectName = The name of the sprite source.
	//   spriteSource = The sprite source to add.
	// Returns:
	//   The sprite source that was added.
	SpriteSource* AddSpriteSource(const std::string& objectName, SpriteSource* spriteSource);

	// Adds a pre-made texture required by a game object.
	// Params:
	//   objectName = The name of the texture.
	//   texture = The texture to add.
	// Returns:
	//   The texture that was added.
	Texture* AddTexture(const std::string& objectName, Texture* texture);

	// To be called when a level has finished loading its resources.
	void OnLevelLoad();

	// To be called when a level has finished unloading its resources.
	void OnLevelUnload();

	// Unloads all resources used by the resource manager.
	void Unload();

private:
	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	enum class ResourceType
	{
		MESH = 0,
		SPRITESOURCE,
		TEXTURE,
		RESOURCETYPE_MAX
	};

	struct Resource
	{
	public:
		//------------------------------------------------------------------------------
		// Public Functions:
		//------------------------------------------------------------------------------

		// Conversion constructor
		// Params:
		//   mesh = The mesh resource.
		Resource(Mesh* mesh);

		// Conversion constructor
		// Params:
		//   spriteSource = The sprite source resource.
		Resource(SpriteSource* spriteSource);

		// Conversion constructor
		// Params:
		//   texture = The texture resource.
		Resource(Texture* texture);

		// Destructor
		~Resource();

		//------------------------------------------------------------------------------
		// Public Variables:
		//------------------------------------------------------------------------------

		ResourceType type;
		unsigned references;
		union
		{
			void* genericPointer;
			Mesh* mesh;
			SpriteSource* spriteSource;
			Texture* texture;
		};
	};

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Creates a texture if the given texture name is not "none"
	// Params:
	//   textureName = The file to load the texture from.
	Texture* CreateTextureFromFile(const std::string& textureName);

	// Gets the number of references to the specified resource.
	// Params:
	//   objectName = The name of the object.
	// Returns:
	//   How many times the resource has been referenced.
	unsigned GetResourceReferenceCount(const std::string& objectName);
	
	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Container for all resources
	std::map<std::string, Resource*> resources;
};

//------------------------------------------------------------------------------
