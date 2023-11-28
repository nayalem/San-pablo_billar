#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "Juego_Billar.h"

using namespace sf;
using namespace std;

class MyWindow
{
	RenderWindow* window;

	int state;			

	Menu* menu;			
	Menu* additional_window;		
	Juego_Billar* game;			

	Image* icon;		

	void crear_menu_principal();
	void borrar_menu_principal();

	void crear_juego();
	void borrar_juego();

	void crear_intro();
	void borrar_intro();

	void crear_fin_juego();
	void borrar_fin_juego();

public:
	MyWindow(VideoMode mode, string name);

	RenderWindow* get_window() const;
	void event_handling();

	~MyWindow();
};
