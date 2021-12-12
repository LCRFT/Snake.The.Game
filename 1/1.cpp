﻿#include <string>
#include <fstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "snake_code.h"
#include "GameField.h"
#include <SDL_mixer.h>
#include <ctime>
#include <iostream>

const int SCREEN_WIDTH = 800;//кратне 16
const int SCREEN_HEIGHT = 400;//кратне 16

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int SPRITE_SIZE = 16;

const int TIME_FOR_STEP = 200;//50

const int OTSTUP = 5;

const int MAXIMUM_WIDTH = SCREEN_WIDTH / SPRITE_SIZE;
const int MAXUMUM_HEIGHT = SCREEN_HEIGHT / SPRITE_SIZE;

Mix_Music* gMusic = NULL;

using namespace std;

int main(int argc, char* argv[])
{
    srand(time(0));


    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)            //SDL require initialization at the start of the program
        return 1;
    window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        return 2;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        return 3;
    if (TTF_Init() != 0)
    {
        SDL_Quit();
        return 1;
    }

    SDL_Texture* head = IMG_LoadTexture(renderer, "head.png");
    SDL_Texture* body = IMG_LoadTexture(renderer, "body.png");
    SDL_Texture* fr[3];// = IMG_LoadTexture(renderer, "fruit.png");
    fr[0]= IMG_LoadTexture(renderer, "fruit1.png");
    fr[1] = IMG_LoadTexture(renderer, "fruit2.png");
    fr[2] = IMG_LoadTexture(renderer, "fruit3.png");

    if (head == nullptr || body == nullptr || fr == nullptr)
        return 100;

    GameField gameField(SCREEN_WIDTH, SCREEN_HEIGHT, OTSTUP * SPRITE_SIZE, renderer);

    Snake snake(head, body, 0, OTSTUP * SPRITE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE_SIZE, OTSTUP);
    Direction direction = RIGHT;

    Fruit fruit(fr[0], (rand() % MAXIMUM_WIDTH) * SPRITE_SIZE, (rand() % (MAXUMUM_HEIGHT - OTSTUP) + OTSTUP) * SPRITE_SIZE, SPRITE_SIZE, 15);
    int lastTime = 0;

    int temp;

    bool gameOver = false;

    int moveResult;

    SDL_Keycode lastPressedKey= SDLK_0;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    gMusic = Mix_LoadMUS("1.mp3");
    if (gMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    Mix_PlayMusic(gMusic, -1);

    SDL_Event e;
    while (1)
    {
        SDL_PollEvent(&e);
        if (e.type == SDL_EventType::SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) break;

        SDL_RenderClear(renderer);

        if (e.type == SDL_KEYDOWN)
        {
            lastPressedKey = e.key.keysym.sym;
            if (lastPressedKey == SDLK_SPACE)
            {
                gameField.restart();
                gameField.addScore(0);
                fruit = Fruit(fr[0], (rand() % MAXIMUM_WIDTH) * SPRITE_SIZE, (rand() % (MAXUMUM_HEIGHT - OTSTUP) + OTSTUP) * SPRITE_SIZE, SPRITE_SIZE, 15);
                snake = Snake(head, body, 0, OTSTUP * SPRITE_SIZE, SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE_SIZE, OTSTUP);
                direction = RIGHT;
                gameOver = false;
                
                Mix_PlayMusic(gMusic, -1);
            }
        }

        temp = SDL_GetTicks();
        if (temp - lastTime >= TIME_FOR_STEP)   //frametime sync using SDL timer for stable game speed 
        {
            if (!gameOver)
            {
                moveResult = snake.move(direction, MAXIMUM_WIDTH, MAXUMUM_HEIGHT, fruit);
                if (moveResult == -1)
                {
                    Mix_HaltMusic();
                    gameOver = true;
                }
                if (moveResult > 1)
                    gameField.addScore(moveResult);
            }

            switch (lastPressedKey) //snake can't turn full 180 in 1 frame
            {
            case SDLK_a: case SDLK_LEFT:
                if (direction != RIGHT)
                    direction = LEFT;
                break;
            case SDLK_d: case SDLK_RIGHT:
                if (direction != LEFT)
                    direction = RIGHT;
                break;
            case SDLK_w: case SDLK_UP:
                if (direction != DOWN)
                    direction = UP;
                break;
            case SDLK_s: case SDLK_DOWN:
                if (direction != UP)
                    direction = DOWN;
            }

            lastTime = temp;
        }

        if (!gameOver)
        {
            fruit.draw(renderer, 0);
            snake.draw(renderer);
        }

        gameField.draw(gameOver);



        SDL_RenderPresent(renderer);
    }


    gameField.writeBest();
    SDL_DestroyTexture(head);
    SDL_DestroyTexture(body);
    SDL_DestroyTexture(fr[0]);
    SDL_DestroyTexture(fr[1]);
    SDL_DestroyTexture(fr[2]);
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
