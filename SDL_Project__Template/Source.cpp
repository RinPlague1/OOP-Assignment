
#include "Source.h"

int main(int argc, char* argv[])
{
	SDL_Window* wnd{ nullptr };

	if (SDL_Init(SDL_INIT_EVERYTHING) == !0)
	{
		std::cout << "SDL INIT ERROR";
		return -1;
	}
	wnd = SDL_CreateWindow("Super WoodChopMan - LumberTim",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1024, 576, 0);

	if (wnd == nullptr)
	{
		std::cout << "SDL window error" << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_Renderer* rend{ nullptr };
	rend = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (rend == nullptr)
	{
		std::cout << "SDL renderer error" << std::endl;
		SDL_Quit();
		SDL_DestroyWindow(wnd);
		return -1;
	}

	if (TTF_Init() < 0)
	{
		std::cout << "Error with TTF initialising" << TTF_GetError() << std::endl;
	}

	int music = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	if (music != 0)
	{
		std::cout << "error initialising mixer" << Mix_GetError() << std::endl;
	}
	std::cout << "init complete" << std::endl;
	///////////////////////SDL_SETUP_COMPLETE///////////////////////

	srand(time(NULL));

	SDL_Texture* playButton{ nullptr };
	SDL_Texture* characterButton{ nullptr };
	SDL_Texture* quitButton{ nullptr };
	SDL_Texture* backButton{ nullptr };			//makes the textures global
	SDL_Texture* helpButton{ nullptr };

	std::vector<SDL_Texture*> buttonTextures;

	buttonTextures.push_back(playButton);
	buttonTextures.push_back(characterButton);
	buttonTextures.push_back(quitButton);		//pushes the textures to the button vector
	buttonTextures.push_back(backButton);
	buttonTextures.push_back(helpButton);

	std::vector<SDL_Rect> buttonStateRects;

	std::vector<SDL_Texture*> textTexture;
	std::vector<SDL_Rect> textRects;



	SDL_Texture* menuBackgroundTexture{ nullptr };
	SDL_Texture* backgroundTexture{ nullptr };
	SDL_Texture* logTexture{ nullptr };
	SDL_Texture* RightlogTexture{ nullptr };		//creates the texture pointers
	SDL_Texture* LeftlogTexture{ nullptr };
	SDL_Texture* PlayerSpriteSheet{ nullptr };
	SDL_Texture* highScoreTexture{ nullptr };

	SDL_Texture* digitTexture{ nullptr };
	std::vector <SDL_Texture*> digitTextureVector;
	std::vector<SDL_Rect> digitDestRects;

	SDL_Rect characterChangeLocation = {	//the loaction for the change character screen
		492, 330,
		40, 71
	};
	
	TTF_Font* font{ nullptr };
	font = TTF_OpenFont("Fonts/PixelifySans-Regular.ttf", 32);
	std::cout << "font loaded" << std::endl;
	
	SDL_Color scoreColour = { 255, 255, 255 };

	SDL_Rect highScoreDestRect;

	
	std::vector<std::vector<SDL_Rect>> imgRectVector;	//tree sheet src locations
	std::vector<Tree> treePieces;
	std::vector<SDL_Rect> treePieceDestRect;

	std::vector<std::vector<SDL_Rect>> playerRectLocations;
	std::vector<Button> menuButtons;

	Mix_Music* bgMusic = Mix_LoadMUS("Sounds/soundtrack.mp3");
	//https://pixabay.com/sound-effects/8bit-music-for-game-68698/

	
	Mix_Chunk* chopNoise{ nullptr };

	//https://pixabay.com/sound-effects/chopping-wood-96709/
	chopNoise = Mix_LoadWAV("Sounds/woodChop.mp3");
	
	int startTime;
	int endTime;
	int timeTaken;

	bool setup = false;
	bool setupText = false;

	int logsChopped = 0;
	int highScore = 0;

	std::cout << "all loaded";
	
	
	initialiseButtons(rend, buttonTextures, buttonStateRects, menuButtons);	//loads the buttons for the menus
	std::cout << "\nbuttons initialised";

	initialise(rend, menuBackgroundTexture, backgroundTexture, logTexture, RightlogTexture, LeftlogTexture, imgRectVector, PlayerSpriteSheet, playerRectLocations);	//renders the scene for the game
	std::cout << "\ninit run";

	initialiseTreePieces(treePieces, treePieceDestRect, logsChopped); //generates starting tree array
	std::cout << "\ninit tree run";

	initialiseText(font, scoreColour, logsChopped, digitTextureVector, rend, digitDestRects);	//generates the digit textures for the score count
	std::cout << "\ninit text run";

	player playerCharacter(308, 335, 45, 71, p_idle, p_lumberTim);
	
	bool gameover = false;
	
	gameState gameLoopState = menu;

	int musicPlay = Mix_PlayMusic(bgMusic, -1);	//starts bg music
	int musicVol = Mix_VolumeMusic(64);

	
	while (gameover == false)	//game loop
	{


		switch (gameLoopState)	// enum for the states
		{
		case menu:

			SDL_RenderClear(rend);

			SDL_RenderCopy(rend, menuBackgroundTexture, &imgRectVector.at(0).at(0), &imgRectVector.at(0).at(1));

			SDL_Event mousePosition;
			while (SDL_PollEvent(&mousePosition) == true)
			{
				switch (mousePosition.type)
				{
				case SDL_QUIT:
					gameover = true;
					break;

				}

				for (int i = 0; i < 5; i++)
				{
					if (i == 3)
					{
						i = 4;
					}

					menuButtons.at(i).handleMouseEvent(&mousePosition);	// lets the menu buttons handle their own mouse events
				}
			}

			rerenderMenuButtons(rend, buttonTextures, buttonStateRects, menuButtons);

			if (menuButtons.at(0).getCurrentState() == button_mouse_down)	//gives the buttons state control of the game
			{
				gameLoopState = game;
				setup = false;
				menuButtons.at(0).setCurrentState(button_idle);
			}

			if (menuButtons.at(1).getCurrentState() == button_mouse_down)
			{
				gameLoopState = character_select;
				menuButtons.at(1).setCurrentState(button_idle);
			}

			if (menuButtons.at(2).getCurrentState() == button_mouse_down)
			{
				gameLoopState = quit;
				menuButtons.at(2).setCurrentState(button_idle);
			}

			if (menuButtons.at(4).getCurrentState() == button_mouse_down)
			{
				gameLoopState = instructions;
				menuButtons.at(4).setCurrentState(button_idle);
			}

			SDL_RenderPresent(rend);
			break;



		case instructions:
			SDL_RenderClear(rend);

			SDL_RenderCopy(rend, menuBackgroundTexture, &imgRectVector.at(0).at(0), &imgRectVector.at(0).at(1));

			if (setupText == false)
			{
				initialiseHelp(rend, textTexture, textRects, font, scoreColour);	// only runs one time to avoid constand reuse of create texture
				setupText = true;
			}

			SDL_Event mouseInstructionPosition;
			while (SDL_PollEvent(&mouseInstructionPosition) == true)
			{
				switch (mouseInstructionPosition.type)
				{
				case SDL_QUIT:
					gameover = true;
					break;

				}

				for (int i = 3; i < 4; i++)
				{
					menuButtons.at(i).handleMouseEvent(&mouseInstructionPosition);	//same handle event as the main menu buttons 
				}
			}

			showHelp(rend, font, scoreColour, highScoreTexture, textTexture, textRects);	//shows the help text

			drawBackButton(rend, buttonTextures, buttonStateRects, menuButtons);

			if (menuButtons.at(3).getCurrentState() == button_mouse_down)	//same state control as before as well
			{
				gameLoopState = menu;
				menuButtons.at(3).setCurrentState(button_idle);
			}


			SDL_RenderPresent(rend);

			break;

		case game:


			startTime = SDL_GetTicks();

			SDL_RenderClear(rend);

			SDL_RenderCopy(rend, backgroundTexture, &imgRectVector.at(0).at(0), &imgRectVector.at(0).at(1));
			updatePlayer(playerCharacter, playerRectLocations, rend, PlayerSpriteSheet, gameover);

			if (setup == false)
			{
				if (PlayerSpriteSheet == NULL)	// checks if the player has selected a character in the character select and if not defaults to lumber tim
				{
					SDL_Surface* playerSpriteSheetSurface_Tim = SDL_LoadBMP("sprites/LumberTim.bmp");

					PlayerSpriteSheet = SDL_CreateTextureFromSurface(rend, playerSpriteSheetSurface_Tim);
					SDL_FreeSurface;
				}


				logsChopped = 0; // resets score
				playerCharacter.setplayerState(p_left);

				SDL_RenderCopy(rend, logTexture, &imgRectVector.at(1).at(0), &treePieceDestRect.at(0));	//renders bottom trees so theyre blank for the first 3
				SDL_RenderCopy(rend, logTexture, &imgRectVector.at(1).at(0), &treePieceDestRect.at(1));
				SDL_RenderCopy(rend, logTexture, &imgRectVector.at(1).at(0), &treePieceDestRect.at(2));

				SDL_RenderCopy(rend, PlayerSpriteSheet, &playerRectLocations.at(1).at(0), &playerRectLocations.at(1).at(1));

				for (int i = 2; i < 7; i++)	//renders the rest of the logs with the random generation
				{
					calculateLogSetupTexture(rend, logTexture, RightlogTexture, LeftlogTexture, imgRectVector, treePieces.at(i), treePieceDestRect, i, treePieces.at(i - 1));
				}
				setup = true;
			}


			SDL_Event Event;
			while (SDL_PollEvent(&Event) == true)
			{
				switch (Event.type)
				{
				case SDL_QUIT:
					gameover = true;
					break;

				case SDL_KEYDOWN:	//main logic of the game


					if (Event.key.keysym.sym == SDLK_LEFT)	// player click left
					{
						playerCharacter.setplayerState(p_left);

						int checkTrunk = treePieces.at(1).getTrunkStatus();	//checks for a trunk 
						if (checkTrunk == left)	
						{
							if (logsChopped > highScore)
							{
								highScore = logsChopped;
							}

							gameLoopState = game_over;
						}
						else if (checkTrunk == none || checkTrunk == right)
						{
							treePieces.erase(treePieces.begin());
							treePieceDestRect.erase(treePieceDestRect.begin());	//removes the first tree piece when its chopped

							generateNewTreePiece(treePieces, treePieceDestRect, logsChopped);

							int chopChannel = Mix_PlayChannel(-1, chopNoise, 0);	//plays chop sound
							int chopVol = Mix_Volume(-1, 32);

							logsChopped++;

							for (int i = 0; i < 8; i++)	// brings every log piece down one in the vector
							{

								treePieces.at(i).setVectorPos(treePieces.at(i).getVectorPos() - 1);

								treePieces.at(i).setY(treePieces.at(i).getY() + 80);

								treePieces.at(i).formatRect();
								treePieceDestRect.at(i) = treePieces.at(i).getDestRect();
							}

							updatePlayer(playerCharacter, playerRectLocations, rend, PlayerSpriteSheet, gameover);	//updates the players sprite to which side its on

						
							updateScore(rend, font, scoreColour, logsChopped, highScore, highScoreTexture, highScoreDestRect, digitTextureVector, gameLoopState);
						}
					}

					if (Event.key.keysym.sym == SDLK_RIGHT)	//player click right
					{
						playerCharacter.setplayerState(p_right);

						int checkTrunk = treePieces.at(1).getTrunkStatus();
						if (checkTrunk == right)
						{
							if (logsChopped > highScore)
							{
								highScore = logsChopped;
							}


							gameLoopState = game_over;
						}
						else if (checkTrunk == none || checkTrunk == left)
						{
							
							treePieces.erase(treePieces.begin());
							treePieceDestRect.erase(treePieceDestRect.begin());

							generateNewTreePiece(treePieces, treePieceDestRect, logsChopped);

							int chopChannel = Mix_PlayChannel(-1, chopNoise, 0);
							int chopVol = Mix_Volume(-1, 32);

							logsChopped++;

							for (int i = 0; i < 8; i++)
							{

								treePieces.at(i).setVectorPos(treePieces.at(i).getVectorPos() - 1);

								treePieces.at(i).setY(treePieces.at(i).getY() + 80);

								treePieces.at(i).formatRect();
								treePieceDestRect.at(i) = treePieces.at(i).getDestRect();
							}

							updatePlayer(playerCharacter, playerRectLocations, rend, PlayerSpriteSheet, gameover);	//updates the players sprite to which side its on

							
							updateScore(rend, font, scoreColour, logsChopped, highScore, highScoreTexture, highScoreDestRect, digitTextureVector, gameLoopState);
						}
					}
				default:
					break;
				}
			}

			
			for (int i = 0; i < 7; i++)	//rerenders the tree pieces
			{
				if (i == 0)
				{
					rerenderNewTreeImage(rend, logTexture, RightlogTexture, LeftlogTexture, imgRectVector, treePieces.at(i), treePieceDestRect, i, i, treePieces);
				}
				else
				{
					rerenderNewTreeImage(rend, logTexture, RightlogTexture, LeftlogTexture, imgRectVector, treePieces.at(i), treePieceDestRect, i, (i - 1), treePieces);
				}


			}

			

			//SDL_RenderCopy(rend, PlayerSpriteSheet, &playerRectLocations.at(1).at(0), &playerRectLocations.at(1).at(1));

			

			SDL_RenderPresent(rend);

			endTime = SDL_GetTicks();
			timeTaken = (endTime - startTime);

			if (timeTaken > 1)
			{
				SDL_Delay(32.3333f - timeTaken);
			}

			break;


		case character_select:
			SDL_RenderClear(rend);

			SDL_RenderCopy(rend, menuBackgroundTexture, &imgRectVector.at(0).at(0), &imgRectVector.at(0).at(1));
			
			SDL_RenderCopy(rend, PlayerSpriteSheet, &playerRectLocations.at(0).at(0), &characterChangeLocation);	//shows the current selected player

			SDL_Event mouseCharacterPosition;
			while (SDL_PollEvent(&mouseCharacterPosition) == true)
			{
				int characterChosen = playerCharacter.getCharacterChosen();

				switch (mouseCharacterPosition.type)
				{
				case SDL_QUIT:
					gameover = true;
					break;

				case SDL_KEYDOWN:
					if (mouseCharacterPosition.key.keysym.sym == SDLK_LEFT)	// goes throght the index of characters by -1 step
					{
						characterChosen -= 1;
						if (characterChosen < 0)
						{
							characterChosen = 1;
						}
						playerCharacter.setCharacterChosenNum(characterChosen);
					}

					if (mouseCharacterPosition.key.keysym.sym == SDLK_RIGHT)// goes throght the index of characters by +1 step
					{
						characterChosen += 1;
						if (characterChosen > 1)
						{
							characterChosen = 0;
						}
						playerCharacter.setCharacterChosenNum(characterChosen);
					}
					
				}
				changeCharacter(rend, playerCharacter, playerRectLocations, PlayerSpriteSheet);
				for (int i = 3; i < 4; i++)	// handles for back button
				{
					menuButtons.at(i).handleMouseEvent(&mouseCharacterPosition);
				}
			}

			drawBackButton(rend, buttonTextures, buttonStateRects, menuButtons);

			

			if (menuButtons.at(3).getCurrentState() == button_mouse_down)
			{
				gameLoopState = menu;
				menuButtons.at(3).setCurrentState(button_idle);
			}


			SDL_RenderPresent(rend);

			break;


		case game_over:
			SDL_RenderClear(rend);

			SDL_RenderCopy(rend, menuBackgroundTexture, &imgRectVector.at(0).at(0), &imgRectVector.at(0).at(1));

			SDL_Event mouseGameOverPosition;
			while (SDL_PollEvent(&mouseGameOverPosition) == true)
			{
				switch (mouseGameOverPosition.type)
				{
				case SDL_QUIT:
					gameover = true;
					break;

				}

				for (int i = 3; i < 4; i++)
				{
					menuButtons.at(i).handleMouseEvent(&mouseGameOverPosition);
				}
			}
			updateScore(rend, font, scoreColour, logsChopped ,highScore, highScoreTexture, highScoreDestRect, digitTextureVector, gameLoopState);	//displays score and highscore from that go
			drawBackButton(rend, buttonTextures, buttonStateRects, menuButtons);

			if (menuButtons.at(3).getCurrentState() == button_mouse_down)
			{
				gameLoopState = menu;
				menuButtons.at(3).setCurrentState(button_idle);
			}


			SDL_RenderPresent(rend);
			
			break;
		
		case quit:
			gameover = true;
			break;

		}

		
		

	}
	
	TTF_CloseFont(font);
	TTF_Quit();

	Mix_FreeChunk(chopNoise);
	Mix_FreeMusic(bgMusic);
	Mix_Quit();

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(wnd);
	SDL_Quit();

	return 0;
}



void initialiseButtons(SDL_Renderer* _rend, std::vector<SDL_Texture*>& _buttonTextures, std::vector<SDL_Rect>& _spriteStateRects, std::vector<Button> &_menuButtons)
{
	SDL_Surface* playButtonSurface = SDL_LoadBMP("sprites/Play-Button.bmp");
	_buttonTextures.at(0) = SDL_CreateTextureFromSurface(_rend, playButtonSurface);
	SDL_FreeSurface;

	SDL_Surface* characterButtonSurface = SDL_LoadBMP("sprites/Character-Select.bmp");
	_buttonTextures.at(1) = SDL_CreateTextureFromSurface(_rend, characterButtonSurface);
	SDL_FreeSurface;

	SDL_Surface* quitButtonSurface = SDL_LoadBMP("sprites/Quit-Button.bmp");
	_buttonTextures.at(2) = SDL_CreateTextureFromSurface(_rend, quitButtonSurface);
	SDL_FreeSurface;

	SDL_Surface* backButtonSurface = SDL_LoadBMP("sprites/Back-Button.bmp");
	_buttonTextures.at(3) = SDL_CreateTextureFromSurface(_rend, backButtonSurface);
	SDL_FreeSurface;

	SDL_Surface* helpButtonSurface = SDL_LoadBMP("sprites/Help-Button.bmp");
	_buttonTextures.at(4) = SDL_CreateTextureFromSurface(_rend, helpButtonSurface);
	SDL_FreeSurface;


	////////////////////////////// Locations in the spritesheet //////////////////////////////

	SDL_Rect spriteStateIdle = {				
		0, 0,
		80, 50
	};

	SDL_Rect spriteStateHover = {
		85, 0,
		80, 50
	};

	SDL_Rect spriteStateClick = {
		170, 0,
		80, 50
	};

	SDL_Rect helpSpriteStateIdle = {
		0, 0,
		25, 25
	};

	SDL_Rect helpSpriteStateHover = {
		30, 0,
		25, 25
	};

	SDL_Rect helpSpriteStateClick = {
		60, 0,
		25, 25
	};

	_spriteStateRects.push_back(spriteStateIdle);
	_spriteStateRects.push_back(spriteStateHover);
	_spriteStateRects.push_back(spriteStateClick);

	_spriteStateRects.push_back(helpSpriteStateIdle);
	_spriteStateRects.push_back(helpSpriteStateHover);
	_spriteStateRects.push_back(helpSpriteStateClick);

	Button playButton(472, 300, 80, 50, button_idle);
	_menuButtons.push_back(playButton);
	 
	Button characterButton(472, 360, 80, 50, button_idle);
	_menuButtons.push_back(characterButton);

	Button quitButton(472, 420, 80, 50, button_idle);
	_menuButtons.push_back(quitButton);

	Button backButton(472, 420, 80, 50, button_idle);
	_menuButtons.push_back(backButton);

	Button helpButton(562, 300, 25, 25, button_idle);
	_menuButtons.push_back(helpButton);
	//SDL_RenderCopy(_rend, button)
}

void initialiseText(TTF_Font* _font, SDL_Color _colour, int _logsChopped, std::vector<SDL_Texture*> &_fontTexture, SDL_Renderer* _rend, std::vector<SDL_Rect> &_textDestRect)
{
	//////////////////////////////pre rendered digit textures for the score//////////////////////////////

	SDL_Surface* surface_zero{ nullptr };
	surface_zero = TTF_RenderText_Solid(_font, "0", _colour);
	SDL_Texture* texture_zero = SDL_CreateTextureFromSurface(_rend, surface_zero);
	_fontTexture.push_back(texture_zero);

	SDL_FreeSurface(surface_zero);
	_textDestRect.push_back({ 980, 20, surface_zero->w, surface_zero->h });

	SDL_Surface* surface_one{ nullptr };
	surface_one = TTF_RenderText_Solid(_font, "1", _colour);
	SDL_Texture* texture_one = SDL_CreateTextureFromSurface(_rend, surface_one);
	_fontTexture.push_back(texture_one);

	SDL_FreeSurface(surface_one);
	_textDestRect.push_back({ 980, 20, surface_one->w, surface_one->h });

	SDL_Surface* surface_two{ nullptr };
	surface_two = TTF_RenderText_Solid(_font, "2", _colour);
	SDL_Texture* texture_two = SDL_CreateTextureFromSurface(_rend, surface_two);
	_fontTexture.push_back(texture_two);

	SDL_FreeSurface(surface_two);
	_textDestRect.push_back({ 980, 20, surface_two->w, surface_two->h });

	SDL_Surface* surface_three{ nullptr };
	surface_three = TTF_RenderText_Solid(_font, "3", _colour);
	SDL_Texture* texture_three = SDL_CreateTextureFromSurface(_rend, surface_three);
	_fontTexture.push_back(texture_three);

	SDL_FreeSurface(surface_three);
	_textDestRect.push_back({ 980, 20, surface_three->w, surface_three->h });

	SDL_Surface* surface_four{ nullptr };
	surface_four = TTF_RenderText_Solid(_font, "4", _colour);
	SDL_Texture* texture_four = SDL_CreateTextureFromSurface(_rend, surface_four);
	_fontTexture.push_back(texture_four);

	SDL_FreeSurface(surface_four);
	_textDestRect.push_back({ 980, 20, surface_four->w, surface_four->h });

	SDL_Surface* surface_five{ nullptr };
	surface_five = TTF_RenderText_Solid(_font, "5", _colour);
	SDL_Texture* texture_five = SDL_CreateTextureFromSurface(_rend, surface_five);
	_fontTexture.push_back(texture_five);

	SDL_FreeSurface(surface_five);
	_textDestRect.push_back({ 980, 20, surface_five->w, surface_five->h });

	SDL_Surface* surface_six{ nullptr };
	surface_six = TTF_RenderText_Solid(_font, "6", _colour);
	SDL_Texture* texture_six = SDL_CreateTextureFromSurface(_rend, surface_six);
	_fontTexture.push_back(texture_six);

	SDL_FreeSurface(surface_six);
	_textDestRect.push_back({ 980, 20, surface_six->w, surface_six->h });

	SDL_Surface* surface_seven{ nullptr };
	surface_seven = TTF_RenderText_Solid(_font, "7", _colour);
	SDL_Texture* texture_seven = SDL_CreateTextureFromSurface(_rend, surface_seven);
	_fontTexture.push_back(texture_seven);

	SDL_FreeSurface(surface_seven);
	_textDestRect.push_back({ 980, 20, surface_seven->w, surface_seven->h });

	SDL_Surface* surface_eight{ nullptr };
	surface_eight = TTF_RenderText_Solid(_font, "8", _colour);
	SDL_Texture* texture_eight = SDL_CreateTextureFromSurface(_rend, surface_eight);
	_fontTexture.push_back(texture_eight);

	SDL_FreeSurface(surface_eight);
	_textDestRect.push_back({ 980, 20, surface_eight->w, surface_eight->h });

	SDL_Surface* surface_nine{ nullptr };
	surface_nine = TTF_RenderText_Solid(_font, "9", _colour);
	SDL_Texture* texture_nine = SDL_CreateTextureFromSurface(_rend, surface_nine);
	_fontTexture.push_back(texture_nine);

	SDL_FreeSurface(surface_nine);
	_textDestRect.push_back({ 980, 20, surface_nine->w, surface_nine->h });



}

void initialise(SDL_Renderer* _rend, SDL_Texture* &menuBackgroundTexture, SDL_Texture* &backgroundTexture, SDL_Texture* &logTexture, SDL_Texture* &RightlogTexture, SDL_Texture* &LeftlogTexture, std::vector<std::vector<SDL_Rect>> &_rectVector, SDL_Texture* &playerSpriteSheet, std::vector<std::vector<SDL_Rect>> &_playerRectLocations)
{
	SDL_Surface* menuBackgroundImage = SDL_LoadBMP("sprites/Menu-Background.bmp");

	menuBackgroundTexture = SDL_CreateTextureFromSurface(_rend, menuBackgroundImage);
	SDL_FreeSurface;


	SDL_Surface* backgroundImage = SDL_LoadBMP("sprites/Background.bmp");

	backgroundTexture = SDL_CreateTextureFromSurface(_rend, backgroundImage);
	SDL_FreeSurface;


	SDL_Surface* logImage = SDL_LoadBMP("sprites/Log.bmp");

	logTexture = SDL_CreateTextureFromSurface(_rend, logImage);
	SDL_FreeSurface;


	SDL_Surface* RightlogImage = SDL_LoadBMP("sprites/Log_Right.bmp");

	RightlogTexture = SDL_CreateTextureFromSurface(_rend, RightlogImage);
	SDL_FreeSurface;


	SDL_Surface* LeftlogImage = SDL_LoadBMP("sprites/Log_Left.bmp");
	
	LeftlogTexture = SDL_CreateTextureFromSurface(_rend, LeftlogImage);
	SDL_FreeSurface;


	int bgWidth, bgHeight;
	int logWidth, logHeight;
	int lLogWidth, lLogHeight;
	int rLogWidth, rLogHeight;

	SDL_QueryTexture(backgroundTexture, 0, 0, &bgWidth, &bgHeight);
	SDL_QueryTexture(logTexture, 0, 0, &logWidth, &logHeight);
	SDL_QueryTexture(LeftlogTexture, 0, 0, &lLogWidth, &lLogHeight);
	SDL_QueryTexture(RightlogTexture, 0, 0, &rLogWidth, &rLogHeight);

	std::vector<SDL_Rect> bgRects;
	std::vector<SDL_Rect> logRects;
	std::vector<SDL_Rect> lLogRects;
	std::vector<SDL_Rect> rLogRects;


	SDL_Rect bgSrc = {
		0, 0,
		bgWidth, bgHeight
	};
	bgRects.push_back(bgSrc);

	SDL_Rect logSrc = {
		0, 0,
		logWidth, logHeight
	};
	logRects.push_back(logSrc);

	SDL_Rect lLogSrc = {
		0, 0,
		lLogWidth, lLogHeight
	};
	lLogRects.push_back(lLogSrc);

	SDL_Rect rLogSrc = {
		0, 0,
		rLogWidth, rLogHeight
	};
	rLogRects.push_back(rLogSrc);



	SDL_Rect bgDest = {
	0, 0,
	bgWidth, bgHeight
	};
	bgRects.push_back(bgDest);

	SDL_Rect logDest = {
		0, 0,
		logWidth, logHeight
	};
	logRects.push_back(logDest);

	SDL_Rect lLogDest = {
		0, 0,
		lLogWidth, lLogHeight
	};
	lLogRects.push_back(lLogDest);

	SDL_Rect rLogDest = {
		0, 0,
		rLogWidth, rLogHeight
	};
	rLogRects.push_back(rLogDest);

	_rectVector.push_back(bgRects);
	_rectVector.push_back(logRects);
	_rectVector.push_back(lLogRects);
	_rectVector.push_back(rLogRects);

	std::vector<SDL_Rect> playerIdleRects;
	std::vector<SDL_Rect> playerLeftRects;
	std::vector<SDL_Rect> playerRightRects;


	//_playerRectLocations
	SDL_Rect playerIdle = {
		0, 0,
		40, 71
	};

	SDL_Rect playerIdleDest = {
		0, 0,
		40, 71
	};
	playerIdleRects.push_back(playerIdle);
	playerIdleRects.push_back(playerIdleDest);

	SDL_Rect playerLeft = {
		60, 0,
		60, 71
	};

	SDL_Rect playerLeftDest = {
		395, 345,
		60, 71
	};
	playerLeftRects.push_back(playerLeft);
	playerLeftRects.push_back(playerLeftDest);

	SDL_Rect playerRight = {
		120, 0,
		60, 71
	};

	SDL_Rect playerRightDest = {
		590, 345,
		60, 71
	};
	playerRightRects.push_back(playerRight);
	playerRightRects.push_back(playerRightDest);
	
	_playerRectLocations.push_back(playerIdleRects);
	_playerRectLocations.push_back(playerLeftRects);
	_playerRectLocations.push_back(playerRightRects);


}

void initialiseTreePieces(std::vector<Tree> &_treePieces, std::vector<SDL_Rect> &_destRectVector, int _logsChopped)
{

	Tree startingLog({0, 0, 0, 0}, none, 0, 328, 330, 368, 80);
	for (int i = 0; i < 4; i++)	//forces the first three logs to be blank
	{
		_treePieces.push_back(startingLog);
		_treePieces.at(i).setVectorPos(i);
		_treePieces.at(i).setY(330 - (80 * (i)));
		_treePieces.at(i).formatRect();

		_destRectVector.push_back(_treePieces.at(i).getDestRect());
	}

	for (int i = 0; i < 4; i++)	// the next logs are randomly with trunks
	{
		_treePieces.push_back(startingLog);

		int sideChance = rand() % 4 + 1;

		switch (sideChance)
		{
		case 1:
			if (_treePieces.at(3 + i).getTrunkStatus() == right)
			{
				_treePieces.at(4 + i).setTrunkStatus(none);
			}
			else
			{
				_treePieces.at(4 + i).setTrunkStatus(left);
			}
			break;
		
		case 2:
			if (_treePieces.at(3 + i).getTrunkStatus() == left)
			{
				_treePieces.at(4 + i).setTrunkStatus(none);
			}
			else
			{
				_treePieces.at(4 + i).setTrunkStatus(right);
			}
			break;
		
		default:
			break;
		}
	
		_treePieces.at(4 + i).setVectorPos(4 + i);
		_treePieces.at(4 + i).setY(330 - (80 * (4 + i)));
		_treePieces.at(4 + i).formatRect();

		_destRectVector.push_back(_treePieces.at(4 + i).getDestRect());


	}


}

void initialiseHelp(SDL_Renderer* _rend, std::vector<SDL_Texture*>& _textTexture, std::vector<SDL_Rect>& _textRects, TTF_Font* _font, SDL_Color _colour)
{
	SDL_Surface* title{ nullptr };
	title = TTF_RenderText_Solid(_font, "LumberTim -- SuperWoodChop Man ", _colour);
	SDL_Texture* titleTex = SDL_CreateTextureFromSurface(_rend, title);
	SDL_FreeSurface(title);

	_textTexture.push_back(titleTex);


	SDL_Surface* lineOne{ nullptr };
	lineOne = TTF_RenderText_Solid(_font, "- You need to chop the logs of the tree to increase your score, watch out though as branches can squash you", _colour);
	SDL_Texture* lineOneTex = SDL_CreateTextureFromSurface(_rend, lineOne);
	SDL_FreeSurface(lineOne);

	_textTexture.push_back(lineOneTex);


	SDL_Surface* lineTwo{ nullptr };
	lineTwo = TTF_RenderText_Solid(_font, "- Use the arrow keys to switch sides and avoid the branches and chop the exposed side", _colour);
	SDL_Texture* lineTwoTex = SDL_CreateTextureFromSurface(_rend, lineTwo);
	SDL_FreeSurface(lineTwo);

	_textTexture.push_back(lineTwoTex);


	SDL_Surface* lineThree{ nullptr };
	lineThree = TTF_RenderText_Solid(_font, "- use the arrow keys in the character select menu to change sprite", _colour);
	SDL_Texture* lineThreeTex = SDL_CreateTextureFromSurface(_rend, lineThree);
	SDL_FreeSurface(lineThree);

	_textTexture.push_back(lineThreeTex);



	SDL_Rect titleDest = {
		50, 75,
		400, 32
	};
	_textRects.push_back(titleDest);


	SDL_Rect lineOneDest = {
		50, 120,
		950, 32
	};
	_textRects.push_back(lineOneDest);


	SDL_Rect lineTwoDest = {
		50, 165,
		950, 32
	};
	_textRects.push_back(lineTwoDest);


	SDL_Rect lineThreeDest = {
		50, 210,
		950, 32
	};
	_textRects.push_back(lineThreeDest);


}



void rerenderMenuButtons(SDL_Renderer* _rend, std::vector<SDL_Texture*>& _buttonTextures, std::vector<SDL_Rect>& _spriteStateRects, std::vector<Button>& _menuButtons)
{
	//rect locations of the buttons
	SDL_Rect Play = {
		472, 300,
		80, 50
	};
	
	SDL_Rect Character = {
		472, 360,
		80, 50
	};

	SDL_Rect Quit = {
		472, 420,
		80, 50
	};

	SDL_Rect Help = {
		562, 300,
		25, 25
	};

	for (int i = 0; i < 5; i++)
	{
		if (i == 3)
		{
			i = 4;
		}
		switch (i)
		{
		case 0:
			switch (_menuButtons.at(i).getCurrentState())	//rerenders the button images based on their state
			{
			case button_idle:
				SDL_RenderCopy(_rend, _buttonTextures.at(0), &_spriteStateRects.at(0), &Play);
				break;

			case button_mouse_over:
				SDL_RenderCopy(_rend, _buttonTextures.at(0), &_spriteStateRects.at(1), &Play);
				break;

			case button_mouse_down:

				SDL_RenderCopy(_rend, _buttonTextures.at(0), &_spriteStateRects.at(2), &Play);
				break;
			}

		case 1:
			switch (_menuButtons.at(i).getCurrentState())
			{
			case button_idle:
				SDL_RenderCopy(_rend, _buttonTextures.at(1), &_spriteStateRects.at(0), &Character);
				break;

			case button_mouse_over:
				SDL_RenderCopy(_rend, _buttonTextures.at(1), &_spriteStateRects.at(1), &Character);
				break;

			case button_mouse_down:

				std::cout << "click";

				SDL_RenderCopy(_rend, _buttonTextures.at(1), &_spriteStateRects.at(2), &Character);
				break;
			}

		case 2:
			switch (_menuButtons.at(i).getCurrentState())
			{
			case button_idle:
				SDL_RenderCopy(_rend, _buttonTextures.at(2), &_spriteStateRects.at(0), &Quit);
				break;

			case button_mouse_over:
				SDL_RenderCopy(_rend, _buttonTextures.at(2), &_spriteStateRects.at(1), &Quit);
				break;

			case button_mouse_down:

				std::cout << "click";

				SDL_RenderCopy(_rend, _buttonTextures.at(2), &_spriteStateRects.at(2), &Quit);
				break;
			}

		case 4:
			switch (_menuButtons.at(i).getCurrentState())
			{
			case button_idle:
				SDL_RenderCopy(_rend, _buttonTextures.at(4), &_spriteStateRects.at(3), &Help);
				break;

			case button_mouse_over:
				SDL_RenderCopy(_rend, _buttonTextures.at(4), &_spriteStateRects.at(4), &Help);
				break;

			case button_mouse_down:

				std::cout << "click";

				SDL_RenderCopy(_rend, _buttonTextures.at(4), &_spriteStateRects.at(5), &Help);
				break;
			}

		}
		
		
	}
}
 
void showHelp(SDL_Renderer* _rend, TTF_Font* _font, SDL_Color _colour, SDL_Texture*& _fontTexture, std::vector<SDL_Texture*>& _textTexture, std::vector<SDL_Rect>& _textRects)
{

	SDL_RenderCopy(_rend, _textTexture.at(0), 0, &_textRects.at(0));
	SDL_RenderCopy(_rend, _textTexture.at(1), 0, &_textRects.at(1));
	SDL_RenderCopy(_rend, _textTexture.at(2), 0, &_textRects.at(2));
	SDL_RenderCopy(_rend, _textTexture.at(3), 0, &_textRects.at(3));

}

void drawBackButton(SDL_Renderer* _rend, std::vector<SDL_Texture*>& _buttonTextures, std::vector<SDL_Rect>& _spriteStateRects, std::vector<Button>& _menuButtons)
{
	//button loaction
	SDL_Rect Back = {
		472, 420,
		80, 50
	};


	switch (_menuButtons.at(3).getCurrentState())
	{
	case button_idle:
		SDL_RenderCopy(_rend, _buttonTextures.at(3), &_spriteStateRects.at(0), &Back);
		break;

	case button_mouse_over:
		SDL_RenderCopy(_rend, _buttonTextures.at(3), &_spriteStateRects.at(1), &Back);
		break;

	case button_mouse_down:

		std::cout << "click";

		SDL_RenderCopy(_rend, _buttonTextures.at(3), &_spriteStateRects.at(2), &Back);
		break;
	}
}


void changeCharacter(SDL_Renderer* _rend, player _playerCharacter, std::vector<std::vector<SDL_Rect>> _playerRectLocations, SDL_Texture*& _PlayerSpriteSheet)
{
	SDL_Surface* playerSpriteSheetSurface_Tim = SDL_LoadBMP("sprites/LumberTim.bmp");
	SDL_Surface* playerSpriteSheetSurface_Hunter = SDL_LoadBMP("sprites/TheHunter.bmp");
	
	//changes character based on player character value

	switch (_playerCharacter.getCharacterChosen())
	{
	case p_lumberTim:
		if (_PlayerSpriteSheet != nullptr)
		{
			SDL_DestroyTexture(_PlayerSpriteSheet);
		}
		_PlayerSpriteSheet = SDL_CreateTextureFromSurface(_rend, playerSpriteSheetSurface_Tim);
		break;

	case p_theHunter:
		if (_PlayerSpriteSheet != nullptr)
		{
			SDL_DestroyTexture(_PlayerSpriteSheet);
		}
		_PlayerSpriteSheet = SDL_CreateTextureFromSurface(_rend, playerSpriteSheetSurface_Hunter);
		break;

	}
	SDL_FreeSurface(playerSpriteSheetSurface_Tim);
	SDL_FreeSurface(playerSpriteSheetSurface_Hunter);


}


void calculateLogSetupTexture(SDL_Renderer* _rend, SDL_Texture* logTexture, SDL_Texture* RightlogTexture, SDL_Texture* LeftlogTexture, std::vector<std::vector<SDL_Rect>> &_imgRectVector, Tree _inputTree, std::vector<SDL_Rect> _treePieceDestRect, int _vectorPos, Tree _prevPos)
{
	int trunkSide = _inputTree.getTrunkStatus();

	switch (trunkSide)	//renders the log based on teh tree pieces state
	{
	case 0:

		SDL_RenderCopy(_rend, logTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
		break;

	case 1:
		if (_prevPos.getTrunkStatus() == right)
		{
			_inputTree.setTrunkStatus(none);
			SDL_RenderCopy(_rend, logTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
			break;
		}
		else
		{
			SDL_RenderCopy(_rend, LeftlogTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
			break;
		}
		
	

	case 2:
		if (_prevPos.getTrunkStatus() == left)
		{
			_inputTree.setTrunkStatus(none);
			SDL_RenderCopy(_rend, logTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
			break;
		}
		else
		{
			SDL_RenderCopy(_rend, RightlogTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
			break;
		}
		

	default:
		break;
	}
}

void generateNewTreePiece(std::vector<Tree>& _treePieces, std::vector<SDL_Rect>& _destRectVector, int _logsChopped)
{
	Tree newLog({ 0, 0, 0, 0 }, none, 0, 328, 330, 368, 80);
	int index = 7;

	_treePieces.push_back(newLog);
	_treePieces.at(index) = newLog;
	_treePieces.at(index).setVectorPos(index);
	_treePieces.at(index).setY(_treePieces.at(index - 1).getY() - 80);

	int sideChance = rand() % 4 + 1;	// random chance for trunk = 25%

	switch (sideChance)
	{
	case 1:
		if (_treePieces.at(index - 1).getTrunkStatus() == right)
		{
			_treePieces.at(index).setTrunkStatus(none);
		}
		else
		{
			_treePieces.at(index).setTrunkStatus(left);
		}
		break;

	case 2:
		if (_treePieces.at(index - 1).getTrunkStatus() == left)
		{
			_treePieces.at(index).setTrunkStatus(none);
		}
		else
		{
			_treePieces.at(index).setTrunkStatus(right);
		}

		break;

	default:
		break;
	}

	_treePieces.at(index).formatRect();

	_destRectVector.push_back(_treePieces.at(index).getDestRect());
}

void rerenderNewTreeImage(SDL_Renderer* _rend, SDL_Texture* logTexture, SDL_Texture* RightlogTexture, SDL_Texture* LeftlogTexture, std::vector<std::vector<SDL_Rect>>& _imgRectVector, Tree _inputTree, std::vector<SDL_Rect> _treePieceDestRect, int _vectorPos, int _prevVectorPos, std::vector<Tree>& _treePieces)
{
	int trunkSideRerender = _inputTree.getTrunkStatus();
	if (_inputTree.getVectorPos() == 0)
	{
		switch (trunkSideRerender)
		{
		case 0:

			SDL_RenderCopy(_rend, logTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
			break;

		case 1:


			SDL_RenderCopy(_rend, LeftlogTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
			break;




		case 2:

			SDL_RenderCopy(_rend, RightlogTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
			break;

		}
	} else { 
		for (int i = 0; i < 7; i++)
		{

			switch (trunkSideRerender)
			{
			case 0:

				SDL_RenderCopy(_rend, logTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
				break;

			case 1:
				
				SDL_RenderCopy(_rend, LeftlogTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
				break;

			case 2:
			
				SDL_RenderCopy(_rend, RightlogTexture, &_imgRectVector.at(1).at(0), &_treePieceDestRect.at(_vectorPos));
				break;
		


			default:
				break;
			}
		}
	}
}

void updatePlayer(player _playerCharacter, std::vector<std::vector<SDL_Rect>> _playerRectLocations, SDL_Renderer* _rend, SDL_Texture* _PlayerSpriteSheet, bool &_gameOver)
{
	int currentplayerState = _playerCharacter.getplayerState();

	switch (currentplayerState)
	{
	case 0:
		SDL_RenderCopy(_rend, _PlayerSpriteSheet, &_playerRectLocations.at(0).at(0), &_playerRectLocations.at(0).at(1));
		break;

	case 1:
		SDL_RenderCopy(_rend, _PlayerSpriteSheet, &_playerRectLocations.at(1).at(0), &_playerRectLocations.at(1).at(1));
		break;

	case 2:
		SDL_RenderCopy(_rend, _PlayerSpriteSheet, &_playerRectLocations.at(2).at(0), &_playerRectLocations.at(2).at(1));
		break;

	case 3:
		_gameOver = true;
		break;

	}
}

void updateScore(SDL_Renderer* _rend, TTF_Font* _font, SDL_Color _colour, int _logsChopped, int _highscore, SDL_Texture*& _fontTexture, SDL_Rect& _textDestRect, std::vector <SDL_Texture*> digitTextureVector, gameState _currentGameState)
{
	int digits = 0;
	int tens = 0;
	int hundreds = 0;
	int thousands = 0;

	for (int scoreCount = 0; scoreCount < _logsChopped; scoreCount++)	//adds score to the counter so it can be translated to the score board
	{
		digits++;
		if (digits == 10)
		{
			digits = 0;
			tens++;
		}

		if (tens == 10)
		{
			tens = 0;
			hundreds++;
		}

		if (hundreds == 10)
		{
			hundreds = 0;
			thousands++;
		}

		//std::cout << thousands << hundreds << tens << digits;

	}
	
		SDL_Surface* score{ nullptr };
		score = TTF_RenderText_Solid(_font, "Score: ", _colour);
		
		_fontTexture = SDL_CreateTextureFromSurface(_rend, score);
		SDL_FreeSurface(score);

		if (_currentGameState == game)
		{
			SDL_Rect newScorePos = { 800, 20, score->w, score->h };

			SDL_RenderCopy(_rend, _fontTexture, 0, &newScorePos);

			SDL_Rect thousandsSpace = {
			900, 20,
			20, 40
			};
			SDL_Rect hundredsSpace = {
				921, 20,
				20, 40
			};
			SDL_Rect tensSpace = {
				942, 20,
				20, 40
			};
			SDL_Rect digitsSpace = {
				963, 20,
				20, 40
			};
			SDL_RenderCopy(_rend, digitTextureVector.at(thousands), 0, &thousandsSpace);
			SDL_RenderCopy(_rend, digitTextureVector.at(hundreds), 0, &hundredsSpace);
			SDL_RenderCopy(_rend, digitTextureVector.at(tens), 0, &tensSpace);
			SDL_RenderCopy(_rend, digitTextureVector.at(digits), 0, &digitsSpace);
		}
		else if (_currentGameState == game_over)
		{
			SDL_Rect newScorePos = {380, 340, score->w, score->h };

			SDL_RenderCopy(_rend, _fontTexture, 0, &newScorePos);
			SDL_Rect thousandsSpace = {
				550, 340,
				20, 40
			};
			SDL_Rect hundredsSpace = {
				571, 340,
				20, 40
			};
			SDL_Rect tensSpace = {
				592, 340,
				20, 40
			};
			SDL_Rect digitsSpace = {
				613, 340,
				20, 40
			};
			SDL_RenderCopy(_rend, digitTextureVector.at(thousands), 0, &thousandsSpace);
			SDL_RenderCopy(_rend, digitTextureVector.at(hundreds), 0, &hundredsSpace);
			SDL_RenderCopy(_rend, digitTextureVector.at(tens), 0, &tensSpace);
			SDL_RenderCopy(_rend, digitTextureVector.at(digits), 0, &digitsSpace);
		}
		

		
	
	if (_currentGameState == game_over)
	{
		SDL_Surface* highScore{ nullptr };
		highScore = TTF_RenderText_Solid(_font, "High Score: ", _colour);


		_fontTexture = SDL_CreateTextureFromSurface(_rend, highScore);
		SDL_Rect highScorePos = { 380, 300, highScore->w, highScore->h };
		SDL_FreeSurface(highScore);
		SDL_RenderCopy(_rend, _fontTexture, 0, &highScorePos);


		int hDigits = 0;
		int hTens = 0;
		int hHundreds = 0;
		int hThousands = 0;

		for (int scoreCount = 0; scoreCount < _highscore; scoreCount++)
		{
			hDigits++;
			if (hDigits == 10)
			{
				hDigits = 0;
				hTens++;
			}

			if (hTens == 10)
			{
				hTens = 0;
				hHundreds++;
			}

			if (hHundreds == 10)
			{
				hHundreds = 0;
				hThousands++;
			}

			//std::cout << thousands << hundreds << tens << digits;

		}

		
		SDL_Rect thousandsHSpace = {
			550, 300,
			20, 40
		};
		SDL_Rect hundredsHSpace = {
			571, 300,
			20, 40
		};
		SDL_Rect tensHSpace = {
			592, 300,
			20, 40
		};
		SDL_Rect digitsHSpace = {
			613, 300,
			20, 40
		};

		SDL_RenderCopy(_rend, digitTextureVector.at(hThousands), 0, &thousandsHSpace);
		SDL_RenderCopy(_rend, digitTextureVector.at(hHundreds), 0, &hundredsHSpace);
		SDL_RenderCopy(_rend, digitTextureVector.at(hTens), 0, &tensHSpace);
		SDL_RenderCopy(_rend, digitTextureVector.at(hDigits), 0, &digitsHSpace);
	}













}
