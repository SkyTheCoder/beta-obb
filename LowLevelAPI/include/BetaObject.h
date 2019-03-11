//------------------------------------------------------------------------------
//
// File Name:	BetaObject.h
// Author(s):	Jeremy Kings (j.kings)
// Project:		BetaFramework
// Course:		WANIC VGP2
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include <unordered_map> // unordered_map
#include <type_traits>   // alignment_of

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

// This class provides a common interface for things like engine systems,
// levels/game states, game objects, and components. It is an abstract 
// class, so while objects of this class are illegal, game systems and objects 
// should almost always inherit from this class or one of its subclasses.

class BetaObject
{
public:
	//------------------------------------------------------------------------------
	// Constructors and Destructors:
	//------------------------------------------------------------------------------

	// Constructor
	// Params:
	//   name = The name of this object.
	//   parent = The parent object to this beta object (defaults to no parent).
	BetaObject(const std::string& name, BetaObject* parent = nullptr);

	// Copy constructor
	BetaObject(const BetaObject& other);

	// Assignment operator
	BetaObject& operator=(const BetaObject& other);

	// Destructor
	// This function is virtual to ensure destruction works properly
	// with inherited classes. It is PURE virtual (= 0) to force the class to
	// be abstract without forcing the user to implement any functions.
	virtual ~BetaObject() = 0;

	//------------------------------------------------------------------------------
	// Game Loop Functions:
	//------------------------------------------------------------------------------

	// Loads external resources associated with this object. 
	// Occurs when a level starts for the first time.
	virtual void Load();

	// Resets instance data and reinstantiates child objects. 
	// Occurs when a level is loaded or restarted.
	virtual void Initialize();

	// Executes all operations that should occur every frame.
	// Params:
	//   dt = The change in time between last frame and the current frame.
	virtual void Update(float dt);

	// Updates components using a fixed timestep (usually just for physics).
	// Params:
	//	 dt = A fixed change in time, usually 1/60th of a second.
	virtual void FixedUpdate(float dt);

	// Draw this object. Could be a sprite, text, or debug visuals.
	virtual void Draw();

	// Removes any objects that will be recreated in Initialize.
	virtual void Shutdown();

	// Unloads external assets associated with this object. 
	// Occurs when changing levels.
	virtual void Unload();

	//------------------------------------------------------------------------------
	// Other Public Functions:
	//------------------------------------------------------------------------------

	// Returns the name of the object.
	const std::string& GetName() const;

	// Returns the globally unique id of the object.
	const GUID GetID() const;

	// Retrieve the object that contains this object.
	BetaObject* GetParent() const;

	// Set the object that contains this object.
	void SetParent(BetaObject* object);

	// Returns a pointer to an object with the given GUID, if it exists.
	// Returns nullptr if the object does not exist (was destroyed).
	static BetaObject* GetObjectByID(GUID id);

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Generates a unique identifier for the object.
	void GenerateID();

	//------------------------------------------------------------------------------
	// Private Data:
	//------------------------------------------------------------------------------

	// Name of the object.
	std::string name;

	// ID of the object
	GUID id;

	// Object that contains this object.
	BetaObject* parent;

	// All systems, levels, and game objects that have been created
	static std::unordered_map<GUID, BetaObject*> masterObjectList;
};

//------------------------------------------------------------------------------
// Global Functions:
//------------------------------------------------------------------------------

namespace std 
{
	// Hash function for GUIDs (for unordered map)
	template<> struct hash<GUID>
	{
		// There's already a hash for GUID/UUID in MSVC, but it's not
		// a std::hash function object, so we have to do this...
		size_t operator()(const GUID& guid) const noexcept {
			RPC_STATUS status = RPC_S_OK;
			return ::UuidHash(&const_cast<GUID&>(guid), &status);
		}
	};

	// Less than operator for GUIDs (for set)
	bool operator<(const GUID& first, const GUID& second);

	// Output operator for GUIDs
	std::ostream& operator<<(ostream& os, const GUID& guid);
}
