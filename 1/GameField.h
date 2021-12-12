#pragma once
#include <string>
#include <fstream>
#include "SDL.h"
#include "SDL_ttf.h"

class GameField
{
private:
	int screenWidth;
	int screenHeight;
	int fieldOtstup;
	int score;
	int bestScore;

	SDL_Renderer* renderer;

	TTF_Font* font;
	SDL_Color color;
	SDL_Texture* scoreTexture;
	SDL_Texture* bestScoreTexture;
	SDL_Texture* gameOverMessage;

	SDL_Texture* renderText(std::string);

public:
	GameField(int, int, int, SDL_Renderer*);
	void addScore(int);
	void draw(bool);
	void writeBest();
	void restart();

	~GameField();
};