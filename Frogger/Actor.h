#pragma once
#include "Entity.h"
#include "ResourceHolder.h"
#include "ResourceIdentifier.h"
#include "Animation2.h"
#include "TextNode.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML\System\Clock.hpp>
/*
    Team: std::pair(Ali-Ibrahim)
    Date: 2020-12-07
    Purpose: Create a frogger replica using GEX Engine
*/
class Actor : public Entity
{
public:
    enum class Type {
         Frog, Vehicle1,
         Vehicle2, Vehicle3,
         Vehicle4, Vehicle5,
         Log1,Log2,
         Log3,Turtle1,
         Turtle2,Lake,
         Crocodile,Bat
    };
    enum class State {
         Dead, Idle, Jump, Motion, count
    };
    enum class Direction
    {
        Left, Right, Up, Down
    };
public:
                                        Actor(Type type, const TextureHolder_t& textures, const FontHolder_t& fonts);
                                        ~Actor() = default;
    unsigned int                        getCategory() const override;
    sf::FloatRect                       getBoundingRect() const override;
    float                               getMaxSpeed() const;
    bool                                isMarkedForRemoval() const override;
    void                                setState(State state);
    Actor::State                        getState() const;
    void                                setDirection(Actor::Direction direction);
    bool                                isBatTouched() const;
    bool                                setIsBatTouched(bool b);

    void                                setFrogScore(int score);
    size_t                              getFrogScore() const;
private:
    void                                updateStates();
    void                                updateCurrent(sf::Time dt, CommandQueue& commands) override;
    void                                drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
    void                                updateMovementPattern(sf::Time dt);

private:
    Type                                type_;
    State                               state_;
    mutable sf::Sprite                  sprite_;
    sf::Sprite				            death;
    std::map<std::pair<Actor::State, Actor::Direction>, Animation2>         animations_;
    Direction                           direction_;
    TextNode*                           healthDisplay_;
    float                               travelDistance_;
    std::size_t                         directionIndex_;

    float                               worldBounderiesWidth;

    bool                                batTouched;

    size_t                              frogScore;
    float                               countdown;
    sf::Time                            elapsedTime;
    sf::Clock                           clock;


};