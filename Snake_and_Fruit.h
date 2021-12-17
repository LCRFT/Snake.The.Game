#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <vector>

class Snake
{
private:
	SDL_Texture* sprite_body= nullptr;
	std::vector<SDL_Rect> vec_body_pos;
	SDL_Texture* sprite_head = nullptr;
	SDL_Rect rect_head_pos;
	SDL_Rect rect_body_pos;
	int size, bound_width, bound_height;
	
	double angle = 90;
	SDL_Keycode Direction=SDLK_RIGHT;
public:
	Snake(SDL_Texture* head,int sprite_size,int screen_size_w, int screen_size_h,SDL_Texture* body)
	{
		rect_head_pos.w = sprite_size;
		rect_head_pos.h = sprite_size;
		rect_head_pos.x = (screen_size_w / 2)* sprite_size;
		rect_head_pos.y = (screen_size_h / 2)* sprite_size;
		sprite_head = head;
		
		size = sprite_size;
		rect_body_pos.w = sprite_size;
		rect_body_pos.h = sprite_size;
		sprite_body = body;
		vec_body_pos.push_back(rect_body_pos);
		bound_width = screen_size_w * sprite_size- sprite_size;
		bound_height = screen_size_h * sprite_size - sprite_size;
		
	}
	
	void change_direction(SDL_Keycode way)
	{
		Direction = way;
		switch (Direction)
		{
		case SDLK_UP:
			angle = 90 * 0;
			break;
		case SDLK_DOWN:
			angle = 90 * 2;
			break;
		case SDLK_RIGHT:
			angle = 90 * 1;
		case SDLK_LEFT:
			angle = 90 * 3;
		}
	}
	void draw_snake(SDL_Renderer* renderer)
	{
			SDL_RenderCopyEx(renderer, sprite_head, NULL, &rect_head_pos, angle, NULL, SDL_FLIP_NONE);
			for (int i = 0; i < vec_body_pos.size(); ++i)
			{
				SDL_RenderCopyEx(renderer, sprite_body, NULL, &vec_body_pos[i], 0, NULL, SDL_FLIP_NONE);
			}
	}
	void multiply(SDL_Keycode way)
	{
		switch (Direction)
		{
		case SDLK_RIGHT:
		{
		rect_body_pos.x = vec_body_pos[vec_body_pos.size() - 1].x - size;
		rect_body_pos.y = vec_body_pos[vec_body_pos.size() - 1].y;
		break;
		}
		case SDLK_LEFT:
		{
			rect_body_pos.x = vec_body_pos[vec_body_pos.size() - 1].x + size;
			rect_body_pos.y = vec_body_pos[vec_body_pos.size() - 1].y;
			break;
		}
		case SDLK_UP:
		{
			rect_body_pos.x = vec_body_pos[vec_body_pos.size() - 1].x;
			rect_body_pos.y = vec_body_pos[vec_body_pos.size() - 1].y- size;
			break;
		}
		case SDLK_DOWN:
		{
			rect_body_pos.x = vec_body_pos[vec_body_pos.size() - 1].x;
			rect_body_pos.y = vec_body_pos[vec_body_pos.size() - 1].y + size;
			break;
		}
		}
		vec_body_pos.push_back(rect_body_pos);
	}
	void move()
	{
		if (!Death())
		{
			for (int i = vec_body_pos.size() - 1; i >= 0; --i)
			{
				if (i == 0)
				{
					vec_body_pos[0].x = rect_head_pos.x;
					vec_body_pos[0].y = rect_head_pos.y;
				}
				else
				{
					vec_body_pos[i].x = vec_body_pos[i - 1].x;
					vec_body_pos[i].y = vec_body_pos[i - 1].y;
				}

			}
			switch (Direction)
			{
			case SDLK_UP:
				rect_head_pos.y -= size;
				break;
			case SDLK_RIGHT:
				rect_head_pos.x += size;
				break;
			case SDLK_LEFT:
				rect_head_pos.x -= size;
				break;
			case SDLK_DOWN:
				rect_head_pos.y += size;
			}
		}
	}
	bool Death()
	{
		if (rect_head_pos.x<0|| rect_head_pos.x >= bound_width|| rect_head_pos.y<0 || rect_head_pos.y > bound_height)
		{
			return true;
		}
		for (int i = 0; i < vec_body_pos.size(); ++i)
		{
			if (rect_head_pos.x == vec_body_pos[i].x && rect_head_pos.y == vec_body_pos[i].y)
				return true;
		}
		return false;
	}
	SDL_Rect* get_location()
	{
		return &rect_head_pos;
	}
	bool Collision_head(SDL_Rect* Fruit)
	{
		if (rect_head_pos.x == Fruit->x && rect_head_pos.y == Fruit->y)
		{
			return true;
		}
		return false;
	}


};

class Fruit
{
private:
	SDL_Texture* sprite_fruit = nullptr;
	SDL_Rect rect_fruit_pos;
	int width, height,fruit_size;
public:
	SDL_Rect* get_location()
	{
		return &rect_fruit_pos;
	}

	Fruit(SDL_Texture* sprite1, const int sprite_size, const int screen_size_w, const int screen_size_h)
	{
		rect_fruit_pos.w = sprite_size;
		rect_fruit_pos.h = sprite_size;
		width = screen_size_w;
		height = screen_size_h;
		fruit_size = sprite_size;
		sprite_fruit = sprite1;
	}
	void random_spawn(int rnd_x, int rnd_y)
	{
		rect_fruit_pos.x = rnd_x;
		std::cout << rect_fruit_pos.x << std::endl;
		rect_fruit_pos.y = rnd_y;
		std::cout << rect_fruit_pos.y << std::endl;
	}
	void fruit_draw(SDL_Renderer* renderer)
	{

		SDL_RenderCopyEx(renderer, sprite_fruit, NULL, &rect_fruit_pos, 0, NULL, SDL_FLIP_NONE);
	}
};

void GameOver(SDL_Rect* head_pos, SDL_Rect* fruit_pos)
{

}

SDL_Texture* show_text(SDL_Renderer* renderer,std::string message, TTF_Font* font, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if(texture!=nullptr)
	return texture;
	else
	{
		std::cout << "Failed to load message!" << std::endl;
		return nullptr;
	}
}

void draw_message(SDL_Renderer* renderer, std::string message, SDL_Texture* mes_texture, SDL_Rect* mes_pos, TTF_Font* font, SDL_Color textColor,int score,int highscore)
{
	message = "Score:" + std::to_string(score) + "   " + "Highscore:" + std::to_string(highscore) + "   " + "Game over! Press 'R' to retry.";
	mes_texture = show_text(renderer, message, font, textColor);
	SDL_RenderCopy(renderer, mes_texture, NULL, mes_pos);
}

