#pragma once

#include "Bola_forma.h"
#include "Line.h"
#include "Point2D.h"

namespace BillarSFML
{
	struct Ecuacion
	{
		float A, B, C;

		Ecuacion(float a, float b, float c) : A(a), B(b), C(c) {}
		Ecuacion() {}
	};

	Ecuacion get_ecuacion_linea_en_2_puntos(punto2D a, punto2D b)
	{
		punto2D vectorAB;
		vectorAB.x = b.x - a.x;
		vectorAB.y = b.y - a.y;

		float dy = vectorAB.x / vectorAB.y;

		Ecuacion result;

		result.A = dy;
		result.B = -1;
		result.C = a.x - a.y*dy;

		return result;
	}

	bool is_colision_bola_linea(Bola_forma* a, Line* b)
	{
		if ((b->start.x - a->get_size() < a->get_pocision_forma().x &&
			b->start.y - a->get_size() < a->get_pocision_forma().y) &&
			(b->end.x + a->get_size() > a->get_pocision_forma().x &&
				b->end.y + a->get_size() > a->get_pocision_forma().y))
		{
			Line line;
			line.start.x = b->start.x - a->get_pocision_forma().x;
			line.start.y = b->start.y - a->get_pocision_forma().y;
			line.end.x = b->end.x - a->get_pocision_forma().x;
			line.end.y = b->end.y - a->get_pocision_forma().y;

			Ecuacion e = get_ecuacion_linea_en_2_puntos(line.start, line.end);

			punto2D near;
			near.x = -((e.A * e.C) / (e.A*e.A + e.B*e.B));
			near.y = -((e.B * e.C) / (e.A*e.A + e.B*e.B));

			float length_to_near_point = sqrt(near.x*near.x + near.y*near.y);

			if (a->get_size() < length_to_near_point)
				return false;
			else
				return true;
		}
		else
			return false;
	}
}
