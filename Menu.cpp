#include "Menu.h"

Menu::Menu(RenderWindow* w) : window(w) {	background = new Dibujar_recta(w); }

void Menu::add_Boton(Button* b) { buttons.push_back(b); }
void Menu::add_Dibujar_texto(Dibujar_texto* t) { textes.push_back(t); }

int Menu::events_obj()
{
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		for (auto it = buttons.begin(); it != buttons.end(); it++)
		{
			if ((*it)->es_colision(Mouse::getPosition().x - window->getPosition().x, Mouse::getPosition().y - window->getPosition().y))
			{
				return (*it)->get_salto();
			}
		}
	}

	return this_state;
}

void Menu::draw()
{
	background->draw();

	for (auto it = buttons.begin(); it != buttons.end(); it++)
	{
		if (*it)
			(*it)->draw();
	}

	for (auto it = textes.begin(); it != textes.end(); it++)
	{
		if (*it)
			(*it)->draw();
	}
}

Menu::~Menu()
{
	for (auto it = buttons.begin(); it != buttons.end(); it++)
	{
		if (*it)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}

	for (auto it = textes.begin(); it != textes.end(); it++)
	{
		if (*it)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}

	if (background)
	{
		delete background;
		background = nullptr;
	}

	if (t_background)
	{
		delete t_background;
		t_background = nullptr;
	}
}

void Menu::set_background(Texture* t)
{
	t_background = t;
	background->set_textura(t);
}

int Menu::get_salto_estado() const { return this_state; }
void Menu::set_salto_estado(int n) { this_state = n; }