#include "Player.h"
#include <algorithm>
#include "CommandQueue.h"
#include "Actor.h"
/*
	Team: std::pair(Ali-Ibrahim)
	Date: 2020-12-07
	Purpose: Create a frogger replica using GEX Engine
*/
Player::Player()
	: currentMissionStatus(MissionStatus::Running)
	, sounds()
{
	initializeKeyBindings();
	initializeActions();
}

void Player::initializeKeyBindings()
{
	keyBindings[sf::Keyboard::Left] = Action::MoveLeft;
	keyBindings[sf::Keyboard::Right] = Action::MoveRight;
	keyBindings[sf::Keyboard::Up] = Action::MoveUp;
	keyBindings[sf::Keyboard::Down] = Action::MoveDown;
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = keyBindings.find(event.key.code);
		if (found != keyBindings.end())
			commands.push(actionBindings[found->second]);
		
	}
}



void Player::setMissionStatus(MissionStatus status)
{
	currentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return currentMissionStatus;
}


void Player::initializeActions()
{
	const float playerSpeed = 2750.f;

	actionBindings[Action::MoveLeft].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.setDirection(Actor::Direction::Left);
			a.setState(Actor::State::Jump);
			a.accelerate(sf::Vector2f(-playerSpeed, 0.f));
		});
	actionBindings[Action::MoveRight].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.setDirection(Actor::Direction::Right);
			a.setState(Actor::State::Jump);
			a.accelerate(sf::Vector2f(playerSpeed, 0.f));
		});
	actionBindings[Action::MoveUp].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.setDirection(Actor::Direction::Up);
			a.setState(Actor::State::Jump);
			a.setFrogScore(10);
			a.accelerate(sf::Vector2f(0.f, -playerSpeed));
		});
	actionBindings[Action::MoveDown].action = derivedAction<Actor>(
		[playerSpeed](Actor& a, sf::Time dt) {
			a.setDirection(Actor::Direction::Down);
			a.setState(Actor::State::Jump);
			a.accelerate(sf::Vector2f(-0.f, playerSpeed));
		});

	for (auto& pair : actionBindings)
			pair.second.category = Category::Frog;
}

