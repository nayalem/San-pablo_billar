#include "Button.h"

Button::Button(RenderWindow* w) : Rectangulo_forma(w), Dibujar_texto(w), saltar_a_estado(0) {}

void Button::set_pocision(int x, int y)
{
	float height = Dibujar_recta::get_alto();
	float width = Dibujar_recta::get_ancho();

	float t_h = Dibujar_texto::get_size();
	float t_w = t_h / 1.2;

	string ss = Dibujar_texto::get_text();

	Dibujar_recta::set_pocision_dibujada_recta(x - width / 2, y - height / 2);
	Rectangulo_forma::set_inicio_punto(x - width / 2, y - height*0.1);
	Rectangulo_forma::set_height(height);
	Rectangulo_forma::set_width(width);
	Dibujar_texto::set_pocision(x - t_w / 2 * (ss.size() + 1) / 2, y - t_h / 4 * 3);
}

void Button::set_pocision(punto2D p) { set_pocision(p.x, p.y); }

punto2D Button::get_pocision() const 
{ 
	float height = Dibujar_recta::get_alto();
	float width = Dibujar_recta::get_ancho();

	punto2D r;
	r.x = Dibujar_recta::get_pocision_dibujada_recta().x + width / 2;
	r.y = Dibujar_recta::get_pocision_dibujada_recta().y + height / 2;

	return r;
}

void Button::set_textura(Texture* t) 
{ 
	Dibujar_recta::set_textura(t); 
	texture = t; 
}

void Button::draw()
{
	Rectangulo_forma::draw();
	Dibujar_texto::draw();
}

int Button::get_salto() const { return saltar_a_estado; }

Button::~Button() 
{ 
	if (texture)
	{
		delete texture;
		texture = nullptr;
	}
}

int Button::get_alto() const { return Dibujar_recta::get_alto(); }
int Button::get_ancho() const {	return Dibujar_recta::get_ancho(); }

bool Button::es_colision(float x, float y) { return Rectangulo_forma::es_colision(x, y); }

void Button::set_salto(int n) { saltar_a_estado = n; }