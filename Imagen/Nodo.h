#pragma once

#include "definiciones.h"
#include "tipos.h"

namespace Imagen
{
	class Nodo
	{
		//Atributos
		public:
			Linea linea;	
			Palabra palabra;
			Caracter caracter;
			Coordenada coordenada;
			Nodo *nodo_siguiente;
		
		//Métodos
		public:
			Nodo(Linea linea, Nodo *nodo_siguiente);
			Nodo(Palabra palabra, Nodo *nodo_siguiente);
			Nodo(Caracter caracter, Nodo *nodo_siguiente);
			Nodo(Coordenada coordenada, Nodo *nodo_siguiente);
			~Nodo(void);
	};
}
