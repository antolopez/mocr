#include "StdAfx.h"
#include "KNN.h"

#include "Math.h"
#include "String.h"

namespace BBDD
{
	KNN::KNN(String perfil, InterfazBBDD *db, int k)
	{
		bbdd = db;

		this->k = k;

		int numero_instancias;
		int *instancias = bbdd->ConsultarInstancias(perfil, &numero_instancias);

		instanciasBBDD = new Caracter[numero_instancias];

		numeroInstancias=numero_instancias;

		for(int i=0;i<numero_instancias;i++)
		{
			Caracter caracter_patron;

			BYTE *caracteristicas_basicas = bbdd->ConsultarCaracteristicaBasica(instancias[i], &caracter_patron.relacionAspecto);

			caracter_patron.numeroHuecos = caracteristicas_basicas[0];
			caracter_patron.numeroCruces = caracteristicas_basicas[1];
			caracter_patron.numeroPuntosTerminales = caracteristicas_basicas[2];

			delete caracteristicas_basicas;

			caracter_patron.caracteristicasDireccionalesB = bbdd->ConsultarCaracteristicaDireccionalB(instancias[i]);
			caracter_patron.caracteristicasDireccionalesR = bbdd->ConsultarCaracteristicaDireccionalR(instancias[i]);
			caracter_patron.caracteristicasEstructurales = bbdd->ConsultarCaracteristicaEstructural(instancias[i]);
			caracter_patron.caracteristicasConcavidad = bbdd->ConsultarCaracteristicaConcavidad(instancias[i]);

			caracter_patron.reconocido = bbdd->ConsultarInstancia(instancias[i]);

			instanciasBBDD[i]=caracter_patron;
		}

	}

	KNN::~KNN(void)
	{
	}

	double KNN::ClasificarCaracter(Caracter *caracter)
	{
		double *diferencias;
		String *reconocidos;

		diferencias = new double[k];
		reconocidos = new String[k];

		for(int v=0; v<k; v++)
			diferencias[v] = -1;

		for(int v=0; v<k; v++)
			reconocidos[v] = NULL;
		
		
		int vecinos_encontrados=0;
		int iguales_final;
		double diferencia_maxima=-1, diferencia=0, diferencia_final=-1;
		
		for(int i=0;i<numeroInstancias;i++)
		{
			
			diferencia=0;

			Caracter caracter_patron = instanciasBBDD[i];
				 
			diferencia+= 5*sqrt(pow((double)caracter_patron.numeroHuecos - caracter->numeroHuecos, 2));
			diferencia+= 5*sqrt(pow((double)caracter_patron.numeroCruces - caracter->numeroCruces, 2));
			diferencia+= 5*sqrt(pow((double)caracter_patron.numeroPuntosTerminales - caracter->numeroPuntosTerminales, 2));
			diferencia+= 5*sqrt(pow(caracter_patron.relacionAspecto - caracter->relacionAspecto, 2));

			for(int s=0;s<9;s++)
				for(int c=0;c<12;c++)
				{
					diferencia+= sqrt(pow((double)caracter_patron.caracteristicasDireccionalesB[s][c] - caracter->caracteristicasDireccionalesB[s][c], 2));
					diferencia+= sqrt(pow((double)caracter_patron.caracteristicasDireccionalesR[s][c] - caracter->caracteristicasDireccionalesR[s][c], 2));
					diferencia+= sqrt(pow((double)caracter_patron.caracteristicasEstructurales[s][c] - caracter->caracteristicasEstructurales[s][c], 2));
					
					if(c<8)
						diferencia+= sqrt(pow((double)caracter_patron.caracteristicasConcavidad[s][c] - caracter->caracteristicasConcavidad[s][c], 2));
				}


			if(vecinos_encontrados<k)
			{
				if(diferencia_maxima<=diferencia || diferencia_maxima==-1)
					diferencia_maxima=diferencia;
				
				int v;
				for(v=0; v<k && diferencias[v]!=-1; v++);

				diferencias[v]=diferencia;
				reconocidos[v] = caracter_patron.reconocido;

				vecinos_encontrados++;
			}
			else
			{
				if(diferencia_maxima>diferencia)
				{
					int v;

					for(v=0; v<k && diferencias[v]!=diferencia_maxima; v++);

					diferencias[v]=diferencia;
					reconocidos[v] = caracter_patron.reconocido;

					diferencia_maxima=diferencia;

					for(int v=0; v<k; v++)
						if(diferencias[v]>diferencia_maxima)
							diferencia_maxima = diferencias[v];
				}
			}			
		}


		for(int v=0; v<k; v++)
		{
			int iguales = 1;
			double suma_diferencias=diferencias[v];

			if(reconocidos[v]!=NULL)
			{
				for(int v2=v+1; v2<k; v2++)
				{
					if(reconocidos[v2]!=NULL)
					{
						if(!strcmp(reconocidos[v], reconocidos[v2]))
						{
							iguales++;
							suma_diferencias += diferencias[v2];
							reconocidos[v2] = NULL;
						}
					}
				}
			

				if(diferencia_final>(suma_diferencias / iguales) || diferencia_final==-1 || (iguales>=(iguales_final * 2)))
				{
					iguales_final = iguales;
					diferencia_final = suma_diferencias / iguales;
					caracter->reconocido = reconocidos[v];
				}
			}
		}

		return -diferencia_final;
	}
}
