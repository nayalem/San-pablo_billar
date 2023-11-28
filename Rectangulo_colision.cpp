#include "Rectangulo_colision.h"

Rectangulo_colision::Rectangulo_colision() : formas_colision(), width(0), height(0)
{
	start_point.x = 0;
	start_point.y = 0;
}
Rectangulo_colision::Rectangulo_colision(punto2D p, float w, float h) : formas_colision(), width(w), height(h) { start_point = p; }

float Rectangulo_colision::get_ancho() const { return width; }
float Rectangulo_colision::get_alto() const { return height; }

punto2D Rectangulo_colision::get_inicio_punto() const { return start_point; }
punto2D Rectangulo_colision::get_fin_punto() const
{
	punto2D a;
	a.x = start_point.x + width;
	a.y = start_point.y + height;
	return a;
}

void Rectangulo_colision::set_width(float n) { width = n; }
void Rectangulo_colision::set_height(float n) { height = n; }
void Rectangulo_colision::set_inicio_punto(punto2D p) { start_point = p; }
void Rectangulo_colision::set_inicio_punto(float x, float y)
{
	start_point.x = x;
	start_point.y = y;
}

void Rectangulo_colision::set_fin_punto(punto2D p)
{
	width = p.x - start_point.x;
	height = p.y - start_point.y;
}

void Rectangulo_colision::set_fin_punto(float x, float y)
{
	width = x - start_point.x;
	height = y - start_point.y;
}

bool Rectangulo_colision::es_colision(float x, float y)
{
	punto2D end_point = get_fin_punto();
	end_point.x -= start_point.x;
	end_point.y -= start_point.y;

	punto2D point;
	point.x = x - start_point.x;
	point.y = y - start_point.y;

	if ((end_point.x > point.x && end_point.y > point.y) && (0 < point.x && 0 < point.y))
		return true;
	else
		return false;
}

Rectangulo_colision::~Rectangulo_colision() {}