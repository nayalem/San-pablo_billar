#pragma once

#include "Dibujar_texto.h"
#include "Dibujar_recta.h"

#include "Rectangulo_forma.h"

class Button : private Rectangulo_forma, public Dibujar_texto
{
	Texture* texture;

	int saltar_a_estado;		
public:
	Button(RenderWindow* w);

	void set_pocision(int x, int y);
	void set_pocision(punto2D p);
	punto2D get_pocision() const;

	void set_textura(Texture* t);

	int get_alto() const;
	int get_ancho() const;

	int get_salto() const;
	void set_salto(int n);

	void draw();

	bool es_colision(float x, float y);

	~Button();
};