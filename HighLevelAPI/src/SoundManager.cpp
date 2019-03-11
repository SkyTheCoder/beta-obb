//------------------------------------------------------------------------------
//
// File Name:	SoundManager.cpp
// Author(s):	David Cohen (david.cohen)
// Project:		Project 8
// Course:		WANIC VGP2 2018-2019
//
// Copyright © 2018 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "SoundManager.h"

// Systems
#include <fmod_errors.h>

namespace
{
	// Helper function for making sure fmod functions succeed.
	// Params:
	//   result = The value return from the fmod function.
	void FMOD_Assert(FMOD_RESULT result)
	{
		// Check if the result indicates a failure.
		if (result != FMOD_OK)
		{
			// Print out the error code and the associated error message.
			std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
		}
	}
};

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Constructor - initializes FMOD.
SoundManager::SoundManager() : BetaObject("SoundManager"), numSounds(0), numBanks(0),
	musicChannel(nullptr), effectsChannelGroup(nullptr), system(nullptr), studioSystem(nullptr),
	musicVolume(1.0f), effectsVolume(0.0f),
	audioFilePath("Audio/"), bankFilePath("Banks/"), eventPrefix("event:/")
{
	// Setup the studio system.
	FMOD_Assert(FMOD::Studio::System::create(&studioSystem));
	FMOD_Assert(studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));

	// Get the low level system.
	FMOD_Assert(studioSystem->getLowLevelSystem(&system));

	// Create the sound effects channel group.
	FMOD_Assert(system->createChannelGroup("SoundEffects", &effectsChannelGroup));
}

// Destructor
SoundManager::~SoundManager()
{
	// Unload the studio system.
	FMOD_Assert(studioSystem->release());
}

// Update the FMOD system.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
void SoundManager::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	// Update all sounds.
	FMOD_Assert(studioSystem->update());
}

// Shutdown the sound manager.
void SoundManager::Shutdown(void)
{
	// Unload all sounds.
	for (size_t i = 0; i < numSounds; i++)
	{
		FMOD_Assert(soundList[i]->release());
	}
	numSounds = 0;

	// Unload all banks.
	for (size_t i = 0; i < numBanks; i++)
	{
		FMOD_Assert(bankList[i]->unload());
	}
	numBanks = 0;
}

// Creates a non-looping FMOD sound.
// Params:
//	 filename = Name of the sound file (WAV).
void SoundManager::AddEffect(const std::string& filename)
{
	AddSound(filename, FMOD_DEFAULT);
}

// Creates an FMOD stream for a music file.
// Params:
//	 filename = Name of the music file (MP3).
void SoundManager::AddMusic(const std::string& filename)
{
	AddSound(filename, FMOD_DEFAULT | FMOD_LOOP_NORMAL);
}

// Creates an FMOD sound bank
// Params:
//   filename = Name of the FMOD bank file.
void SoundManager::AddBank(const std::string& filename)
{
	// Build the full bank file path.
	std::string fullFilePath = "Assets/" + audioFilePath + bankFilePath + filename;
	FMOD::Studio::Bank* bank;

	// Load the bank file and add it to the bank list.
	FMOD_Assert(studioSystem->loadBankFile(fullFilePath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
	bankList[numBanks++] = bank;
}

// Plays a sound with the specified name.
// Params:
//	 name = The name of the sound to be played.
// Return:
//   The channel that was used to play the given sound.
FMOD::Channel* SoundManager::PlaySound(const std::string& name_)
{
	// Temporary variables to store sound properties.
	char soundName[100] = { 0 };
	FMOD_MODE soundMode;

	// Loop through all loaded sounds.
	for (size_t i = 0; i < numSounds; i++)
	{
		// Get the current sound's name.
		FMOD_Assert(soundList[i]->getName(soundName, 100));
		
		// Skip the sound if its name does not match.
		if (strcmp(soundName, name_.c_str()))
			continue;

		// Get the current sound's mode.
		FMOD_Assert(soundList[i]->getMode(&soundMode));

		if (soundMode & FMOD_CREATESTREAM)
		{
			// If the sound is an MP3, it's music.
			return PlayMusic(soundList[i]);
		}
		else
		{
			// If the sound is not an MP3, it's a sound effect.
			return PlayEffect(soundList[i]);
		}
	}

	return nullptr;
}

// Starts an audio event with the given name.
// The event must be in one of the loaded banks.
// Params:
//	 name = The name of the event that will be started.
FMOD::Studio::EventInstance* SoundManager::PlayEvent(const std::string& name_)
{
	// Build the full event name.
	std::string fullEventName = eventPrefix + name_;
	FMOD::Studio::EventDescription* eventDescription;
	FMOD::Studio::EventInstance* eventInstance;

	// Find the specified event.
	FMOD_Assert(studioSystem->getEvent(fullEventName.c_str(), &eventDescription));

	// Create a new instance of the specified event.
	FMOD_Assert(eventDescription->createInstance(&eventInstance));

	return eventInstance;
}

// Set the volume of the music channel.
// Params:
//	 volume = New value for the music volume (0.0 to 1.0).
void SoundManager::SetMusicVolume(float volume)
{
	musicVolume = volume;

	// If music is playing, set its volume.
	if (musicChannel != nullptr)
		FMOD_Assert(musicChannel->setVolume(musicVolume));
}

// Set the volume of the sound effects channel group.
// Params:
//	 volume = New value for the FX volume (0.0 to 1.0).
void SoundManager::SetEffectsVolume(float volume)
{
	effectsVolume = volume;

	// Set the volume of all sound effects.
	FMOD_Assert(effectsChannelGroup->setVolume(effectsVolume));
}

// Get the volume of the music channel.
// Return:
//	 volume = Current value for the music volume.
float SoundManager::GetMusicVolume() const
{
	return musicVolume;
}

// Get the volume of the sound effects channel group.
// Return:
//	 volume = Current value for the FX volume.
float SoundManager::GetEffectsVolume() const
{
	return effectsVolume;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Different behaviors for SFX vs BGM

// Plays a sound effect.
// Params:
//   sound = The sound instance.
// Returns:
//   The channel the sound is using.
FMOD::Channel* SoundManager::PlayEffect(FMOD::Sound* sound) const
{
	FMOD::Channel* channel;

	// Play the sound in the sound effects channel group.
	FMOD_Assert(system->playSound(sound, effectsChannelGroup, false, &channel));

	return channel;
}

// Plays a music track.
// Params:
//   sound = The sound instance.
// Returns:
//   The channel the music is using.
FMOD::Channel* SoundManager::PlayMusic(FMOD::Sound* sound)
{
	// If music is currently playing, stop it.
	if (musicChannel != nullptr)
		FMOD_Assert(musicChannel->stop());

	// Play the music.
	FMOD_Assert(system->playSound(sound, nullptr, false, &musicChannel));

	return musicChannel;
}

// Add FX/BGM helper

// Loads a sound file and adds it to the sound list.
// Params:
//   filename = The name of the sound to load.
//   mode = The mode to use when loading the sound.
void SoundManager::AddSound(const std::string& filename, FMOD_MODE mode)
{
	// Build the full sound file path.
	std::string fullFilePath = "Assets/" + audioFilePath + filename;
	FMOD::Sound* sound;

	// Load the sound file with the specified mode and add it to the sound list.
	FMOD_Assert(system->createSound(fullFilePath.c_str(), mode, nullptr, &sound));
	soundList[numSounds++] = sound;
}

//------------------------------------------------------------------------------
