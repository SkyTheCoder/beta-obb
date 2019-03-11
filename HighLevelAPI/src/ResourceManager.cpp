//------------------------------------------------------------------------------
//
// File Name:	ResourceManager.cpp
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

#include "ResourceManager.h"

// Systems
#include "Mesh.h"
#include "SpriteSource.h"
#include "Texture.h"
#include "MeshHelper.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor(s)
ResourceManager::ResourceManager()
{
}

// Destructor
ResourceManager::~ResourceManager()
{
	//Unload();
}

// Retrieve an existing mesh required by a game object.
// Params:
//   objectName = The name of the mesh.
//   createIfNotFound = Whether to create a default quad mesh if no mesh is found.
Mesh* ResourceManager::GetMesh(const std::string& objectName, bool createIfNotFound)
{
	// If the resource does not exist, create it if told to.
	if (GetResourceReferenceCount("mesh" + objectName) == 0)
	{
		if (!createIfNotFound)
			return nullptr;

		return AddMesh(objectName, CreateQuadMesh(Vector2D(1.0f, 1.0f), Vector2D(0.5f, 0.5f)));
	}

	return resources.at("mesh" + objectName)->mesh;
}

// Retrieve an existing mesh required by a game object, or create it if no mesh is found.
// Params:
//   objectName = The name of the mesh.
//   textureSize = The UV size of the mesh to create if the mesh was not found.
//   extents = The extents of the mesh to create if the mesh was not found.
Mesh* ResourceManager::GetMesh(const std::string& objectName, Vector2D textureSize, Vector2D extents)
{
	// If the resource does not exist, create it.
	if (GetResourceReferenceCount("mesh" + objectName) == 0)
		return AddMesh(objectName, CreateQuadMesh(textureSize, extents));

	return resources.at("mesh" + objectName)->mesh;
}

// Retrieve an existing mesh required by a game object, or create it if no mesh is found.
// Params:
//   objectName = The name of the mesh.
//   rows = The number of rows in the UV of the mesh to create if the mesh was not found.
//   cols = The number of columns in the UV of the mesh to create if the mesh was not found.
//   extents = The extents of the mesh to create if the mesh was not found.
Mesh* ResourceManager::GetMesh(const std::string& objectName, int numCols, int numRows, Vector2D extents)
{
	// If the resource does not exist, create it.
	if (GetResourceReferenceCount("mesh" + objectName) == 0)
		return AddMesh(objectName, CreateQuadMesh(Vector2D(1.0f / numCols, 1.0f / numRows), extents));

	return resources.at("mesh" + objectName)->mesh;
}

// Retrieve an existing mesh required by a game object, or create it if no mesh is found.
// Params:
//   objectName = The name of the mesh.
//   spriteSource = The sprite source to base UV size off of. This is used when creating the mesh if the mesh was not found.
//   extents = The extents of the mesh to create if the mesh was not found.
Mesh* ResourceManager::GetMesh(const std::string& objectName, SpriteSource* spriteSource, Vector2D extents)
{
	// If the resource does not exist, create it.
	if (GetResourceReferenceCount("mesh" + objectName) == 0)
		return AddMesh(objectName, CreateQuadMesh(Vector2D(1.0f / spriteSource->GetNumCols(), 1.0f / spriteSource->GetNumRows()), extents));

	return resources.at("mesh" + objectName)->mesh;
}

// Retrieve an existing mesh required by a game object, or create it if no mesh is found.
// Params:
//   objectName = The name of the mesh.
//   color0 = The color of the first vertex in the mesh to create if the mesh was not found.
//   color1 = The color of the second vertex in the mesh to create if the mesh was not found.
//   color2 = The color of the third vertex in the mesh to create if the mesh was not found.
// Returns:
//   The mesh with the specified name.
Mesh* ResourceManager::GetMesh(const std::string& objectName, const Color& color0, const Color& color1, const Color& color2)
{
	// If the resource does not exist, create it.
	if (GetResourceReferenceCount("mesh" + objectName) == 0)
		return AddMesh(objectName, CreateTriangleMesh(color0, color1, color2));

	return resources.at("mesh" + objectName)->mesh;
}

// Retrieve a sprite source that uses a given texture, create it if not found.
// Params:
//	 textureName = Filename of the texture used by the sprite source.
//	 numCols = The number of columns in the sprite sheet.
//	 numRows = The number of rows in the sprite sheet.
//   createIfNotFound = Whether to create a sprite source if no sprite source is found.
SpriteSource* ResourceManager::GetSpriteSource(const std::string& textureName, int numCols, int numRows, bool createIfNotFound)
{
	// If the resource does not exist, create it if told to.
	if (GetResourceReferenceCount("spriteSource" + textureName) == 0)
	{
		if (!createIfNotFound)
			return nullptr;

		return AddSpriteSource(textureName, new SpriteSource(numCols, numRows, GetTexture(textureName)));
	}

	return resources.at("spriteSource" + textureName)->spriteSource;
}

// Retrieve a sprite source that uses a given texture, create it if not found.
// Params:
//	 textureName = Filename of the texture used by the sprite source.
//   createIfNotFound = Whether to create a sprite source if no sprite source is found.
Texture* ResourceManager::GetTexture(const std::string& textureName, bool createIfNotFound)
{
	// If the resource does not exist, create it if told to.
	if (GetResourceReferenceCount("texture" + textureName) == 0)
	{
		if (!createIfNotFound)
			return nullptr;

		return AddTexture(textureName, CreateTextureFromFile(textureName));
	}

	return resources.at("texture" + textureName)->texture;
}

// Adds a pre-made mesh required by a game object.
// Params:
//   objectName = The name of the mesh.
//   mesh = The mesh to add.
// Returns:
//   The mesh that was added.
Mesh* ResourceManager::AddMesh(const std::string& objectName, Mesh* mesh)
{
	Resource* resource = new Resource(mesh);
	auto emplaced = resources.emplace("mesh" + objectName, resource);

	// If the resource already existed, delete the new resource since we are not keeping track of it.
	if (!emplaced.second)
	{
		delete resource;
	}

	resource = emplaced.first->second;
	++resource->references;
	return resource->mesh;
}

// Adds a pre-made sprite source required by a game object.
// Params:
//   objectName = The name of the sprite source.
//   spriteSource = The sprite source to add.
// Returns:
//   The sprite source that was added.
SpriteSource* ResourceManager::AddSpriteSource(const std::string& objectName, SpriteSource* spriteSource)
{
	Resource* resource = new Resource(spriteSource);
	auto emplaced = resources.emplace("spriteSource" + objectName, resource);

	// If the resource already existed, delete the new resource since we are not keeping track of it.
	if (!emplaced.second)
	{
		delete resource;
	}

	resource = emplaced.first->second;
	++resource->references;
	return resource->spriteSource;
}

// Adds a pre-made texture required by a game object.
// Params:
//   objectName = The name of the texture.
//   texture = The texture to add.
// Returns:
//   The texture that was added.
Texture* ResourceManager::AddTexture(const std::string& objectName, Texture* texture)
{
	Resource* resource = new Resource(texture);
	auto emplaced = resources.emplace("texture" + objectName, resource);

	// If the resource already existed, delete the new resource since we are not keeping track of it.
	if (!emplaced.second)
	{
		delete resource;
	}

	resource = emplaced.first->second;
	++resource->references;
	return resource->texture;
}

// To be called when a level has finished loading its resources.
void ResourceManager::OnLevelLoad()
{
	// Loop through all resources.
	for (auto it = resources.begin(); it != resources.end();)
	{
		// If this resource has no references after loading the next level (left over from the last level), remove it.
		if (it->second->references == 0)
		{
			delete it->second;
			it = resources.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// To be called when a level has finished unloading its resources.
void ResourceManager::OnLevelUnload()
{
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		it->second->references = 0;
	}
}

// Unloads all resources used by the resource manager.
void ResourceManager::Unload()
{
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		delete it->second;
	}

	resources.clear();
}

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// Conversion constructor
// Params:
//   mesh = The mesh resource.
ResourceManager::Resource::Resource(Mesh* mesh) : type(ResourceType::MESH), references(0), mesh(mesh)
{
}

// Conversion constructor
// Params:
//   spriteSource = The sprite source resource.
ResourceManager::Resource::Resource(SpriteSource* spriteSource) : type(ResourceType::SPRITESOURCE), references(0), spriteSource(spriteSource)
{
}

// Conversion constructor
// Params:
//   texture = The texture resource.
ResourceManager::Resource::Resource(Texture* texture) : type(ResourceType::TEXTURE), references(0), texture(texture)
{
}

// Destructor
ResourceManager::Resource::~Resource()
{
	// Deallocate the appropriate type.
	switch (type)
	{
	case ResourceType::MESH:
		delete mesh;
		break;
	case ResourceType::SPRITESOURCE:
		delete spriteSource;
		break;
	case ResourceType::TEXTURE:
		delete texture;
		break;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Creates a texture if the given texture name is not "none"
// Params:
//   textureName = The file to load the texture from.
Texture* ResourceManager::CreateTextureFromFile(const std::string& textureName)
{
	if (textureName == "none")
		return nullptr;
	return Texture::CreateTextureFromFile(textureName);
}

// Gets the number of references to the specified resource.
// Params:
//   objectName = The name of the object.
// Returns:
//   How many times the resource has been referenced.
unsigned ResourceManager::GetResourceReferenceCount(const std::string& objectName)
{
	auto it = resources.find(objectName);

	if (it == resources.end())
		return 0;

	return it->second->references;
}

//------------------------------------------------------------------------------
