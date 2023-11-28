#include "Dibujar_texto.h"

extern const char* dir_font;

Dibujar_texto::Dibujar_texto(RenderWindow* w) : window(w)
{
	font.loadFromFile(dir_font);
	text.setFont(font);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(20);
}

void Dibujar_texto::set_text(string ss) { text.setString(ss); }
string Dibujar_texto::get_text() const { return text.getString(); }
int Dibujar_texto::get_size() const { return text.getCharacterSize(); }

void Dibujar_texto::set_color(sf::Color c) { text.setFillColor(c); }
void Dibujar_texto::set_size(int n) { text.setCharacterSize(n); }

Dibujar_texto::~Dibujar_texto() {}

void Dibujar_texto::draw() { window->draw(text); }

void Dibujar_texto::set_pocision(int x, int y) { text.setPosition(x, y); }