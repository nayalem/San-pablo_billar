#pragma once

#include "Rectangulo_colision.h"
#include "Dibujar_recta.h"

class Rectangulo_forma : public Dibujar_recta, public Rectangulo_colision
{
public:
	Rectangulo_forma();
	Rectangulo_forma(RenderWindow* w);

	void set_textura(Texture* t);
	void set_pocision_forma(float x, float y);
	punto2D get_pocision_forma() const;

	void events_obj();
	void draw();
	void actualizar(float time);

	~Rectangulo_forma();
};