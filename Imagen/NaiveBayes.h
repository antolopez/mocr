#pragma once
#include "clasificador.h"

namespace BBDD
{
	typedef struct ParametrosNormal
	{
		double media;
		double desviacion;
	}ParametrosNormal;

	class NaiveBayes : public Clasificador
	{
		private:
			double *probabilidadCaracteres;//Probabilidades de las clases
			ParametrosNormal **probabilidadVariables;
			int numeroCaracteres;//Número de clases
			int *caracteres;
			

		public:
			NaiveBayes(String perfil, InterfazBBDD *db);
		public:
			~NaiveBayes(void);

			double ClasificarCaracter(Caracter *caracter);
	};
}

