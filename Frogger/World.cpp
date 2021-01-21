#include "World.h"
#include <limits>
#include <algorithm>
#include "DataTables.h"
#include "SoundNode.h"
#include <iostream>
/*
	Team: std::pair(Ali-Ibrahim)
	Date: 2020-12-07
	Purpose: Create a frogger replica using GEX Engine
*/
World::World(sf::RenderTarget& outputTarget, FontHolder_t& fonts, SoundPlayer& sounds)
	: target(outputTarget)
	, sceneTexture()
	, worldView(target.getDefaultView())
	, textures()
	, fonts(fonts)
	, sounds(sounds)
	, sceneGraph()
	, sceneLayers()
	, commandQueue()
	, worldBounds(0.f, 0.f, worldView.getSize().x, worldView.getSize().y) //length of background scroller
	, spawnPosition(worldView.getSize().x / 2.f, worldBounds.height - worldView.getSize().y / 2)
	, batsVisited(0)
	, playerActor(nullptr)
	, timebar(sf::Vector2f(300.f,25.f))
{
	sceneTexture.create(target.getSize().x, target.getSize().y);
	loadTextures();
	buildScene();
	worldView.setCenter(spawnPosition);

	playerScoreHud.setFont(fonts.get(FontID::Main));
	playerScoreHud.setString("Score:");
	playerScoreHud.setPosition(620.f-playerScoreHud.getGlobalBounds().width,650.f);

	timerText.setFont(fonts.get(FontID::Main));
	timerText.setString("Timer");
	timerText.setPosition(300.f,625.f);

	timebar.setFillColor(sf::Color::Green);
	timebar.setPosition(200.f, 675.f);
}

void World::update(sf::Time dt)
{
	//reset player velocity
	playerActor->setVelocity(0.f, 0.f);

	setPlayerHudScoreText(playerActor->getFrogScore());

	while (!commandQueue.isEmpty()) {
		sceneGraph.onCommand(commandQueue.pop(), dt);
	}
	adaptPlayerVelocity();
	//Collision detection and response(may destroy entities)

	spawnEnemies();
	handleCollisions();

	sceneGraph.update(dt, getCommands());
	adaptPlayerPosition();
	decreaseTimer();
	updateSounds();
	if (timebar.getSize().x <= 0.f) {
		playerActor->damage(100);
		playerActor->destroy();
		playerLives.pop_back();
		resetTimer();
	}

}

void World::draw()
{
	target.setView(worldView);
	target.draw(sceneGraph);
	target.draw(playerScoreHud);

	for (auto& playerLife : playerLives) {
		target.draw(playerLife);
	}
	target.draw(timerText);
	target.draw(timebar);

}

CommandQueue& World::getCommands()
{
	return commandQueue;
}

bool World::hasAlivePlayer() const
{
	
	return playerLives.size() > 0 ;
}

bool World::hasPlayerReachedEnd() const
{
	
	return batsVisited == 5;
}

void World::loadTextures()
{
	textures.load(TextureID::FrogMap, "Media/Textures/Frogger/FroggerMap.png");
	textures.load(TextureID::FrogArtifacts, "Media/Textures/Frogger/frogsSpriteSheet.png");
	textures.load(TextureID::Frog, "Media/Textures/Frogger/frogs.png");

	textures.load(TextureID::Vehicle1, "Media/Textures/Frogger/car1.png");
	textures.load(TextureID::Vehicle2, "Media/Textures/Frogger/car2.png");
	textures.load(TextureID::Vehicle3, "Media/Textures/Frogger/car3.png");
	textures.load(TextureID::Vehicle4, "Media/Textures/Frogger/car4.png");
	textures.load(TextureID::Vehicle5, "Media/Textures/Frogger/car5.png");

	textures.load(TextureID::Log1, "Media/Textures/Frogger/log1.png");
	textures.load(TextureID::Log2, "Media/Textures/Frogger/log2.png");
	textures.load(TextureID::Log3, "Media/Textures/Frogger/log3.png");

	textures.load(TextureID::Crocodile, "Media/Textures/Frogger/Crocodile.png");
	textures.load(TextureID::Bat, "Media/Textures/Frogger/frogSuccess.png");


	textures.load(TextureID::Lake, "Media/Textures/Frogger/lake.png");

	textures.load(TextureID::Health, "Media/Textures/Frogger/frogLife.png");
}

void World::buildScene()
{
	for (std::size_t i = 0; i < LayerCount; ++i) {

		Category::Type category =
			(i == LowerAir) ? Category::Type::SceneAirLayer : Category::Type::None;

		SceneNode::Ptr layer(new SceneNode(category));
		sceneLayers[i] = layer.get();

		sceneGraph.attachChild(std::move(layer));
	}

	//add sound effect node
	std::unique_ptr<SoundNode> soundNode(new SoundNode(sounds));
	sceneGraph.attachChild(std::move(soundNode));

	//prepare background texture
	sf::Texture& texture = textures.get(TextureID::FrogMap);
	//texture.setRepeated(true);

	float viewHeight = worldView.getSize().y;
	sf::IntRect textureRect(worldBounds);
	textureRect.height += static_cast<int>(viewHeight);

	std::unique_ptr<SpriteNode> frogMap(new SpriteNode(texture, textureRect));
	frogMap->setPosition(0, 0);
	sceneLayers[Background]->attachChild(std::move(frogMap));


	//add player aircraft
	std::unique_ptr<Actor> leader(new Actor(Actor::Type::Frog, textures, fonts));
	playerActor = leader.get();
	playerActor->setPosition(worldView.getSize().x / 2.f, 585);
	playerActor->setVelocity(80.f, 0.f);
	sceneLayers[UpperAir]->attachChild(std::move(leader));


	std::unique_ptr<Actor> bat1(new Actor(Actor::Type::Bat, textures, fonts));
	bat1->setPosition(50, 20);
	sceneLayers[LowerAir]->attachChild(std::move(bat1));

	std::unique_ptr<Actor> bat2(new Actor(Actor::Type::Bat, textures, fonts));
	bat2->setPosition(185, 20);
	sceneLayers[LowerAir]->attachChild(std::move(bat2));

	std::unique_ptr<Actor> bat3(new Actor(Actor::Type::Bat, textures, fonts));
	bat3->setPosition(325, 20);
	sceneLayers[LowerAir]->attachChild(std::move(bat3));

	std::unique_ptr<Actor> bat4(new Actor(Actor::Type::Bat, textures, fonts));
	bat4->setPosition(460, 20);
	sceneLayers[LowerAir]->attachChild(std::move(bat4));

	std::unique_ptr<Actor> bat5(new Actor(Actor::Type::Bat, textures, fonts));
	bat5->setPosition(600, 20);
	sceneLayers[LowerAir]->attachChild(std::move(bat5));

	addEnemies();

	std::unique_ptr<Actor> lake(new Actor(Actor::Type::Lake, textures, fonts));
	lake->setPosition(5, 70);
	sceneLayers[LowerAir]->attachChild(std::move(lake));

	playerLives = std::vector<sf::Sprite>(3);
	std::generate(playerLives.begin(), playerLives.end(), [&]() {
		return sf::Sprite(textures.get(TextureID::Health));
		});
	
	for (int i = 0; i < playerLives.size(); ++i) {
		playerLives.at(i).setPosition
		(playerLives.at(i).getGlobalBounds().width * i, 650.f);
	}

}

void World::addEnemies()
{

	/*SPAWN CARS AND LOGS HERE*/
	addEnemy(Actor::Type::Vehicle1, -100.f, 505.f);
	addEnemy(Actor::Type::Vehicle1, -350.f, 505.f);
	addEnemy(Actor::Type::Vehicle1, -600.f, 505.f);
	
	
	addEnemy(Actor::Type::Vehicle2, -100.f, 460.f);
	addEnemy(Actor::Type::Vehicle2, -350.f, 460.f);
	addEnemy(Actor::Type::Vehicle2, -600.f, 460.f);
	
	
	
	addEnemy(Actor::Type::Vehicle4, 800.f, 445.f);
	addEnemy(Actor::Type::Vehicle4, 1200.f, 445.f);
	addEnemy(Actor::Type::Vehicle4, 1500.f, 445.f);
	
	
	
	addEnemy(Actor::Type::Vehicle3, -100.f, 370.f);
	addEnemy(Actor::Type::Vehicle3, -350.f, 370.f);
	addEnemy(Actor::Type::Vehicle3, -600.f, 370.f);
	
	
	
	addEnemy(Actor::Type::Vehicle5, 800.f, 355.f);
	addEnemy(Actor::Type::Vehicle5, 1200.f, 355.f);
	addEnemy(Actor::Type::Vehicle5, 1500.f, 355.f);


	addEnemy(Actor::Type::Log1, -100.f, 207.f);
	addEnemy(Actor::Type::Log1, -350.f, 207.f);
	addEnemy(Actor::Type::Log1, -600.f, 207.f);


	addEnemy(Actor::Type::Log2, -100.f, 75.f);
	addEnemy(Actor::Type::Log2, -350.f, 75.f);
	addEnemy(Actor::Type::Crocodile, -500.f, 60.f);
	addEnemy(Actor::Type::Log2, -700.f, 75.f);


	addEnemy(Actor::Type::Log3, -100.f, 160.f);
	addEnemy(Actor::Type::Log3, -350.f, 160.f);
	addEnemy(Actor::Type::Log3, -600.f, 160.f);


	
	addEnemy(Actor::Type::Turtle1, 740.f, 130.f);
	addEnemy(Actor::Type::Turtle1, 900.f, 130.f);
	addEnemy(Actor::Type::Turtle1, 1100.f, 130.f);
	addEnemy(Actor::Type::Turtle1, 1300.f, 130.f);


	addEnemy(Actor::Type::Turtle2, 740.f, 260.f);
	addEnemy(Actor::Type::Turtle2, 1000.f, 260.f);
	addEnemy(Actor::Type::Turtle2, 1200.f, 260.f);

}

void World::addEnemy(Actor::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, relX, relY);
	enemySpawnPoints.push_back(spawn);

}

void World::spawnEnemies()
{
	while (!enemySpawnPoints.empty())
	{
		auto& spawn = enemySpawnPoints.back();

		std::unique_ptr<Actor> enemy(new Actor(spawn.type, textures, fonts));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->getCategory() == Category::Log || enemy->getCategory() == Category::Turtle ?
			sceneLayers[LowerAir]->attachChild(std::move(enemy)) :
			sceneLayers[UpperAir]->attachChild(std::move(enemy));

		// Enemy is spawned, remove from the list to spawn
		enemySpawnPoints.pop_back();
	}
}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = playerActor->getVelocity();
	// If moving diagonally, normalize the velocity
	if (velocity.x != 0.f && velocity.y != 0.f)
		playerActor->setVelocity(velocity / std::sqrt(2.f));

}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;
	const float hudDistance = 70.f;
	sf::Vector2f position = playerActor->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - (borderDistance + hudDistance));
	playerActor->setPosition(position);
}

void World::adaptNPCPosition()
{
	Command adaptPosition;
	adaptPosition.category = Category::Crocodile;
	adaptPosition.action = derivedAction<Actor>([this](Actor& enemy, sf::Time)
		{
			sf::FloatRect viewBounds = getViewBounds();
			const float borderDistance = 40.f;

			sf::Vector2f position = enemy.getPosition();
			position.x = std::max(position.x, viewBounds.left + borderDistance);
			position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
			position.y = std::max(position.y, viewBounds.top + borderDistance);
			position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
			enemy.setPosition(position);
		});
	commandQueue.push(adaptPosition);
}

void World::updateSounds()
{
	sounds.setListenerPosition(playerActor->getWorldPosition());
	sounds.removeStoppedSounds();
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(worldView.getCenter() - worldView.getSize() / 2.f, worldView.getSize());
}


bool World::matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}

void World::setPlayerHudScoreText(size_t score)
{
	playerScoreHud.setString("Score: " + std::to_string(score));

}

void World::decreaseTimer()
{
	if (timebar.getSize().x == 100) {
		sounds.play(EffectID::Time);
	}
	if(timebar.getSize().x > 0)
		timebar.setSize(sf::Vector2f(timebar.getSize().x -0.3f ,timebar.getSize().y));
}

void World::resetTimer()
{
	timebar.setSize(sf::Vector2f(300.f, 25.f));
}

void World::removeLife()
{
	if (!playerLives.empty()) {
		playerLives.pop_back();
		resetTimer();
	}
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	sceneGraph.checkSceneCollision(sceneGraph, collisionPairs);

	for (SceneNode::Pair pair : collisionPairs)
	{
		if (matchesCategories(pair, Category::Frog, Category::Vehicle))
		{
			auto& frog = static_cast<Actor&>(*pair.first);
			auto& vehicle = static_cast<Actor&>(*pair.second);

			frog.damage(100);
			frog.destroy();
			sounds.play(EffectID::Squash);
			removeLife();

		}
		if (matchesCategories(pair, Category::Frog, Category::Log)) {
			auto& frog = static_cast<Actor&>(*pair.first);
			auto& log = static_cast<Actor&>(*pair.second);
			frog.move(log.getVelocity().x, 0.f);
			std::cout << log.getVelocity().x << std::endl;
			break;

		}
		
		if (matchesCategories(pair, Category::Frog, Category::Turtle)) {
			auto& frog = static_cast<Actor&>(*pair.first);
			auto& turtle = static_cast<Actor&>(*pair.second);
			if(turtle.getState() == Actor::State::Idle)
				frog.move(turtle.getVelocity().x, 0.f);

			if (turtle.getState() == Actor::State::Motion) {
				frog.damage(100);
				frog.destroy();
				sounds.play(EffectID::Plunk);
				removeLife();
			}
			break;

		}

		if (matchesCategories(pair, Category::Frog, Category::Bat)) {
			auto& frog = static_cast<Actor&>(*pair.first);
			auto& Bat = static_cast<Actor&>(*pair.second);
			if (Bat.isBatTouched()) {
				frog.damage(100);
				frog.destroy();
				removeLife();
			}
			if (!Bat.isBatTouched()) {
				frog.setFrogScore(30);
				batsVisited += 1;
				sounds.play(EffectID::Success);
				frog.destroy();
				resetTimer();
			}
			Bat.setIsBatTouched(true);
			
			break;
		
		}

		if (matchesCategories(pair, Category::Frog, Category::Lake)) {
			auto& frog = static_cast<Actor&>(*pair.first);
			auto& lake = static_cast<Actor&>(*pair.second);
			frog.damage(100);
			frog.destroy();
			sounds.play(EffectID::Plunk);
			removeLife();
		}

		if (matchesCategories(pair, Category::Log, Category::Crocodile)) {
			auto& log = static_cast<Actor&>(*pair.first);
			auto& crocodile = static_cast<Actor&>(*pair.second);

			crocodile.setPosition(crocodile.getPosition().x - log.getPosition().x,crocodile.getPosition().y);
		}
	}
}




