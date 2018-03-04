#pragma once
#include "clasificador.h"

namespace BBDD
{
	class KNN : public Clasificador
	{
		private:
			Caracter *instanciasBBDD;
			int numeroInstancias;
			int k;

		public:
			KNN(String perfil, InterfazBBDD *db, int k);
		public:
			~KNN(void);

			double ClasificarCaracter(Caracter *caracter);
	};
}
