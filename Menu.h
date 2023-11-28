#pragma once

#include <vector>
#include "Dibujar_texto.h"
#include "Button.h"
#include <SFML/Audio.hpp>

using namespace std;

extern const char* dir_ball_s;

class Menu
{
	vector<Button* > buttons;		
	vector<Dibujar_texto* > textes;		

	Dibujar_recta* background;			
	Texture* t_background;			

	RenderWindow* window;
	int this_state;					
public:
	Menu(RenderWindow* w);

	void add_Boton(Button* b);			
	void add_Dibujar_texto(Dibujar_texto* t);	

	void set_background(Texture* t);	
	
	int get_salto_estado() const;
	void set_salto_estado(int n);

	int events_obj();
	void draw();

	~Menu();
};