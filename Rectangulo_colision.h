#pragma once

#include "formas_colision.h"
#include "Point2D.h"

class Rectangulo_colision : public formas_colision
{
private:
	punto2D start_point;
	float width;
	float height;

public:
	Rectangulo_colision();
	Rectangulo_colision(punto2D p, float w, float h);

	float get_ancho() const;
	float get_alto() const;

	punto2D get_inicio_punto() const;
	punto2D get_fin_punto() const;

	void set_width(float n);
	void set_height(float n);
	void set_inicio_punto(punto2D p);
	void set_inicio_punto(float x, float y);
	void set_fin_punto(punto2D p);
	void set_fin_punto(float x, float y);

	bool es_colision(float x, float y);

	~Rectangulo_colision();
};