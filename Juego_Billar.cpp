#include "Juego_Billar.h"
#include "Vector_rad.h"
#include "Ecuacion.h"
#include <cmath>

#include <iostream>

#include "convert_string.h"

using namespace std;

const float PI = 3.14159265;
const int SIZE_OF_BALL = 16;
const float COS_OR_SIN_45 = 0.707106;

const float MIN_POWER = 0;
const float MAX_POWER = 1;

extern const char* dir_error_s;
extern const char* dir_ball_s;
extern const char* dir_border_s;
extern const char* dir_cue_s;
extern const char* dir_hole_s;

extern const char* dir_ball;
extern const char* dir_cue;
extern const char* dir_select;
extern const char* dir_power;
extern const char* dir_board;

Juego_Billar::Juego_Billar(RenderWindow* w) : window(w), board(nullptr), 
	is_end_move(true), move_number(0), is_end_game(false), this_state(3),
	draw_error(false)
{
	
}

Juego_Billar::~Juego_Billar()
{
	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		if (*it)
			delete *it;
	}

	if (board)
		delete board;

	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if (*it)
			delete *it;
	}

	for (auto it = borders.begin(); it != borders.end(); it++)
	{
		if (*it)
			delete *it;
	}

	for (auto it = holes.begin(); it != holes.end(); it++)
		if (*it)
			delete *it;

	if (selected)
		delete selected;

	if (power_bar)
		delete power_bar;

	if (select_rect)
		delete select_rect;

	if (billiard_cue)
		delete billiard_cue;

	if (info_error)
		delete info_error;

	if (info_moves)
		delete info_moves;
}

void Juego_Billar::start()
{
	Texture* texture_board = new Texture;
	texture_board->loadFromFile(dir_board);
	textures.push_back(texture_board);

	board = new Dibujar_recta();
	board->set_window(window);
	board->set_textura(texture_board);
	board->set_pocision_dibujada_recta(window->getSize().x - 1200, window->getSize().y - 670);

	Texture* texture_power = new Texture;
	texture_power->loadFromFile(dir_power);
	textures.push_back(texture_power);

	power_bar = new Rectangulo_forma(window);
	power_bar->set_textura(texture_power);
	power_bar->set_pocision_forma(board->get_pocision_dibujada_recta().x - 100, board->get_pocision_dibujada_recta().y);

	Texture* t_select_rect = new Texture;
	t_select_rect->loadFromFile(dir_select);
	textures.push_back(t_select_rect);

	select_rect = new Dibujar_recta(window);
	select_rect->set_textura(t_select_rect);
	select_rect->set_pocision_dibujada_recta(board->get_pocision_dibujada_recta().x - 100, board->get_pocision_dibujada_recta().y);
	show_select_rect = false;

	Texture* t_cue = new Texture;
	t_cue->loadFromFile(dir_cue);
	textures.push_back(t_cue);

	billiard_cue = new Dibujar_recta(window);
	billiard_cue->set_textura(t_cue);
	billiard_cue->set_pocision_dibujada_recta(-1000, -1000);

	Texture* texture_ball = new Texture;
	texture_ball->loadFromFile(dir_ball);
	textures.push_back(texture_ball);

	for (int i = 0; i < MAX_NUMBER_OF_BALLS; i++)
	{
		Bola_forma* b = new Bola_forma(window);
		b->set_textura(texture_ball);

		balls.push_back(b);
	}
	int h = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			balls.at(h)->set_pocision_forma(board->get_pocision_dibujada_recta().x + 800 + (28 + 1) * i, board->get_pocision_dibujada_recta().y + 335 + (16 + 1) * i - (32 + 1) * j);
			h++;

			if (h >= MAX_NUMBER_OF_BALLS)
				break;
		}
		if (h >= MAX_NUMBER_OF_BALLS)
			break;
	}

	if (h == MAX_NUMBER_OF_BALLS - 1)
	{
		balls.at(h)->set_pocision_forma(board->get_pocision_dibujada_recta().x + 300, 335 + board->get_pocision_dibujada_recta().y);
	}

	init_bordes();
	init_agujeros();
	selected = new Seleciona_bola(window);

	info_moves = new Dibujar_texto(window);
	info_moves->set_color(sf::Color::Black);
	info_moves->set_size(40);
	string ss = "Numero de intentos: ";
	ss += BillarSFML::convert_string::to_string<int>(move_number);
	info_moves->set_text(ss);
	info_moves->set_pocision(100, 50);

	info_error = new Dibujar_texto(window);
	info_error->set_color(sf::Color::Red);
	info_error->set_size(40);
	info_error->set_text("la bola no toco las otras bolas");
	info_error->set_pocision(650, 50);
}

int Juego_Billar::event_obj()
{
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if(*it)
			(*it)->events_obj();
	}

	selected->event_obj(is_end_move);

	power_bar->events_obj();

	auto abs_pos_mouse = Mouse::getPosition();
	punto2D pos_mouse_on_rect;
	pos_mouse_on_rect.x = abs_pos_mouse.x - window->getPosition().x - power_bar->get_pocision_forma().x - 8;
	pos_mouse_on_rect.y = abs_pos_mouse.y - window->getPosition().y - power_bar->get_pocision_forma().y - 40;
	
	if (Mouse::isButtonPressed(Mouse::Button::Left) && 
		(pos_mouse_on_rect.x >= 0 && pos_mouse_on_rect.x < power_bar->Rectangulo_colision::get_ancho()) && 
		(pos_mouse_on_rect.y >= 0 && pos_mouse_on_rect.y < power_bar->Rectangulo_colision::get_alto()))
	{
		if (selected->get_Bola())
		{
			if (selected->get_Bola()->get_size() == 16)
			{
				float power = (1 - pos_mouse_on_rect.y / abs(power_bar->Rectangulo_colision::get_alto() - 0)) * abs(MIN_POWER - MAX_POWER);
				power_value = power;
				is_set_power = true;
				show_select_rect = true;
				select_rect->set_pocision_dibujada_recta(select_rect->get_pocision_dibujada_recta().x, pos_mouse_on_rect.y + power_bar->get_pocision_forma().y + 2);
			}
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Space) && is_set_power)
	{
		is_set_power = false;

		if (selected->get_Bola())
		{
			if (selected->get_Bola()->get_size() == 16)
			{
				selected->get_Bola()->set_energia(power_value);

				selected->get_Bola()->deseleccionar_bola();
				selected->block = false;

				old_pos = selected->get_Bola()->get_pocision_forma();

				selected->s = nullptr;
				selected->remove_posicion();

				billiard_cue->set_pocision_dibujada_recta(-1000, -1000);
			}
		}

		show_select_rect = false;
	}

	if (is_end_game)
		return 7;

	if(Keyboard::isKeyPressed(Keyboard::Key::Escape))
		return 2;

	return this_state;
}

void Juego_Billar::actualizar(float time)
{
	string ss = "N° Intentos: ";
	ss += BillarSFML::convert_string::to_string<int>(move_number);
	ss += "   ";
	ss += "N° Fallos: ";
	ss += BillarSFML::convert_string::to_string<int>(fines_number);
	info_moves->set_text(ss);

	if (balls.size() < 2)
		is_end_game = true;

	for (auto it = balls.begin(); it != balls.end(); it++)
		if (!(*it))
			balls.erase(it);

	if (!selected->get_Bola())
		billiard_cue->set_pocision_dibujada_recta(-1000, -1000);

	if (increment_move_number && is_end_move)
	{
		if (is_collision_balls) 
			failures_number++;
		else
			is_set_penalty = true;

		if (is_set_penalty)
		{
			fines_number++;
			draw_error = true;
		}
		else
			draw_error = false;

		move_number++;
	}
		
	
	if (is_end_move)
	{
		increment_move_number = false;
		is_set_penalty = false;
		is_collision_balls = false;
	}
	else
		increment_move_number = true;

	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		for (auto jt = it + 1; jt != balls.end(); jt++)
		{
			bool b = colision_bola_bola((*it), (*jt));
			if (b)
			{
				is_collision_balls = b;

				if ((*it)->get_energia() == 0.0)
					(*it)->set_energia(0.01);

			}
		}

		for (auto jt = borders.begin(); jt != borders.end(); jt++)
		{
			if (colision_bola_linea((*it), (*jt)))
			{
				correctas_bolas_en_linea((*it), (*jt));
				break;
			}
		}

		for (auto jt = holes.begin(); jt != holes.end(); jt++)
		{
			if (colision_bola_agujero(*it, *jt))
			{
				if (!is_collision_balls)
				{
					(*it)->set_pocision_forma(old_pos.x, old_pos.y);
					(*it)->set_energia(0);
				}
				else
				{
					auto t = *it;

					it = balls.erase(it);
					delete t;

					failures_number--;

					if (it == balls.end())
						break;
				}
			}
		}
		if (it == balls.end())
			break;

	}

	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if (*it)
			(*it)->actualizar(time);
	}

	bool temp = true;
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if ((*it)->get_energia() != 0)
		{
			is_end_move = false;
			temp = false;
			break;
		}
	}
	if (temp)
		is_end_move = true;

	if (increment_move_number && is_end_move && !is_collision_balls && balls.size() < MAX_NUMBER_OF_BALLS)
	{
		bool ok = false;
		Bola_forma* sh = new Bola_forma(window);
		sh->set_textura(textures.at(4));

		while (!ok)
		{
			int dx, dy;
			srand(::time(NULL));

			dx = board->get_pocision_dibujada_recta().x + 86 + rand() % 1027;
			dy = board->get_pocision_dibujada_recta().y + 86 + rand() % 498;

			sh->set_pocision_forma(dx, dy);

			bool b = false;
			for (auto it = balls.begin(); it != balls.end(); it++)
			{
				b = colision_bola_bola(sh, (*it));
				if (b)
				{
					ok = false;
					break;
				}
			}

			if (!b)
				ok = true;
		}

		balls.push_back(sh);
	}

	power_bar->actualizar(time);

	selected->actualizar(balls, is_end_move);

	if (selected->get_Bola() && selected->get_Bola()->get_size() == 16 && selected->get_Bola()->is_seleccionado() && selected->block && !selected->get_Bola()->is_set_directo())
	{
		auto b = selected->get_Bola();

		Vector2i pos_mouse = Mouse::getPosition();
		Vector2i pos_wind = window->getPosition();
		pos_mouse.x -= pos_wind.x;
		pos_mouse.y -= pos_wind.y;

		punto2D pos_ball;
		pos_ball.x = b->get_pos_x() - pos_mouse.x;
		pos_ball.y = b->get_pos_y() + 16 + 24 - pos_mouse.y;

		float size_vec = sqrt(pos_ball.x*pos_ball.x + pos_ball.y*pos_ball.y);

		float cos_a = pos_ball.x / size_vec;
		float sin_a = pos_ball.y / size_vec;

		float a = BillarSFML::get_angulo(sin_a, cos_a);

		float old = billiard_cue->get_rotar_angulo();
		a = a * 180 / PI;
		a = a - old;
		a += 90;

		billiard_cue->set_rotar_angulo(a);


		float angle = a * PI / 180;
		punto2D sh;
		int pixel = 1;
		sh.x = sin(angle) * pixel;
		sh.y = cos(angle) * pixel;
		billiard_cue->set_pocision_dibujada_recta(selected->get_Bola()->get_pos_x() + sh.x, selected->get_Bola()->get_pos_y() + sh.y );
	}

	if (!selected->block)
		is_set_power = false;
}

bool Juego_Billar::correctas_bolas_en_linea(Bola_forma* a, BillarSFML::Line* b)
{
	if (!a)
		return false;

	switch (b->dir)
	{
	case BillarSFML::Line::direct::RIGHT:
	{
		while (a->es_colision(b->start.x, b->start.y) || a->es_colision(b->end.x, b->end.y))
		{
			auto pos = a->get_pocision_forma();
			pos.x++;
			a->set_pocision_forma(pos.x, pos.y);
		}
		auto pos = a->get_pocision_forma();
		pos.x++;
		a->set_pocision_forma(pos.x, pos.y);

		break;
	}

	case BillarSFML::Line::direct::LEFT:
	{
		while (a->es_colision(b->start.x, b->start.y) || a->es_colision(b->end.x, b->end.y))
		{
			auto pos = a->get_pocision_forma();
			pos.x--;
			a->set_pocision_forma(pos.x, pos.y);
		}
		auto pos = a->get_pocision_forma();
		pos.x--;
		a->set_pocision_forma(pos.x, pos.y);

		break;
	}

	case BillarSFML::Line::direct::DOWN:
	{
		while (a->es_colision(b->start.x, b->start.y) || a->es_colision(b->end.x, b->end.y))
		{
			auto pos = a->get_pocision_forma();
			pos.y++;
			a->set_pocision_forma(pos.x, pos.y);
		}
		auto pos = a->get_pocision_forma();
		pos.y++;
		a->set_pocision_forma(pos.x, pos.y);

		break;
	}
	case BillarSFML::Line::direct::UP:
	{
		while (a->es_colision(b->start.x, b->start.y) || a->es_colision(b->end.x, b->end.y))
		{
			auto pos = a->get_pocision_forma();
			pos.y--;
			a->set_pocision_forma(pos.x, pos.y);
		}

		auto pos = a->get_pocision_forma();
		pos.y--;
		a->set_pocision_forma(pos.x, pos.y);

		break;
	}

	default:
		break;
	}
	
	return true;
}

void Juego_Billar::draw()
{
	window->clear(sf::Color::White);
	
	board->draw();

	for (auto it = balls.begin(); it != balls.end(); it++)
	{	
		if (*it)
			(*it)->draw();
	}

	selected->draw();

	power_bar->draw();

	if (show_select_rect)
		select_rect->draw();

	if (selected->get_Bola() && selected->get_Bola()->get_size() == 16)
		billiard_cue->draw();

	info_moves->draw();
	if (draw_error)
		info_error->draw();
}

bool Juego_Billar::colision_bola_linea(Bola_forma* a, BillarSFML::Line* b)
{
	if (!a)
		return false;

	punto2D balls[4];
	for (int i = 0; i < 4; i++)
		balls[i] = a->get_pocision_forma();
	
	balls[0].x += SIZE_OF_BALL;
	balls[1].y += SIZE_OF_BALL;
	balls[2].x -= SIZE_OF_BALL;
	balls[3].y -= SIZE_OF_BALL;

	switch (b->dir)
	{
	case BillarSFML::Line::direct::RIGHT:
	{
		int i = 2;
		{
			if (b->start.x >= balls[i].x && (balls[i].y > b->start.y) && (balls[i].y < b->end.y))
			{
				float angle_border = BillarSFML::get_angulo_2_puntos(b->start, b->end);

				float new_angle = a->get_angulo_energia() + (angle_border - a->get_angulo_energia()) * 2;
				a->set_angulo_energia(new_angle);
				a->set_energia(a->get_energia() * 0.9);

				if (b->start.x > a->get_pocision_forma().x - SIZE_OF_BALL)
				{
					float temp = a->get_pocision_forma().x - SIZE_OF_BALL - b->start.x;
					a->set_pocision_forma(a->get_pocision_forma().x - temp, a->get_pocision_forma().y);
				}

				return true;
			}
		}
		break;
	}

	case BillarSFML::Line::direct::LEFT:
	{
		int i = 0;
		{
			if (b->start.x <= balls[i].x && (balls[i].y > b->start.y) && (balls[i].y < b->end.y))
			{
				float angle_border = BillarSFML::get_angulo_2_puntos(b->start, b->end);

				float new_angle = a->get_angulo_energia() + (angle_border - a->get_angulo_energia()) * 2;
				a->set_angulo_energia(new_angle);
				a->set_energia(a->get_energia() * 0.9);

				if (b->start.x > a->get_pocision_forma().x + SIZE_OF_BALL)
				{
					float temp = a->get_pocision_forma().x + SIZE_OF_BALL - b->start.x;
					a->set_pocision_forma(a->get_pocision_forma().x + temp, a->get_pocision_forma().y);
				}

				return true;
			}
		}
		break;
	}

	case BillarSFML::Line::direct::DOWN:
	{
		int i = 3;
		{
			if (b->start.y >= balls[i].y && (balls[i].x > b->start.x) && (balls[i].x < b->end.x))
			{
				float angle_border = BillarSFML::get_angulo_2_puntos(b->start, b->end);

				float new_angle = a->get_angulo_energia() + (angle_border - a->get_angulo_energia()) * 2;
				a->set_angulo_energia(new_angle);
				a->set_energia(a->get_energia() * 0.9);

				if (b->start.y > a->get_pocision_forma().y + SIZE_OF_BALL)
				{
					float temp = a->get_pocision_forma().y + SIZE_OF_BALL - b->start.y;
					a->set_pocision_forma(a->get_pocision_forma().x, a->get_pocision_forma().y + temp);
				}

				return true;
			}
		}
		break;
	}

	case BillarSFML::Line::direct::UP:
	{
		int i = 1;
		{
			if (b->start.y <= balls[i].y && (balls[i].x > b->start.x) && (balls[i].x < b->end.x))
			{
				float angle_border = BillarSFML::get_angulo_2_puntos(b->start, b->end);

				float new_angle = a->get_angulo_energia() + (angle_border - a->get_angulo_energia()) * 2;
				a->set_angulo_energia(new_angle);
				a->set_energia(a->get_energia() * 0.9);

				if (b->start.y > a->get_pocision_forma().y + SIZE_OF_BALL)
				{
					float temp = a->get_pocision_forma().y + SIZE_OF_BALL - b->start.y;
					a->set_pocision_forma(a->get_pocision_forma().x, a->get_pocision_forma().y + temp);
				}

				return true;
			}
		}
		break;
	}

	case BillarSFML::Line::direct::RIGHT_UP:
	{
		if (BillarSFML::is_colision_bola_linea(a, b))
		{
			float angle_border = BillarSFML::get_angulo_2_puntos(b->start, b->end);

			float new_angle = a->get_angulo_energia() + (angle_border - a->get_angulo_energia()) * 2;
			a->set_angulo_energia(new_angle);
			a->set_energia(a->get_energia() * 0.9);
		}

		break;
	}
	case BillarSFML::Line::direct::RIGHT_DOWN:
	{
		if (BillarSFML::is_colision_bola_linea(a, b))
		{
			float angle_border = BillarSFML::get_angulo_2_puntos(b->start, b->end);

			float new_angle = a->get_angulo_energia() + (angle_border - a->get_angulo_energia()) * 2;
			a->set_angulo_energia(new_angle);
			a->set_energia(a->get_energia() * 0.9);
		}

		break;
	}
	case BillarSFML::Line::direct::LEFT_UP:
	{
		if (BillarSFML::is_colision_bola_linea(a, b))
		{
			float angle_border = BillarSFML::get_angulo_2_puntos(b->start, b->end);

			float new_angle = a->get_angulo_energia() + (angle_border - a->get_angulo_energia()) * 2;
			a->set_angulo_energia(new_angle);
			a->set_energia(a->get_energia() * 0.9);
		}

		break;
	}
	case BillarSFML::Line::direct::LEFT_DOWN:
	{
		if (BillarSFML::is_colision_bola_linea(a, b))
		{
			float angle_border = BillarSFML::get_angulo_2_puntos(b->start, b->end);

			float new_angle = a->get_angulo_energia() + (angle_border - a->get_angulo_energia()) * 2;
			a->set_angulo_energia(new_angle);
			a->set_energia(a->get_energia() * 0.9);
		}

		break;
	}

	}

	return false;
}

bool Juego_Billar::colision_bola_agujero(Bola_forma* a, Bola_colision* b)
{
	if (!a)
		return false;

	if (b->es_colision(a->get_pocision_forma().x, a->get_pocision_forma().y))
	{
		return true;
	}
	
	return false;
}

bool Juego_Billar::colision_bola_bola(Bola_forma* a, Bola_forma* b)
{
	punto2D ball_a, ball_b, ball_b1;

	ball_a = a->get_pocision_forma();
	ball_b = b->get_pocision_forma();

	ball_b1.x = ball_b.x - ball_a.x;
	ball_b1.y = ball_b.y - ball_a.y;

	float length_vector = sqrt(ball_b1.x*ball_b1.x + ball_b1.y*ball_b1.y);

	if (length_vector > a->get_size() * 2)
	{
		return false;
	}

	float f1, f11, f21;

	f1 = a->get_angulo_energia();
	f21 = BillarSFML::get_angulo_2_puntos(ball_a, ball_b);

	float t1, t2, t;
	t1 = f21 + PI / 2;
	t2 = f21 - PI / 2;

	if (abs(f1 - t1) <= (PI / 2))
		t = t1;
	else
		t = t2;
	f11 = f1 - (f1 - t) * 2;


	float value_a = a->get_energia();
	float angle_old_new;
	angle_old_new = f1 - f11;

	float value_f11 = abs(value_a*sin(angle_old_new));

	BillarSFML::Vector_rad F11;
	F11.set_angulo(f11);
	F11.set_value(value_f11);

	BillarSFML::Vector_rad F21;
	F21.set_angulo(f21);				

	angle_old_new = f1 - f21;
	float value_f21 = abs(value_a * cos(angle_old_new));
	F21.set_value(value_f21);


	float f2, f22, f12;

	f2 = b->get_angulo_energia();
	f12 = BillarSFML::get_angulo_2_puntos(ball_b, ball_a);

	t1 = f12 + PI / 2;
	t2 = f12 - PI / 2;

	if (abs(f2 - t1) <= (PI / 2))
		t = t1;
	else
		t = t2;

	f22 = f2 - (f2 - t) * 2;

	float value_b = b->get_energia();

	angle_old_new = f2 - f22;
	float value_f22 = abs(value_b * sin(angle_old_new));

	BillarSFML::Vector_rad F22;
	F22.set_angulo(f22);
	F22.set_value(value_f22);

	BillarSFML::Vector_rad F12;
	F12.set_angulo(f12);

	angle_old_new = f2 - f12;
	float value_f12 = abs(value_b * cos(angle_old_new));
	F12.set_value(value_f12);

	BillarSFML::Vector_rad F1, F2;
	F1 = F11 + F12;
	F2 = F22 + F21;

	a->set_angulo_energia(F1.get_angulo());
	a->set_energia(F1.get_value());

	b->set_angulo_energia(F2.get_angulo());
	b->set_energia(F2.get_value());

	return true;
}

void Juego_Billar::init_bordes()
{
	punto2D start_pos = board->get_pocision_dibujada_recta();

	BillarSFML::Line* elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 66;
	elem->start.y += 94;
	
	elem->end = start_pos;
	elem->end.x += 66;
	elem->end.y += 576;

	elem->dir = BillarSFML::Line::direct::RIGHT;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 1126;
	elem->start.y += 95;

	elem->end = start_pos;
	elem->end.x += 1126;
	elem->end.y += 575;

	elem->dir = BillarSFML::Line::direct::LEFT;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 94;
	elem->start.y += 66;

	elem->end = start_pos;
	elem->end.x += 565;
	elem->end.y += 66;

	elem->dir = BillarSFML::Line::direct::DOWN;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 628;
	elem->start.y += 66;

	elem->end = start_pos;
	elem->end.x += 1099;
	elem->end.y += 66;

	elem->dir = BillarSFML::Line::direct::DOWN;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 94;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 564;
	elem->end.y += 603;

	elem->dir = BillarSFML::Line::direct::UP;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 628;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 1099;
	elem->end.y += 603;

	elem->dir = BillarSFML::Line::direct::UP;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 46;
	elem->start.y += 74;

	elem->end = start_pos;
	elem->end.x += 66;
	elem->end.y += 94;

	elem->dir = BillarSFML::Line::direct::RIGHT_UP;
	borders.push_back(elem);
	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 46;
	elem->start.y += 596;

	elem->end = start_pos;
	elem->end.x += 66;
	elem->end.y += 576;

	elem->dir = BillarSFML::Line::direct::RIGHT_DOWN;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 1146;
	elem->start.y += 75;

	elem->end = start_pos;
	elem->end.x += 1126;
	elem->end.y += 95;

	elem->dir = BillarSFML::Line::direct::LEFT_UP;
	borders.push_back(elem);
	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 1146;
	elem->start.y += 595;

	elem->end = start_pos;
	elem->end.x += 1126;
	elem->end.y += 575;

	elem->dir = BillarSFML::Line::direct::LEFT_DOWN;
	borders.push_back(elem);
	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 74;
	elem->start.y += 46;

	elem->end = start_pos;
	elem->end.x += 94;
	elem->end.y += 66;

	elem->dir = BillarSFML::Line::direct::LEFT_DOWN;
	borders.push_back(elem);
	
	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 577;
	elem->start.y += 46;

	elem->end = start_pos;
	elem->end.x += 565;
	elem->end.y += 66;

	elem->dir = BillarSFML::Line::direct::RIGHT_DOWN;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 616;
	elem->start.y += 46;

	elem->end = start_pos;
	elem->end.x += 628;
	elem->end.y += 66;

	elem->dir = BillarSFML::Line::direct::LEFT_DOWN;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 1119;
	elem->start.y += 46;

	elem->end = start_pos;
	elem->end.x += 1099;
	elem->end.y += 66;

	elem->dir = BillarSFML::Line::direct::RIGHT_DOWN;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 94;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 74;
	elem->end.y += 623;

	elem->dir = BillarSFML::Line::direct::RIGHT_UP;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 564;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 576;
	elem->end.y += 623;

	elem->dir = BillarSFML::Line::direct::RIGHT_UP;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 616;
	elem->start.y += 623;

	elem->end = start_pos;
	elem->end.x += 628;
	elem->end.y += 603;

	elem->dir = BillarSFML::Line::direct::LEFT_UP;
	borders.push_back(elem);

	elem = new BillarSFML::Line;
	elem->start = start_pos;
	elem->start.x += 1099;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 1119;
	elem->end.y += 623;

	elem->dir = BillarSFML::Line::direct::RIGHT_DOWN;
	borders.push_back(elem);
}

void Juego_Billar::init_agujeros()
{
	punto2D start_pos = board->get_pocision_dibujada_recta();

	Bola_colision* hole = new Bola_colision;
	hole->set_size(SIZE_OF_BALL * 1.25);

	hole->set_pos(start_pos.x + 58, start_pos.y + 58 - 40);
	holes.push_back(hole);

	hole = new Bola_colision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 1136, start_pos.y + 58 - 40);
	holes.push_back(hole);

	hole = new Bola_colision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 58, start_pos.y + 613 - 40);
	holes.push_back(hole);

	hole = new Bola_colision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 1136, start_pos.y + 613 - 40);
	holes.push_back(hole);

	hole = new Bola_colision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 597, start_pos.y + 46 - 40);
	holes.push_back(hole);

	hole = new Bola_colision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 597, start_pos.y + 625 - 40);
	holes.push_back(hole);
}

bool Juego_Billar::fin_juego() const { return is_end_game; }
int Juego_Billar::get_movimientos() const { return move_number; }
int Juego_Billar::get_multas() const { return fines_number; }
int Juego_Billar::get_fallos() const { return failures_number; }
