#include "StdAfx.h"
#include "NaiveBayes.h"

#include "Math.h"
#include "String.h"

namespace BBDD
{
	NaiveBayes::NaiveBayes(String perfil, InterfazBBDD *db)
	{
		bbdd = db;

		int numero_total_instancias = 0;
		
		caracteres = bbdd->ConsultarCaracteres(&numeroCaracteres);
		
		probabilidadCaracteres = new double[numeroCaracteres];
		
		probabilidadVariables = new ParametrosNormal*[numeroCaracteres];
		for(int c=0; c<numeroCaracteres; c++)
		{
			probabilidadVariables[c] = new ParametrosNormal[400];//400= 4 + (12*3 + 8) * 9

			//Inicialización
			for(int v=0; v<400; v++)
			{
				probabilidadVariables[c][v].media = 0;
				probabilidadVariables[c][v].desviacion = 0;
			}
		}	

		for(int c=0; c<numeroCaracteres; c++)
		{
			int numero_instancias;
			int *instancias = bbdd->ConsultarInstanciasCaracter(perfil, caracteres[c], &numero_instancias);

			probabilidadCaracteres[c] = numero_instancias;
			numero_total_instancias += numero_instancias;

			//Calculamos la media y desviación para cada una de las variables			
			for(int i=0; i<numero_instancias; i++)
			{
				double relacion_aspecto;

				BYTE *caracteristicas_basicas = bbdd->ConsultarCaracteristicaBasica(instancias[i], &relacion_aspecto);

				probabilidadVariables[c][0].media += caracteristicas_basicas[0];
				probabilidadVariables[c][0].desviacion += caracteristicas_basicas[0] * caracteristicas_basicas[0];

				probabilidadVariables[c][1].media += caracteristicas_basicas[1];
				probabilidadVariables[c][1].desviacion += caracteristicas_basicas[1] * caracteristicas_basicas[1];
				
				probabilidadVariables[c][2].media += caracteristicas_basicas[2];
				probabilidadVariables[c][2].desviacion += caracteristicas_basicas[2] * caracteristicas_basicas[2];

				probabilidadVariables[c][3].media += relacion_aspecto;
				probabilidadVariables[c][3].desviacion += relacion_aspecto * relacion_aspecto;

				int **caracteristicas_direccionalesB = bbdd->ConsultarCaracteristicaDireccionalB(instancias[i]);
				BYTE **caracteristicas_direccionalesR = bbdd->ConsultarCaracteristicaDireccionalR(instancias[i]);
				BYTE **caracteristicas_estructurales = bbdd->ConsultarCaracteristicaEstructural(instancias[i]);
				BYTE **caracteristicas_concavidad = bbdd->ConsultarCaracteristicaConcavidad(instancias[i]);

				for(int s=0; s<9; s++)
					for(int v=0; v<12; v++)
					{
						probabilidadVariables[c][4 + (s*12 + v)].media += caracteristicas_direccionalesB[s][v];
						probabilidadVariables[c][4 + (s*12 + v)].desviacion += caracteristicas_direccionalesB[s][v] * caracteristicas_direccionalesB[s][v];

						probabilidadVariables[c][112 + (s*12 + v)].media += caracteristicas_direccionalesR[s][v];
						probabilidadVariables[c][112 + (s*12 + v)].desviacion += caracteristicas_direccionalesR[s][v] * caracteristicas_direccionalesR[s][v];

						probabilidadVariables[c][220 + (s*12 + v)].media += caracteristicas_estructurales[s][v];
						probabilidadVariables[c][220 + (s*12 + v)].desviacion += caracteristicas_estructurales[s][v] * caracteristicas_estructurales[s][v];

						if(v<8)
						{
							probabilidadVariables[c][328 + (s*8 + v)].media += caracteristicas_concavidad[s][v];
							probabilidadVariables[c][328 + (s*8 + v)].desviacion += caracteristicas_concavidad[s][v] * caracteristicas_concavidad[s][v];
						}
					}				

				delete caracteristicas_basicas;
				delete caracteristicas_direccionalesB;
				delete caracteristicas_direccionalesR;
				delete caracteristicas_estructurales;
				delete caracteristicas_concavidad;				
			}

			delete instancias;
		}

		//Calculamos las probabilidades y los parámetros de la normal
		for(int c=0; c<numeroCaracteres; c++)
		{
			for(int v=0; v<400; v++)
			{
				double a = probabilidadVariables[c][v].desviacion;
				double s = probabilidadVariables[c][v].media;
				double n = probabilidadCaracteres[c];

				if(n!=0)
				{
					probabilidadVariables[c][v].media = s / n;
					probabilidadVariables[c][v].desviacion = sqrt( (a - ((s*s)/n)) / (n) );					
				}
			}

			if(numero_total_instancias!=0)
				probabilidadCaracteres[c] /= numero_total_instancias;
		}

	}

	NaiveBayes::~NaiveBayes(void)
	{
	}

	double NaiveBayes::ClasificarCaracter(Caracter *caracter)
	{
		double constante_normal = log(1 / sqrt(2 * PI));

		long double probabilidad_final = -1;

		double p;

		for(int c=0;c<numeroCaracteres; c++)
		{
			if(probabilidadCaracteres[c]!=0)
			{
				long double probabilidad = -log(probabilidadCaracteres[c]);
			
				if(probabilidadVariables[c][0].desviacion!=0)
					probabilidad += 1* (- (constante_normal + log(1/probabilidadVariables[c][0].desviacion) - (pow(caracter->numeroHuecos - probabilidadVariables[c][0].media, 2) / (2 * pow(probabilidadVariables[c][0].desviacion, 2)))));
				else
				{
					p = (probabilidadVariables[c][0].media+1)/(caracter->numeroHuecos+1);
					
					if(p>1)
						p = 1/p;

					probabilidad += -log(p)*100;
				}

				if(probabilidadVariables[c][1].desviacion!=0)
					probabilidad += 1* (- (constante_normal + log(1/probabilidadVariables[c][1].desviacion) - (pow(caracter->numeroCruces - probabilidadVariables[c][1].media, 2) / (2 * pow(probabilidadVariables[c][1].desviacion, 2)))));
				else
				{
					p = (probabilidadVariables[c][1].media+1)/(caracter->numeroCruces+1);
					
					if(p>1)
						p = 1/p;

					probabilidad += -log(p)*100;
				}
				
				if(probabilidadVariables[c][2].desviacion!=0)
					probabilidad += 1* (- (constante_normal + log(1/probabilidadVariables[c][2].desviacion) - (pow(caracter->numeroPuntosTerminales - probabilidadVariables[c][2].media, 2) / (2 * pow(probabilidadVariables[c][2].desviacion, 2)))));
				else
				{
					p = (probabilidadVariables[c][2].media+1)/(caracter->numeroPuntosTerminales+1);
					
					if(p>1)
						p = 1/p;

					probabilidad += -log(p)*100;
				}

				if(probabilidadVariables[c][3].desviacion!=0)
					probabilidad += 1* (- (constante_normal + log(1/probabilidadVariables[c][3].desviacion) - (pow(caracter->relacionAspecto - probabilidadVariables[c][3].media, 2) / (2 * pow(probabilidadVariables[c][3].desviacion, 2)))));
				else
				{
					p = (probabilidadVariables[c][3].media+1)/(caracter->relacionAspecto+1);
					
					if(p>1)
						p = 1/p;

					probabilidad += -log(p)*100;
				}
				

				for(int s=0; s<9; s++)
					for(int v=0;v<12;v++)
					{
						if(probabilidadVariables[c][4 + (s*12 + v)].desviacion!=0)
							probabilidad += 1* (- (constante_normal + log(1/probabilidadVariables[c][4 + (s*12 + v)].desviacion) - (pow(caracter->caracteristicasDireccionalesB[s][v] - probabilidadVariables[c][4 + (s*12 + v)].media, 2) / (2 * pow(probabilidadVariables[c][4 + (s*12 + v)].desviacion, 2)))));
						else
						{
							p = (probabilidadVariables[c][4 + (s*12 + v)].media+1)/(caracter->caracteristicasDireccionalesB[s][v]+1);
							
							if(p>1)
								p = 1/p;

							probabilidad += -log(p)*100;
						}								

						if(probabilidadVariables[c][112 + (s*12 + v)].desviacion!=0)
							probabilidad += 1* (- (constante_normal + log(1/probabilidadVariables[c][112 + (s*12 + v)].desviacion) - (pow(caracter->caracteristicasDireccionalesR[s][v] - probabilidadVariables[c][112 + (s*12 + v)].media, 2) / (2 * pow(probabilidadVariables[c][112 + (s*12 + v)].desviacion, 2)))));
						else
						{
							p = (probabilidadVariables[c][112 + (s*12 + v)].media+1)/(caracter->caracteristicasDireccionalesR[s][v]+1);
							
							if(p>1)
								p = 1/p;

							probabilidad += -log(p)*100;
						}									
						
						if(probabilidadVariables[c][220 + (s*12 + v)].desviacion!=0)
							probabilidad += 1* (- (constante_normal + log(1/probabilidadVariables[c][220 + (s*12 + v)].desviacion) - (pow(caracter->caracteristicasEstructurales[s][v] - probabilidadVariables[c][220 + (s*12 + v)].media, 2) / (2 * pow(probabilidadVariables[c][220 + (s*12 + v)].desviacion, 2)))));
						else
						{
							p = (probabilidadVariables[c][220 + (s*12 + v)].media+1)/(caracter->caracteristicasEstructurales[s][v]+1);
							
							if(p>1)
								p = 1/p;

							probabilidad += -log(p)*100;
						}						

						if(v<8)
							if(probabilidadVariables[c][328 + (s*8 + v)].desviacion!=0)
								probabilidad += 1* (- (constante_normal + log(1/probabilidadVariables[c][328 + (s*8 + v)].desviacion) - (pow(caracter->caracteristicasConcavidad[s][v] - probabilidadVariables[c][328 + (s*8 + v)].media, 2) / (2 * pow(probabilidadVariables[c][328 + (s*8 + v)].desviacion, 2)))));
							else
							{
								p = (probabilidadVariables[c][328 + (s*8 + v)].media+1)/(caracter->caracteristicasConcavidad[s][v]+1);
								
								if(p>1)
									p = 1/p;

								probabilidad += -log(p)*100;
							}		
								
							
					}
			
		

				if(probabilidad<probabilidad_final || probabilidad_final==-1)
				{
					probabilidad_final = probabilidad;
					caracter->reconocido = bbdd->ConsultarCaracter(caracteres[c]);
				}
			}
		}

		return -probabilidad_final;
	}
}
