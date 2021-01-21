#pragma once
/*
	Team: std::pair(Ali-Ibrahim)
	Date: 2020-12-07
	Purpose: Create a frogger replica using GEX Engine
*/
namespace Category
{
	enum Type
	{
		None = 0,				
		SceneAirLayer = 1 << 0,	
		SoundEffect = 1 << 1,
		Frog = 1 << 2,
		Vehicle = 1 <<3,
		Log = 1 << 4,
		Turtle= 1 << 5,
		Crocodile = 1 << 6,
		Bat = 1 << 7,
		Lake = 1 << 8,

		Actor = Frog | Vehicle | Log | Turtle | Crocodile | Bat | Lake,
	};
}