#pragma once

#include "Nodo.h"

namespace Imagen
{
	class Pila
	{
		//Atributos
		private:
			Nodo *cima;
			int numero_elementos;

		//Métodos
		public:
			Pila(void);
			~Pila(void);
			void Apilar(Linea l);
			void Desapilar(Linea *l);
			void Apilar(Palabra p);
			void Desapilar(Palabra *p);
			void Apilar(Caracter c);
			void Desapilar(Caracter *c);
			void Apilar(Coordenada c);
			void Desapilar(Coordenada *c);
			bool EsVacia();
			int NumeroElementos();
	};
}
