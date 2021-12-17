
#include <fstream>
#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <ctime>
#include <iostream>
#include <string>
#include "Snake_and_Fruit.h"

const int scr_width = 800;//кратне 20
const int scr_height = 400;//кратне 20

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int size_of_sprite = 20;
int loop = 200;
int score = 0,highscore = 0;

const int max_width = scr_width / size_of_sprite;
const int max_height = scr_height / size_of_sprite;


//Mix_Music* gMusic = NULL;
enum Direction{Up=SDLK_UP,Down= SDLK_DOWN,Right = SDLK_RIGHT,Left=SDLK_LEFT};

int main(int argc, char* args[])
{
    srand(time(0));

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return 1;
    window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, scr_width, scr_height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        return 2;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        return 3;
    if (TTF_Init() != 0)
    {
        return 4;
    }
    SDL_Color textColor = { 255, 255, 255, 255 };
    TTF_Font* font = TTF_OpenFont("American_TextC.ttf", 100);
    
    SDL_Texture* background = IMG_LoadTexture(renderer, "background_image.png");
    SDL_Texture* head = IMG_LoadTexture(renderer, "head.png");
    SDL_Texture* fruit = IMG_LoadTexture(renderer, "fruit.png");
    SDL_Texture* body = IMG_LoadTexture(renderer, "body.png");
    SDL_Texture* game_over_mes=background;
    SDL_Rect game_over_pos;// = { ,,0,0 };
    game_over_pos.x = 0;// (scr_width / 4);
    game_over_pos.y = (scr_height / 3);
    game_over_pos.w = 800;
    game_over_pos.h = 200;
    std::string message;

    bool GameOver = false;

    Snake Hero(head, size_of_sprite, max_width, max_height,body);

    Fruit Cross(fruit, size_of_sprite, max_width, max_height);

    Cross.random_spawn((rand() % max_width + 1) * size_of_sprite, (rand() % max_height + 1) * size_of_sprite);

    Mix_Music* gMusic = NULL;
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    gMusic = Mix_LoadMUS("1.mp3");
    if (gMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    Mix_PlayMusic(gMusic, -1);
    Mix_VolumeMusic(10);
    
    SDL_Keycode lastPressedKey = SDLK_AGAIN, way=Right;
    int last_time = 0,time_present;
    bool quit = false,fruit_spawn=true;
    SDL_Event e;
    while (!quit)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer, background, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
        SDL_PollEvent(&e);
        if (e.type == SDL_EventType::SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) break;
        if (e.type == SDL_KEYDOWN)
        {
            lastPressedKey = e.key.keysym.sym;
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (lastPressedKey == SDLK_r)
            {
                score = 0;
                Hero = Snake(head, size_of_sprite, max_width, max_height, body);
                Mix_PlayMusic(gMusic, -1);
                
            }
            if (lastPressedKey == Up)
            {
                if (way != Down)
                {
                    Hero.change_direction(Up);
                    way = Up;
                }
            }
            if (lastPressedKey == Down)
            {
                if (way != Up)
                {
                    Hero.change_direction(Down);
                    way = Down;
                }
            }
            if (lastPressedKey == Left)
            {
                if (way !=Right)
                {
                    Hero.change_direction(Left);
                    way = Left;
                }
            }
            if (lastPressedKey == Right)
            {
                if (way != Left)
                {
                    Hero.change_direction(Right);
                    way = Right;
                }
            }
        }

        if(!Hero.Death())
        {
        time_present = SDL_GetTicks();
        if (time_present - last_time >= loop)
        {
            Hero.move();
            if (Hero.Collision_head(Cross.get_location()))
            {
                score += 10;
                if (score > highscore)
                    highscore = score;
                Hero.multiply(way);
                Cross.random_spawn((rand() % (max_width - 2)) * size_of_sprite, (rand() % (max_height - 2)) * size_of_sprite);
            }
            last_time = time_present;
        }
            Cross.fruit_draw(renderer);
            Hero.draw_snake(renderer);
        }
        else
        {
            Mix_HaltMusic();
            draw_message(renderer, message, game_over_mes, &game_over_pos, font, textColor,score,highscore);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}

