#include "Bola_colision.h"
#include "Point2D.h"
#include <cmath>

Bola_colision::Bola_colision() : formas_colision(), size(0)
{
	pos.x = 0;
	pos.y = 0;
}

void Bola_colision::set_pos(float x, float y) { pos.x = x; pos.y = y; }

void Bola_colision::set_size(float n) { size = n; }

float Bola_colision::get_pos_x() const { return pos.x; }
float Bola_colision::get_pos_y() const { return pos.y; }
punto2D Bola_colision::get_pos() const { return pos; }

float Bola_colision::get_size() const { return size; }

Bola_colision::~Bola_colision() {}

bool Bola_colision::es_colision(float x, float y)
{
	punto2D ball;
	ball.x = pos.x + 8;
	ball.y = pos.y + 40; 

	punto2D point;
	point.x = x - ball.x;
	point.y = y - ball.y;

	ball.x = 0;
	ball.y = 0;

	float punto_radio = sqrt(point.x*point.x + point.y*point.y);

	if (punto_radio <= this->size)
		return true;
	else
		return false;
}