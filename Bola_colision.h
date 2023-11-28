#pragma once

#include "formas_colision.h"
#include "Point2D.h"

class Bola_colision : public formas_colision
{
	punto2D pos;
	float size;			

public:
	Bola_colision();

	void set_pos(float x, float y);
	void set_size(float n);

	float get_pos_x() const;
	float get_pos_y() const;
	punto2D get_pos() const;
	float get_size() const;

	bool es_colision(float x, float y);

	~Bola_colision();
};