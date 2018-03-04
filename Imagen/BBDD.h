//BBDD.h

#pragma once

//Includes de c�digo nuestro
#include "definiciones.h"
#include "tipos.h"
#include "sqlite3.h"

namespace BBDD 
{
	public class InterfazBBDD
	{
		//Atributos
		private:
			bool bbdd_abierta;
			sqlite3 *bbdd;//Variable necesaria para hacer todas las operaciones con la BBDD

		
		//M�todos
		public:
			//Constructor	
			InterfazBBDD(void);		

			//M�todos para abrir y cerrar la conexi�n con una bbdd
			int AbrirBBDD(String ruta);
			int CerrarBBDD();

			//M�todos para gestionar los perfiles
			int InsertarPerfil(String nombre);
			int BorrarPerfil(String nombre);

			//M�todos para gestionar los distintos caracteres/cadenas de plantilla y aprendidos
			int InsertarCaracter(String cadena);
			int BorrarCaracter(String cadena);
			String ConsultarCaracter(int id_caracter);
			int *ConsultarCaracteres(int *numero_caracteres);
			
			//M�todos para gestionar las instancias iniciales y aprendidas de los distintos caracteres
			int InsertarInstancia(String nombre_perfil, String cadena, int numero_huecos, int numero_cruces, int numero_terminales, double relacion_aspecto, int **caracteristicas_direccionalesB, BYTE **caracteristicas_direccionalesR, BYTE **caracteristicas_estructurales, BYTE **caracteristicas_concavidad);
			int *ConsultarInstancias(String nombre_perfil, int *numero_instancias);
			int *ConsultarInstanciasCaracter(String nombre_perfil, int id_caracter, int *numero_instancias_caracter);
			String ConsultarInstancia(int id_instancia);
			
			//M�todos para consultar las distintas caracter�sticas de una instancia
			BYTE* ConsultarCaracteristicaBasica(int instancia, double *relacion_aspecto);
			int** ConsultarCaracteristicaDireccionalB(int instancia);
			BYTE** ConsultarCaracteristicaDireccionalR(int instancia);
			BYTE** ConsultarCaracteristicaEstructural(int instancia);
			BYTE** ConsultarCaracteristicaConcavidad(int instancia);

		private:
			//M�todos para obtener id�s a partr del String asociado
			int idPerfil(String nombre);
		
		public:
			int idCaracter(String cadena);			
	};
}
