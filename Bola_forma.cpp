#include "Bola_forma.h"
#include <iostream>

const float PI = 3.14159265;

Bola_forma::Bola_forma() : Dibujar_recta(), Bola_colision(), energy() 
{
	set_size(16);
	is_set_direct_ = false;
	is_select = false;
	set_pocision_forma(0, 0);
}
Bola_forma::Bola_forma(RenderWindow* w) : Dibujar_recta(w), Bola_colision(), energy() 
{
	set_size(16);
	is_set_direct_ = false;
	is_select = false;
	set_pocision_forma(0, 0);
}

void Bola_forma::set_energia(float n) { energy.set_value(abs(n)); }
void Bola_forma::set_angulo_energia(float n) { energy.set_angulo(n); }
void Bola_forma::set_textura(Texture* t) { Dibujar_recta::set_textura(t); }

void Bola_forma::set_pocision_forma(float x, float y)
{
	set_pos(x, y);

	float h, w;
	h = get_alto();
	w = get_ancho();
	
	Dibujar_recta::set_pocision_dibujada_recta(x - (w / 2), y - (h / 2));
}

punto2D Bola_forma::get_pocision_forma() const { return get_pos(); }

void Bola_forma::events_obj()
{
	if (delay < 200)
	{
		int t;
		t = timer.getElapsedTime().asMilliseconds();
		timer.restart();
		delay += t;
		return;
	}

	if (Mouse::isButtonPressed(Mouse::Button::Left) && is_select && !is_set_direct_)
	{
		Vector2i pos_mouse = Mouse::getPosition();
		Vector2i pos_wind = get_RenderWindow()->getPosition();
		pos_mouse.x -= pos_wind.x;
		pos_mouse.y -= pos_wind.y;

		punto2D pos_ball;
		pos_ball.x = get_pos_x() - pos_mouse.x;
		pos_ball.y = get_pos_y() + 40 - pos_mouse.y;

		float size_vec = sqrt(pos_ball.x*pos_ball.x + pos_ball.y*pos_ball.y);

		float cos = pos_ball.x / size_vec;
		float sin = pos_ball.y / size_vec;

		float a = BillarSFML::get_angulo(sin, cos);
		energy.set_angulo(a);

		is_set_direct_ = true;

		timer.restart();
		delay = 0;
	}

	if (Mouse::isButtonPressed(Mouse::Button::Right) && is_select && is_set_direct_)
	{
		is_set_direct_ = false;

		timer.restart();
		delay = 0;
	}
}

void Bola_forma::draw() { Dibujar_recta::draw(); }

void Bola_forma::actualizar(float time)
{
	if (!(int(energy.get_value() * 10000)))
		return;
	dx = energy.get_velocidad()*cos(energy.get_angulo()) * time;
	dy = energy.get_velocidad()*sin(energy.get_angulo()) * time;

	punto2D pos_shape = get_pocision_forma();
	pos_shape.x += dx;
	pos_shape.y += dy;
	set_pocision_forma(pos_shape.x, pos_shape.y);

	energy.actualizar(time);
}

Bola_forma::~Bola_forma() {}

float Bola_forma::get_energia() const { return energy.get_value(); }
float Bola_forma::get_angulo_energia() const { return energy.get_angulo(); }

void Bola_forma::seleccionar_bola(bool b) 
{
	if(this)
		is_select = b; 

	delay = 0;
	timer.restart();
}

void Bola_forma::deseleccionar_bola() 
{ 
	if (this->get_size() == 16)
	{
		is_select = false;
		is_set_direct_ = false;
	}


	delay = 0;
	timer.restart();
}

bool Bola_forma::is_set_directo() const { return is_set_direct_; }
bool Bola_forma::is_seleccionado() const { return is_select; }