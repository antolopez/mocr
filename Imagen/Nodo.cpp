#include "stdafx.h"
#include "Nodo.h"


namespace Imagen
{
	Nodo::Nodo(Linea l, Nodo *siguiente)
	{
		linea = l;		

		nodo_siguiente=siguiente;
	}

	Nodo::Nodo(Palabra p, Nodo *siguiente)
	{
		palabra = p;		

		nodo_siguiente=siguiente;
	}

	Nodo::Nodo(Caracter c, Nodo *siguiente)
	{
		caracter = c;

		nodo_siguiente=siguiente;
	}

	Nodo::Nodo(Coordenada c, Nodo *siguiente)
	{
		coordenada = c;

		nodo_siguiente=siguiente;
	}



	Nodo::~Nodo(void)
	{
		if(nodo_siguiente!=NULL)
			delete nodo_siguiente;
	}
}
