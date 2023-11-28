#pragma once

#include "Energia.h"
#include "Bola_colision.h"
#include "Dibujar_recta.h"

class Bola_forma : public Dibujar_recta, public Bola_colision
{
private:
	Energia energy;	

	bool is_select;
	bool is_set_direct_;	

	int delay;		
	Clock timer;	

	float dx, dy;	
public:
	Bola_forma();
	Bola_forma(RenderWindow* w);

	float get_energia() const;
	float get_angulo_energia() const;

	void set_energia(float n);
	void set_angulo_energia(float n);

	void seleccionar_bola(bool b = true);
	void deseleccionar_bola();

	void set_textura(Texture* t);
	
	void set_pocision_forma(float x, float y);
	punto2D get_pocision_forma() const;

	bool is_set_directo() const;
	bool is_seleccionado() const;

	void events_obj();
	void draw();
	void actualizar(float time);

	~Bola_forma();
};