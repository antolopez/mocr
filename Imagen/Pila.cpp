#include "stdafx.h"
#include "Pila.h"


namespace Imagen
{	
	Pila::Pila(void)
	{
		cima=NULL;
		numero_elementos=0;
	}

	Pila::~Pila(void)
	{
		if(cima!=NULL)
			delete cima;
	}

	void Pila::Apilar(Linea l)
	{
		cima=new Nodo(l,cima);

		numero_elementos++;
	}

	void Pila::Desapilar(Linea *l)
	{
		*l=cima->linea;

		Nodo *aux = cima;
		cima=cima->nodo_siguiente;

		aux->nodo_siguiente=NULL;
		delete aux;

		numero_elementos--;
	}

	void Pila::Apilar(Palabra p)
	{
		cima=new Nodo(p,cima);

		numero_elementos++;
	}

	void Pila::Desapilar(Palabra *p)
	{
		*p=cima->palabra;

		Nodo *aux = cima;
		cima=cima->nodo_siguiente;

		aux->nodo_siguiente=NULL;
		delete aux;

		numero_elementos--;
	}

	void Pila::Apilar(Caracter c)
	{
		cima=new Nodo(c,cima);

		numero_elementos++;
	}

	void Pila::Desapilar(Caracter *c)
	{
		*c=cima->caracter;

		Nodo *aux = cima;
		cima=cima->nodo_siguiente;

		aux->nodo_siguiente=NULL;
		delete aux;

		numero_elementos--;
	}

	void Pila::Apilar(Coordenada c)
	{
		cima=new Nodo(c,cima);

		numero_elementos++;
	}

	void Pila::Desapilar(Coordenada *c)
	{
		*c=cima->coordenada;

		Nodo *aux = cima;
		cima=cima->nodo_siguiente;

		aux->nodo_siguiente=NULL;
		delete aux;

		numero_elementos--;
	}

	bool Pila::EsVacia()
	{
		return cima==NULL;
	}

	int Pila::NumeroElementos()
	{
		return numero_elementos;
	}
}
