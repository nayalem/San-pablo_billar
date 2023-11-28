#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class Dibujar_texto
{
	Font font;		
	Text text;		

	RenderWindow* window;	

public:
	Dibujar_texto(RenderWindow* w);
	
	void set_text(string ss);
	void set_color(sf::Color c);
	void set_size(int n);

	string get_text() const;
	int get_size() const;

	void draw();

	~Dibujar_texto();

	void set_pocision(int x, int y);
};