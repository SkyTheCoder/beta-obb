//------------------------------------------------------------------------------
//
// File Name:	Engine.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "BetaObject.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Engine : public BetaObject
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	// Starts the game loop and runs until quit state is reached.
	// Params:
	//   screenWidth  = The width of the window that will be created in pixels.
	//   screenHeight = The height of the window that will be created in pixels.
	//   framerateCap = The maximum framerate of the engine.
	//   debugConsole = If true, creates a debug console.
	void Start(unsigned screenWidth, unsigned screenHeight, unsigned framerateCap = 200, bool debugConsole = true);

	// Stops the engine and shuts everything down
	void Stop();

	// Add an additional module to the engine, which will be updated.
	// Params:
	//   module = A pointer to the module that will be added to the engine. 
	//      This module should be dynamically allocated.
	void AddModule(BetaObject* module);

	// Retrieves the module with the given type if it exists.
	// Params:
	//   ModuleType = A template parameter that specifies the class type 
	//      of the module being added to the engine.
	// Return:
	//   A pointer to the specified module.
	template<typename ModuleType>
	ModuleType* GetModule()
	{
		size_t numModules = modules.size();
		for (size_t i = 0; i < numModules; ++i)
		{
			// If the type name contains the name we're searching for, return it
			if (typeid(ModuleType) == typeid(*modules[i]) ||
				dynamic_cast<ModuleType*>(modules[i]) != nullptr)
				return dynamic_cast<ModuleType*>(modules[i]);
		}

		return nullptr;
	}

	// Returns the path that contains all assets
	const std::string& GetFilePath() const;

	// Sets the path used for assets
	// Params:
	//   directory = The relative path to the master assets directory. Textures, 
	//      objects, shaders, and sound files should be placed in subdirectories within this directory.
	void SetFilePath(const std::string& directory = "Assets/");

	// Set whether pressing the escape key will close the program.
	void SetCloseOnEscape(bool closeOnEscape);

	// Returns the single instance of the Engine class
	static Engine& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Function Declarations:
	//------------------------------------------------------------------------------

	// Constructor/destructor private to prevent accidental instantiation/destruction.
	Engine();
	~Engine();

	// Disable copy constructor and assignment operator
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	// Initialize custom modules.
	void Initialize() override;

	// Update all engine modules.
	// Params:
	//	 dt = Change in time (in seconds) since the last game loop.
	void Update(float dt) override;

	// Shutdown custom modules.
	void Shutdown() override;

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	// Is the engine currently running?
	bool isRunning;

	// Pointers to additional engine modules
	std::vector<BetaObject*> modules;

	// Assets directory
	std::string assetsPath;

	// Should the program stop when escape is pressed?
	bool closeOnEscape;
};

/*----------------------------------------------------------------------------*/
