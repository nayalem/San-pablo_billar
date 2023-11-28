#include "MyWindow.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>

#include "Dibujar_texto.h"
#include "Button.h"

#include "convert_string.h"

using namespace sf;
using namespace std;

extern const int window_width;
extern const int window_height;

extern const char* direccion_boton;

MyWindow::MyWindow(VideoMode mode, string name) : state(0)
{
	window = new RenderWindow(mode, name);
	window->setFramerateLimit(144);
}

RenderWindow* MyWindow::get_window() const { return window; }

MyWindow::~MyWindow()
{
	if (window)
		delete window;

	if (icon)
		delete icon;
}

void MyWindow::event_handling()
{
	Clock timer;
	float time = 0;

	sf::Event event;
	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window->close();
		}

		switch (state)
		{
		case 0:
		{
			crear_intro();
			
			state = 1;
			break;
		}

		case 1:
		{
			if (time > 5000)
				state = 2;

			time += timer.getElapsedTime().asMilliseconds();
			timer.restart();
			
			menu->draw();

			if (state == 2)
			{
				borrar_intro();
				crear_menu_principal();
			}

			break;
		}

		case 2:
		{
			window->clear();
			state = menu->events_obj();
			menu->draw();

			if (state == 3)
			{
				borrar_menu_principal();
				crear_juego();
			}

			break;
		}

		case 3:			
		{
			time = timer.getElapsedTime().asMicroseconds();
			timer.restart();
			time /= 800;

			state = game->event_obj();

			game->actualizar(time);

			game->draw();

			if (state == 2)
			{
				crear_menu_principal();
				borrar_juego();
			}
			
			if (state == 7)
			{
				crear_fin_juego();
			}
			break;
		}
		
		case 6:
		{
			borrar_menu_principal();
			borrar_juego();
			
			window->close();
			break;
		}

		case 7:
		{
			time = timer.getElapsedTime().asMicroseconds();
			timer.restart();
			time /= 800;

			game->actualizar(time);

			game->draw();

			state = additional_window->events_obj();
			additional_window->draw();

			if (state == 2)
			{
				borrar_fin_juego();
				borrar_juego();

				crear_menu_principal();
			}

			break;
		}

		}

		window->display();
	}
}

void MyWindow::crear_intro()
{
	menu = new Menu(window);
	menu->set_salto_estado(1);

	Dibujar_texto* t1 = new Dibujar_texto(window);
	t1->set_color(sf::Color::White);
	t1->set_size(72);
	t1->set_text("Juego de Billar 1 jugador");
	t1->set_pocision(540, 140);
	menu->add_Dibujar_texto(t1);

	t1 = new Dibujar_texto(window);
	t1->set_color(sf::Color::White);
	t1->set_size(40);
	t1->set_text("");
	t1->set_pocision(330, 220);
	menu->add_Dibujar_texto(t1);

	t1 = new Dibujar_texto(window);
	t1->set_color(sf::Color::White);
	t1->set_size(40);
	t1->set_text("2023");
	t1->set_pocision(600, 660);
	menu->add_Dibujar_texto(t1);
}

void MyWindow::borrar_intro()
{
	if (menu)
		delete menu;
	menu = nullptr;
}

void MyWindow::crear_menu_principal()
{
	menu = new Menu(window);
	menu->set_salto_estado(2);

	Texture* t_button = new Texture;
	t_button->loadFromFile(direccion_boton);
	Button* b = new Button(window);
	b->set_textura(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("Nuevo Juego");
	b->set_pocision(650, 150);
	b->set_salto(3);
	menu->add_Boton(b);


	t_button = new Texture;
	t_button->loadFromFile(direccion_boton);
	b = new Button(window);
	b->set_textura(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("Cerrar");
	b->set_pocision(650, 510);
	b->set_salto(6);
	menu->add_Boton(b);
}

void MyWindow::borrar_menu_principal()
{
	if (menu)
		delete menu;
	menu = nullptr;
}

void MyWindow::crear_juego()
{
	game = new Juego_Billar(window);
	game->start();
}

void MyWindow::borrar_juego()
{
	if (game)
		delete game;
	game = nullptr;

}

void MyWindow::crear_fin_juego()
{
	additional_window = new Menu(window);
	additional_window->set_salto_estado(7);

	Texture* texture = new Texture;
	additional_window->set_background(texture);

	int moves = game->get_movimientos();
	int failures = game->get_fallos();
	int fines = game->get_multas();

	Dibujar_texto* t = new Dibujar_texto(window);
	t->set_color(sf::Color::White);
	t->set_size(72);
	t->set_text("Juego Finalizado");
	t->set_pocision(530, 60);
	additional_window->add_Dibujar_texto(t);

	t = new Dibujar_texto(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	t->set_text("Numero de movimientos:");
	t->set_pocision(150, 180);
	additional_window->add_Dibujar_texto(t);

	t = new Dibujar_texto(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	t->set_text("Numero de fallos:");
	t->set_pocision(150, 250);
	additional_window->add_Dibujar_texto(t);

	t = new Dibujar_texto(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	t->set_text("Numero de bolas encajadas:");
	t->set_pocision(150, 320);
	additional_window->add_Dibujar_texto(t);

	t = new Dibujar_texto(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	string ss;
	ss = BillarSFML::convert_string::to_string<int>(moves);
	t->set_text(ss);
	t->set_pocision(850, 180);
	additional_window->add_Dibujar_texto(t);

	t = new Dibujar_texto(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	ss = BillarSFML::convert_string::to_string<int>(failures);
	t->set_text(ss);
	t->set_pocision(850, 250);
	additional_window->add_Dibujar_texto(t);

	t = new Dibujar_texto(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	ss = BillarSFML::convert_string::to_string<int>(fines);
	t->set_text(ss);
	t->set_pocision(850, 320);
	additional_window->add_Dibujar_texto(t);

	Texture* t_button = new Texture;
	t_button->loadFromFile(direccion_boton);
	Button* b = new Button(window);
	b->set_textura(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("OK");
	b->set_pocision(650, 625);
	b->set_salto(2);
	additional_window->add_Boton(b);
}

void MyWindow::borrar_fin_juego()
{
	if (additional_window)
		delete additional_window;
	additional_window = nullptr;
}