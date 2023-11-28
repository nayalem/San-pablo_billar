#pragma once

#include "Point2D.h"
namespace BillarSFML
{
	class Vector_rad
	{
	private:
		float angle;	
		float value;

	public:
		Vector_rad();
		Vector_rad(float a, float v);

		void set_vector(Vector_rad);
		void set_vector(float a, float v);

		void set_angulo(float n);
		void set_value(float n);

		float get_angulo() const;
		float get_value() const;

		float get_proteccion_X() const;
		float get_proteccion_Y() const;

		Vector_rad operator+(Vector_rad b);

		Vector_rad operator*(float b);

		Vector_rad operator/(float b);
		
		bool operator==(Vector_rad b);
		bool operator!=(Vector_rad b);
				
		void normalizar_vector();		
		void reverse_direct();		

		friend bool is_colineal(Vector_rad a, Vector_rad b);	

		~Vector_rad();
	};

	float get_angulo(float sin, float cos);

	float get_angulo_2_puntos(punto2D A, punto2D B);
}
