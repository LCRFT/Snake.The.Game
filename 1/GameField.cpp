#include "GameField.h"

GameField::GameField(int screenWidth, int screenHeight, int otstup, SDL_Renderer* renderer)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->fieldOtstup = otstup;
	score = 0;

	this->renderer = renderer;

	std::ifstream file("best.txt", std::ios::in);
	bestScore = 0;
	if (file.is_open())
		file >> bestScore;

	font = TTF_OpenFont("American_TextC.ttf", 50);
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	
	scoreTexture = renderText("Score: " + std::to_string(score));
	bestScoreTexture = renderText("Best score: " + std::to_string(bestScore));
	gameOverMessage = renderText("Game over!    Press SPACE to retry.");
}


SDL_Texture* GameField::renderText(std::string message)
{
	SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		TTF_CloseFont(font);
		SDL_FreeSurface(surf);
	}

	SDL_FreeSurface(surf);
	return texture;
}

void GameField::addScore(int dif)
{
	score += dif;
	if (score > bestScore)
		bestScore = score;
	scoreTexture = renderText("Score: " + std::to_string(score));
	bestScoreTexture = renderText("Best score: " + std::to_string(bestScore));
}

void GameField::draw(bool gameOver)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderDrawLine(renderer, 0, fieldOtstup, 0, screenHeight);
	SDL_RenderDrawLine(renderer, 0, fieldOtstup, screenWidth, fieldOtstup);
	SDL_RenderDrawLine(renderer, 0, screenHeight - 1, screenWidth, screenHeight - 1);
	SDL_RenderDrawLine(renderer, screenWidth - 1, fieldOtstup, screenWidth - 1, screenHeight);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_Rect location;
	location.x = 0;
	location.y = 0;
	SDL_QueryTexture(scoreTexture, NULL, NULL, &location.w, &location.h);
	SDL_RenderCopy(renderer, scoreTexture, NULL, &location);

	location.x = screenWidth / 2;
	SDL_QueryTexture(bestScoreTexture, NULL, NULL, &location.w, &location.h);
	SDL_RenderCopy(renderer, bestScoreTexture, NULL, &location);


	if (gameOver)
	{
		SDL_QueryTexture(gameOverMessage, NULL, NULL, &location.w, &location.h);
		location.x = screenWidth / 2 - location.w / 2;
		location.y = (fieldOtstup + screenHeight) / 2 - location.h / 2;
		SDL_RenderCopy(renderer, gameOverMessage, NULL, &location);
	}

}

void GameField::writeBest()
{
	std::ofstream file("best.txt", std::ios::out);
	if (file.is_open())
		file << bestScore;
}

void GameField::restart()
{
	score = 0;
}

GameField::~GameField()
{
	SDL_DestroyTexture(scoreTexture);
	SDL_DestroyTexture(bestScoreTexture);
}