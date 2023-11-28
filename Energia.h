#pragma once

#include "Vector_rad.h"

class Energia : public BillarSFML::Vector_rad
{
public:
	Energia();
	Energia(float e, float a = 0);

	float get_velocidad() const;
	void set_velocidad(float n);
	
	void actualizar(float time);

	~Energia();
};
