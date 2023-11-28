#pragma once

#include "Point2D.h"
#include "Dibujar_recta.h"
#include "Bola_forma.h"
#include "Juego_Billar.h"
#include <vector>

class Seleciona_bola
{
	Bola_forma* s;				
	Dibujar_recta* cirkle;			
	Texture* texture_cirkle;	
	RenderWindow* window;		

	bool block;					

	Clock timer;				
	int delay;					

public:
	Seleciona_bola(RenderWindow* w);

	Bola_forma* get_Bola() const;

	void event_obj(bool is_end_move);
	void actualizar(std::vector<Bola_forma*> arr, bool is_end_move);
	void draw();

	void remove_posicion();

	friend class Juego_Billar;

	~Seleciona_bola();
};