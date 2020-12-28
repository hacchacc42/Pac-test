#include "Pacman.h"

#include <sstream>

#include <time.h>

#include <iostream>

//Constructor Method
Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500), _cherryFrameTime(500)
{
	//Local variable
	int i;

	for (i = 0;i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->frame = rand() % 1;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->currentFrameTime = rand() % 500 + 50;
	}

	srand(static_cast <unsigned> (time(0)));

	//Initialise member variables
	//pacman
	_pacman = new Player();
	_pacman->dead = false;
	_frameCount = 0;
	_paused = false;
	_pacmanDirection = 0;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	_pacman->speedMultiplier = 1.0f;

	//munchies
	_munchieCurrentFrameTime = 0;
	_started = true; //temp

	//Intialise ghost chararcter
	_ghosts[0] = new MovingEnemy();
	_ghosts[0]->direction = 0;
	_ghosts[0]->speed = 0.2f;

	//Initialise Sound Effects
	_pop = new SoundEffect();

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	if (!Audio::IsInitialised())
	{
		cout << "Audio is not intialised" << endl;
	}

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

//Destructor method
Pacman::~Pacman()
{
	//Clean up pointers
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
	delete _pacman->position;
	delete _pacman;
	delete _ghosts;
	
	//Clean up the Texture
	delete _munchies[0]->texture;
	delete _ghosts[0]->texture;

	int nCount = 0;
	for (nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->sourceRect;
		delete _munchies[nCount];
	}

	//Clean up sound effect
	delete _pop;

	//Order of operation is important, array deleted last
	delete[] _munchies;

	//remaining clean up...
}

void Pacman::LoadContent()
{
	// Load sound effects
	_pop->Load("Sound/pop.wav");
	if (!_pop->IsLoaded())
	{
		cout << "_pop member sound effect has not loaded" << endl;
	}

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	//Intialise ghost character
	_ghosts[0]->texture = new Texture2D();
	_ghosts[0]->texture->Load("Textures/GhostBlue.png", false);
	_ghosts[0]->position = new Vector2(355.0f, 355.0f);
	_ghosts[0]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);

	// Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Texture/Munchie.png", false);

	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = _munchieBlueTexture;
		_munchies[i]->invTexture = _munchieInvertedTexture;
		_munchies[i]->sourceRect = new Rect(0, 0, 12, 12);
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth() + 1), (rand() % Graphics::GetViewportHeight() + 1));
	}

	//load cherry
	_cherryInvertedTexture = new Texture2D();
	_cherryInvertedTexture->Load("Textures/CherryInverted.png", false);
	_cherryTex = new Texture2D();
	_cherryTex->Load("Textures/Cherry.png", false);
	//_cherryPosition = new Vector2(300, 450.0f);
	//_cherryRect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry = new Enemy();
	_cherry->position = new Vector2(300, 450.0f);
	_cherry->texture = _cherryTex;
	_cherry->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry->invTexture = _cherryInvertedTexture;

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Parameter
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::updateMunchie(int elapsedTime)
{
	_munchieCurrentFrameTime += elapsedTime;

	if (_munchieCurrentFrameTime > _cMunchieFrameTime)
	{

		_munchieCurrentFrameTime = 0;
	}
}

void Pacman::updateCherry(int elapsedTime)
{

}

void Pacman::updateMunchie(Enemy* pMunchie, int elapsedTime) // called 50 times per update
{
	pMunchie->currentFrameTime += elapsedTime;

	if (pMunchie->currentFrameTime > _cMunchieFrameTime)
	{
		pMunchie->frame++;

		if (pMunchie->frame >= 2)
			pMunchie->frame = 0;

		pMunchie->currentFrameTime = 0;
	}
}

void Pacman::updatePacman(int elapsedTime)
{
	_pacmanCurrentFrameTime += elapsedTime;

	if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
	{
		_pacmanFrame++;
		_frameCount++;

		if (_frameCount >= 2)
		{
			_frameCount = 0;
		}

		if (_pacmanFrame >= 2)
			_pacmanFrame = 0;

		_pacmanCurrentFrameTime = 0;
	}

	_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
	_pacmanSourceRect->X = _pacmanSourceRect->Height * _pacmanFrame;
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pausekey)
{
	//Checks if P key is Pressed
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (state->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;
}

void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{
	if (ghost->direction == 0) //Moves Right
	{
		ghost->position->X += ghost->speed * elapsedTime;
	}
	else if (ghost->direction == 1) //Moves Left
	{
		ghost->position->X -= ghost->speed * elapsedTime;
	}
	if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) //Hits Right Edge
	{
		ghost->direction = 1; //change direction
	}
	else if (ghost->position->X <= 0) //Hits Left edge
	{
		ghost->direction = 0; //change direction
	}
}

void Pacman::CheckViewportCollision()
{
	int WinWidth = Graphics::GetViewportWidth();
	int PacWidth = 32;
	int WinHeight = Graphics::GetViewportHeight();
	int PacHeight = 32;

	// Checks if Pacman is trying to disappear
	if (_pacmanPosition->X + _pacmanSourceRect->Width > WinWidth) //1024 is game width
	{
		// Pacman hit right wall - reset his postion
		_pacmanPosition->X = PacWidth - _pacmanSourceRect->Width;
	}
	// Checks if Pacman is trying to disappear
	if (_pacmanPosition->X + _pacmanSourceRect->Width < PacWidth) //32 is game width
	{
		// Pacman hit Left wall - reset his postion
		_pacmanPosition->X = WinWidth - _pacmanSourceRect->Width;
	}
	// Checks if Pacman is trying to disappear
	if (_pacmanPosition->Y + _pacmanSourceRect->Width < PacHeight) //32 is game width
	{
		// Pacman hit Upper wall - reset his postion
		_pacmanPosition->Y = WinHeight - _pacmanSourceRect->Width;
	}
	// Checks if Pacman is trying to disappear
	if (_pacmanPosition->Y + _pacmanSourceRect->Width > WinHeight) //1024 is game width
	{
		// Pacman hit Lower wall - reset his postion
		_pacmanPosition->Y = PacHeight - _pacmanSourceRect->Width;
	}
}


void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	//Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (!_paused)
	{
		Input(elapsedTime, keyboardState, mouseState, Input::Keys::R);
		updatePacman(elapsedTime);
		UpdateGhost(_ghosts[0], elapsedTime);
	}

	if (!_started)
	{
		//check for start
		if (keyboardState->IsKeyDown(Input::Keys::RETURN))
		{
			_started = true;
		}
	}
	else
	{
		CheckPaused(keyboardState, Input::Keys::P);
		if (!_paused)
		{
			Input(elapsedTime, keyboardState, mouseState, Input::Keys::R);
			updatePacman(elapsedTime);
			UpdateGhost(_ghosts[0], elapsedTime);
			CheckViewportCollision();
		}
	}
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		updateMunchie(_munchies[i], elapsedTime);
	}
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		if (CheckCollision(_ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height, _pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height))
		{
			cout << "POG";
		}
	}
}

void Pacman::CheckGhostCollision()
{
	//local variables
	int i = 0;
	int bottom1 = _pacmanPosition->Y + _pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacmanPosition->X;
	int left2 = 0;
	int right1 = _pacmanPosition->X + _pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacmanPosition->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		// Populate variable with Ghost data
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
}

/*void Pacman::CheckMunchiesCollision()
{
	//local variables
	int i = 0;
	int bottom1 = _pacmanPosition->Y + _pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacmanPosition->X;
	int left2 = 0;
	int right1 = _pacmanPosition->X + _pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacmanPosition->Y;
	int top2 = 0;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		// Populate variable with Ghost data
		bottom2 = _munchies[i]->position->Y + _munchies[i]->sourceRect->Height;
		left2 = _munxhies[i]->position->X;
		right2 = _munchies[i]->position->X + _munchies[i]->sourceRect->Width;
		top2 = _munchies[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
}*/

bool Pacman::CheckCollision(int x1, int y1, int width1, int height1,
	int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;
	if (bottom1 < top2)
	{
		return false;
	}
	if (top1 > bottom2)
	{
		return false;
	}
	if (right1 < left2)
	{
		return false;
	}
	if (left1 > right2)
	{
		return false;
	}

	return true;
}

void Pacman::Draw(int elapsedTime)
{
	// Draws pacman
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect);
	}

	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	//draw ghosts
	SpriteBatch::Draw(_ghosts[0]->texture, _ghosts[0]->position, _ghosts[0]->sourceRect);

	//for loop over munchie array
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		
		if (_frameCount == 0)
		{
			SpriteBatch::Draw(_munchies[i]->invTexture, _munchies[i]->position, _munchies[i]->sourceRect);
		}
		else
		{
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect);
		}
	}


	if (_frameCount ==0)
	{
		// Draws Red Munchie
		//SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		SpriteBatch::Draw(_cherry->invTexture, _cherry->position, _cherry->sourceRect);
	}
	else
	{
		// Draw Blue Munchie
		//SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->sourceRect);
	}

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "Paused!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState, Input::Keys reset)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;

	// Handle Mouse Input - Reposition Cherry
	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		_cherry->position->X = mouseState->X;
		_cherry->position->Y = mouseState->Y;
	}
	//Method body to be written by the developer
	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D))
	{
		//plus sign for negative numbers
		_pacmanPosition->X += pacmanSpeed; //Moves Pacman
		//across +X axis (right)
		_pacmanDirection = 0;
	}
	// Checks if A key is pressed 
	if (state->IsKeyDown(Input::Keys::A))
	{
		//minus sign for negative numbers
		_pacmanPosition->X -= pacmanSpeed; //Moves Pacman
		//across +X axis (right)
		_pacmanDirection = 2;
	}
	// Checks if W key is pressed 
	if (state->IsKeyDown(Input::Keys::W))
	{
		//plus sign for negative numbers
		_pacmanPosition->Y -= pacmanSpeed; //Moves Pacman
		//across +X axis (right)
		_pacmanDirection = 3;
	}
	// Checks if S key is pressed 
	if (state->IsKeyDown(Input::Keys::S))
	{
		//minus sign for negative numbers
		_pacmanPosition->Y += pacmanSpeed; //Moves Pacman
		//across +X axis (right)
		_pacmanDirection = 1;
	}
	// Speed Multiplier
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		// Apply multiplier
		_pacman->speedMultiplier = 2.0f;
	}
	else
	{
		// Reset multiplier
		_pacman->speedMultiplier = 1.0f;
	}
}
