#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define MUNCHIECOUNT 50
#define GHOSTCOUNT 1

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

// Structure Definition
struct Player
{
	bool dead;
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	int currentFrameTime;
	int direction;
	int frame;
	float speedMultiplier;
};

struct Enemy
{
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Texture2D* invTexture;
	Vector2* position;
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

//class Definition
class Pacman : public Game
{
private:
	// Data to represent Pacman
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;
	int _pacmanDirection;
	int _pacmanFrame;
	int _pacmanCurrentFrameTime;
	float _pacmanSpeedMultiplier;
	const int _cPacmanFrameTime;

    Player* _pacman;
	Enemy* _munchies[MUNCHIECOUNT];
	Enemy* _cherry;
	MovingEnemy* _ghosts[GHOSTCOUNT];
	SoundEffect* _pop;

	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;
	const int _cMunchieFrameTime;
	int _munchieFrame;
	int _munchieCurrentFrameTime;

	// Data to represent cherry
	Rect* _cherryRect;
	Texture2D* _cherryTex;
	Texture2D* _cherryInvertedTexture;
	const int _cherryFrameTime;
	int _cherryFrame;
	int _cherryCurrentFrameTime;
	Vector2* _cherryPosition;

	// Position for String
	Vector2* _stringPosition;

	//Constant data for Game Variables
	const float _cPacmanSpeed;

	// Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;\
	bool _paused;
	bool _pKeyDown = false;
	bool _started;
	bool _spaceKeyDown = false;

	//Input method
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState*mouseState, Input::Keys reset);

	//Check method
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void CheckGhostCollision();

	//update methods
	void updatePacman(int elapsedTime);
	void updateMunchie(int elapsedTime);
	void updateMunchie(Enemy*, int elapsedTime);
	void updateCherry(int elapsedTime);
	void UpdateGhost(MovingEnemy*, int elapsedTime);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	bool CheckCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
};