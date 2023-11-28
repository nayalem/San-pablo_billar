#pragma once

#include <SFML/Graphics.hpp>
#include "Point2D.h"

using namespace sf;

class Dibujar_recta
{
private:
	Sprite sprite;

	RenderWindow* window;

public:
	Dibujar_recta();
	Dibujar_recta(RenderWindow* w);

	RenderWindow* get_RenderWindow() const;
	punto2D get_pocision_dibujada_recta() const;
	float get_rotar_angulo() const;

	float get_alto() const;
	float get_ancho() const;

	void set_window(RenderWindow* w);
	void set_pocision_dibujada_recta(int x, int y);
	void set_rotar_angulo(float angle);
	void set_textura(Texture* t);

	void move(float x, float y);
	void draw();

	~Dibujar_recta();
};