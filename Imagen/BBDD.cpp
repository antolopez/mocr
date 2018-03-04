// Archivo DLL principal.

#include "stdafx.h"

#include "BBDD.h"

#include <fstream>
#include "string.h"


using namespace std;

namespace BBDD
{

	//============================================================================
	// NOMBRE: InterfazBBDD
	//
	// DESCRIPCIÓN: Constructor por defecto.
	//				
	// ARGUMENTOS: -
	//
	// SALIDA: -
	//============================================================================
	InterfazBBDD::InterfazBBDD()
	{
		bbdd_abierta = false;
	}
	

	//============================================================================
	// NOMBRE: AbrirBBDD
	//
	// DESCRIPCIÓN: Abre la bbdd pasada por parámetro, creándola si no existe (creando además las tablas necesarias).
	//
	// ARGUMENTOS: String ruta -> Camino completo al archivo en disco		   
	//
	// SALIDA: Devuelve un código de error -> Un valor entero que indica:
	//										 0 -> Operación realizada satisfactoriamente
	//										-1 -> Error. Problemas al abrir/crear la bbdd
	//										-2 -> Error. Problema al crear alguna tabla										
	//============================================================================
	int InterfazBBDD::AbrirBBDD(String ruta)
	{
		bool existe_bbdd;

		//Comprobamos si la bbdd ya existe o si debemos crearla
		ifstream archivo;
		archivo.open(ruta, ios::binary|ios::in);//Intentamos abrir el archivo en modo binario y lectura
		existe_bbdd = !archivo.fail();//Comprobamos si ha habido problemas al abrir el archivo
		
		//Abrimos la bbdd
		//Si ya había una abierta la cerramos antes
		if(bbdd_abierta)
			CerrarBBDD();

		int codigo_error = sqlite3_open(ruta, &bbdd);

		if(codigo_error!=SQLITE_OK)
			return -1;

		bbdd_abierta = true;

		if(!existe_bbdd)//Si no existía la bbdd tenemos que crear las tablas
		{
			//Creamos las tablas 
			String crear_tabla_perfiles_sql = "CREATE TABLE Perfiles \
												(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
												 nombre TEXT NOT NULL UNIQUE);";
			
			String crear_tabla_caracteres_sql = "CREATE TABLE Caracteres \
												  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
												   cadena TEXT NOT NULL UNIQUE);";

			String crear_tabla_instancias_sql = "CREATE TABLE Instancias \
												  (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, \
												   idPerfil INTEGER NOT NULL, \
												   idCaracter INTEGER NOT NULL);";

			String crear_tabla_caracteristicas_basicas_sql = "CREATE TABLE CaracteristicasBasicas \
												  (idInstancia INTEGER NOT NULL PRIMARY KEY, \
												   numeroHuecos INTEGER NOT NULL, \
												   numeroCruces INTEGER NOT NULL, \
												   numeroTerminales INTEGER NOT NULL, \
												   relacionAspecto DOUBLE NOT NULL);";

			String crear_tabla_caracteristicas_direccionalesB_sql = "CREATE TABLE CaracteristicasDireccionalesB \
																	  (idInstancia INTEGER NOT NULL, \
																	   seccion INTEGER NOT NULL, \
																	   c1 INTEGER NOT NULL, \
																	   c2 INTEGER NOT NULL, \
																	   c3 INTEGER NOT NULL, \
																	   c4 INTEGER NOT NULL, \
																	   c5 INTEGER NOT NULL, \
																	   c6 INTEGER NOT NULL, \
																	   c7 INTEGER NOT NULL, \
																	   c8 INTEGER NOT NULL, \
																	   c9 INTEGER NOT NULL, \
																	   c10 INTEGER NOT NULL, \
																	   c11 INTEGER NOT NULL, \
																	   c12 INTEGER NOT NULL, \
																	   PRIMARY KEY (idInstancia, seccion));";

			String crear_tabla_caracteristicas_direccionalesR_sql = "CREATE TABLE CaracteristicasDireccionalesR \
																	  (idInstancia INTEGER NOT NULL, \
																	   seccion INTEGER NOT NULL, \
																	   c1 INTEGER NOT NULL, \
																	   c2 INTEGER NOT NULL, \
																	   c3 INTEGER NOT NULL, \
																	   c4 INTEGER NOT NULL, \
																	   c5 INTEGER NOT NULL, \
																	   c6 INTEGER NOT NULL, \
																	   c7 INTEGER NOT NULL, \
																	   c8 INTEGER NOT NULL, \
																	   c9 INTEGER NOT NULL, \
																	   c10 INTEGER NOT NULL, \
																	   c11 INTEGER NOT NULL, \
																	   c12 INTEGER NOT NULL, \
																	   PRIMARY KEY (idInstancia, seccion));";

			String crear_tabla_caracteristicas_estructurales_sql = "CREATE TABLE CaracteristicasEstructurales \
																	  (idInstancia INTEGER NOT NULL, \
																	   seccion INTEGER NOT NULL, \
																	   c1 INTEGER NOT NULL, \
																	   c2 INTEGER NOT NULL, \
																	   c3 INTEGER NOT NULL, \
																	   c4 INTEGER NOT NULL, \
																	   c5 INTEGER NOT NULL, \
																	   c6 INTEGER NOT NULL, \
																	   c7 INTEGER NOT NULL, \
																	   c8 INTEGER NOT NULL, \
																	   c9 INTEGER NOT NULL, \
																	   c10 INTEGER NOT NULL, \
																	   c11 INTEGER NOT NULL, \
																	   c12 INTEGER NOT NULL, \
																	   PRIMARY KEY (idInstancia, seccion));";

			String crear_tabla_caracteristicas_concavidad_sql = "CREATE TABLE CaracteristicasConcavidad \
																	  (idInstancia INTEGER NOT NULL, \
																	   seccion INTEGER NOT NULL, \
																	   c1 INTEGER NOT NULL, \
																	   c2 INTEGER NOT NULL, \
																	   c3 INTEGER NOT NULL, \
																	   c4 INTEGER NOT NULL, \
																	   c5 INTEGER NOT NULL, \
																	   c6 INTEGER NOT NULL, \
																	   c7 INTEGER NOT NULL, \
																	   c8 INTEGER NOT NULL, \
																	   PRIMARY KEY (idInstancia, seccion));";
		

			codigo_error = sqlite3_exec(bbdd, crear_tabla_perfiles_sql, NULL, NULL, NULL);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_exec(bbdd, crear_tabla_caracteres_sql, NULL, NULL, NULL);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_exec(bbdd, crear_tabla_instancias_sql, NULL, NULL, NULL);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_exec(bbdd, crear_tabla_caracteristicas_basicas_sql, NULL, NULL, NULL);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_exec(bbdd, crear_tabla_caracteristicas_direccionalesB_sql, NULL, NULL, NULL);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_exec(bbdd, crear_tabla_caracteristicas_direccionalesR_sql, NULL, NULL, NULL);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_exec(bbdd, crear_tabla_caracteristicas_estructurales_sql, NULL, NULL, NULL);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_exec(bbdd, crear_tabla_caracteristicas_concavidad_sql, NULL, NULL, NULL);

			if(codigo_error!=SQLITE_OK)
				return -2;			
		}

		return 0;
	}


	//============================================================================
	// NOMBRE: CerrarBBDD
	//
	// DESCRIPCIÓN: Cierra la bbdd siempre que sea posible.
	//
	// ARGUMENTOS: -  
	//
	// SALIDA: Devuelve un código de error -> Un valor entero que indica:
	//										 0 -> Operación realizada satisfactoriamente
	//										-1 -> Error. No hay ninguna bbdd abierta
	//										-2 -> Error. La bbdd no puede cerrarse por estar en uso									
	//============================================================================
	int InterfazBBDD::CerrarBBDD()
	{
		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return -1;

		int codigo_error = sqlite3_close(bbdd);

		if(codigo_error==SQLITE_BUSY)//Comprobamos si la bbdd está aún en uso
			return -2;

		//La bbdd se ha cerrado correctamente
		bbdd_abierta = false;

		return 0;	
	}

	
	//============================================================================
	// NOMBRE: InsertarPerfil
	//
	// DESCRIPCIÓN: Introduce un nuevo perfil en la bbdd.
	//
	// ARGUMENTOS: String nombre -> Nombre del perfil a insertar		   
	//
	// SALIDA: Devuelve un código de error -> Un valor entero que indica:
	//										 0 -> Operación realizada satisfactoriamente
	//										-1 -> Error. Problemas con la sintaxis de la consulta
	//										-2 -> Error. Problemas al asociar los parámetros de la consulta
	//										-3 -> Error. Problemas al ejecutar la consulta
	//										-4 -> Error. Problemas al finalizar la consulta
	//										-5 -> Error. No hay ninguna bbdd abierta
	//============================================================================
	int InterfazBBDD::InsertarPerfil(String nombre)
	{
		int codigo_error;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return -5;

		sqlite3_stmt *insertar_perfil_stmt;//Consulta precompilada

		String insertar_perfil_sql = "INSERT INTO Perfiles (nombre) VALUES (?);";

		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, insertar_perfil_sql, -1, &insertar_perfil_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_text(insertar_perfil_stmt, 1, nombre, -1, SQLITE_STATIC);

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(insertar_perfil_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(insertar_perfil_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		return 0;
	}


	//============================================================================
	// NOMBRE: BorrarPerfil
	//
	// DESCRIPCIÓN: Elimina un perfil de la bbdd, en caso de que exista.
	//
	// ARGUMENTOS: String nombre -> Nombre del perfil a eliminar		   
	//
	// SALIDA: Devuelve un código de error -> Un valor entero que indica:
	//										 0 -> Operación realizada satisfactoriamente
	//										-1 -> Error. Problemas con la sintaxis de la consulta
	//										-2 -> Error. Problemas al asociar los parámetros de la consulta
	//										-3 -> Error. Problemas al ejecutar la consulta
	//										-4 -> Error. Problemas al finalizar la consulta
	//										-5 -> Error. No hay una bbdd abierta
	//============================================================================
	int InterfazBBDD::BorrarPerfil(String nombre)
	{
		int codigo_error;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return -5;
	
		sqlite3_stmt *borrar_perfil_stmt;//Consulta precompilada

		//Borramos las características asociadas al perfil
		//Características Básicas
		String borrar_perfil_sql = "DELETE FROM CaracteristicasBasicas WHERE idInstancia IN (SELECT id FROM Instancias \
																								WHERE idPerfil=?);";
		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, borrar_perfil_sql, -1, &borrar_perfil_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_int(borrar_perfil_stmt, 1, idPerfil(nombre));

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		//Características Direccionales B
		borrar_perfil_sql = "DELETE FROM CaracteristicasDireccionalesB WHERE idInstancia IN (SELECT id FROM Instancias \
																								WHERE idPerfil=?);";
		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, borrar_perfil_sql, -1, &borrar_perfil_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_int(borrar_perfil_stmt, 1, idPerfil(nombre));

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		//Características Direccionales R
		 borrar_perfil_sql = "DELETE FROM CaracteristicasDireccionalesR WHERE idInstancia IN (SELECT id FROM Instancias \
																								WHERE idPerfil=?);";
		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, borrar_perfil_sql, -1, &borrar_perfil_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_int(borrar_perfil_stmt, 1, idPerfil(nombre));

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		//Características Estructurales
		borrar_perfil_sql = "DELETE FROM CaracteristicasEstructurales WHERE idInstancia IN (SELECT id FROM Instancias \
																								WHERE idPerfil=?);";
		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, borrar_perfil_sql, -1, &borrar_perfil_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_int(borrar_perfil_stmt, 1, idPerfil(nombre));

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		//Características Concavidad
		borrar_perfil_sql = "DELETE FROM CaracteristicasConcavidad WHERE idInstancia IN (SELECT id FROM Instancias \
																								WHERE idPerfil=?);";
		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, borrar_perfil_sql, -1, &borrar_perfil_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_int(borrar_perfil_stmt, 1, idPerfil(nombre));

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		//Borramos las instancias asociadas al perfil
		borrar_perfil_sql = "DELETE FROM Instancias WHERE idPerfil=?;";
		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, borrar_perfil_sql, -1, &borrar_perfil_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_int(borrar_perfil_stmt, 1, idPerfil(nombre));

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;
		
		//Borramos el perfil
		borrar_perfil_sql = "DELETE FROM Perfiles WHERE nombre=?;";

		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, borrar_perfil_sql, -1, &borrar_perfil_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_text(borrar_perfil_stmt, 1, nombre, -1, SQLITE_STATIC);

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(borrar_perfil_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;	

		return 0;
	}


	//============================================================================
	// NOMBRE: InsertarCaracter
	//
	// DESCRIPCIÓN: Introduce un nuevo 'carácter' (realmente puede ser una cadena) en la bbdd.
	//
	// ARGUMENTOS: String cadena -> Cadena de texto que representa al 'carácter'	   
	//
	// SALIDA: Devuelve un código de error -> Un valor entero que indica:
	//										 0 -> Operación realizada satisfactoriamente
	//										-1 -> Error. Problemas con la sintaxis de la consulta
	//										-2 -> Error. Problemas al asociar los parámetros de la consulta
	//										-3 -> Error. Problemas al ejecutar la consulta
	//										-4 -> Error. Problemas al finalizar la consulta
	//										-5 -> Error. No hay una bbdd abierta
	//============================================================================
	int InterfazBBDD::InsertarCaracter(String cadena)
	{
		int codigo_error;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return -5;

		sqlite3_stmt *insertar_caracter_stmt;//Consulta precompilada

		String insertar_caracter_sql = "INSERT INTO Caracteres (cadena) VALUES (?);";

		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, insertar_caracter_sql, -1, &insertar_caracter_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_text(insertar_caracter_stmt, 1, cadena, -1, SQLITE_STATIC);

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(insertar_caracter_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(insertar_caracter_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		return 0;
	}


	//============================================================================
	// NOMBRE: BorrarCaracter
	//
	// DESCRIPCIÓN: Elimina un 'carácter' (realmente puede ser una cadena) de la bbdd, en caso de que exista.
	//
	// ARGUMENTOS: String cadena -> Cadena de texto que representa al 'carácter' a eliminar	   
	//
	// SALIDA: Devuelve un código de error -> Un valor entero que indica:
	//										 0 -> Operación realizada satisfactoriamente
	//										-1 -> Error. Problemas con la sintaxis de la consulta
	//										-2 -> Error. Problemas al asociar los parámetros de la consulta
	//										-3 -> Error. Problemas al ejecutar la consulta
	//										-4 -> Error. Problemas al finalizar la consulta
	//										-5 -> Error. No hay una bbdd abierta
	//============================================================================
	int InterfazBBDD::BorrarCaracter(String cadena)
	{
		int codigo_error;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return -5;

		sqlite3_stmt *borrar_caracter_stmt;//Consulta precompilada

		String borrar_caracter_sql = "DELETE FROM Caracteres WHERE cadena=?;";

		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, borrar_caracter_sql, -1, &borrar_caracter_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la consulta
		codigo_error = sqlite3_bind_text(borrar_caracter_stmt, 1, cadena, -1, SQLITE_STATIC);

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(borrar_caracter_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(borrar_caracter_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		return 0;
	}


	//============================================================================
	// NOMBRE: ConsultarCaracter
	//
	// DESCRIPCIÓN: Devuelve la cadena que representa el 'carácter' con la id pasada por parámetro, en caso de que exista.
	//
	// ARGUMENTOS: int id_caracter -> Id del carácter a obtener	   
	//
	// SALIDA: Cadena de texto que representa al 'carácter', en caso de no existir o estar 
	//		   cerrada la bbdd, devolveremos la cadena vacía
	//============================================================================
	String InterfazBBDD::ConsultarCaracter(int id_caracter)
	{
		String cadena = "", cadena_aux;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return cadena;

		sqlite3_stmt *caracter_stmt;//Consulta precompilada

		String caracter_sql = "SELECT cadena FROM Caracteres \
								  WHERE id = ?;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, caracter_sql, -1, &caracter_stmt, NULL);

		//Asociamos los parámetros de la consulta
		sqlite3_bind_int(caracter_stmt, 1, id_caracter);

		//Ejecutamos la consulta
		if(sqlite3_step(caracter_stmt)==SQLITE_ROW)		
		{
			//Recogemos el dato
			cadena_aux = (String)sqlite3_column_text(caracter_stmt, 0);	

			cadena = new char[strlen(cadena_aux) + 1];

			//Copiamos el string porque al finalizar la consulta sqlite lo elimina
			memcpy(cadena, cadena_aux, strlen(cadena_aux) + 1);
		}

		//Finalizamos la consulta
		sqlite3_finalize(caracter_stmt);
					
		return cadena;
	}


	//============================================================================
	// NOMBRE: ConsultarCaracteres
	//
	// DESCRIPCIÓN: Consulta que devuelve las id´s de todos los caracteres de la bbdd.
	//
	// ARGUMENTOS: int *numero_caracteres -> Dejaremos el número de caracteres existentes en la bbdd 
	//			 
	// SALIDA: Devuelve un array con todas las instancias encontradas o NULL en caso de no encontrar ninguna
	//============================================================================
	int* InterfazBBDD::ConsultarCaracteres(int *numero_caracteres)
	{
		*numero_caracteres = 0;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return NULL;

		//Obtenemos el número de caracteres en la bbdd
		sqlite3_stmt *numero_caracteres_stmt;//Consulta precompilada

		String numero_caracteres_sql = "SELECT COUNT(id) FROM Caracteres;";
		
		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, numero_caracteres_sql, -1, &numero_caracteres_stmt, NULL);

		//Ejecutamos la consulta y recogemos el numero de instancias
		while(sqlite3_step(numero_caracteres_stmt)==SQLITE_ROW)
			*numero_caracteres = sqlite3_column_int(numero_caracteres_stmt, 0);

		//Finalizamos la consulta
		sqlite3_finalize(numero_caracteres_stmt);


		//Obtenemos todas las instancias del perfil dado
		int *caracteres = new int[*numero_caracteres];

		sqlite3_stmt *consultar_caracteres_stmt;//Consulta precompilada

		String consultar_caracteres_sql = "SELECT id FROM Caracteres;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, consultar_caracteres_sql, -1, &consultar_caracteres_stmt, NULL);

		//Ejecutamos la consulta y recogemos las instancias
		for(int i=0;sqlite3_step(consultar_caracteres_stmt)==SQLITE_ROW;i++)
			caracteres[i] = sqlite3_column_int(consultar_caracteres_stmt, 0);

		//Finalizamos la consulta
		sqlite3_finalize(consultar_caracteres_stmt);

		return caracteres;											
	}


	//============================================================================
	// NOMBRE: InsertarInstancia
	//
	// DESCRIPCIÓN: Inserta una nueva instancia en la bbdd.
	//
	// ARGUMENTOS: String nombre_perfil						-> Nombre del perfil al que pertenece la instancia
	//			   String cadena							-> Cadena de texto que representa al 'carácter' de la instancia	 
	//			   int numero_huecos						-> Número de huecos encontrado en la imagen de la instancia
	//			   int numero_cruces						-> Número de cruces encontrado en la imagen de la instancia	
	//			   double relacion_aspecto					-> Ancho entre el alto del carácter
	//			   int numero_terminales					-> Número de puntos terminales encontrado en la imagen de la instancia	
	//			   BYTE **caracteristicas_direccionalesB	-> Matriz 9x12 con las características direccionales básicas de la instancia
	//			   BYTE **caracteristicas_direccionalesR	-> Matriz 9x12 con las características direccionales reales de la instancia
	//			   BYTE **caracteristicas_estructurales		-> Matriz 9x12 con las características estructurales de la instancia
	//			   BYTE **caracteristicas_concavidad		-> Matriz 9x8 con las características de concavidad de la instancia
	//
	// SALIDA: Devuelve un código de error -> Un valor entero que indica:
	//										 0 -> Operación realizada satisfactoriamente
	//										-1 -> Error. Problemas con la sintaxis de la consulta
	//										-2 -> Error. Problemas al asociar los parámetros de la consulta
	//										-3 -> Error. Problemas al ejecutar la consulta
	//										-4 -> Error. Problemas al finalizar la consulta
	//										-5 -> Error. No hay una bbdd abierta
	//============================================================================
	int InterfazBBDD::InsertarInstancia(String nombre_perfil, String cadena, int numero_huecos, int numero_cruces, int numero_terminales, double relacion_aspecto, int **caracteristicas_direccionalesB, BYTE **caracteristicas_direccionalesR, BYTE **caracteristicas_estructurales, BYTE **caracteristicas_concavidad)
	{
		int codigo_error;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return -5;

		
		//Creamos la instancia
		sqlite3_stmt *crear_instancia_stmt;//Orden precompilada

		String crear_instancia_sql = "INSERT INTO Instancias (idPerfil, idCaracter) \
										VALUES (?1, ?2);";

		//Precompilamos la orden
		codigo_error = sqlite3_prepare_v2(bbdd, crear_instancia_sql, -1, &crear_instancia_stmt, NULL);

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la orden
		codigo_error = sqlite3_bind_int(crear_instancia_stmt, 1, idPerfil(nombre_perfil));	

		if(codigo_error!=SQLITE_OK)
			return -2;

		codigo_error = sqlite3_bind_int(crear_instancia_stmt, 2, idCaracter(cadena));	

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la orden
		codigo_error = sqlite3_step(crear_instancia_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		

		//Finalizamos la orden
		codigo_error = sqlite3_finalize(crear_instancia_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;


		//Obtenemos el identificador de la instancia creada
		sqlite3_stmt *numero_instancia_stmt;//Consulta precompilada

		String numero_instancia_sql = "SELECT id FROM Instancias \
										ORDER BY id  DESC;"; 

		//Precompilamos la consulta
		codigo_error = sqlite3_prepare_v2(bbdd, numero_instancia_sql, -1, &numero_instancia_stmt, NULL);

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Ejecutamos la consulta
		codigo_error = sqlite3_step(numero_instancia_stmt);

		if(codigo_error!=SQLITE_ROW)
			return -3;	

		//Recogemos el resultado
		int numero_instancia = sqlite3_column_int(numero_instancia_stmt, 0);
		
		//Finalizamos la consulta
		codigo_error = sqlite3_finalize(numero_instancia_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;


		//Insertamos las características básicas de la instancia
		sqlite3_stmt *insertar_caracteristica_stmt;//Orden precompilada

		String insertar_caracteristica_sql = "INSERT INTO CaracteristicasBasicas (idInstancia, numeroHuecos, numeroCruces, numeroTerminales, relacionAspecto) \
													VALUES (?1, ?2, ?3, ?4, ?5);";

		//Precompilamos la orden
		codigo_error = sqlite3_prepare_v2(bbdd, insertar_caracteristica_sql, -1, &insertar_caracteristica_stmt, NULL);  

		if(codigo_error!=SQLITE_OK)
			return -1;

		//Asociamos los parámetros de la orden
		codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 1, numero_instancia);

		if(codigo_error!=SQLITE_OK)
			return -2;

		codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 2, numero_huecos);

		if(codigo_error!=SQLITE_OK)
			return -2;

		codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 3, numero_cruces);

		if(codigo_error!=SQLITE_OK)
			return -2;

		codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 4, numero_terminales);

		if(codigo_error!=SQLITE_OK)
			return -2;

		codigo_error = sqlite3_bind_double(insertar_caracteristica_stmt, 5, relacion_aspecto);

		if(codigo_error!=SQLITE_OK)
			return -2;

		//Ejecutamos la orden
		codigo_error = sqlite3_step(insertar_caracteristica_stmt);

		if(codigo_error!=SQLITE_DONE)
			return -3;		
		
		//Finalizamos la orden
		codigo_error = sqlite3_finalize(insertar_caracteristica_stmt);

		if(codigo_error!=SQLITE_OK)
			return -4;

		
		//Insertamos las características para cada una de las secciones de la imagen del 'carácter'
		for(int s=0;s<9;s++)
		{
			//Insertamos las características direccionales básicas de la sección s
			insertar_caracteristica_sql = "INSERT INTO CaracteristicasDireccionalesB (idInstancia, seccion, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12) \
													VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14);";

			//Precompilamos la orden
			codigo_error = sqlite3_prepare_v2(bbdd, insertar_caracteristica_sql, -1, &insertar_caracteristica_stmt, NULL);  

			if(codigo_error!=SQLITE_OK)
				return -1;

			//Asociamos los parámetros de la orden
			codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 1, numero_instancia);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 2, s);

			if(codigo_error!=SQLITE_OK)
				return -2;

			for(int c=0;c<12;c++)
			{
				codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, c+3, caracteristicas_direccionalesB[s][c]);

				if(codigo_error!=SQLITE_OK)
					return -2;
			}		

			//Ejecutamos la orden
			codigo_error = sqlite3_step(insertar_caracteristica_stmt);

			if(codigo_error!=SQLITE_DONE)
				return -3;		
			
			//Finalizamos la orden
			codigo_error = sqlite3_finalize(insertar_caracteristica_stmt);

			if(codigo_error!=SQLITE_OK)
				return -4;


			//Insertamos las características direccionales reales de la sección s
			insertar_caracteristica_sql = "INSERT INTO CaracteristicasDireccionalesR (idInstancia, seccion, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12) \
													VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14);";

			//Precompilamos la orden
			codigo_error = sqlite3_prepare_v2(bbdd, insertar_caracteristica_sql, -1, &insertar_caracteristica_stmt, NULL);  

			if(codigo_error!=SQLITE_OK)
				return -1;
			
			//Asociamos los parámetros de la orden
			codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 1, numero_instancia);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 2, s);

			if(codigo_error!=SQLITE_OK)
				return -2;

			for(int c=0;c<12;c++)
			{
				codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, c+3, caracteristicas_direccionalesR[s][c]);

				if(codigo_error!=SQLITE_OK)
					return -2;
			}		

			//Ejecutamos la orden
			codigo_error = sqlite3_step(insertar_caracteristica_stmt);

			if(codigo_error!=SQLITE_DONE)
				return -3;		
			
			//Finalizamos la orden
			codigo_error = sqlite3_finalize(insertar_caracteristica_stmt);

			if(codigo_error!=SQLITE_OK)
				return -4;


			//Insertamos las características estructurales de la sección s
			insertar_caracteristica_sql = "INSERT INTO CaracteristicasEstructurales (idInstancia, seccion, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12) \
													VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12, ?13, ?14);";

			//Precompilamos la consulta
			codigo_error = sqlite3_prepare_v2(bbdd, insertar_caracteristica_sql, -1, &insertar_caracteristica_stmt, NULL);  

			if(codigo_error!=SQLITE_OK)
				return -1;
			
			//Asociamos los parámetros de la orden
			codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 1, numero_instancia);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 2, s);

			if(codigo_error!=SQLITE_OK)
				return -2;

			for(int c=0;c<12;c++)
			{
				codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, c+3, caracteristicas_estructurales[s][c]);

				if(codigo_error!=SQLITE_OK)
					return -2;
			}		

			//Ejecutamos la orden
			codigo_error = sqlite3_step(insertar_caracteristica_stmt);

			if(codigo_error!=SQLITE_DONE)
				return -3;		
			
			//Finalizamos la orden
			codigo_error = sqlite3_finalize(insertar_caracteristica_stmt);

			if(codigo_error!=SQLITE_OK)
				return -4;

			
			//Insertamos las características de concavidad de la sección s
			insertar_caracteristica_sql = "INSERT INTO CaracteristicasConcavidad (idInstancia, seccion, c1, c2, c3, c4, c5, c6, c7, c8) \
													VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10);";

			
			//Precompilamos la orden
			codigo_error = sqlite3_prepare_v2(bbdd, insertar_caracteristica_sql, -1, &insertar_caracteristica_stmt, NULL);  

			if(codigo_error!=SQLITE_OK)
				return -1;

			//Asociamos los parámetros de la orden
			codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 1, numero_instancia);

			if(codigo_error!=SQLITE_OK)
				return -2;

			codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, 2, s);

			if(codigo_error!=SQLITE_OK)
				return -2;

			for(int c=0;c<8;c++)
			{
				codigo_error = sqlite3_bind_int(insertar_caracteristica_stmt, c+3, caracteristicas_concavidad[s][c]);

				if(codigo_error!=SQLITE_OK)
					return -2;
			}		

			//Ejecutamos la orden
			codigo_error = sqlite3_step(insertar_caracteristica_stmt);

			if(codigo_error!=SQLITE_DONE)
				return -3;		
			
			//Finalizamos la orden
			codigo_error = sqlite3_finalize(insertar_caracteristica_stmt);

			if(codigo_error!=SQLITE_OK)
				return -4;
		}

		return 0;
	}


	//============================================================================
	// NOMBRE: ConsultarInstancias
	//
	// DESCRIPCIÓN: Consulta que devuelve todos los id´s de las instancias de un determinado perfil.
	//
	// ARGUMENTOS: String nombre_perfil		-> Nombre del perfil de las instancias que queremos recoger
	//			   int *numero_instancias	-> Dejaremos el número de instancias de nombre_perfil existentes en la bbdd 
	//			 
	// SALIDA: Devuelve un array con todas las instancias encontradas o NULL en caso de no encontrar ninguna
	//============================================================================
	int* InterfazBBDD::ConsultarInstancias(String nombre_perfil, int *numero_instancias)
	{
		*numero_instancias = 0;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return NULL;

		//Obtenemos el número de instancias con el perfil requerido en la bbdd
		sqlite3_stmt *numero_instancias_stmt;//Consulta precompilada

		String numero_instancias_sql = "SELECT COUNT(id) FROM Instancias \
											WHERE idPerfil=?;";
		
		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, numero_instancias_sql, -1, &numero_instancias_stmt, NULL);

		//Asociamos los parámetros de la orden
		sqlite3_bind_int(numero_instancias_stmt, 1, idPerfil(nombre_perfil));	

		//Ejecutamos la consulta y recogemos el numero de instancias
		while(sqlite3_step(numero_instancias_stmt)==SQLITE_ROW)
			*numero_instancias = sqlite3_column_int(numero_instancias_stmt, 0);

		//Finalizamos la consulta
		sqlite3_finalize(numero_instancias_stmt);


		//Obtenemos todas las instancias del perfil dado
		int *instancias = new int[*numero_instancias];

		sqlite3_stmt *consultar_instancias_stmt;//Consulta precompilada

		String consultar_instancias_sql = "SELECT id FROM Instancias \
												WHERE idPerfil=?;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, consultar_instancias_sql, -1, &consultar_instancias_stmt, NULL);

		//Asociamos los parámetros de la orden
		sqlite3_bind_int(consultar_instancias_stmt, 1, idPerfil(nombre_perfil));	

		//Ejecutamos la consulta y recogemos las instancias
		for(int i=0;sqlite3_step(consultar_instancias_stmt)==SQLITE_ROW;i++)
			instancias[i] = sqlite3_column_int(consultar_instancias_stmt, 0);

		//Finalizamos la consulta
		sqlite3_finalize(consultar_instancias_stmt);

		return instancias;											
	}


	//============================================================================
	// NOMBRE: ConsultarInstanciasCaracter
	//
	// DESCRIPCIÓN: Consulta que devuelve todos los id´s de las instancias de un determinado perfil y carácter.
	//
	// ARGUMENTOS: String nombre_perfil		-> Nombre del perfil de las instancias que queremos recoger
	//			   int id_caracter			-> id del carácter de las instancias que queremos recoger	
	//			   int *numero_instancias	-> Dejaremos el número de instancias de nombre_perfil y caracter id_caracter existentes en la bbdd 
	//			 
	// SALIDA: Devuelve un array con todas las instancias encontradas o NULL en caso de no encontrar ninguna
	//============================================================================
	int* InterfazBBDD::ConsultarInstanciasCaracter(String nombre_perfil, int id_caracter, int *numero_instancias)
	{
		*numero_instancias = 0;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return NULL;

		//Obtenemos el número de instancias con el perfil requerido en la bbdd
		sqlite3_stmt *numero_instancias_stmt;//Consulta precompilada

		String numero_instancias_sql = "SELECT COUNT(id) FROM Instancias \
											WHERE idPerfil=?1 AND idCaracter=?2;";
		
		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, numero_instancias_sql, -1, &numero_instancias_stmt, NULL);

		//Asociamos los parámetros de la orden
		sqlite3_bind_int(numero_instancias_stmt, 1, idPerfil(nombre_perfil));	

		sqlite3_bind_int(numero_instancias_stmt, 2, id_caracter);

		//Ejecutamos la consulta y recogemos el numero de instancias
		while(sqlite3_step(numero_instancias_stmt)==SQLITE_ROW)
			*numero_instancias = sqlite3_column_int(numero_instancias_stmt, 0);

		//Finalizamos la consulta
		sqlite3_finalize(numero_instancias_stmt);


		//Obtenemos todas las instancias del perfil dado
		int *instancias = new int[*numero_instancias];

		sqlite3_stmt *consultar_instancias_stmt;//Consulta precompilada

		String consultar_instancias_sql = "SELECT id FROM Instancias \
												WHERE idPerfil=?1 AND idCaracter=?2;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, consultar_instancias_sql, -1, &consultar_instancias_stmt, NULL);

		//Asociamos los parámetros de la orden
		sqlite3_bind_int(consultar_instancias_stmt, 1, idPerfil(nombre_perfil));	

		sqlite3_bind_int(consultar_instancias_stmt, 2, id_caracter);

		//Ejecutamos la consulta y recogemos las instancias
		for(int i=0;sqlite3_step(consultar_instancias_stmt)==SQLITE_ROW;i++)
			instancias[i] = sqlite3_column_int(consultar_instancias_stmt, 0);

		//Finalizamos la consulta
		sqlite3_finalize(consultar_instancias_stmt);

		return instancias;											
	}


	//============================================================================
	// NOMBRE: ConsultarInstancia
	//
	// DESCRIPCIÓN: Devuelve la cadena que representa el 'carácter' asociado a la instancia, en caso de que exista.
	//
	// ARGUMENTOS: int id_caracter -> Id del carácter a obtener	   
	//
	// SALIDA: Cadena de texto que representa al 'carácter', en caso de no existir o estar 
	//		   cerrada la bbdd, devolveremos la cadena vacía
	//============================================================================
	String InterfazBBDD::ConsultarInstancia(int id_instancia)
	{
		String cadena = "", cadena_aux;

		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return cadena;	
		
		sqlite3_stmt *instancia_stmt;//Consulta precompilada

		String instancia_sql = "SELECT cadena FROM Caracteres, Instancias \
								  WHERE Instancias.id = ? AND Instancias.idCaracter = Caracteres.id;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, instancia_sql, -1, &instancia_stmt, NULL);

		//Asociamos los parámetros de la consulta
		sqlite3_bind_int(instancia_stmt, 1, id_instancia);		

		//Ejecutamos la consulta
		if(sqlite3_step(instancia_stmt)==SQLITE_ROW)		
		{
			//Recogemos el dato
			cadena_aux=(String)sqlite3_column_text(instancia_stmt, 0);	

			cadena = new char[strlen(cadena_aux) + 1];
			
			//Copiamos el string porque al finalizar la consulta sqlite lo elimina
			memcpy(cadena, cadena_aux, strlen(cadena_aux) + 1);
		}

		//Finalizamos la consulta
		sqlite3_finalize(instancia_stmt);
					
		return cadena;
	}


	//============================================================================
	// NOMBRE: ConsultarCaracteristicaBasica
	//
	// DESCRIPCIÓN: Devuelve un array con las características básicas de la instancia pasada por parámetro.
	//
	// ARGUMENTOS: int instancia			-> Id de la instancia a obtener	
	//			   double *relacion_aspecto -> Aquí devolveremos la relación de aspecto de la instancia consultada
	//
	// SALIDA: Array de bytes de 3 posiciones, una por característica básica de la instancia, en caso de no existir o estar 
	//		   cerrada la bbdd, devolveremos NULL
	//============================================================================
	BYTE* InterfazBBDD::ConsultarCaracteristicaBasica(int instancia, double *relacion_aspecto)
	{
		BYTE *caracteristicas_basicas = NULL;
		
		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return caracteristicas_basicas;

		sqlite3_stmt *consultar_caracteristicas_stmt;//Consulta precompilada

		String consultar_caracteristicas_sql = "SELECT numeroHuecos,numeroCruces, numeroTerminales  FROM CaracteristicasBasicas \
													WHERE idInstancia=?;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, consultar_caracteristicas_sql, -1, &consultar_caracteristicas_stmt, NULL);

		//Asociamos los parámetros de la consulta
		sqlite3_bind_int(consultar_caracteristicas_stmt, 1, instancia);	

		//Ejecutamos la consulta
		if(sqlite3_step(consultar_caracteristicas_stmt)==SQLITE_ROW)
		{
			caracteristicas_basicas = new BYTE[3];

			caracteristicas_basicas[0] = sqlite3_column_int(consultar_caracteristicas_stmt, 0);
			caracteristicas_basicas[1] = sqlite3_column_int(consultar_caracteristicas_stmt, 1);
			caracteristicas_basicas[2] = sqlite3_column_int(consultar_caracteristicas_stmt, 2);
			(*relacion_aspecto) = sqlite3_column_double(consultar_caracteristicas_stmt, 3);
		}

		//Finalizamos la consulta
		sqlite3_finalize(consultar_caracteristicas_stmt);

		return caracteristicas_basicas;				
	}	
	

	//============================================================================
	// NOMBRE: ConsultarCaracteristicaDireccionalB
	//
	// DESCRIPCIÓN: Devuelve un array con las características direccionales binarias de la instancia pasada por parámetro.
	//
	// ARGUMENTOS: int instancia -> Id de la instancia a obtener	   
	//
	// SALIDA: Matriz 9x12 de bytes, con las características de todas las secciones de la instancia, en caso de no existir o estar 
	//		   cerrada la bbdd, devolveremos NULL
	//============================================================================
	int** InterfazBBDD::ConsultarCaracteristicaDireccionalB(int instancia)
	{
		int **caracteristicas_direccionalesB = NULL;
		
		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return caracteristicas_direccionalesB;
		
		sqlite3_stmt *consultar_caracteristicas_stmt;//Consulta precompilada

		String consultar_caracteristicas_sql = "SELECT c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12  FROM CaracteristicasDireccionalesB \
													WHERE idInstancia=? \
													ORDER BY seccion;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, consultar_caracteristicas_sql, -1, &consultar_caracteristicas_stmt, NULL);

		//Asociamos los parámetros de la consulta
		sqlite3_bind_int(consultar_caracteristicas_stmt, 1, instancia);	

		for(int s=0;sqlite3_step(consultar_caracteristicas_stmt)==SQLITE_ROW;s++)
		{
			if(s==0)
				caracteristicas_direccionalesB = new int*[9];//Reservamos espacio para las 9 secciones de la instancia

			caracteristicas_direccionalesB[s] = new int[12];//Reservamos el espacio necesario para las características de cada sección
			
			//Recogemos los datos de la sección s
			caracteristicas_direccionalesB[s][0] = sqlite3_column_int(consultar_caracteristicas_stmt, 0);
			caracteristicas_direccionalesB[s][1] = sqlite3_column_int(consultar_caracteristicas_stmt, 1);
			caracteristicas_direccionalesB[s][2] = sqlite3_column_int(consultar_caracteristicas_stmt, 2);
			caracteristicas_direccionalesB[s][3] = sqlite3_column_int(consultar_caracteristicas_stmt, 3);
			caracteristicas_direccionalesB[s][4] = sqlite3_column_int(consultar_caracteristicas_stmt, 4);
			caracteristicas_direccionalesB[s][5] = sqlite3_column_int(consultar_caracteristicas_stmt, 5);
			caracteristicas_direccionalesB[s][6] = sqlite3_column_int(consultar_caracteristicas_stmt, 6);
			caracteristicas_direccionalesB[s][7] = sqlite3_column_int(consultar_caracteristicas_stmt, 7);
			caracteristicas_direccionalesB[s][8] = sqlite3_column_int(consultar_caracteristicas_stmt, 8);
			caracteristicas_direccionalesB[s][9] = sqlite3_column_int(consultar_caracteristicas_stmt, 9);
			caracteristicas_direccionalesB[s][10] = sqlite3_column_int(consultar_caracteristicas_stmt, 10);
			caracteristicas_direccionalesB[s][11] = sqlite3_column_int(consultar_caracteristicas_stmt, 11);
		}

		//Finalizamos la consulta
		sqlite3_finalize(consultar_caracteristicas_stmt);

		return caracteristicas_direccionalesB;				
	}
	

	//============================================================================
	// NOMBRE: ConsultarCaracteristicaDireccionalR
	//
	// DESCRIPCIÓN: Devuelve un array con las características direccionales reales de la instancia pasada por parámetro.
	//
	// ARGUMENTOS: int instancia -> Id de la instancia a obtener	   
	//
	// SALIDA: Matriz 9x12 de bytes, con las características de todas las secciones de la instancia, en caso de no existir o estar 
	//		   cerrada la bbdd, devolveremos NULL
	//============================================================================
	BYTE** InterfazBBDD::ConsultarCaracteristicaDireccionalR(int instancia)
	{
		BYTE **caracteristicas_direccionalesR = NULL;
		
		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return caracteristicas_direccionalesR;
		
		sqlite3_stmt *consultar_caracteristicas_stmt;//Consulta precompilada

		String consultar_caracteristicas_sql = "SELECT c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12  FROM CaracteristicasDireccionalesR \
													WHERE idInstancia=? \
													ORDER BY seccion;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, consultar_caracteristicas_sql, -1, &consultar_caracteristicas_stmt, NULL);

		//Asociamos los parámetros de la consulta
		sqlite3_bind_int(consultar_caracteristicas_stmt, 1, instancia);	

		for(int s=0;sqlite3_step(consultar_caracteristicas_stmt)==SQLITE_ROW;s++)
		{
			if(s==0)
				caracteristicas_direccionalesR = new BYTE*[9];//Reservamos espacio para las 9 secciones de la instancia

			caracteristicas_direccionalesR[s] = new BYTE[12];//Reservamos el espacio necesario para las características de cada sección
			
			//Recogemos los datos de la sección s
			caracteristicas_direccionalesR[s][0] = sqlite3_column_int(consultar_caracteristicas_stmt, 0);
			caracteristicas_direccionalesR[s][1] = sqlite3_column_int(consultar_caracteristicas_stmt, 1);
			caracteristicas_direccionalesR[s][2] = sqlite3_column_int(consultar_caracteristicas_stmt, 2);
			caracteristicas_direccionalesR[s][3] = sqlite3_column_int(consultar_caracteristicas_stmt, 3);
			caracteristicas_direccionalesR[s][4] = sqlite3_column_int(consultar_caracteristicas_stmt, 4);
			caracteristicas_direccionalesR[s][5] = sqlite3_column_int(consultar_caracteristicas_stmt, 5);
			caracteristicas_direccionalesR[s][6] = sqlite3_column_int(consultar_caracteristicas_stmt, 6);
			caracteristicas_direccionalesR[s][7] = sqlite3_column_int(consultar_caracteristicas_stmt, 7);
			caracteristicas_direccionalesR[s][8] = sqlite3_column_int(consultar_caracteristicas_stmt, 8);
			caracteristicas_direccionalesR[s][9] = sqlite3_column_int(consultar_caracteristicas_stmt, 9);
			caracteristicas_direccionalesR[s][10] = sqlite3_column_int(consultar_caracteristicas_stmt, 10);
			caracteristicas_direccionalesR[s][11] = sqlite3_column_int(consultar_caracteristicas_stmt, 11);
		}

		//Finalizamos la consulta
		sqlite3_finalize(consultar_caracteristicas_stmt);

		return caracteristicas_direccionalesR;				
	}
	

	//============================================================================
	// NOMBRE: ConsultarCaracteristicaEstructural
	//
	// DESCRIPCIÓN: Devuelve un array con las características direccionales reales de la instancia pasada por parámetro.
	//
	// ARGUMENTOS: int instancia -> Id de la instancia a obtener	   
	//
	// SALIDA: Matriz 9x12 de bytes, con las características de todas las secciones de la instancia, en caso de no existir o estar 
	//		   cerrada la bbdd, devolveremos NULL
	//============================================================================
	BYTE** InterfazBBDD::ConsultarCaracteristicaEstructural(int instancia)
	{
		BYTE **caracteristicas_estructurales = NULL;
		
		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return caracteristicas_estructurales;
		
		sqlite3_stmt *consultar_caracteristicas_stmt;//Consulta precompilada

		String consultar_caracteristicas_sql = "SELECT c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12  FROM CaracteristicasEstructurales \
													WHERE idInstancia=? \
													ORDER BY seccion;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, consultar_caracteristicas_sql, -1, &consultar_caracteristicas_stmt, NULL);

		//Asociamos los parámetros de la consulta
		sqlite3_bind_int(consultar_caracteristicas_stmt, 1, instancia);	

		for(int s=0;sqlite3_step(consultar_caracteristicas_stmt)==SQLITE_ROW;s++)
		{
			if(s==0)
				caracteristicas_estructurales = new BYTE*[9];//Reservamos espacio para las 9 secciones de la instancia

			caracteristicas_estructurales[s] = new BYTE[12];//Reservamos el espacio necesario para las características de cada sección
			
			//Recogemos los datos de la sección s
			caracteristicas_estructurales[s][0] = sqlite3_column_int(consultar_caracteristicas_stmt, 0);
			caracteristicas_estructurales[s][1] = sqlite3_column_int(consultar_caracteristicas_stmt, 1);
			caracteristicas_estructurales[s][2] = sqlite3_column_int(consultar_caracteristicas_stmt, 2);
			caracteristicas_estructurales[s][3] = sqlite3_column_int(consultar_caracteristicas_stmt, 3);
			caracteristicas_estructurales[s][4] = sqlite3_column_int(consultar_caracteristicas_stmt, 4);
			caracteristicas_estructurales[s][5] = sqlite3_column_int(consultar_caracteristicas_stmt, 5);
			caracteristicas_estructurales[s][6] = sqlite3_column_int(consultar_caracteristicas_stmt, 6);
			caracteristicas_estructurales[s][7] = sqlite3_column_int(consultar_caracteristicas_stmt, 7);
			caracteristicas_estructurales[s][8] = sqlite3_column_int(consultar_caracteristicas_stmt, 8);
			caracteristicas_estructurales[s][9] = sqlite3_column_int(consultar_caracteristicas_stmt, 9);
			caracteristicas_estructurales[s][10] = sqlite3_column_int(consultar_caracteristicas_stmt, 10);
			caracteristicas_estructurales[s][11] = sqlite3_column_int(consultar_caracteristicas_stmt, 11);
		}

		//Finalizamos la consulta
		sqlite3_finalize(consultar_caracteristicas_stmt);

		return caracteristicas_estructurales;				
	}
	

	//============================================================================
	// NOMBRE: ConsultarCaracteristicaConcavidad
	//
	// DESCRIPCIÓN: Devuelve un array con las características direccionales reales de la instancia pasada por parámetro.
	//
	// ARGUMENTOS: int instancia -> Id de la instancia a obtener	   
	//
	// SALIDA: Matriz 9x8 de bytes, con las características de todas las secciones de la instancia, en caso de no existir o estar 
	//		   cerrada la bbdd, devolveremos NULL
	//============================================================================
	BYTE** InterfazBBDD::ConsultarCaracteristicaConcavidad(int instancia)
	{
		BYTE **caracteristicas_concavidad = NULL;
		
		if(!bbdd_abierta)//Comprobamos si existe una bbdd abierta
			return caracteristicas_concavidad;
		
		sqlite3_stmt *consultar_caracteristicas_stmt;//Consulta precompilada

		String consultar_caracteristicas_sql = "SELECT c1, c2, c3, c4, c5, c6, c7, c8  FROM CaracteristicasConcavidad \
													WHERE idInstancia=? \
													ORDER BY seccion;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, consultar_caracteristicas_sql, -1, &consultar_caracteristicas_stmt, NULL);

		//Asociamos los parámetros de la consulta
		sqlite3_bind_int(consultar_caracteristicas_stmt, 1, instancia);	

		for(int s=0;sqlite3_step(consultar_caracteristicas_stmt)==SQLITE_ROW;s++)
		{
			if(s==0)
				caracteristicas_concavidad = new BYTE*[9];//Reservamos espacio para las 9 secciones de la instancia

			caracteristicas_concavidad[s] = new BYTE[8];//Reservamos el espacio necesario para las características de cada sección
			
			//Recogemos los datos de la sección s
			caracteristicas_concavidad[s][0] = sqlite3_column_int(consultar_caracteristicas_stmt, 0);
			caracteristicas_concavidad[s][1] = sqlite3_column_int(consultar_caracteristicas_stmt, 1);
			caracteristicas_concavidad[s][2] = sqlite3_column_int(consultar_caracteristicas_stmt, 2);
			caracteristicas_concavidad[s][3] = sqlite3_column_int(consultar_caracteristicas_stmt, 3);
			caracteristicas_concavidad[s][4] = sqlite3_column_int(consultar_caracteristicas_stmt, 4);
			caracteristicas_concavidad[s][5] = sqlite3_column_int(consultar_caracteristicas_stmt, 5);
			caracteristicas_concavidad[s][6] = sqlite3_column_int(consultar_caracteristicas_stmt, 6);
			caracteristicas_concavidad[s][7] = sqlite3_column_int(consultar_caracteristicas_stmt, 7);		
		}

		//Finalizamos la consulta
		sqlite3_finalize(consultar_caracteristicas_stmt);

		return caracteristicas_concavidad;				
	}


	//============================================================================
	// NOMBRE: idPerfil
	//
	// DESCRIPCIÓN: Devuelve la id del perfil pasado por parámetro.
	//
	// ARGUMENTOS: String nombre -> Nombre del perfil del que queremos obtener la id	   
	//
	// SALIDA: Id del perfil deseado, en caso de que éste no exista devolveremos -1
	//============================================================================
	int InterfazBBDD::idPerfil(String nombre)
	{
		int id_perfil=-1;

		sqlite3_stmt *id_perfil_stmt;//Consulta precompilada

		String id_perfil_sql = "SELECT id FROM Perfiles \
								  WHERE nombre = ?;";

		//Precompilamos la consulta
		sqlite3_prepare_v2(bbdd, id_perfil_sql, -1, &id_perfil_stmt, NULL);

		//Asociamos los parámetros de la consulta
		sqlite3_bind_text(id_perfil_stmt, 1, nombre, -1, SQLITE_STATIC);

		//Ejecutamos la consulta
		if(sqlite3_step(id_perfil_stmt)==SQLITE_ROW)
			id_perfil = sqlite3_column_int(id_perfil_stmt, 0);

		//Finalizamos la consulta
		sqlite3_finalize(id_perfil_stmt);
					
		return id_perfil;
	}


	//============================================================================
	// NOMBRE: idCaracter
	//
	// DESCRIPCIÓN: Devuelve la id del 'carácter' pasado por parámetro.
	//
	// ARGUMENTOS: String cadena -> Cadena de texto del 'caracter' del que queremos obtener la id	   
	//
	// SALIDA: Id del 'carácter' deseado, en caso de que éste no exista devolveremos -1
	//============================================================================
	int InterfazBBDD::idCaracter(String cadena)
	{
		int id_caracter=-1;

		sqlite3_stmt *id_caracter_stmt;//Consulta precompilada

		String id_caracter_sql = "SELECT id FROM Caracteres \
									WHERE cadena = ?;";

		//Precompilamos la consulta
		int cod_err = sqlite3_prepare_v2(bbdd, id_caracter_sql, -1, &id_caracter_stmt, NULL);

		//Asociamos los parámetros de la consulta
		cod_err = sqlite3_bind_text(id_caracter_stmt, 1, cadena, -1, SQLITE_STATIC);

		//Ejecutamos la consulta
		if(sqlite3_step(id_caracter_stmt)==SQLITE_ROW)
			id_caracter = sqlite3_column_int(id_caracter_stmt, 0);

		//Finalizamos la consulta
		sqlite3_finalize(id_caracter_stmt);
					
		return id_caracter;
	}	

}

