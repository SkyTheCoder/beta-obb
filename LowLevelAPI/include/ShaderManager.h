//------------------------------------------------------------------------------
//
// File Name:	ShaderManager.h
// Author(s):	Jeremy Kings
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
// Forward Declarations:
//------------------------------------------------------------------------------

class Vector2D;
class Color;
struct Matrix3D;

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class ShaderManager
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	void Shutdown();

	unsigned LoadProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	void SetCurrentShader(int index);
	int GetCurrentShader() const;

	void SetShaderVariable(const std::string& name, int value);
	void SetShaderVariable(const std::string& name, float value);
	void SetShaderVariable(const std::string& name, const Vector2D& value);
	void SetShaderVariable(const std::string& name, const Color& value);
	void SetShaderVariable(const std::string& name, const Matrix3D& transform);

	static ShaderManager& GetInstance();

	//------------------------------------------------------------------------------
	// Public Consts:
	//------------------------------------------------------------------------------

	// No shader currently in use
	static const int shaderIndexNone;

	// Shader program loaded incorrectly
	static const unsigned programIdInvalid;

private:
	//------------------------------------------------------------------------------
	// Private Structures:
	//------------------------------------------------------------------------------

	struct ShaderProgram
	{
		// Functions
		ShaderProgram(unsigned id, const std::string& vertexShader, const std::string& pixelShader);

		// ShaderProgram operators
		bool operator==(const ShaderProgram& other);

		// Variables
		unsigned id;
		std::string vertexShader;
		std::string pixelShader;
	};

	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Constructor/destructor private to prevent accidental instantiation/destruction
	ShaderManager();
	~ShaderManager();

	// Copy constructor, assignment operator disabled
	ShaderManager(const ShaderManager& other) = delete;
	ShaderManager& operator=(const ShaderManager& other) = delete;

	// Loads shaders into OpenGL from strings/files
	void AddShaderFromString(unsigned program, const std::string& source, int glType);
	void AddShaderFromFile(unsigned program, const std::string& filename, int glType);

	// Reads contents from a file into a string
	std::string ReadFromFile(const std::string& filename);

	// Getting IDs of shader variables
	int GetUniformLocation(const std::string& name);
	int GetAttributeLocation(const std::string& name);

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	std::vector<ShaderProgram> programs;
	int currentShaderIndex;

	std::string shaderPath;

	// Storing locations for faster lookup
	std::vector<std::map<std::string, int>> uniformLocations;
	std::vector<std::map<std::string, int>> attributeLocations;
};


//------------------------------------------------------------------------------
