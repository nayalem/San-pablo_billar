#include "Seleciona_bola.h"
#include <vector>

extern const char* dir_select_ball;

Seleciona_bola::Seleciona_bola(RenderWindow* w) : s(nullptr), window(w), block(false), delay(0)
{
	cirkle = new Dibujar_recta(w);

	texture_cirkle = new Texture;
	texture_cirkle->loadFromFile(dir_select_ball);
	cirkle->set_textura(texture_cirkle);
}

void Seleciona_bola::event_obj(bool is_end_move) 
{
	if (delay < 500)
	{
		int t;
		t = timer.getElapsedTime().asMilliseconds();
		timer.restart();
		delay += t;
		return;
	}

	if (!is_end_move)
		return;

	if (s && block)
	{
		if (s->get_size() != 16)
		{
			block = false;

			timer.restart();
			delay = 0;
		}
	}
	else if (!s)
	{
		block = false;

		timer.restart();
		delay = 0;
	}

	if (Mouse::isButtonPressed(Mouse::Button::Left) && s && (block == false))
	{
		s->seleccionar_bola();
		block = true;

		timer.restart();
		delay = 0;
	}
	
	if (Mouse::isButtonPressed(Mouse::Button::Right) && block)
	{
		if (s)
			if (!s->is_set_directo())
			{
				s->deseleccionar_bola();
				block = false;

				timer.restart();
				delay = 0;
			}
	}

}

void Seleciona_bola::actualizar(vector<Bola_forma*> arr, bool is_end_move)
{
	if (s)
		if (!s->is_seleccionado())
			block = false;

	if (!is_end_move)
		return;

	if (block == false)
	{
		Bola_forma* selected = nullptr;
		for (auto it = arr.begin(); it != arr.end(); it++)
		{
			if (*it == nullptr)
				continue;

			auto mouse = Mouse::getPosition();
			auto pos_window = window->getPosition();

			mouse.x -= pos_window.x;
			mouse.y -= pos_window.y;

			if ((*it)->get_size() != 16)
				continue;

			bool coll = (*it)->es_colision(mouse.x, mouse.y);
			if (coll)
				selected = (*it);
		}

		if (selected == nullptr)
		{
			s = nullptr;
			remove_posicion();
			return;
		}

		s = selected;
		cirkle->set_pocision_dibujada_recta(s->get_pos().x - cirkle->get_ancho() / 2, s->get_pos().y - cirkle->get_alto() / 2);
	}
}

void Seleciona_bola::draw()
{
	if (cirkle)
		if(!((cirkle->get_pocision_dibujada_recta().x == -100) && (cirkle->get_pocision_dibujada_recta().y == -100)))
			cirkle->draw();
}

Seleciona_bola::~Seleciona_bola()
{
	if (cirkle)
		delete cirkle;
	if (texture_cirkle)
		delete texture_cirkle;
}

Bola_forma* Seleciona_bola::get_Bola() const { return s; }

void Seleciona_bola::remove_posicion() { cirkle->set_pocision_dibujada_recta(-100, -100); }