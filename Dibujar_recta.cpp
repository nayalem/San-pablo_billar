#include "Dibujar_recta.h"

using namespace std;

Dibujar_recta::Dibujar_recta() : window(nullptr) {}
Dibujar_recta::Dibujar_recta(RenderWindow* w) : window(w) {}

RenderWindow* Dibujar_recta::get_RenderWindow() const { return window; }

punto2D Dibujar_recta::get_pocision_dibujada_recta() const 
{ 
	Vector2f a = sprite.getPosition(); 
	punto2D res;
	res.x = a.x;
	res.y = a.y;

	return res;
}

float Dibujar_recta::get_alto() const
{
	IntRect a = sprite.getTextureRect();
	return a.height;
}
float Dibujar_recta::get_ancho() const
{
	IntRect a = sprite.getTextureRect();
	return a.width;
}

float Dibujar_recta::get_rotar_angulo() const { return sprite.getRotation(); }

void Dibujar_recta::set_window(RenderWindow* w) { window = w; }
void Dibujar_recta::set_pocision_dibujada_recta(int x, int y) { sprite.setPosition(x, y); }

void Dibujar_recta::set_rotar_angulo(float angle) { sprite.rotate(angle); }
void Dibujar_recta::set_textura(Texture* t) { sprite.setTexture((*t)); }

void Dibujar_recta::move(float x, float y) { sprite.move(x, y); }
void Dibujar_recta::draw() { window->draw(sprite); }

Dibujar_recta::~Dibujar_recta() {}