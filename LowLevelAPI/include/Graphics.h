//------------------------------------------------------------------------------
//
// File Name:	Graphics.h
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

#include "Matrix2D.h"
#include "Color.h"
#include "Camera.h"
#include "Texture.h"
#include "Shapes2D.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class Matrix2D;
class Renderer;

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

enum BlendMode
{
	BM_None = 0,
	BM_Blend,
	BM_Add,
	BM_Multiply,
	BM_Screen,

	// Keep this one last
	BM_Num
};

enum RenderMode
{
	RM_None = -1,
	RM_Forward,

	// Keep this one last
	RM_Num,
};

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

class Graphics
{
public:
	//------------------------------------------------------------------------------
	// Public Functions:
	//------------------------------------------------------------------------------

	//////////////////////
	// ENGINE FUNCTIONS //
	//////////////////////

	void Init(unsigned width, unsigned height);
	void FrameStart();
	void Draw(unsigned arrayObjectID, unsigned drawMode, unsigned numVertices);
	void FrameEnd();
	void Shutdown();

	/////////////////////
	// COMMON SETTINGS //
	/////////////////////

	// Otherwise uncolored pixels will have this color
	const Color& GetBackgroundColor() const;
	void SetBackgroundColor(const Color& color = Colors::Black);

	// Color to blend with whole screen
	const Color& GetScreenTintColor() const;
	void SetScreenTintColor(const Color& color = Colors::White);

	// Color to blend with the current sprite's color
	void SetSpriteBlendColor(const Color& color = Colors::White);

	// Texture to use when drawing
	void SetTexture(const Texture* texture, const Vector2D& uv = Vector2D());

	// Transform
	void SetTransform(const Matrix2D& matrix, float depth = 0.0f);
	void SetTransform(const Vector2D& translation, const Vector2D& scale = Vector2D(1.0f, 1.0f), float rotation = 0.0f, float depth = 0.0f);

	// Camera
	Camera& GetDefaultCamera();
	Camera& GetCurrentCamera();
	void SetCurrentCamera(Camera& camera);

	// Screen to world
	const BoundingRectangle GetScreenWorldDimensions() const;
	Vector2D ScreenToWorldPosition(const Vector2D& screenPosition, const Camera* camera = nullptr) const;

	////////////////////
	// OTHER SETTINGS //
	////////////////////

	// Test whether depth is currently enabled.
	bool IsDepthEnabled() const;
	// Enable (limited) perspective and depth test.
	// WARNING: Objects drawn at the same depth may occlude each other.
	void SetDepthEnabled(bool enabled);

	// Sets how sprites are blended
	void SetBlendMode(BlendMode mode, bool forceSet = false);

	// Test whether the application is in fullscreen mode.
	bool IsFullScreen() const;
	// Set whether the window is fullscreen.
	void SetFullScreen(bool fullscreen);

	// Set the resolution of the window.
	// Params:
	//   width = The new width of the window.
	//   height = The new height of the window.
	void SetResolution(unsigned width, unsigned height);

	///////////////////////
	// ADVANCED SETTINGS //
	///////////////////////

	// Test whether vertical sync is currently on
	bool GetUseVsync() const;
	// Turn vertical sync on or off - will cause performance issues on some machines
	void SetUseVSync(bool useVsync);

	// Set the dimensions of the viewport.
	// Params:
	//   width = The new width of the viewport.
	//   height = The new height of the viewport.
	void SetViewport(int width, int height);

	// Sets the current render mode - currently only one setting available.
	void SetRenderMode(RenderMode mode);

	// Has graphics been initialized?
	bool IsInitialized() const;

	// Gets the single instance of the Graphics class.
	static Graphics& GetInstance();

private:
	//------------------------------------------------------------------------------
	// Private Functions:
	//------------------------------------------------------------------------------

	// Constructor and destructor are private to prevent accidental instantiation/deletion.
	Graphics();
	~Graphics();

	// Copy constructor and assignment are disabled.
	Graphics(const Graphics& other) = delete;
	Graphics& operator=(const Graphics& other) = delete;

	//------------------------------------------------------------------------------
	// Private Variables:
	//------------------------------------------------------------------------------

	class Implementation;
	Implementation* pimpl;
};

//------------------------------------------------------------------------------
