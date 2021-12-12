#include "snake_code.h"

ObjectWithTexture::ObjectWithTexture(SDL_Texture* texture, int x, int y, int size)
{
	this->sprite = texture;
	this->location.x = x;
	this->location.y = y;
	this->location.w = size;
	this->location.h = size;
}

void ObjectWithTexture::draw(SDL_Renderer* renderer, int angle)
{
	SDL_RenderCopyEx(renderer, this->sprite, NULL, &(this->location), angle, NULL, SDL_FLIP_NONE);
}

Snake::Snake(SDL_Texture* head, SDL_Texture* body, int x, int y, int width, int height, int size, int otstup)
{
	this->bodySprite = body;
	this->head = ObjectWithTexture(head, x, y, size);
	this->otstup = otstup;
}

int Snake::move(Direction dir, int maxWidth, int maxHeight, Fruit& fruit)//, std::vector<Fruit> &fruits)
{
	switch (dir)
	{
	case LEFT:
		if (head.location.x - head.location.w < 0)
			return -1;
		if (!body.empty())
		{
			for (int i = body.size() - 1; i > 0; --i)
			{
				body[i].location = body[i - 1].location;
			}
			body.front().location = head.location;
		}
		head.location.x -= head.location.w;
		angle = 270;
		break;

	case RIGHT:
		if (head.location.x + head.location.w == maxWidth * head.location.w)
			return -1;
		if (!body.empty())
		{
			for (int i = body.size() - 1; i > 0; --i)
			{
				body[i].location = body[i - 1].location;
			}
			body.front().location = head.location;
		}
		head.location.x += head.location.w;
		angle = 90;
		break;

	case UP:
		if (head.location.y - head.location.h < otstup * head.location.h)
			return -1;
		if (!body.empty())
		{
			for (int i = body.size() - 1; i > 0; --i)
			{
				body[i].location = body[i - 1].location;
			}
			body.front().location = head.location;
		}
		head.location.y -= head.location.h;
		angle = 0;
		break;

	case DOWN:
		if (head.location.y + head.location.h == maxHeight * head.location.h)
			return -1;
		if (!body.empty())
		{
			for (int i = body.size() - 1; i > 0; --i)
			{
				body[i].location = body[i - 1].location;
			}
			body.front().location = head.location;
		}
		head.location.y += head.location.h;
		angle = 180;
	}
	for (ObjectWithTexture b : body)
		if (head.location.x == b.location.x && head.location.y == b.location.y)
			return -1;
	if (head.location.x == fruit.location.x && head.location.y == fruit.location.y)
	{
		fruit.setLocation(fruit.location.w * (rand() % maxWidth), fruit.location.h * (rand() % (maxHeight - otstup) + otstup));
		//fruit.
		this->grow();
		return fruit.getScore();
	}
	return 0;
}

void Snake::draw(SDL_Renderer* renderer)
{
	head.draw(renderer, angle);
	for (ObjectWithTexture b : body)
		b.draw(renderer, 0);
}

void Snake::grow()
{
	body.push_back(ObjectWithTexture(bodySprite, head.location.x, head.location.y, head.location.w));
}

Fruit::Fruit(SDL_Texture* texture, int x, int y, int size, int score) : ObjectWithTexture(texture, x, y, size)
{
	this->score = score;
}

void Fruit::setLocation(int x, int y)
{
	location.x = x;
	location.y = y;
}
