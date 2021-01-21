#pragma once

#include <SFML/Window/Event.hpp>
#include"Command.h"
#include <map>
#include "SoundPlayer.h"

/*
	Team: std::pair(Ali-Ibrahim)
	Date: 2020-12-07
	Purpose: Create a frogger replica using GEX Engine
*/

//forward decleration
class CommandQueue;

class Player
{
public:
	enum class Action
	{
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		ActionCount
	};
	enum class MissionStatus
	{
		Running,
		Success,
		Failure,
	};

public:
											Player();
	void									initializeKeyBindings();
	void									handleEvent(const sf::Event& event, CommandQueue& commands);
	

	void									setMissionStatus(MissionStatus status);
	MissionStatus							getMissionStatus() const;

private:
	void									initializeActions();
	

	MissionStatus							currentMissionStatus;
	SoundPlayer								sounds;
private:
	std::map<sf::Keyboard::Key, Action>		keyBindings;
	std::map<Action, Command>				actionBindings;
};

