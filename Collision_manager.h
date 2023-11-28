#pragma once

#include <vector>
#include "formas_colision.h"
#include "Bola_colision.h"

using namespace std;

class administrar_colision
{
	vector<formas_colision*> arr;

	bool colision_bola_bola(formas_colision* a, formas_colision* b);
	bool collision_ball_border(formas_colision* a, formas_colision* b);

public:
	administrar_colision();

	bool add_colision(formas_colision* p);
	bool delete_colision(formas_colision* p);

	Bola_colision* buscar_bola_on_mouse() const;

	void actualizar();
	
	~administrar_colision();
};