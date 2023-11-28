#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <cmath>
#include <vector>
#include "Bola_forma.h"
#include "Vector_rad.h"
#include "Seleciona_bola.h"
#include "MyWindow.h"
#include "Juego_Billar.h"
#include <string>

using namespace std;
using namespace sf;

const int window_width = 1300;
const int window_height = 800;

const char* direccion_boton = "res/textures/boton.png";
const char* dir_ball = "res/textures/bola_32.png";
const char* dir_cue = "res/textures/taco.png";
const char* dir_select = "res/textures/select_rect.png";
const char* dir_power = "res/textures/fuerza.png";
const char* dir_board = "res/textures/mesa_billar.png";
const char* dir_select_ball = "res/textures/bola_seleccionada.png";

const char* dir_font = "res/fonts/times new roman.ttf";

int main()
{
	MyWindow* window = new MyWindow(VideoMode(window_width, window_height), "Juego de Billar");
	window->event_handling();

	delete window;

	return 0;
}