#include "Rectangulo_forma.h"

Rectangulo_forma::Rectangulo_forma() : Dibujar_recta(), Rectangulo_colision(){}
Rectangulo_forma::Rectangulo_forma(RenderWindow* w) : Dibujar_recta(w), Rectangulo_colision(){}

void Rectangulo_forma::set_textura(Texture*t) 
{ 
	Dibujar_recta::set_textura(t);
	Rectangulo_colision::set_width(t->getSize().x);
	Rectangulo_colision::set_height(t->getSize().y);
}
void Rectangulo_forma::set_pocision_forma(float x, float y)
{
	set_inicio_punto(x, y);
	set_pocision_dibujada_recta(x, y);
}
punto2D Rectangulo_forma::get_pocision_forma() const { return get_pocision_dibujada_recta(); }

void Rectangulo_forma::events_obj() {}
void Rectangulo_forma::draw() { Dibujar_recta::draw(); }
void Rectangulo_forma::actualizar(float time) {}

Rectangulo_forma::~Rectangulo_forma() {}