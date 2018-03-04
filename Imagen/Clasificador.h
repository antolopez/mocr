//Clasificador.h

#pragma once

//Includes de código nuestro
#include "definiciones.h"
#include "tipos.h"
#include "BBDD.h"

namespace BBDD 
{

	class Clasificador
	{
		protected:
			InterfazBBDD *bbdd;

		public:
			Clasificador(void);
		public:
			~Clasificador(void);

			virtual double ClasificarCaracter(Caracter *caracter)=0;
	};
}
