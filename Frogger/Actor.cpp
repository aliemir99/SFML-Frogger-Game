#include "Actor.h"
#include "ResourceHolder.h"
#include "ResourceIdentifier.h"
#include "JsonFrameParser.h"
#include "Utility.h"
#include "DataTables.h"
#include "TextNode.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>

/*
	Team: std::pair(Ali-Ibrahim)
	Date: 2020-12-07
	Purpose: Create a frogger replica using GEX Engine
*/
namespace
{
	const std::map<Actor::Type, ActorData> TABLE = initializeActorData();
}
Actor::Actor(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts)
	: Entity(100)
	, type_(type)
	, state_(State::Idle)
	, sprite_(textures.get(TABLE.at(type).texture))
	, death(textures.get(TextureID::FrogArtifacts))
	, direction_(Direction::Up)
	, travelDistance_(0.f)
	, directionIndex_(0)
	, worldBounderiesWidth(700.f)
	, batTouched(false)
	, frogScore(0)
	, countdown(0.f)
	, elapsedTime()
	, clock()
{
	for (auto a : TABLE.at(type).animations)
	{

		animations_[a.first] = a.second;
	}

	if (getCategory() == Category::Turtle) {
		setState(State::Motion);
		setDirection(Direction::Left);
	}


	if (getCategory() == Category::Vehicle) {
		if (type_ > (Type)3 && type_ < (Type)6) {
			setDirection(Direction::Left);
		}
		else {
			setDirection(Direction::Right);
		}

		if (direction_ == Direction::Left) {
			centerOrigin(sprite_);
			sprite_.rotate(180);
		}
	}

	if (getCategory() == Category::Log) {
		setDirection(Direction::Right);
	}

	if (getCategory() == Category::Crocodile) {
		setDirection(Direction::Right);
	}

	if (type_ == Actor::Type::Frog) {
		sprite_.setTextureRect(sf::IntRect());
		sprite_.setScale(sf::Vector2f(0.7f, 0.7f));
		centerOrigin(sprite_);
	}

}
unsigned int Actor::getCategory() const
{
	switch (type_)
	{

	case Type::Frog:
		return Category::Frog;
		break;
	case Type::Vehicle1:
	case Type::Vehicle2:
	case Type::Vehicle3:
	case Type::Vehicle4:
	case Type::Vehicle5:
		return Category::Vehicle;
		break;
	case Type::Log1:
	case Type::Log2:
	case Type::Log3:
		return Category::Log;
		break;
	case Type::Turtle1:
	case Type::Turtle2:
		return Category::Turtle;
		break;
	case Type::Lake:
		return Category::Lake;
		break;
	case Type::Crocodile:
		return Category::Crocodile;
		break;
	case Type::Bat:
		return Category::Bat;
		break;
	}
}
void Actor::updateMovementPattern(sf::Time dt)
{
	// movement pattern
	if (Actor::Type::Frog == type_) {
		auto directions = TABLE.at(type_).directions;
		if (!directions.empty())
		{
			if (travelDistance_ > (directions[directionIndex_].distance))
			{
				directionIndex_ = (++directionIndex_) % directions.size();
				travelDistance_ = 0;
			}
			float radians = toRadian(directions[directionIndex_].angle + 90.f);
			float vx = getMaxSpeed() * std::cos(radians);
			float vy = getMaxSpeed() * std::sin(radians);
			setVelocity(vx, vy);
			travelDistance_ += getMaxSpeed() * dt.asSeconds();
		}
	}

	if (getCategory() == Category::Vehicle) {
		switch (direction_)
		{
		case Actor::Direction::Left:
			if (Actor::Type::Vehicle5 == type_) {
				if (this->getPosition().x < 0.f) {
					this->setPosition(700, this->getPosition().y);
				}
				this->move(-1.2f, 0.f);
			}
			else {// VEHICLE 4
				if (this->getPosition().x < 0.f) {
					this->setPosition(700, this->getPosition().y);
				}
				this->move(-1.4f, 0.f);
			}
			break;
		case Actor::Direction::Right:
			if (Actor::Type::Vehicle1 == type_) {
				if (this->getPosition().x - this->getBoundingRect().width > worldBounderiesWidth) {
					this->setPosition(-100, this->getPosition().y);
				}
				this->move(1.6f, 0.f);
			}
			else if (Actor::Type::Vehicle2 == type_) {
				if (this->getPosition().x - this->getBoundingRect().width > worldBounderiesWidth) {
					this->setPosition(-100, this->getPosition().y);
				}
				this->move(1.8f, 0.f);
			}
			else if (Actor::Type::Vehicle3 == type_) {
				if (this->getPosition().x - this->getBoundingRect().width > worldBounderiesWidth) {
					this->setPosition(-100, this->getPosition().y);
				}
				this->move(2.2f, 0.f);
			}
			break;
		default:
			break;
		}
	}

	if (getCategory() == Category::Log) {
		if (this->getPosition().x - this->getBoundingRect().width > worldBounderiesWidth) {
			this->setPosition(-100, this->getPosition().y);
		}
		if (type_ == Actor::Type::Log1) {
			this->move(1.8f, 0.f);
			this->setVelocity(1.8f, 0.f);
		}
		else if (type_ == Actor::Type::Log2) {
			this->move(1.6f, 0.f);
			this->setVelocity(1.6f, 0.f);
		}
		else if (type_ == Actor::Type::Log3) {
			this->move(1.4f, 0.f);
			this->setVelocity(1.4f, 0.f);
		}

	}

	if (getCategory() == Category::Crocodile) {
		if (this->getPosition().x - this->getBoundingRect().width > worldBounderiesWidth) {
			this->setPosition(-325, this->getPosition().y);
		}
		this->move(1.6f, 0.f);
	}

	if (getCategory() == Category::Turtle) {
		if (this->getPosition().x < 0.f) {
			this->setPosition(740, this->getPosition().y);
		}
		this->move(-1.5f, 0.f);
		this->setVelocity(-1.5f, 0.f);
	}

}
sf::FloatRect Actor::getBoundingRect() const
{
	auto box = getWorldTransform().transformRect(sprite_.getGlobalBounds());
	if (Actor::Type::Frog == type_) {
		box.width -= 15; // tighten up bounding box for more realistic collisions
		box.left += 7;
		box.height -= 5;
	}
	else if (getCategory() == Category::Log) {
		//box.height -= 2;
		//box.top += 2;
	}
	else if (getCategory() == Category::Turtle) {
		box.height -= 7;
		box.top += 4;
	

	}
	else if (getCategory() == Category::Lake) {
		box.height -= 15.f;
	}
	else {
		box.height -= 30;
		box.top += 10;
	}

	return box;
}
float Actor::getMaxSpeed() const
{
	return TABLE.at(type_).speed;
}
bool Actor::isMarkedForRemoval() const
{
	return false; // (state_ == State::Dead && animations_.at(state_).isFinished());
}

void Actor::setState(State state)
{
	state_ = state;
	if (Actor::Type::Frog == type_) {
		animations_.at({ state_ ,direction_ }).restart();
	}
}
Actor::State Actor::getState() const
{
	return state_;
}
void Actor::setDirection(Actor::Direction direction)
{
	direction_ = direction;
}
void Actor::updateStates()
{

	if (Actor::Type::Frog == type_) {
		auto pair = std::make_pair(state_, direction_);

		if (isDestroyed())
			state_ = Actor::State::Dead;

		if (Actor::State::Dead == state_ && animations_[pair].isFinished()) {
			setState(Actor::State::Idle);
			this->repair(100);
			this->setPosition(350.f, 580.f);
			animations_[pair].restart();
		}

		if (Direction::Up == direction_ && Actor::State::Jump == state_ && animations_[pair].isFinished())
			state_ = Actor::State::Idle;

		if (Direction::Down == direction_ && Actor::State::Jump == state_ && animations_[pair].isFinished())
			state_ = Actor::State::Idle;

		if (Direction::Left == direction_ && Actor::State::Jump == state_ && animations_[pair].isFinished())
			state_ = Actor::State::Idle;

		if (Direction::Right == direction_ && Actor::State::Jump == state_ && animations_[pair].isFinished())
			state_ = Actor::State::Idle;
	}
	auto pair = std::make_pair(state_, direction_);
	if (getCategory() == Category::Turtle && state_ == State::Idle && direction_ == Direction::Left) {
		elapsedTime = clock.restart();
		setState(State::Idle);
		countdown += elapsedTime.asSeconds();
		if (countdown >= 7.f) {
			setState(State::Motion);
			countdown = 0;
		}
	}
	else if (getCategory() == Category::Turtle && Actor::State::Motion == state_ && Direction::Left == direction_ && animations_[pair].isFinished()) {
			setState(State::Idle);
			animations_[pair].restart();
	}
}


void Actor::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateStates();
	if (Actor::Type::Frog == type_) {
		if (Actor::Type::Frog == type_ && state_ != Actor::State::Dead) {
			auto pair = std::make_pair(state_, direction_);
			auto rec = animations_.at(pair).update(dt);
			sprite_.setTextureRect(rec);
		}

		if (state_ == State::Dead)
		{
			auto pair = std::make_pair(state_, direction_);
			auto rec = animations_.at(pair).update(dt);
			death.setTextureRect(rec);
			auto x = sprite_.getPosition().x;
			auto y = sprite_.getPosition().y;
			death.setPosition(x, y);
		}

		centerOrigin(sprite_);
		if (state_ != State::Dead) // dont move it while dying
			Entity::updateCurrent(dt, commands);
	}

	if (getCategory() == Category::Turtle) {
		auto pair = std::make_pair(state_, direction_);
		if (state_ != Actor::State::Idle) {
			auto rec = animations_[pair].update(dt);
			sprite_.setTextureRect(rec);
		}
		centerOrigin(sprite_);
	}
	updateMovementPattern(dt);
}
void Actor::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (type_ == Type::Frog) {
		if (State::Dead != state_)
			target.draw(sprite_, states);
		else
			target.draw(death, states);
	}
	else if (type_ == Type::Bat) {
		if (isBatTouched()) {
			target.draw(sprite_, states);
		}
	}
	else {
		target.draw(sprite_, states);
	}
}

bool Actor::isBatTouched() const
{
	return batTouched;
}

bool Actor::setIsBatTouched(bool b)
{
	return  batTouched = b;
}

void Actor::setFrogScore(int score)
{
	frogScore += score;
}

size_t Actor::getFrogScore() const
{
	return frogScore;
}

