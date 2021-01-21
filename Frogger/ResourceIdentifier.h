#pragma once
/*
	Team: std::pair(Ali-Ibrahim)
	Date: 2020-12-07
	Purpose: Create a frogger replica using GEX Engine
*/
namespace sf
{
	class Texture;
	class Font;
	class Shader;
	class SoundBuffer;
}

enum class EffectID
{
	Hop,
	Plunk,
	Squash,
	Time,
	Success,

};

enum class MusicID
{
	MenuTheme,
	MissionTheme,
};

enum class TextureID
{

	TitleScreen,
	Particle,

	FrogMap,
	FrogArtifacts,
	Frog,
	Vehicle1,
	Vehicle2,
	Vehicle3,
	Vehicle4,
	Vehicle5,
	Turtle1,
	Turtle2,
	Lake,
	Log1,
	Log2,
	Log3,
	Crocodile,
	Bat,
	Health,


};

enum class FontID
{
	Main
};

enum class ShaderID
{
	BrightnessPass,
	DownSamplePass,
	GaussianBlurPass,
	AddPass,
};


template <typename Resource, typename Identifier>
class ResourceHolder;

using TextureHolder_t = ResourceHolder <sf::Texture, TextureID>;
using FontHolder_t = ResourceHolder <sf::Font, FontID>;
using ShaderHolder_t = ResourceHolder<sf::Shader, ShaderID>;
using SoundBufferHolder_t = ResourceHolder<sf::SoundBuffer, EffectID>;