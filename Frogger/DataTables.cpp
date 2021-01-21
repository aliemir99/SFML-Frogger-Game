#include "DataTables.h"
#include "JsonFrameParser.h"

std::map<Actor::Type, ActorData> initializeActorData()
{
	std::map<Actor::Type, ActorData> data;
	data[Actor::Type::Frog].texture = TextureID::Frog;

	data[Actor::Type::Vehicle1].texture = TextureID::Vehicle1;
	data[Actor::Type::Vehicle2].texture = TextureID::Vehicle2;
	data[Actor::Type::Vehicle3].texture = TextureID::Vehicle3;
	data[Actor::Type::Vehicle4].texture = TextureID::Vehicle4;
	data[Actor::Type::Vehicle5].texture = TextureID::Vehicle5;

	data[Actor::Type::Log1].texture = TextureID::Log1;
	data[Actor::Type::Log2].texture = TextureID::Log2;
	data[Actor::Type::Log3].texture = TextureID::Log3;



	data[Actor::Type::Turtle1].texture = TextureID::FrogArtifacts;
	data[Actor::Type::Turtle2].texture = TextureID::FrogArtifacts;

	data[Actor::Type::Crocodile].texture = TextureID::Crocodile;
	data[Actor::Type::Bat].texture = TextureID::Bat;

	data[Actor::Type::Lake].texture = TextureID::Lake;

	JsonFrameParser frames = JsonFrameParser("Media/Textures/Frogger/frogs.json");
	JsonFrameParser frogUtilities = JsonFrameParser("Media/Textures/Frogger/frog.json");

	
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Down)].addFrameSet(frogUtilities.getFramesFor("death"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Down)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Down)].setRepeating(false);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Left)].addFrameSet(frogUtilities.getFramesFor("death"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Left)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Left)].setRepeating(false);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Right)].addFrameSet(frogUtilities.getFramesFor("death"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Right)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Right)].setRepeating(false);
																							
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Up)].addFrameSet(frogUtilities.getFramesFor("death"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Up)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Dead, Actor::Direction::Up)].setRepeating(false);

	data[Actor::Type::Turtle1].animations[std::make_pair(Actor::State::Motion, Actor::Direction::Left)].addFrameSet(frogUtilities.getFramesBackwardsFor("turtle2"));
	data[Actor::Type::Turtle1].animations[std::make_pair(Actor::State::Motion, Actor::Direction::Left)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Turtle1].animations[std::make_pair(Actor::State::Motion, Actor::Direction::Left)].setRepeating(false);

	data[Actor::Type::Turtle2].animations[std::make_pair(Actor::State::Motion, Actor::Direction::Left)].addFrameSet(frogUtilities.getFramesBackwardsFor("turtle3"));
	data[Actor::Type::Turtle2].animations[std::make_pair(Actor::State::Motion, Actor::Direction::Left)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Turtle2].animations[std::make_pair(Actor::State::Motion, Actor::Direction::Left)].setRepeating(false);


	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Down)].addFrameSet(frames.getFramesFor("frogIdleDown"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Down)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Down)].setRepeating(true);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Left)].addFrameSet(frames.getFramesFor("frogIdleLeft"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Left)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Left)].setRepeating(true);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Right)].addFrameSet(frames.getFramesFor("frogIdleRight"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Right)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Right)].setRepeating(true);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Up)].addFrameSet(frames.getFramesFor("frogIdleUp"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Up)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Idle, Actor::Direction::Up)].setRepeating(true);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Down)].addFrameSet(frames.getFramesFor("frogJumpDown"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Down)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Down)].setRepeating(false);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Left)].addFrameSet(frames.getFramesFor("frogJumpLeft"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Left)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Left)].setRepeating(false);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Right)].addFrameSet(frames.getFramesFor("frogJumpRight"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Right)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Right)].setRepeating(false);

	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Up)].addFrameSet(frames.getFramesFor("frogJumpUp"));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Up)].setDuration(sf::seconds(1.f));
	data[Actor::Type::Frog].animations[std::make_pair(Actor::State::Jump, Actor::Direction::Up)].setRepeating(false);



	return data;
}