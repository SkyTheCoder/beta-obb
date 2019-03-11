//------------------------------------------------------------------------------
//
// File Name:	Animation.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		BetaFramework
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Animation.h"

// Systems
#include "GameObject.h"

// Components

#include "Sprite.h"

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------
// You are free to change the contents of this class as long as you do not
// change the public functions declared in the header.

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Construct a new animation object.
// Params:
//   sprite = The sprite this animation will be manipulating.
Animation::Animation() : Component("Animation"), frameIndex(0), frameCount(0), frameStart(0), frameDelay(0.0f),
	frameDuration(0.0f), isRunning(false), isLooping(false), isDone(false), sprite(nullptr)
{
}

// Clone an animation, returning a dynamically allocated copy.
Component* Animation::Clone() const
{
	return new Animation(*this);
}

// Initialize components.
void Animation::Initialize()
{
	sprite = GetOwner()->GetComponent<Sprite>();
}

// Play a simple animation sequence (0 .. frameCount).
// Params:
//   frameStart = The starting frame for the sequence.
//	 frameCount = The number of frames in the sequence.
//	 frameDuration = The amount of time to wait between frames (in seconds).
//	 isLooping = True if the animation loops, false otherwise.
void Animation::Play(unsigned frameStart_, unsigned frameCount_, float frameDuration_, bool isLooping_)
{
	frameStart = frameStart_;
	frameIndex = frameStart_;
	frameCount = frameCount_;
	frameDuration = frameDuration_;
	frameDelay = frameDuration_;
	isLooping = isLooping_;

	// The animation is now running and is not completed.
	isRunning = true;
	isDone = false;

	// Display the first frame of the animation.
	sprite->SetFrame(frameIndex);
}

// Update the animation.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void Animation::Update(float dt)
{
	// Clear isDone. If the animation is done, it will be overwritten later in the function.
	isDone = false;

	// Exit if the animation is not running.
	if (!isRunning)
	{
		return;
	}

	// Lower the current frame delay.
	frameDelay -= dt;

	// Check if the current frame's time is up.
	if (frameDelay <= 0.0f)
	{
		// Calculate the next frame index.
		unsigned nextFrameIndex = frameIndex + 1;

		// Check if the next frame index is outside the range of the animation.
		if (nextFrameIndex >= frameStart + frameCount)
		{
			// If the animation is looping, set the next frame index back to the start of the animation.
			if (isLooping)
			{
				nextFrameIndex = frameStart;
			}

			// Mark the animation as done.
			isDone = true;
		}

		// Check if the next frame should be set.
		if (!isDone || isLooping)
		{
			frameDelay = frameDuration;
			frameIndex = nextFrameIndex;

			// Display the next frame.
			sprite->SetFrame(frameIndex);
		}
	}
}

// Determine if the animation has reached the end of its sequence.
// Returns:
//	 The value in isDone.
bool Animation::IsDone() const
{
	return isDone;
}

//----------------------------------------------------------------------------
