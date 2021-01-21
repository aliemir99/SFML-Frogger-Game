#pragma once
#include "ResourceHolder.h"
#include "ResourceIdentifier.h"
#include "SceneNode.h"
#include "SpriteNode.h"
#include "Actor.h"
#include "CommandQueue.h"
#include "Command.h"
#include "SoundPlayer.h"
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <array>
/*
	Team: std::pair(Ali-Ibrahim)
	Date: 2020-12-07
	Purpose: Create a frogger replica using GEX Engine
*/

// Forward declaration
namespace sf
{
	class RenderWindow;
	class RenderTarget;
}
class World : private sf::NonCopyable
{
public:
	explicit							World(sf::RenderTarget& outputTarget,FontHolder_t& fonts, SoundPlayer& sounds);
	void								update(sf::Time dt);
	void								draw();

	CommandQueue&						getCommands();

	bool								hasAlivePlayer() const;
	bool								hasPlayerReachedEnd() const;

private:
	void								loadTextures();
	void								buildScene();

	void								addEnemies();
	void								addEnemy(Actor::Type type, float relX, float relY);

	void								spawnEnemies();

	void								adaptPlayerVelocity();
	void								adaptPlayerPosition();
	void								adaptNPCPosition();

	void								updateSounds();

	sf::FloatRect						getViewBounds() const;

	void								handleCollisions();
	bool								matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

	void								setPlayerHudScoreText(size_t score);
	void                                decreaseTimer();
	void								resetTimer();
	void								removeLife();
private:
	enum Layer
	{
		Background,
		LowerAir,
		UpperAir,
		LayerCount
	};

	struct SpawnPoint {
		SpawnPoint(Actor::Type type, float x, float y)
			:type(type), x(x), y(y) {}
		Actor::Type type;
		float x, y;
	};

private:
	sf::RenderTarget&					target;
	sf::RenderTexture					sceneTexture;
	sf::View							worldView;
	TextureHolder_t						textures;
	const FontHolder_t&					fonts;
	SoundPlayer&						sounds;

	SceneNode							sceneGraph;
	std::array<SceneNode*, LayerCount>	sceneLayers;
	CommandQueue						commandQueue;

	sf::FloatRect						worldBounds;
	sf::Vector2f						spawnPosition;

	Actor*								playerActor;
	std::vector<SpawnPoint>				enemySpawnPoints;
	std::vector<Actor*>					activeEnemies;
	std::vector<sf::Sprite>				playerLives;
	size_t								batsVisited;

	sf::Text							playerScoreHud;
	sf::Text							timerText;
	sf::RectangleShape					timebar;
};

