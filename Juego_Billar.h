#pragma once

#include <vector>
#include "Bola_forma.h"
#include "Rectangulo_colision.h"
#include "Seleciona_bola.h"
#include "Line.h"
#include "Dibujar_recta.h"
#include "Rectangulo_forma.h"
#include "Dibujar_texto.h"
#include <SFML/Audio.hpp>

using namespace std;

class Seleciona_bola;

class Juego_Billar
{
	vector<Bola_forma*> balls;
	vector<BillarSFML::Line*> borders;
	vector<Bola_colision*> holes;

	vector<Texture*> textures;

	Seleciona_bola* selected;
	Dibujar_recta* board;
	Dibujar_recta* billiard_cue;

	Rectangulo_forma* power_bar;
	Dibujar_recta* select_rect;

	Dibujar_texto* info_moves;
	Dibujar_texto* info_error;
	bool draw_error;

	RenderWindow* window;

	bool colision_bola_bola(Bola_forma* a, Bola_forma* b);
	bool colision_bola_linea(Bola_forma* a, BillarSFML::Line* b);
	bool colision_bola_agujero(Bola_forma* a, Bola_colision* b);

	bool correctas_bolas_en_linea(Bola_forma* a, BillarSFML::Line* b);

	void init_bordes();
	void init_agujeros();

	float power_value;
	
	int move_number;
	int fines_number;
	int failures_number;

	
	bool is_set_power;				
	bool show_select_rect;			
	bool is_end_move;				
	bool is_collision_balls;		
	bool is_set_penalty;			
	bool increment_move_number;		
	bool is_end_game;				

	punto2D old_pos;				

	int this_state;					
public:
	Juego_Billar(RenderWindow* w);

	void start();

	int event_obj();
	void actualizar(float time);
	void draw();

	bool fin_juego() const;

	int get_movimientos() const;
	int get_multas() const;
	int get_fallos() const;

	~Juego_Billar();

	const int MAX_NUMBER_OF_BALLS = 16;
	
};