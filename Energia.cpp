#include "Energia.h"
#include <cmath>

#include <iostream>

using namespace BillarSFML;

Energia::Energia() : Vector_rad() {}
Energia::Energia(float e, float a) : Vector_rad(a, e) {}

float Energia::get_velocidad() const { return sqrt(get_value()); }

void Energia::set_velocidad(float n) { set_value(n*n); }

void Energia::actualizar(float time)
{
	float val = get_value();

	if (val == 0)
		return;

	if (val > 0.1)
		val -= val*0.01;
	else
		val -= val*0.008;


	if (val < 0.001)
		set_value(0);
	else
		set_value(val);
}

Energia::~Energia(){}