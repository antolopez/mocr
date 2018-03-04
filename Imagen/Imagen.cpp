// Archivo DLL principal.
#include "stdafx.h"
#include <string.h>
#include <math.h>
#include "Imagen.h"


#include <fstream>
#include <string>

using namespace std;


namespace Imagen
{

	//============================================================================
	// NOMBRE: Texto
	//
	// DESCRIPCI�N: Constructor por defecto. 				
	//
	// ARGUMENTOS:	-
	//
	// SALIDA: -
	//============================================================================
	Texto::Texto(void)
	{
		//Asignamos todos los elementos de la imagen a NULL
		inicio_DIB = NULL;
		bitmapinfoheader = NULL;
		bitmapinfo = NULL;
		paleta = NULL;
		mapa_bits = NULL;
		umbralizada = false;
		nombreImagen = "";

		//Elementos de segmentaci�n a NULL
		lineas = NULL;
		numeroLineas = 0;

		//Elementos de reconocimiento a NULL
		perfil = ""; 
		rutaBBDD = "";
		clasificador = NULL;
		bbdd = NULL;	
		textoReconocido = "";
		numeroVecinos = 1;
	}


	//============================================================================
	// NOMBRE: Texto
	//
	// DESCRIPCI�N: Constructor que crea una imagen no segmentada, con las dimensiones y colores especificados.				
	//
	// ARGUMENTOS:	int columnas			-> N� de columnas de la imagen
	//				int filas				-> N� de filas de la imagen
	//				int profundidad			-> Bits/p�xel de la imagen
	//				RGBQUAD *paleta_origen	-> Puntero a la paleta que queremos copiar, si es NULL se reservar� espacio para la paleta, pero no se copia
	//				int tamano_paleta		-> tama�o de la paleta que queremso copiar.
	//
	// SALIDA: -
	//============================================================================
	Texto::Texto(int columnas, int filas, int profundidad, RGBQUAD *paleta_origen, int tamano_paleta)
	{
		//Propiedades de la imagen
		nombreImagen = "";
		umbralizada = false;

		//Elementos de segmentaci�n a NULL
		lineas = NULL;
		numeroLineas = 0;

		//Elementos de reconocimiento a NULL
		perfil = ""; 
		rutaBBDD = "";
		clasificador = NULL;
		bbdd = NULL;	
		textoReconocido = "";

		//Creamos la imagen
		//Calculamos el n� de bytes necesarios para el mapa de bits
		int tamano_fila = columnas*profundidad/8;
		int columnas_relleno = 0;
		if((tamano_fila % 4)!=0)
			columnas_relleno = 4 - (tamano_fila % 4);
		
		int tamano_mapa_bits = filas * (tamano_fila + columnas_relleno);
			
		DWORD tamano_DIB = sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD) + tamano_mapa_bits;
		inicio_DIB=new BYTE[tamano_DIB];//Reservamos memoria para el DIB

		//Asignamos punteros
		bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
		bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
		if(tamano_paleta!=0)
			paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
		else//No hay paleta
			paleta=NULL;

		if(paleta!=NULL)
			mapa_bits = ((BYTE*)paleta + tamano_paleta * sizeof(RGBQUAD));
		else//No hay paleta
			mapa_bits = ((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
		//Fin asignaci�n punteros
		
		//Creamos la estructura BITMAPINFOHEADER del DIB
		bitmapinfoheader->biSize = sizeof(BITMAPINFOHEADER);
		bitmapinfoheader->biWidth = columnas;
		bitmapinfoheader->biHeight = filas;
		bitmapinfoheader->biPlanes = 1;
		bitmapinfoheader->biBitCount = profundidad;
		bitmapinfoheader->biCompression = BI_RGB;
		bitmapinfoheader->biSizeImage = tamano_mapa_bits;
		bitmapinfoheader->biXPelsPerMeter = 0;
		bitmapinfoheader->biYPelsPerMeter = 0;
		bitmapinfoheader->biClrUsed = tamano_paleta;
		bitmapinfoheader->biClrImportant = 0;
		//FIN estructura BITMAPINFOHEADER		

		if(paleta!=NULL && paleta_origen!=NULL)//Copiamos la paleta
			memcpy(paleta, paleta_origen, tamano_paleta * sizeof(RGBQUAD));

		memset(mapa_bits, 255, tamano_mapa_bits);//Inicializamos el mapa de bits
	}


	//============================================================================
	// NOMBRE: ~Texto
	//
	// DESCRIPCI�N: Destructor. 				
	//
	// ARGUMENTOS:	-
	//
	// SALIDA: -
	//============================================================================
	Texto::~Texto(void)
	{
		//Borramos el mapa de bits
		if(inicio_DIB!=NULL)
			delete inicio_DIB;

		//ARREGLAR ESTO Y BORRAR LOS DEM�S STRINGS
		//Borramos el nombre de la imagen
		//delete nombreImagen;

		//Borramos la informaci�n de segmentaci�n
		if(lineas!=NULL)
		{
			for(int l=0; l<numeroLineas; l++)
			{
				if(lineas[l].palabras!=NULL)
				{
					for(int p=0; p<lineas[l].numeroPalabras; p++)
					{
						if(lineas[l].palabras[p].caracteres!=NULL)
							delete lineas[l].palabras[p].caracteres;
					}
					
					delete lineas[l].palabras;
				}				
			}

			delete lineas;
		}

		//Borramos los objetos necesarios para el reconocimiento
		if(clasificador!=NULL)
			delete clasificador;
		
		if(bbdd!=NULL)
			delete bbdd;
	}


	//============================================================================
	// NOMBRE: =
	//
	// DESCRIPCI�N: Sobrecarga operador =				
	//
	// ARGUMENTOS:	-
	//
	// SALIDA: -
	//============================================================================
	Texto& Texto::operator=(const Texto& origen)
	{
		//Copiamos propiedades imagen
		umbralizada = origen.umbralizada;
		//Copiamos el nombre de la imagen
		nombreImagen = new char[strlen(origen.nombreImagen)+1];
		strcpy(nombreImagen, origen.nombreImagen); 
			
		//Copiamos la imagen en s�
		if(origen.inicio_DIB!= NULL)
		{
			//Calculamos tama�o paleta
			int tamano_paleta = 0;

			if(origen.bitmapinfoheader->biBitCount<=8)
			{
				if(origen.bitmapinfoheader->biClrUsed!=0)
					tamano_paleta = origen.bitmapinfoheader->biClrUsed;
				else
					tamano_paleta = (int)pow(2.0, origen.bitmapinfoheader->biBitCount);
			}
			//Fin c�lculo tama�o paleta
						
			DWORD tamano_DIB = sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD) + origen.bitmapinfoheader->biSizeImage;

			//Borramos por si estamos reutilizando un aimagen anterior
			//Borramos el mapa de bits
			if(inicio_DIB!=NULL)
				delete inicio_DIB;

			//Borramos la informaci�n de segmentaci�n
			if(lineas!=NULL)
			{
				for(int l=0; l<numeroLineas; l++)
				{
					if(lineas[l].palabras!=NULL)
					{
						for(int p=0; p<lineas[l].numeroPalabras; p++)
						{
							if(lineas[l].palabras[p].caracteres!=NULL)
								delete lineas[l].palabras[p].caracteres;
						}
						
						delete lineas[l].palabras;
					}				
				}

				delete lineas;
			}

			inicio_DIB = new BYTE[tamano_DIB];//reservamos memoria para el DIB

			//Asignaci�n de punteros
			bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
			bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
			if(tamano_paleta!=0)
				paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
			else//No hay paleta
				paleta = NULL;

			mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
			//Fin de asignaci�n de punteros
				
			memcpy(inicio_DIB, origen.inicio_DIB, tamano_DIB);//Copiamos el DIB

			//Copiamos la informaci�n de segmentaci�n
			if(origen.lineas!=NULL)
			{
				numeroLineas = origen.numeroLineas;

				lineas = new Linea[numeroLineas];

				memcpy(lineas, origen.lineas, numeroLineas * sizeof(Linea));

				for(int l=0; l<origen.numeroLineas; l++)
				{
					if(origen.lineas[l].palabras!=NULL)
					{
						Palabra *palabras = new Palabra[origen.lineas[l].numeroPalabras];
						memcpy(palabras, origen.lineas[l].palabras, origen.lineas[l].numeroPalabras * sizeof(Palabra));

						lineas[l].numeroPalabras = origen.lineas[l].numeroPalabras;
						lineas[l].palabras=palabras;

						for(int p=0; p<origen.lineas[l].numeroPalabras; p++)
						{
							if(origen.lineas[l].palabras[p].caracteres!=NULL)
							{
								Caracter *caracteres = new Caracter[origen.lineas[l].palabras[p].numeroCaracteres];
								memcpy(caracteres, origen.lineas[l].palabras[p].caracteres, origen.lineas[l].palabras[p].numeroCaracteres * sizeof(Caracter));

								lineas[l].palabras[p].numeroCaracteres = origen.lineas[l].palabras[p].numeroCaracteres;
								lineas[l].palabras[p].caracteres=caracteres;
							}
						}
					}
				}
			}
			else
			{
				lineas = NULL;
				numeroLineas = 0;
			}
		}

		return *this;
	}		


	//============================================================================
	// NOMBRE: GetDIB
	//
	// DESCRIPCI�N: Devuelve la direcci�n del DIB en memoria				
	//
	// ARGUMENTOS:	-
	//
	// SALIDA: Direcci�n del DIB en memoria	como BYTE*
	//============================================================================
	BYTE* Texto::GetDIB()
	{
		return inicio_DIB;
	}


	//============================================================================
	// NOMBRE: SetDIB
	//
	// DESCRIPCI�N: Establece el DIB de una imagen				
	//
	// ARGUMENTOS:	BYTE *DIB -> Direcci�n del DIB a guardar
	//
	// SALIDA: -
	//============================================================================
	void Texto::SetDIB(BYTE *DIB)
	{
		//Establecemos los punteros iniciales
		inicio_DIB = DIB;
		bitmapinfo = (LPBITMAPINFO)DIB;
		bitmapinfoheader = (LPBITMAPINFOHEADER)DIB;

		//Si el tama�o de la imagen no est� calculado lo hacemos nosotros
		if(bitmapinfoheader->biSizeImage==0)
		{
			int tamano_elemento = bitmapinfoheader->biBitCount / 8;
			int tamano_fila = bitmapinfoheader->biWidth * tamano_elemento;
			
			//Calculamos las columnas de relleno
			int columnas_relleno = 0;
			if(tamano_fila%4!=0)
				columnas_relleno=4-(tamano_fila%4);

			//Establecemos el tama�o de la imagen
			bitmapinfoheader->biSizeImage = bitmapinfoheader->biHeight * (tamano_fila + columnas_relleno);
		}
	
		//Calculos necesarios para conocer el tama�o de la paleta
		int colores_usados;
		if(bitmapinfoheader->biClrUsed==0)//Se usan todos los colores posibles
			colores_usados = (int)pow(2.0, bitmapinfoheader->biBitCount);
		else
			colores_usados = bitmapinfoheader->biClrUsed;

		
		//Asignamos puntero de paleta y de mapa de bits
		if(bitmapinfoheader->biBitCount<=8)//256 niveles o menos
		{			
			paleta = bitmapinfo->bmiColors;
			mapa_bits = (BYTE*)bitmapinfoheader + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colores_usados;
		}
		else
		{
			paleta = NULL;//no hay paleta
			mapa_bits = (BYTE*)bitmapinfoheader + sizeof(BITMAPINFOHEADER);
		}
	}


	//============================================================================
	// NOMBRE: GetMapaBits
	//
	// DESCRIPCI�N: Devuelve la direcci�n del mapa de bits en memoria				
	//
	// ARGUMENTOS:	-
	//
	// SALIDA: Direcci�n del mapa de bits en memoria como BYTE*
	//============================================================================
	BYTE* Texto::GetMapaBits()
	{
		return mapa_bits;
	}


	//============================================================================
	// NOMBRE: GetNombreImagen
	//
	// DESCRIPCI�N: Retorna el nombre de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Una cadena de texto que indica el nombre de la imagen con extensi�n incluida			
	//============================================================================
	String Texto::GetNombreImagen()
	{
		return nombreImagen;
	}


	//============================================================================
	// NOMBRE: SetNombreImagen
	//
	// DESCRIPCI�N: Modifica el nombre de la imagen.
	//
	// ARGUMENTOS: String nombre -> Nuevo nombre para la imagen (con extensi�n incluida)
	//
	// SALIDA: -			
	//============================================================================
	void Texto::SetNombreImagen(String nombre)
	{
		nombreImagen = nombre;
	}
	

	//============================================================================
	// NOMBRE: GetPerfil
	//
	// DESCRIPCI�N: Retorna el asociado al texto de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Una cadena de texto que indica el perfil asociado al texto de la imagen			
	//============================================================================
	String Texto::GetPerfil()
	{
		return perfil;
	}


	//============================================================================
	// NOMBRE: SetPerfil
	//
	// DESCRIPCI�N: Modifica el perfil asociado al texto de la imagen.
	//
	// ARGUMENTOS: String nombre -> Perfil del texto asociado a la imagen
	//
	// SALIDA: -			
	//============================================================================
	void Texto::SetPerfil(String nombre)
	{
		perfil = nombre;
	}


	//============================================================================
	// NOMBRE: GetRutaBBDD
	//
	// DESCRIPCI�N: Retorna la ruta de la bbdd de los patrones.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Una cadena de texto que indica la ruta de la bbdd			
	//============================================================================
	String Texto::GetRutaBBDD()
	{
		return rutaBBDD;
	}


	//============================================================================
	// NOMBRE: SetRutaBBDD
	//
	// DESCRIPCI�N: Modifica la ruta de la bbdd de los patrones.
	//
	// ARGUMENTOS: String ruta -> Ruta de la bbdd
	//
	// SALIDA: -			
	//============================================================================
	void Texto::SetRutaBBDD(String ruta)
	{
		rutaBBDD = ruta;
	}


	//============================================================================
	// NOMBRE: GetTextoReconocido
	//
	// DESCRIPCI�N: Retorna el texto que ha sido reconocido o aprendido.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: String con el supuesto texto contenido en la imagen	
	//============================================================================
	String Texto::GetTextoReconocido()
	{
		return textoReconocido;
	}


	//============================================================================
	// NOMBRE: SetTextoReconocido
	//
	// DESCRIPCI�N: Modifica el texto reconocido o aprendido.
	//
	// ARGUMENTOS: String texto -> Texto reconocido o aprendido
	//
	// SALIDA: -			
	//============================================================================
	void Texto::SetTextoReconocido(String texto)
	{
		textoReconocido = texto;
	}


	//============================================================================
	// NOMBRE: GetNumeroFilas
	//
	// DESCRIPCI�N: Retorna el n�mero de filas de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor entero que indica el n�mero de filas			
	//============================================================================
	int Texto::GetNumeroFilas()
	{
		return bitmapinfoheader->biHeight;
	}


	//============================================================================
	// NOMBRE: GetNumeroColumnas
	//
	// DESCRIPCI�N: Retorna el n�mero de columnas de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor entero que indica el n�mero de columnas			
	//============================================================================
	int Texto::GetNumeroColumnas()
	{
		return bitmapinfoheader->biWidth;
	}
	
	
	//============================================================================
	// NOMBRE: GetTamano
	//
	// DESCRIPCI�N: Retorna el tama�o de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor entero que indica el tama�o de la imagen en bytes			
	//============================================================================
	int Texto::GetTamano()
	{
		return bitmapinfoheader->biSizeImage;
	}


	//============================================================================
	// NOMBRE: GetProfundidad
	//
	// DESCRIPCI�N: Retorna la profundidad de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor entero que indica la profundidad en bits/p�xel			
	//============================================================================
	int Texto::GetProfundidad()
	{
		return bitmapinfoheader->biBitCount;
	}

	
	//============================================================================
	// NOMBRE: EstaUmbralizada
	//
	// DESCRIPCI�N: Comprueba si la imagen est� umbralizada.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor booleano que indica:
	//			true  -> La imagen est� umbralizada
	//			false -> La imagen no est� umbralizada	
	//============================================================================
	bool Texto::EstaUmbralizada()
	{
		//En principio suponemos que la imagen no est�aumbralizada
		umbralizada = false;

		//Calculamos el n�mero de lementos de la paleta
		int tamano_paleta = 0;

		if(bitmapinfoheader->biBitCount<=8)
		{
			if(bitmapinfoheader->biClrUsed!=0)
				tamano_paleta = bitmapinfoheader->biClrUsed;
			else
				tamano_paleta = (int)pow(2.0, bitmapinfoheader->biBitCount);
		}
		//Fin calculo tama�o paleta

		int i;

		for(i=0; i<tamano_paleta && paleta[i].rgbBlue==paleta[i].rgbGreen && paleta[i].rgbGreen==paleta[i].rgbRed; i++);

		if(i!=tamano_paleta || tamano_paleta==0)//tamano_paleta==0, por si la imagen no tiene paleta
			return false;

		//Est� en niveles de gris, ahora comprobamos que est� umbralizada
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
				if(mapa_bits[posicion(i,j,1)]!=0 && mapa_bits[posicion(i,j,1)]!=255)
					return false;

		//La imagen est� umbralizada
		//Actualizamos la propiedad de umbralizada
		umbralizada = true;
		return true;
	}


	//============================================================================
	// NOMBRE: ExisteCoordenada
	//
	// DESCRIPCI�N: Comprueba si una coordenada pertenece a la imagen.
	//
	// ARGUMENTOS:	int columna	->  Componente columna de la coordenada
	//				int fila	->  Componente fila de la coordenada
	//
	// SALIDA: Un valor booleano que indica:
	//			true  -> La coordenada pertenece a la imagen
	//			false -> La coordenada NO pertenece a la imagen
	//============================================================================
	bool Texto::ExisteCoordenada(int columna, int fila)
	{
		return (columna>=0) && (columna<bitmapinfoheader->biWidth) && (fila>=0) && (fila<bitmapinfoheader->biHeight);
	}

	//============================================================================
	// NOMBRE: ExisteCoordenada
	//
	// DESCRIPCI�N: Comprueba si una coordenada pertenece al segmento.
	//
	// ARGUMENTOS:	int columna			->  Componente columna de la coordenada
	//				int fila			->  Componente fila de la coordenada
	//				Palabra segmento	->  Segmento de la imagen
	//
	// SALIDA: Un valor booleano que indica:
	//			true  -> La coordenada pertenece al segmento
	//			false -> La coordenada NO pertenece al segmento	
	//============================================================================
	bool Texto::ExisteCoordenada(int columna, int fila, Palabra segmento)
	{
		return (columna>=segmento.esquina_superior_izquierda.x) && (columna<=segmento.esquina_inferior_derecha.x) && (fila>=segmento.esquina_superior_izquierda.y) && (fila<=segmento.esquina_inferior_derecha.y);
	}


	//============================================================================
	// NOMBRE: EsIgual
	//
	// DESCRIPCI�N: Comprueba si la imagen (256 colores, si no se devolver� false autom�ticamente) actual es igual a la pasada por par�metro (en dimensiones y valores).
	//
	// ARGUMENTOS:	Texto *imagen -> Imagen de 256 colores, si no se devolver� false autom�ticamente
	//
	// SALIDA: Un valor booleano que indica:
	//			true  -> Las im�genes no son iguales
	//			false -> Las im�genes no son iguales o alguna no es de 8 bits de profundidad
	//============================================================================
	bool Texto::EsIgual(Texto *imagen)
	{
		//Comprobamos las profundidades de las im�genes
		if(bitmapinfoheader->biBitCount!=8 || imagen->bitmapinfoheader->biBitCount!=8)
			return false;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=imagen->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=imagen->bitmapinfoheader->biWidth)
			return false;

		//Comprobamos los valores del mapa de bits
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				//Al primer p�xel distinto que encontremos devolvemos false
				if(mapa_bits[posicion(i, j, 1)]!=imagen->mapa_bits[posicion(i, j, 1)])
					return false;
			}

		//Las im�genes son iguales
		return true;
	}


	//======================================================================
	// NOMBRE: Escalacion
	//
	// DESCRIPCI�N: Escala una imagen.
	//
	// ARGUMENTOS:	double factor_columnas  -> Factor de redimensionado del ancho de la imagen
	//				double factor_filas		-> Factor de redimensionado del alto de la imagen
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Factores negativos no permitidos
	//			2 -> Error. Imagen no es de 1, 4, 8 � 24 bits/p�xel
	//======================================================================
	int Texto::Escalacion(double factor_columnas, double factor_filas)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos que los factores de escalado sean positivos
		if(factor_columnas<0 || factor_filas<0)
			return 1;

		//Comprobamos que la profundidad de la imagen est� soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;

		if(factor_columnas!=1 || factor_filas!=1)//Si los dos son 1 no hay que escalar nada
		{
			int tamano_pixel = profundidad/8;

			//Calculamos el tama�o de la paleta
			int tamano_paleta = 0;
			if(bitmapinfoheader->biBitCount<=8)
			{
				if(bitmapinfoheader->biClrUsed!=0)
					tamano_paleta = bitmapinfoheader->biClrUsed;
				else
					tamano_paleta = (int)pow(2.0, bitmapinfoheader->biBitCount);
			}

			//Creamos la imagen donde guardaremos la imagen actual escalada
			Texto *imagen_auxiliar = new Texto((int)(bitmapinfoheader->biWidth * factor_columnas), (int)(bitmapinfoheader->biHeight * factor_filas), profundidad, paleta, tamano_paleta);
			
			//Hacemos el escalado viendo donde cae cada pixel de la imagen escalada en la imagen auxiliar
			for(int i=0; i<imagen_auxiliar->bitmapinfoheader->biHeight; i++)
				for(int j=0; j<imagen_auxiliar->bitmapinfoheader->biWidth; j++)
				{
					int coordenadaX = (int)(j / factor_columnas);
					int coordenadaY = (int)(i / factor_filas);

					if(ExisteCoordenada(coordenadaX, coordenadaY))
					{
						//Si existe la coordenada en la imagen actual cogemos el valor del pixel
						int posicion_imagen_this = posicion(coordenadaY, coordenadaX, tamano_pixel);
						memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)], &mapa_bits[posicion_imagen_this], tamano_pixel);
					}
				}
				
				//Sustituimos la imagen actual por la imagen escalada
				delete inicio_DIB;//boramos imagen oiginal

				//Reasignaci�n punteos
				inicio_DIB = imagen_auxiliar->inicio_DIB;
				bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
				bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
				if(imagen_auxiliar->paleta!=NULL)
					paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
				else//No hay paleta
					paleta = NULL;
				mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
				//Fin reasignaci�n punteros
		}

		return 0;
	}


	//======================================================================
	// NOMBRE: EscalacionLineal
	//
	// DESCRIPCI�N: Escala una imagen con interpolaci�n lineal.
	//
	// ARGUMENTOS:	double factor_columnas  -> Factor de redimensionado del ancho de la imagen
	//				double factor_filas		-> Factor de redimensionado del alto de la imagen
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Factores negativos no permitidos
	//			2 -> Error. Imagen no es de 1, 4, 8 � 24 bits/p�xel
	//======================================================================
	int Texto::EscalacionLineal(double factor_columnas, double factor_filas)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos que los factores de escalado sean positivos
		if(factor_columnas<0 || factor_filas<0)
			return 1;

		//Comprobamos que la profundidad de la imagen est� soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;

		if(factor_columnas!=1 || factor_filas!=1)//Si los dos son 1 no hay que escalar nada
		{
			int tamano_pixel = profundidad/8;

			//Calculamos el tama�o de la paleta
			int tamano_paleta = 0;
			if(bitmapinfoheader->biBitCount<=8)
			{
				if(bitmapinfoheader->biClrUsed!=0)
					tamano_paleta = bitmapinfoheader->biClrUsed;
				else
					tamano_paleta = (int)pow(2.0, bitmapinfoheader->biBitCount);
			}

			//Creamos la imagen donde guardaremos la imagen actual escalada
			Texto *imagen_auxiliar = new Texto((int)(bitmapinfoheader->biWidth * factor_columnas), (int)(bitmapinfoheader->biHeight * factor_filas), profundidad, paleta, tamano_paleta);
			
			//Hacemos el escalado viendo donde cae cada pixel de la imagen escalada en la imagen auxiliar
			for(int i=0; i<imagen_auxiliar->bitmapinfoheader->biHeight; i++)
				for(int j=0; j<imagen_auxiliar->bitmapinfoheader->biWidth; j++)
				{
					double coordenadaX = (j / factor_columnas);
					double coordenadaY = (i / factor_filas);

					if(ExisteCoordenada((int)coordenadaX, (int)coordenadaY))
					{
						int l;
						for(l=0; l<numeroLineas; l++)
						{
							if(lineas[l].esquina_superior_izquierda.y-3==(int)coordenadaY)
								break; 
							
							if(lineas[l].esquina_inferior_derecha.y+3==(int)coordenadaY)
								break;							
						}

						if(l==numeroLineas)
						{
							//Si existe la coordenada en la imagen actual cogemos el valor del pixel
							int posicion_imagen_this = posicion((int)coordenadaY, (int)coordenadaX, tamano_pixel);
							memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)], &mapa_bits[posicion_imagen_this], tamano_pixel);
						}
						else
						{
							//Si existe la coordenada en la imagen actual cogemos el valor del pixel
							int posicion_imagen_this = posicion((int)coordenadaY, (int)coordenadaX, tamano_pixel);
							memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)], 255, tamano_pixel);
						}

						
						/*//Si existe la coordenada en la imagen actual cogemos el valor del pixel
						int posicion_imagen_this = posicion((int)coordenadaY, (int)coordenadaX, tamano_pixel);
						//memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)], &mapa_bits[posicion_imagen_this], tamano_pixel);
						
						int l = redondear(coordenadaX);
						int k = redondear(coordenadaY);

						double a = coordenadaX - l;
						double b = coordenadaY - k;
						
						double nivel_gris = (1-a) * (1-b) * mapa_bits[posicion_imagen_this] + a * (1-b) * mapa_bits[posicion((int)coordenadaY, (int)coordenadaX+1, tamano_pixel)] + (1-a) * (b) * mapa_bits[posicion((int)coordenadaY+1, (int)coordenadaX, tamano_pixel)] + a  * b * mapa_bits[posicion((int)coordenadaY+1, (int)coordenadaX+1, tamano_pixel)];
						
						if(nivel_gris>=128)
							imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)] = 255;
						else
							imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)] = 0;*/

					}					
				}
			
				
				//Sustituimos la imagen actual por la imagen escalada
				delete inicio_DIB;//boramos imagen oiginal

				//Reasignaci�n punteos
				inicio_DIB = imagen_auxiliar->inicio_DIB;
				bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
				bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
				if(imagen_auxiliar->paleta!=NULL)
					paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
				else//No hay paleta
					paleta = NULL;
				mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
				//Fin reasignaci�n punteros
		}

		for(int l=0; l<numeroLineas; l++)
		{
			lineas[l].esquina_superior_izquierda.y *= factor_filas; 
			lineas[l].esquina_inferior_derecha.y *= factor_filas;

			lineas[l].esquina_inferior_derecha.x = bitmapinfoheader->biWidth - 1;

			memset(&mapa_bits[posicion(lineas[l].esquina_superior_izquierda.y, 0, 1)], 0, bitmapinfoheader->biWidth);
			memset(&mapa_bits[posicion(lineas[l].esquina_inferior_derecha.y, 0, 1)], 0, bitmapinfoheader->biWidth);
		}

		return 0;
	}

	
	//======================================================================
	// NOMBRE: Rotacion
	//
	// DESCRIPCI�N: Rota una imagen.
	//
	// ARGUMENTOS:	int modo	  ->  Indica modo de rotaci�n
	//										TAMANO_ORIGINAL 0
	//										NUEVO_TAMANO	1
	//				double grados ->  Indica el n� de grados  si <0 ---> izquierda ; si >0 ---> derecha
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Grados no v�lidos
	//			2 -> Error. Imagen no es de 1, 4, 8 � 24 bits/p�xel
	//			3 -> Error.  Valor de Modo no v�lido
	//======================================================================
	int Texto::Rotacion(int modo, double grados)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos si el valor de los grados es v�lido 
		if(fabs(grados)>360)
			return 1;

		//Comprobamos que la profundidad de la imagen est� soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;
		
		if(grados!=0 && grados!=360)//Si son 0 grados o 360 no hacemos nada
		{
			int tamano_pixel = profundidad / 8;

			//Calculamos el tama�o de la paleta
			int tamano_paleta = 0;
			if(bitmapinfoheader->biBitCount<=8)
			{
				if(bitmapinfoheader->biClrUsed!=0)
					tamano_paleta = bitmapinfoheader->biClrUsed;
				else
					tamano_paleta=(int)pow(2.0, bitmapinfoheader->biBitCount);
			}

			//Filtramos el modo
			switch (modo)
			{
				case TAMANO_ORIGINAL://Como no lo vamos a usar en la aplicaci�n dejamos la versi�n sin optimizar completamente, pero mucho m�s clara
				{
					//Creamos la imagen donde guardaremos la imagen actual rotada
					Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight, profundidad, paleta, tamano_paleta);
				
					//Calculamos seno y coseno
					double angulo_radianes = grados * (PI / 180);
					double seno = sin(angulo_radianes);
					double coseno = cos(angulo_radianes);
					
					//Obtenemos las coordenadas del centro de la imagen auxiliar
					int centrox=(imagen_auxiliar->bitmapinfoheader->biWidth - 1) / 2;
					int centroy=(imagen_auxiliar->bitmapinfoheader->biHeight - 1) / 2;
					
					//Realizamos la rotaci�n
					for(int i=0; i<imagen_auxiliar->bitmapinfoheader->biHeight; i++)
					{
						//Calculamos los datos que nos valdr�n para toda la fila (independientes de la columna)
						int y = i - centroy;
						double cosenoy = coseno * y;
						double senoy = seno * y;

						for(int j=0; j<imagen_auxiliar->bitmapinfoheader->biWidth; j++)
						{
							//Calculo de los datos que dependen de la columna
							int x = j - centrox;
														
							//Coordenadas del pixel (i,j) de la imagen rotada en la imagen actual
							int coordenadaX;
							int coordenadaY;
													
							if(grados!=90 && grados!=180 && grados!=270)//Con esta divisi�n se obtienen mejores resultados
							{
								coordenadaX = (int)(/*(int)*/(coseno * x) + /*(int)*/(senoy) + centrox);
								coordenadaY = (int)(/*(int)*/(-seno * x) + /*(int)*/(cosenoy) + centroy);
							}
							else
							{
								coordenadaX = (int)(coseno * x) + (int)(senoy) + centrox;
								coordenadaY = (int)(-seno * x) + (int)(cosenoy) + centroy;
							}
											
							if(ExisteCoordenada(coordenadaX, coordenadaY))
							{
								//Si existe la coordenada en la imagen actual cogemos el valor del pixel
								int posicion_imagen_this = posicion(coordenadaY, coordenadaX, tamano_pixel);
								memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)], &mapa_bits[posicion_imagen_this], tamano_pixel);
							}
							else//REVISAR ---> INTENTAR MEJORAR CON APROXIMACI�N O ALGO
								memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)], 0, tamano_pixel);
						}
					}
						
					delete inicio_DIB;//boramos imagen oiginal

					//Sustituimos la imagen actual por la imagen rotada
					//Reasignaci�n punteos
					inicio_DIB = imagen_auxiliar->inicio_DIB;
					bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
					bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
					if(imagen_auxiliar->paleta!=NULL)
						paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
					else//No hay paleta
						paleta = NULL;
					mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
					//Fin reasignaci�n punteros
				}
				break;
				case NUEVO_TAMANO://Versi�n optimizada disminuyendo el n�mero de c�lculos necesarios
				{
					//Calculamos seno y coseno
					double angulo_radianes = grados * (PI / 180);
					double seno = sin(angulo_radianes);
					double coseno = cos(angulo_radianes);					
				
					//Obtenemos las coordenadas del centro de la imagen
					int centrox = (bitmapinfoheader->biWidth - 1) / 2;
					int centroy = (bitmapinfoheader->biHeight - 1) / 2;
					
					//Calculamos las dimensiones de la imagen rotada
					//Para ello vemos donde caen las esquinas de la imagen actual al ser rotadas
					//y nos quedamos con la menor y mayor coordenada en cada uno de los ejes
					int esquinax, esquinay, min_fila, min_columna, max_fila, max_columna;

					//Esquina superior izquierda
					esquinax = (int)((coseno * (0 - centrox)) + (seno * (0 - centroy)) + centrox);
					esquinay = (int)((-seno * (0 - centrox)) + (coseno * (0 - centroy)) + centroy);

					min_fila = esquinay;
					max_fila = esquinay;

					min_columna = esquinax;
					max_columna = esquinax;

					//Esquina superior derecha
					esquinax = (int)((coseno * ((bitmapinfoheader->biWidth - 1) - centrox)) + (seno * (0 - centroy)) + centrox);
					esquinay = (int)((-seno * ((bitmapinfoheader->biWidth - 1) - centrox)) + (coseno * (0 - centroy)) + centroy);

					if(min_fila>esquinay)
						min_fila = esquinay;

					if(max_fila<esquinay)
						max_fila = esquinay;

					if(min_columna>esquinax)
						min_columna = esquinax;

					if(max_columna<esquinax)
						max_columna = esquinax;

					//Esquina inferior derecha
					esquinax = (int)((coseno * ((bitmapinfoheader->biWidth - 1) - centrox)) + (seno * ((bitmapinfoheader->biHeight - 1) - centroy)) + centrox);
					esquinay = (int)((-seno * ((bitmapinfoheader->biWidth - 1) - centrox)) + (coseno * ((bitmapinfoheader->biHeight - 1) - centroy)) + centroy);

					if(min_fila>esquinay)
						min_fila = esquinay;

					if(max_fila<esquinay)
						max_fila = esquinay;

					if(min_columna>esquinax)
						min_columna = esquinax;

					if(max_columna<esquinax)
						max_columna = esquinax;

					//Esquina inferior izquierda
					esquinax = (int)((coseno * (0 - centrox)) + (seno * ((bitmapinfoheader->biHeight - 1) - centroy)) + centrox);
					esquinay = (int)((-seno * (0 - centrox)) + (coseno * ((bitmapinfoheader->biHeight - 1) - centroy)) + centroy);

					if(min_fila>esquinay)
						min_fila = esquinay;

					if(max_fila<esquinay)
						max_fila = esquinay;

					if(min_columna>esquinax)
						min_columna = esquinax;

					if(max_columna<esquinax)
						max_columna = esquinax;

					//Creamos la imagen donde guardaremos la imagen actual rotada
					Texto *imagen_auxiliar = new Texto(max_columna - min_columna + 1, max_fila - min_fila + 1, profundidad, paleta, tamano_paleta);
			
					//Obtenemos las coordenadas del centro de la imagen auxiliar
					int centrox_imagen_nueva = (imagen_auxiliar->bitmapinfoheader->biWidth - 1) / 2;
					int centroy_imagen_nueva = (imagen_auxiliar->bitmapinfoheader->biHeight - 1) / 2;

					//Guardamos los datos que dependen de la columna pero son independientes de la fila, para ahorrar calculos
					//ya que sino los estaremos recalculando en cada fila, perdiendo mucho tiempo de CPU innecesariamente
					double *cosenox = new double[imagen_auxiliar->bitmapinfoheader->biWidth];
					double *senox = new double[imagen_auxiliar->bitmapinfoheader->biWidth];

					//Calculamos los datos de todas las columnas
					for(int j=0; j<imagen_auxiliar->bitmapinfoheader->biWidth; j++)
					{
						cosenox[j] = coseno * (j - centrox_imagen_nueva);
						senox[j] = seno * (j - centrox_imagen_nueva);
					}
					
					//Realizamos la rotaci�n
					for(int i=0; i<imagen_auxiliar->bitmapinfoheader->biHeight; i++)
					{
						//Calculamos los datos que nos valdr�n para toda la fila (independientes de la columna)
						int y = i - centroy_imagen_nueva;
						double cosenoy = coseno * y;
						double senoy = seno * y;

						//Para una mayor optimizaci�n reducimos las llamadas a la funci�n posici�n a una vez por fila
						for(int p = imagen_auxiliar->posicion(i, 0, tamano_pixel), j=0; j<imagen_auxiliar->bitmapinfoheader->biWidth; j++, p+=tamano_pixel)
						{
							//Calculo de los datos que dependen de la columna
							int x = j - centrox_imagen_nueva;
							
							//Coordenadas del pixel (i,j) de la imagen rotada en la imagen actual
							int coordenadaX = (int)(cosenox[j] + senoy);
							int coordenadaY = (int)(-senox[j] + cosenoy);

							coordenadaX += centrox;
							coordenadaY += centroy;
												
							//Eliminamos la llamada a ExisteCoordenada ya que es m�s eficiente poner el codigo directamente
							if((coordenadaX>=0) && (coordenadaX<bitmapinfoheader->biWidth) && (coordenadaY>=0) && (coordenadaY<bitmapinfoheader->biHeight))
							{
								//Si existe la coordenada en la imagen actual cogemos el valor del pixel
								int posicion_imagen_this=posicion(coordenadaY, coordenadaX, tamano_pixel);
								memcpy(&imagen_auxiliar->mapa_bits[p], &mapa_bits[posicion_imagen_this], tamano_pixel);								
							}
							else//REVISAR ---> INTENTAR MEJORAR CON APROXIMACI�N O ALGO
								memset(&imagen_auxiliar->mapa_bits[p], 255, tamano_pixel);
						}
					}

					//Eliminamos los vectores usados para ahorrarnos c�lculos
					delete cosenox;
					delete senox;
					
					delete inicio_DIB;//boramos imagen oiginal

					//Sustituimos la imagen actual por la imagen rotada
					//Reasignaci�n punteos
					inicio_DIB = imagen_auxiliar->inicio_DIB;
					bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
					bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
					if(imagen_auxiliar->paleta!=NULL)
						paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
					else//No hay paleta
						paleta = NULL;
					mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
					//Fin reasignaci�n punteros
				}				
				break;

				default:
					return 3;
			}
		}

		return 0;
	}


	//======================================================================
	// NOMBRE: Rotacion
	//
	// DESCRIPCI�N: Rota un segmento de la imagen respetando el tama�o del mismo. (Versi�n optimizada disminuyendo el n�mero de c�lculos necesarios)
	//
	// ARGUMENTOS:	Palabra segmento -> Segmento de la imagen
	//				double grados	 -> Indica el n� de grados  si <0 ---> izquierda ; si >0 ---> derecha
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Grados no v�lidos
	//			2 -> Error. Imagen no es de 1, 4, 8 � 24 bits/p�xel			
	//======================================================================
	int Texto::Rotacion(Palabra segmento, double grados)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos si el valor de los grados es v�lido 
		if(fabs(grados)>360)
			return 1;

		//Comprobamos que la profundidad de la imagen est� soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;
		
		if(grados!=0 && grados!=360)//Si son 0 grados o 360 no hacemos nada)
		{
			int tamano_pixel = profundidad / 8;

			//Calculamos el tama�o de la paleta
			int tamano_paleta = 0;
			if(bitmapinfoheader->biBitCount<=8)
			{
				if(bitmapinfoheader->biClrUsed!=0)
					tamano_paleta = bitmapinfoheader->biClrUsed;
				else
					tamano_paleta = (int)pow(2.0, bitmapinfoheader->biBitCount);
			}

			int ancho_segmento = segmento.esquina_inferior_derecha.x - segmento.esquina_superior_izquierda.x + 1;
			int alto_segmento = segmento.esquina_inferior_derecha.y - segmento.esquina_superior_izquierda.y + 1;

			//Creamos la imagen donde guardaremos el segmento actual rotado
			Texto *imagen_auxiliar = new Texto(segmento.esquina_inferior_derecha.x - segmento.esquina_superior_izquierda.x + 1, segmento.esquina_inferior_derecha.y - segmento.esquina_superior_izquierda.y + 1, profundidad, paleta, tamano_paleta);
				
			//Calculamos seno y coseno
			double angulo_radianes = grados * (PI / 180);
			double seno = sin(angulo_radianes);
			double coseno = cos(angulo_radianes);			

			//Obtenemos las coordenadas del centro del segmento
			int centrox = segmento.esquina_superior_izquierda.x + (ancho_segmento - 1) / 2;
			int centroy = segmento.esquina_superior_izquierda.y + (alto_segmento - 1) / 2;			
			
			//Guardamos los datos que dependen de la columna pero son independientes de la fila, para ahorrar calculos
			//ya que sino los estaremos recalculando en cada fila, perdiendo mucho tiempo de CPU innecesariamente
			double *cosenox = new double[imagen_auxiliar->bitmapinfoheader->biWidth];
			double *senox = new double[imagen_auxiliar->bitmapinfoheader->biWidth];

			//Calculamos los datos de todas las columnas
			for(int j=0; j<imagen_auxiliar->bitmapinfoheader->biWidth; j++)
			{
				cosenox[j] = coseno * (j + segmento.esquina_superior_izquierda.x - centrox);
				senox[j] = seno * (j + segmento.esquina_superior_izquierda.x - centrox);
			}				
							
			//Realizamos la rotaci�n
			for(int i=segmento.esquina_superior_izquierda.y; i<=segmento.esquina_inferior_derecha.y; i++)
			{
				//Calculamos los datos que nos valdr�n para toda la fila (independientes de la columna)
				int y = i - centroy;
				double cosenoy = coseno * y;
				double senoy = seno * y;

				//Para una mayor optimizaci�n reducimos las llamadas a la funci�n posici�n a una vez por fila
				for(int p = imagen_auxiliar->posicion(i - segmento.esquina_superior_izquierda.y, 0, tamano_pixel), j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++, p+=tamano_pixel)
				{
					//Calculo de los datos que dependen de la columna
					int x = j - centrox;
					
					//Coordenadas del pixel (i,j) de la imagen rotada en la imagen actual
					int coordenadaX = (int)(cosenox[j - segmento.esquina_superior_izquierda.x] + senoy);
					int coordenadaY = (int)(-senox[j - segmento.esquina_superior_izquierda.x] + cosenoy);

					coordenadaX += centrox;
					coordenadaY += centroy;
										
					//Eliminamos la llamada a ExisteCoordenada ya que es m�s eficiente poner el codigo directamente
					if((coordenadaX>=segmento.esquina_superior_izquierda.x) && (coordenadaX<=segmento.esquina_inferior_derecha.x) && (coordenadaY>=segmento.esquina_superior_izquierda.y) && (coordenadaY<=segmento.esquina_inferior_derecha.y))
					{
						//Si existe la coordenada en el segmento actual cogemos el valor del pixel
						int posicion_imagen_this=posicion(coordenadaY, coordenadaX, tamano_pixel);
						memcpy(&imagen_auxiliar->mapa_bits[p], &mapa_bits[posicion_imagen_this], tamano_pixel);								
					}
					else//REVISAR ---> INTENTAR MEJORAR CON APROXIMACI�N O ALGO
						memset(&imagen_auxiliar->mapa_bits[p], 255, tamano_pixel);
				}
			}
			
			//Eliminamos los vectores usados para ahorrarnos c�lculos
			delete cosenox;
			delete senox;
			
			//Copiamos el segmento rotado en la imagen original
			for(int i=segmento.esquina_superior_izquierda.y; i<=segmento.esquina_inferior_derecha.y; i++)
				memcpy(&mapa_bits[posicion(i, segmento.esquina_superior_izquierda.x, tamano_pixel)], &imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i - segmento.esquina_superior_izquierda.y, 0, tamano_pixel)], imagen_auxiliar->bitmapinfoheader->biWidth);

			delete imagen_auxiliar;//Borramos la imagen auxiliar		
		}

		return 0;
	}


	//======================================================================
	// NOMBRE: Shear
	//
	// DESCRIPCI�N: Aplica Shear un segmento de la imagen respetando el tama�o del mismo. (Versi�n optimizada disminuyendo el n�mero de c�lculos necesarios)
	//
	// ARGUMENTOS:	Palabra segmento -> Segmento de la imagen
	//				double grados	 -> Indica el n� de grados  si <0 ---> izquierda ; si >0 ---> derecha
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Grados no v�lidos
	//			2 -> Error. Imagen no es de 1, 4, 8 � 24 bits/p�xel			
	//======================================================================
	int Texto::Shear(Palabra segmento, double grados)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos si el valor de los grados es v�lido 
		if(fabs(grados)>360)
			return 1;

		//Comprobamos que la profundidad de la imagen est� soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;
		
		if(grados!=0 && grados!=360)//Si son 0 grados o 360 no hacemos nada)
		{
			int tamano_pixel = profundidad / 8;

			//Calculamos el tama�o de la paleta
			int tamano_paleta = 0;
			if(bitmapinfoheader->biBitCount<=8)
			{
				if(bitmapinfoheader->biClrUsed!=0)
					tamano_paleta = bitmapinfoheader->biClrUsed;
				else
					tamano_paleta = (int)pow(2.0, bitmapinfoheader->biBitCount);
			}

			int ancho_segmento = segmento.esquina_inferior_derecha.x - segmento.esquina_superior_izquierda.x + 1;
			int alto_segmento = segmento.esquina_inferior_derecha.y - segmento.esquina_superior_izquierda.y + 1;

			//Creamos la imagen donde guardaremos el segmento actual rotado
			Texto *imagen_auxiliar = new Texto(segmento.esquina_inferior_derecha.x - segmento.esquina_superior_izquierda.x + 1, segmento.esquina_inferior_derecha.y - segmento.esquina_superior_izquierda.y + 1, profundidad, paleta, tamano_paleta);
				
			//Calculamos tangente
			double angulo_radianes = grados * (PI / 180);
			double tangente = tan(-angulo_radianes);

			//Obtenemos las coordenadas del centro del segmento
			int centrox = segmento.esquina_superior_izquierda.x + (ancho_segmento - 1) / 2;
			int centroy = segmento.esquina_superior_izquierda.y + (alto_segmento - 1) / 2;				
							
			//Realizamos el Shear
			for(int i=segmento.esquina_superior_izquierda.y; i<=segmento.esquina_inferior_derecha.y; i++)
			{
				//Calculamos los datos que nos valdr�n para toda la fila (independientes de la columna)
				int y = i - centroy;
				double tangentey = tangente * y;

				//Para una mayor optimizaci�n reducimos las llamadas a la funci�n posici�n a una vez por fila
				for(int p = imagen_auxiliar->posicion(i - segmento.esquina_superior_izquierda.y, 0, tamano_pixel), j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++, p+=tamano_pixel)
				{
					//Calculo de los datos que dependen de la columna
					int x = j - centrox;
					
					//Coordenadas del pixel (i,j) de la imagen rotada en la imagen actual
					int coordenadaX = x + (int)(tangentey);
					int coordenadaY = i;

					coordenadaX += centrox;
															
					//Eliminamos la llamada a ExisteCoordenada ya que es m�s eficiente poner el codigo directamente
					if((coordenadaX>=segmento.esquina_superior_izquierda.x) && (coordenadaX<=segmento.esquina_inferior_derecha.x) && (coordenadaY>=segmento.esquina_superior_izquierda.y) && (coordenadaY<=segmento.esquina_inferior_derecha.y))
					{
						//Si existe la coordenada en el segmento actual cogemos el valor del pixel
						int posicion_imagen_this=posicion(coordenadaY, coordenadaX, tamano_pixel);
						memcpy(&imagen_auxiliar->mapa_bits[p], &mapa_bits[posicion_imagen_this], tamano_pixel);								
					}
					else//REVISAR ---> INTENTAR MEJORAR CON APROXIMACI�N O ALGO
						memset(&imagen_auxiliar->mapa_bits[p], 255, tamano_pixel);
				}
			}			
			
			//Copiamos el segmento rotado en la imagen original
			for(int i=segmento.esquina_superior_izquierda.y; i<=segmento.esquina_inferior_derecha.y; i++)
				memcpy(&mapa_bits[posicion(i, segmento.esquina_superior_izquierda.x, tamano_pixel)], &imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i - segmento.esquina_superior_izquierda.y, 0, tamano_pixel)], imagen_auxiliar->bitmapinfoheader->biWidth);

			delete imagen_auxiliar;//Borramos la imagen auxiliar		
		}

		return 0;					
	}


	//======================================================================	
	// NOMBRE: Enderezar
	//
	// DESCRIPCI�N: Endereza un texto autom�ticamente en caso de estar inclinado (dentro de lo posible).
	//
	// ARGUMENTOS: -
	//								
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada		
	//======================================================================
	int Texto::Enderezar()
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		int angulo = 0;//�ngulo estimado de inclinaci�n del texto
		int desplazamiento = 3;//Incremento inicial a la hora de testear inclinaciones
		int sentido = 1;//Sentido en el que buscamos la inclinaci�n
		BYTE estado = 0;//0 -> inicializamos 'b�squeda'	   1 -> hemos cambiado el sentido en la prueba anterior    2 -> el cambio de sentido ha mejorado el resultado    3 -> hemos encontrado un m�ximo (posiblemente local :( )

		//Creamos una imagen auxiliar sobre la que testear la inclinaci�n
		Texto *texto_auxiliar = new Texto();		

		//Reservamos memoria para almacenar la proyecci�n horizontal de la imagen
		int *proyeccion = new int[bitmapinfoheader->biHeight];
				
		//Variables para guardar la desviaci�n actual y la anterior. Nuestro objetivo es maximizar esta desviaci�n
		double desviacion_antigua;
		double desviacion_nueva;

		//Calculamos la proyecci�n horizontal, as� como su desviaci�n asociada
		proyeccionHorizontal(proyeccion, &desviacion_nueva);

		while(estado!=3)
		{
			//Guardamos la �ltima desviaci�n
			desviacion_antigua = desviacion_nueva;

			//Calculamos el siguiente �ngulo a testear
			angulo += sentido * desplazamiento;

			//Copiamos la imagen original en la auxiliar
			(*texto_auxiliar) = (*this);

			//Rotamos la imagen auxiliar
			texto_auxiliar->Rotacion(NUEVO_TAMANO, angulo);			

			//Borramos la proyecci�n anterior y reservamos memoria para la nueva. No podemos aprovechar la anterior, porque al rotar la imagen podemos tener un n�mero distinto de filas
			delete proyeccion;
			//Reservamos memoria para almacenar la proyecci�n horizontal de la imagen
			proyeccion = new int[texto_auxiliar->bitmapinfoheader->biHeight];

			//Calculamos la proyecci�n horizontal, as� como su desviaci�n asociada
			texto_auxiliar->proyeccionHorizontal(proyeccion, &desviacion_nueva);

			//Si la desviaci�n ha empeorado (disminuido)
			if(desviacion_nueva<=desviacion_antigua)
			{
				//Si el salto de testeo es mayor que 1 lo disminuimos en una unidad
				if(fabs((double)desplazamiento)!=1)
					desplazamiento--;
				else//Si el desplazamiento es de 1
				{					
					switch(estado)
					{
						case 0:
							estado = 1;
						break;
						case 1:
							if(desviacion_nueva==desviacion_antigua)
								estado = 3;
						break;
						case 2:
							estado = 3;
					}
				}

				//Cambiamos el sentido
				sentido *= -1;				
			}
			else//La desviaci�n ha mejorado (incrementado)
			{
				if(estado==1 && fabs((double)desplazamiento)==1)
					estado = 2;
				else//Si no venimos de un cambio de sentido 'reiniciamos' la b�squeda
					estado = 0;
			}
		}

		//Dejamos el �ngulo que ha m�ximizado la desviaci�n, ya que el �ltimo calculado siempre es uno peor
		angulo += sentido * desplazamiento;

		//Enderezamos el texto
		Rotacion(NUEVO_TAMANO, angulo);

		//Borramos la imagen auxiliar usada
		delete texto_auxiliar;

		return 0;
	}


	//======================================================================	
	// NOMBRE: Umbralizar
	//
	// DESCRIPCI�N: Umbralizaci�n binaria de una imagen.
	//
	// ARGUMENTOS:	int umbral		 -> Filtro
	//				int nivel_minimo -> Valor que se dar� a los p�xeles que sean menores que el umbral
	//				int nivel_maximo -> Valor que se dar� a los p�xeles que sean mayores o iguales que el umbral
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Umbral negativo o mayor de 255
	//			2 -> Error. Imagen no es de 1, 4, 8 � 24 bits/p�xel
	//			3 -> Error. Al menos uno de los valores asignados a los rangos es incorrecto.
	//======================================================================
	int Texto::Umbralizar(int umbral, int nivel_minimo, int nivel_maximo)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos que el umbral sea correcto
		if(umbral<0 || umbral>255)
			return 1;
		
		//Comprobamos que la profundidad de la imagen est� soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;		

		//Comprobamos que los niveles m�ximo y m�nimo sean correctos
		if(nivel_minimo<0 || nivel_maximo>255)
			return 3;	
		
		if(profundidad==8)
		{
			//Recorremos la imagen
			for(int i=0; i<bitmapinfoheader->biHeight; i++)
				for(int j=0; j<bitmapinfoheader->biWidth; j++)
				{
					//Obtenemos la posici�n en la paleta del p�xel actual
					int posicion_paleta = mapa_bits[posicion(i, j, 1)];
					
					//Valor en nivel de gris del p�xel actual
					BYTE nivel_gris = (BYTE)((paleta[posicion_paleta].rgbBlue + paleta[posicion_paleta].rgbGreen + paleta[posicion_paleta].rgbRed) / 3);

					//Realizamos la umbralizaci�n
					if(nivel_gris>=umbral)
						mapa_bits[posicion(i, j, 1)] = nivel_maximo;
					else
						mapa_bits[posicion(i, j, 1)] = nivel_minimo;
				}
		}

		if(profundidad==24)
		{
			//Creamos una imagen de 8 bits de profundidad (en concreto de 256 niveles de gris)
			Texto *imagen_auxiliar;
			imagen_auxiliar = new Texto(bitmapinfoheader->biWidth,bitmapinfoheader->biHeight,8,NULL,256);

			//Recorremos la imagen
			for(int i=0; i<bitmapinfoheader->biHeight; i++)
				for(int j=0; j<bitmapinfoheader->biWidth; j++)
				{
					//Obtenemos la posici�n del p�xel actual en el array para ahorrar c�lculos
					int posicion_pixel = posicion(i, j, 3);

					//Valor en nivel de gris del p�xel actual
					BYTE nivel_gris = (BYTE)((mapa_bits[posicion_pixel] + mapa_bits[posicion_pixel + 1] + mapa_bits[posicion_pixel + 2]) / 3);
							
					//Realizamos la umbralizaci�n
					if(nivel_gris>=umbral)
						imagen_auxiliar->mapa_bits[posicion(i, j, 1)] = nivel_maximo;
					else
						imagen_auxiliar->mapa_bits[posicion(i, j, 1)] = nivel_minimo;	
				}
										
				delete inicio_DIB;//Borramos la imagen original

				//Sustituimos la imagen actual por la imagen umbralizada
				//Reasignamos punteros
				inicio_DIB = imagen_auxiliar->inicio_DIB;
				bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
				bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
				//Tendremos una paleta de 256 niveles de gris
				paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
				mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
				//Fin reasignaci�n de punteros				
		}

		//Creamos la paleta de 256 niveles de gris
		for(int i=0; i<256; i++)
		{
			paleta[i].rgbBlue = (BYTE)i;
			paleta[i].rgbGreen = (BYTE)i;
			paleta[i].rgbRed = (BYTE)i;
			paleta[i].rgbReserved = (BYTE)0;
		}
		//Fin paleta de colores

		//Actualizamos la propiedad de umbralizada
		umbralizada = true;

		return 0;
	}


	//======================================================================
	// NOMBRE: SegmentacionLineas
	//
	// DESCRIPCI�N: Segmentaci�n de una imagen de texto en l�neas.
	//
	// ARGUMENTOS: int interlineado_minimo	-> Separaci�n m�nima, en p�xeles, que debe haber entre dos l�neas consecutivas
	//			   int diferencia_minima	-> Diferencia de p�xeles entre la fila actual y la anterior necesaria para que consideremos la presencia de un rengl�n
	//			   int espaciado_medio		-> Espacio medio entre palabras, en p�xeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//======================================================================
	int Texto::SegmentacionLineas(int interlineado_medio, int diferencia_minima, int espaciado_medio)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Separaci�n que dejaremos entre las l�neas en la imagen una vez procesada
		int espaciado = 15;

		//Obtenemos las l�neas de la imagen
		ObtenerLineas(interlineado_medio, diferencia_minima, espaciado_medio);

		//Quitamos las filas blancas de cada rengl�n, tanto por arriba como por abajo
		for(int l=0; l<numeroLineas; l++)
		{
			//Eliminamos las filas blancas superiores
			for(int i=lineas[l].esquina_superior_izquierda.y; i<=lineas[l].esquina_superior_izquierda.y ; i++)
			{
				int j;

				for(j=0; j<bitmapinfoheader->biWidth && mapa_bits[posicion(i,j,1)]==255; j++);

				if(j==bitmapinfoheader->biWidth)//No hay ning�n p�xel en la fila
					lineas[l].esquina_superior_izquierda.y++;
				else
					break;
			}

			//Eliminamos las filas blancas inferiores
			for(int i=lineas[l].esquina_inferior_derecha.y; i>=lineas[l].esquina_superior_izquierda.y ; i--)
			{
				int j;

				for(j=0; j<bitmapinfoheader->biWidth && mapa_bits[posicion(i,j,1)]==255; j++);

				if(j==bitmapinfoheader->biWidth)//No hay ning�n p�xel en la fila
					lineas[l].esquina_inferior_derecha.y--;
				else
					break;
			}			
		}

		//Creamos una nueva imagen con el tama�o necesario para guardar todas las l�neas halladas con una separaci�n interl�nea de 'espaciado' p�xeles
		Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight + espaciado * (numeroLineas + 1), 8, paleta, 256); 
		
		//Creamos la estructura de l�neas de la imagen segmentada
		imagen_auxiliar->lineas = new Linea[numeroLineas];

		//Copiamos las l�neas de la imagen original en la imagen segmentada
		int tamano_pixel = bitmapinfoheader->biBitCount / 8;

		//Calculamos las columnas de relleno
		int columnas_relleno = 0;
		if((bitmapinfoheader->biWidth*tamano_pixel) % 4!=0)
			columnas_relleno = 4 - ((bitmapinfoheader->biWidth * tamano_pixel) % 4);

		int inicio = 0;//Fila donde empieza la primera l�nea en l aimagen original
		if(numeroLineas>0)
			inicio = lineas[0].esquina_superior_izquierda.y; 
		
		//Copiamos las l�neas de la imagen original en la imagen segmentada
		for(int l=0; l<numeroLineas; l++)
		{
			//Asignamos los atributos de una l�nea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			if(l==0)
				imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado;
			else
				imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = imagen_auxiliar->lineas[l-1].esquina_inferior_derecha.y + espaciado;
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = imagen_auxiliar->lineas[l].esquina_superior_izquierda.y + (lineas[l].esquina_inferior_derecha.y - lineas[l].esquina_superior_izquierda.y + 1);				

			//Pintamos de negro una fila inferior y otra superior a la l�nea en la imagen segmentada (varias filas superiores e inferiores para mo interferir en el c�lculo del esqueleto)
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la l�nea de la imagen original en la imagen segmentada
			memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[l].esquina_inferior_derecha.y, 0, 1)], (lineas[l].esquina_inferior_derecha.y - lineas[l].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));
		}
		
		delete inicio_DIB;//Borramos la imagen original

		//Sustituimos la imagen actual por la imagen segmentada
		//Reasignamos punteros
		inicio_DIB = imagen_auxiliar->inicio_DIB;
		bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
		bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
		paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));		
		mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
		lineas = imagen_auxiliar->lineas;
		//Fin reasignaci�n de punteros

		return 0;
	}


	//======================================================================
	// NOMBRE: SegmentacionPalabras
	//
	// DESCRIPCI�N: Segmentaci�n de las l�neas de una imagen de texto en palabras.
	//
	// ARGUMENTOS: int espaciado_medio	-> Espacio medio entre palabras, en p�xeles
	//			   bool pintar_limites	-> Indica si queremos pintar en la imagen los l�mites de las distintas palabras
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//======================================================================
	int Texto::SegmentacionPalabras(int espaciado_medio, bool pintar_limites)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Obtenemos las palabras de cada una de las l�neas de la imagen
		ObtenerSegmentos(espaciado_medio);

		//Pintamos los l�mites de las palabras encontradas
		if(pintar_limites)
		{
			for(int l=0; l<numeroLineas; l++)
				for(int p=0; p<lineas[l].numeroPalabras; p++)
					for(int i=lineas[l].palabras[p].esquina_superior_izquierda.y; i<=lineas[l].palabras[p].esquina_inferior_derecha.y; i++)
							mapa_bits[posicion(i, lineas[l].palabras[p].esquina_superior_izquierda.x, 1)] = 0;
		}

		return 0;
	}


	//####################################ESTA A�N NO LA COMENTO PORQUE ES PROVISIONAL#################################
	//======================================================================
	// NOMBRE: SegmentacionCaracteres
	//
	// DESCRIPCI�N: Segmentaci�n de las palabras de una imagen de texto en caracteres.
	//
	// ARGUMENTOS: int interlineado_minimo	-> Separaci�n m�nima, en p�xeles, que debe haber entre dos l�neas consecutivas
	//			   int diferencia_minima	-> Diferencia de p�xeles entre la fila actual y la anterior necesaria para que consideremos la presencia de un rengl�n
	//			   int espaciado_medio		-> Espacio medio entre palabras, en p�xeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//======================================================================
	int Texto::SegmentacionCaracteres(int grosor_lapiz, double incremento_minimo, bool pintar_limites)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Obtenemos los caracteres de cada una de las palabras de cada una de las l�neas de la imagen
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				ObtenerCaracteres(&lineas[l].palabras[p], grosor_lapiz, incremento_minimo);

		//Pintamos los l�mites de las palabras encontradas
		if(pintar_limites)
		{
			for(int l=0; l<numeroLineas; l++)
				for(int p=0; p<lineas[l].numeroPalabras; p++)
					for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
						for(int i=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.y; i<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.y; i++)
							mapa_bits[posicion(i, lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.x-1, 1)]=0;
		}

		//Eliminamos los espacios sobrantes del rect�ngulo que engloba al caracter (quitamos l�neas y columnas en blanco)
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
					ajustarCaracter(&lineas[l].palabras[p].caracteres[c]);

		return 0;
	}	
	

	//======================================================================
	// NOMBRE: ObtenerLineas
	//
	// DESCRIPCI�N: Obtiene las l�neas de una imagen, guard�ndolas en la estructura correspondiente.
	//
	// ARGUMENTOS: int interlineado_minimo	-> Separaci�n m�nima, en p�xeles, que debe haber entre dos l�neas consecutivas
	//			   int diferencia_minima	-> Diferencia de p�xeles entre la fila actual y la anterior necesaria para que consideremos la presencia de un rengl�n
	//			   int espaciado_medio		-> Espacio medio entre palabras, en p�xeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//======================================================================
	int Texto::ObtenerLineas(int interlineado_medio, int diferencia_minima, int espaciado_medio)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Borramos las l�neas encontradas anteriormente, en caso de que existan
		if(lineas!=NULL)
		{
			delete lineas;
			lineas = NULL;
			numeroLineas = 0;
		}

		//Creamos una imagen auxiliar sobre la que aplicaremos el algoritmo RLSA
		Texto *texto_auxiliar = new Texto();		
		(*texto_auxiliar) = (*this);

		//Aplicamos RLSA sobre la imagen auxiliar para crear unas 'l�neas' consistentes
		texto_auxiliar->RLSA(espaciado_medio);

		//Reservamos espacio para la proyecci�n horizontal de la imagen
		int *proyeccion = new int[bitmapinfoheader->biHeight];

		//Calculamos la proyecci�n horizontal d el aimagen, sin calcular su desviaci�n asociada
		texto_auxiliar->proyeccionHorizontal(proyeccion);

		//Calculamos la 'derivada' de la proyecci�n (diferencia de cada posici�n con la anterior)
		for(int i=0; i<bitmapinfoheader->biHeight - 1; i++)
			proyeccion[i] = proyeccion[i+1] - proyeccion[i];

		//Buscaremos los valles en la proyecci�n, ya que ser� en �stos donde haya una separaci�n entre l�neas
		//Variables necesarias
		int i;//Fila actual de la imagen
		int posicion_inicio;//Primera fila de un posible valle
		int posicion_fin;//�ltima fila de un posible valle
		int elementos_meseta = 0;//Contador de los p�xeles de un valle de la proyeccion
		BYTE estado;//0 -> buscando valle(bajada)    1 -> buscando subida(valle)    2 -> buscando bajada(subida)
		Linea linea;//Variable para ir guardando la l�nea encontrada
		Pila *pila_lineas;//Pila para almacenar todas las l�neas que vamos encontrando

		//Inicializacion
		posicion_inicio = -1;
		posicion_fin = -1;
		//Creamos la pila
		pila_lineas = new Pila();
		//Rellenamos la coordenada izquierda superior de la primera l�nea
		linea.esquina_superior_izquierda.x = 0;
		linea.esquina_superior_izquierda.y = 0;

		//Buscamos la primera subida, que nos indicar� la presencia de un rengl�n
		for(i=0; fabs((double)proyeccion[i])<=diferencia_minima && i<bitmapinfoheader->biHeight; i++);
		
		//A partir de aqui buscamos una bajada, por lo que inicializamos el 'aut�mata' precisamente a ese estado		
		estado = 2;

		//Vamos recorriendo la proyecci�n
		for(i; i<bitmapinfoheader->biHeight - 1; i++)
		{
			//'Aut�mata'
			switch(estado)
			{
				case 0://Si estamos en bajada
					//Si la proyecci�n es mayor que la diferencia m�nima estamos ante una subida en la proyecci�n (mitad de un rengl�n)
					if(proyeccion[i]>diferencia_minima)
						estado = 2;
					else
					{
						if(fabs((double)proyeccion[i])<=diferencia_minima)
					    {
							//Hemos encontrado un 'valle' (realmente para que sea valle depende de la longitud de la meseta, as� como que le siga una subida) 
							 estado = 1;
							 posicion_inicio = i;
							 elementos_meseta = 1;
						}
						//En otro caso (encontramos otra bajada) no tenemos que hacer nada
					}
				break;
				case 1://Si estamos en 'valle'
					if(elementos_meseta<interlineado_medio)
					{
						//Si no se ha superado el n�mero m�nimo de p�xeles en el valle
						if(fabs((double)proyeccion[i])<=diferencia_minima)
						{
							//Si seguimos en una meseta incrementamos el n�mero de p�xeles en la misma
							elementos_meseta++;
							//El estado no varia
						}
						else//Sino no estabamos ante un valle y debemos seguir buscando uno
							estado = 0;
					}
					else//Se ha superado el n�mero de p�xeles m�nimo para considerar un valle real
					{
						if(proyeccion[i]>diferencia_minima)
						{
							//Si hay una subida significa que empieza un nuevo rengl�n, por lo que tenemos que a�adir el que acaba de terminar
							estado = 2;
							posicion_fin = i;

							//A�adimos la l�nea
							linea.esquina_inferior_derecha.x = bitmapinfoheader->biWidth - 1;
							linea.esquina_inferior_derecha.y = posicion_inicio + (posicion_fin - posicion_inicio) / 2;
							//NOTA ---> OTRA OPCION SERIA COGER LA INICIO PARA LA NUEVA L�NEA Y LA FIN PARA LA ACTUAL, PARA COGER LA M�XIMA ZONA POSIBLE DE LA L�NEA)

							pila_lineas->Apilar(linea);

							//Empezamos la nueva linea
							linea.esquina_superior_izquierda.x = 0;
							linea.esquina_superior_izquierda.y = posicion_inicio + (posicion_fin - posicion_inicio) / 2;
						}
						else
						{
							//Si encontramos una bajada pasamos ha dicho estado, ya que no estabamos ante un valle 'real'
							if(proyeccion[i]<-diferencia_minima)
								estado = 0;
							else//Seguimos en un posible valle, incrementamos el n�mero de p�xeles en el mismo
								elementos_meseta++;
						}
					}
				break;
				case 2://Si estamos en una subida
					//Si hemos encontrado una bajada actualizamos el estado
					if(proyeccion[i]<-diferencia_minima)
						estado = 0;
					//En cualquier otro caso no hay cambios
				break;
			}
		}

		//A�adimos la �ltima l�nea
		linea.esquina_inferior_derecha.x = bitmapinfoheader->biWidth - 1;
		linea.esquina_inferior_derecha.y = bitmapinfoheader->biHeight - 1;

		pila_lineas->Apilar(linea);

		//'Copiamos' las l�neas al atributo de nuestra clase Texto
		//Cogemos el n�mero de l�neas que hemos encontrado
		numeroLineas = pila_lineas->NumeroElementos();

		//Reservamos la memoria suficiente para almacenar las l�neas encontradas
		lineas=new Linea[numeroLineas];

		//Pasamos las l�neas encontradas de la pila al vector
		for(int i=numeroLineas - 1; !pila_lineas->EsVacia(); i--)
		{
			pila_lineas->Desapilar(&lineas[i]);
			lineas[i].palabras = NULL;
			lineas[i].numeroPalabras = 0;
		}

		//Borramos las estructuras auxiliares
		delete pila_lineas;
		delete proyeccion;
		delete texto_auxiliar;

		return 0;
	}


	//======================================================================
	// NOMBRE: ObtenerSegmentos
	//
	// DESCRIPCI�N: Obtiene las palabras o segmentos de una imagen, guard�ndolas en la estructura correspondiente.
	//
	// ARGUMENTOS: int distancia_minima	-> Espacio medio entre caracteres, en p�xeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//======================================================================
	int Texto::ObtenerSegmentos(int distancia_minima)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Creamos una imagen auxiliar sobre la que aplicaremos el algoritmo RLSA
		Texto *imagen_auxiliar = new Texto();
		(*imagen_auxiliar) = (*this);

		//Aplicamos RLSA sobre la imagen auxiliar para crear unas 'palabras' consistentes
		imagen_auxiliar->RLSA(distancia_minima);

		//Encontramos los segmentos para todas las l�neas
		for(int l=0; l<numeroLineas; l++)
		{		
			//Borramos los segmentos encontrados anteriormente, en caso de que existan
			if(lineas[l].palabras!=NULL)
			{
				delete lineas[l].palabras;
				lineas[l].palabras = NULL;
				lineas[l].numeroPalabras = 0;
			}
			
			//Buscaremos las palabras de la l�nea actual, para ello buscamos las columnas sin pixeles negros
			//Variables necesarias
			bool pixel_negro_encontrado;//Indica si hay texto en la columna actual
			int i, j;//Fila y columna actuales		
			int posicion_inicio;//Primera columna sin p�xeles negros
			int posicion_fin;//�ltima columna sin p�xeles negros (realmente es la primera con p�xel negro)
			Palabra segmento;//Variable para ir guardando la palabra encontrada
			Pila *pila_segmentos;//Pila para almacenar todos los segmentos que vamos encontrando

			//Inicializacion
			pixel_negro_encontrado = false;			
			posicion_inicio = -1;
			posicion_fin = -1;
			//Creamos la pila
			pila_segmentos = new Pila();
			//Rellenamos la coordenada izquierda superior de la primera l�nea, as� como la fila de la inferior derecha
			segmento.esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			segmento.esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y;
			segmento.esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y;

			//Buscamos la primera columna con texto
			for(j=lineas[l].esquina_superior_izquierda.x; j<=lineas[l].esquina_inferior_derecha.x && !pixel_negro_encontrado; j++)
				for(int i=lineas[l].esquina_superior_izquierda.y; i<=lineas[l].esquina_inferior_derecha.y && !pixel_negro_encontrado; i++)
					if(imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i,j,1)]==0)
						pixel_negro_encontrado = true;

			//Vamos recorriendo el rengl�n por columnas, hasta encontrar una en blanco
			for(j; j<=lineas[l].esquina_inferior_derecha.x; j++)
			{
				pixel_negro_encontrado = false;

				for(i=lineas[l].esquina_superior_izquierda.y; i<=lineas[l].esquina_inferior_derecha.y && !pixel_negro_encontrado; i++)
				{
					if(imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i,j,1)]==0)
						pixel_negro_encontrado = true;					
				}

				pixel_negro_encontrado = false;

				if(i==lineas[l].esquina_inferior_derecha.y + 1)//Columna en blanco
				{
					posicion_inicio = j;

					//Buscamos la primera columna con texto
					for(j; j<=lineas[l].esquina_inferior_derecha.x && !pixel_negro_encontrado; j++)
						for(i = lineas[l].esquina_superior_izquierda.y; i<=lineas[l].esquina_inferior_derecha.y && !pixel_negro_encontrado; i++)
							if(imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i,j,1)]==0)
								pixel_negro_encontrado = true;

					posicion_fin = j;

					//Comprobamos si hemos llegado al final de la l�nea y corregimos la posici�n fin
					if(posicion_fin>=bitmapinfoheader->biWidth)
						posicion_fin--;

					//A�adimos la palabra
					if(pixel_negro_encontrado)
						segmento.esquina_inferior_derecha.x = posicion_inicio + (posicion_fin - posicion_inicio + 1) / 2;
					else//Estamos en la �ltima palabra del rengl�n
						segmento.esquina_inferior_derecha.x = lineas[0].esquina_inferior_derecha.x;					
					
					pila_segmentos->Apilar(segmento);

					//Empiezamos la nueva palabra
					segmento.esquina_superior_izquierda.x = segmento.esquina_inferior_derecha.x;
					//segmento.esquina_superior_izquierda.y --> no hace falta cambiarlo, es igual para todas las palabras
					//segmento.esquina_inferior_derecha.y --> no hace falta cambiarlo, es igual para todas las palabras
				}
			}

			//'Copiamos' las l�neas al atributo de nuestra clase Texto
			//Cogemos el n�mero de palabras que hemos encontrado en la l�nea l
			lineas[l].numeroPalabras = pila_segmentos->NumeroElementos();

			//Reservamos la memoria suficiente para almacenar las palabras encontradas en la l�nea l
			lineas[l].palabras = new Palabra[lineas[l].numeroPalabras];

			//Pasamos las palabras encontradas de la pila al vector
			for(int i=lineas[l].numeroPalabras - 1; !pila_segmentos->EsVacia(); i--)
			{
				pila_segmentos->Desapilar(&lineas[l].palabras[i]);
				lineas[l].palabras[i].caracteres = NULL;
				lineas[l].palabras[i].numeroCaracteres = 0;
			}

			//Borramos la estructuras auxiliar
			delete pila_segmentos;
		}
		
		//Borramos la estructuras auxiliar
		delete imagen_auxiliar;			

		return 0;
	}


	//####################################ESTA A�N NO LA COMENTO PORQUE ES PROVISIONAL#################################
	//======================================================================
	// NOMBRE: ObtenerCaracteres
	//
	// DESCRIPCI�N: Obtiene las caracteres de una palabra, guard�ndolos en la estructura correspondiente.
	//
	// ARGUMENTOS: int distancia_minima	-> Espacio medio entre caracteres, en p�xeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//======================================================================
	int Texto::ObtenerCaracteres(Palabra *palabra, int grosor_lapiz, double incremento_minimo)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		if(grosor_lapiz<=0)
			grosor_lapiz = 1;

		int altura_linea = palabra->esquina_inferior_derecha.y - palabra->esquina_superior_izquierda.y + 1;

		int *proyeccion=new int[bitmapinfoheader->biWidth];

		double *pesos=new double[bitmapinfoheader->biWidth];

		int maximo1; 
		proyeccionVertical(*palabra, proyeccion, &maximo1);	

		/*ofstream archivo;
		
			archivo.open(".\\pesos", ios::app); 
			for(int k=(*palabra).esquina_superior_izquierda.x; k<=(*palabra).esquina_inferior_derecha.x; k++)
			{
				//pesos[k]=pesos[k+1]-pesos[k];				
				
				String s = " --> ";

				archivo << k << s << proyeccion[k] << "\n"; // ... must add it back
				
			}

			archivo << "\n";*/

		maximo1 = (maximo1/grosor_lapiz) + 1;

		int maximo2 = altura_linea;

		maximo2 = (maximo2/grosor_lapiz) + 1;

		int j;

		for(j=(*palabra).esquina_superior_izquierda.x; proyeccion[j]==0 && j<=(*palabra).esquina_inferior_derecha.x; j++);

		//Variables necesarias
		bool fin;
		int posicion_inicio;
		int posicion_fin;
		int elementos_meseta=0;
		BYTE estado;//0 -> buscando meseta(bajada)    1 -> buscando subida(meseta)    2 -> buscando bajada(subida)
		Caracter caracter;
		Pila *pila_caracteres;
		double maximo=-1;
		double bajada;

		//Inicializacion		
		posicion_inicio=-1;
		posicion_fin=-1;
		pila_caracteres=new Pila();

		if(j!=0)
			caracter.esquina_superior_izquierda.x=j-1;
		else
			caracter.esquina_superior_izquierda.x=j;

		caracter.esquina_superior_izquierda.y=(*palabra).esquina_superior_izquierda.y;

		for(int k=j; k<=(*palabra).esquina_inferior_derecha.x; k++)
			{
				int pixeles_cortados=proyeccion[k];

				//Calculo pixel m�s alto
				int i;
				for(i=(*palabra).esquina_superior_izquierda.y; mapa_bits[posicion(i, k, 1)]!=0 && i<=(*palabra).esquina_inferior_derecha.y; i++);
				
				int pixel_mas_alto;

				if(i<=(*palabra).esquina_inferior_derecha.y)
					pixel_mas_alto=altura_linea - (i-(*palabra).esquina_superior_izquierda.y);
				else
					pixel_mas_alto=0;

				//Calculo pixel m�s bajo
				for(i=(*palabra).esquina_inferior_derecha.y; mapa_bits[posicion(i, k, 1)]!=0 && i>=(*palabra).esquina_superior_izquierda.y; i--);
								
				int pixel_mas_bajo;

				if(i>=(*palabra).esquina_superior_izquierda.y)
					pixel_mas_bajo=altura_linea - (i-(*palabra).esquina_superior_izquierda.y);
				else
					pixel_mas_bajo=0;

				double aux2= (double)pixeles_cortados/grosor_lapiz/maximo1;

				double aux3 = (double)(pixel_mas_alto-pixel_mas_bajo+1)/grosor_lapiz/maximo2;

				double peso_actual=(pow(aux2,0.4) + pow(aux2,4)) + (pow(aux3,0.4) + pow(aux3,4));

				pesos[k]=peso_actual;
			}	

			/*ofstream archivo;
		
			archivo.open(".\\pesosdef", ios::app); 
			for(int k=j; k<=(*palabra).esquina_inferior_derecha.x; k++)
			{
				//pesos[k]=pesos[k+1]-pesos[k];				
				
				String s = " --> ";

				archivo << k << s << pesos[k] << "\n"; // ... must add it back
				
			}

			archivo << "\n";*/

			for(int k=j;k<=(*palabra).esquina_inferior_derecha.x; k++)
				pesos[k]=pesos[k+1]-pesos[k];

			/*ofstream archivo2;
		
			archivo2.open(".\\diferencias4", ios::app); 
			for(int k=j; k<=(*palabra).esquina_inferior_derecha.x; k++)
			{
				//pesos[k]=pesos[k+1]-pesos[k];				
				
				String s = " --> ";

				archivo2 << k << s << pesos[k] << "\n"; // ... must add it back
				
			}

			archivo2 << "\n";*/

		while(j<=(*palabra).esquina_inferior_derecha.x)
		{
			for(j;fabs((double)pesos[j])<=incremento_minimo && j<=(*palabra).esquina_inferior_derecha.x;j++);
			//a partir de aqui buscamos una meseta			
			estado=0;

			fin=false;
			bajada=0;

			for(j; j<=(*palabra).esquina_inferior_derecha.x && !fin; j++)
			{
				switch(estado)
				{
					case 0:
						if(pesos[j]>incremento_minimo)
						{
							if(bajada!=0 && pesos[j]>=bajada/10.0)
							{

							//A�adir caracter
								caracter.esquina_inferior_derecha.x=j;
								caracter.esquina_inferior_derecha.y=(*palabra).esquina_inferior_derecha.y;
								//otra opcion seria coger la inicio para la nueva linea y la fin para la actual para coger la maxima zona posible de la linea)

								pila_caracteres->Apilar(caracter);

								//Empieza nuevo caracter
								caracter.esquina_superior_izquierda.x=j;
								caracter.esquina_superior_izquierda.y=(*palabra).esquina_superior_izquierda.y;

								j = caracter.esquina_superior_izquierda.x + 1;
								fin=true;
							}

							bajada=0;
							estado=2;
						}
						else 
						{
							if (fabs((double)pesos[j])<=incremento_minimo)
							{
								 estado=1;
								 posicion_inicio=j;
								 elementos_meseta=1;
								 bajada=0;
							}
							else
							{
								bajada-=pesos[j];
							}
							/*else
							{
								if(pesos[j]<-3 && pesos[j+1]>0)
								{
									//A�adir caracter
									caracter.esquina_inferior_derecha.x=j;
									caracter.esquina_inferior_derecha.y=(*palabra).esquina_inferior_derecha.y;
									//otra opcion seria coger la inicio para la nueva linea y la fin para la actual para coger la maxima zona posible de la linea)

									pila_caracteres->Apilar(caracter);

									//Empieza nuevo caracter
									caracter.esquina_superior_izquierda.x=j;
									caracter.esquina_superior_izquierda.y=(*palabra).esquina_superior_izquierda.y;

									punto_segmentacion_anterior=j;
									fin=true;
								}
							}*/
						}
					break;
					case 1:
						if(elementos_meseta<1)//(grosor_lapiz+1)/2)
						{
							if(fabs((double)pesos[j])<=incremento_minimo)
							{
								elementos_meseta++;
								//el estado no varia
							}
							else
								estado=0;
						}
						else
						{
							if(pesos[j]>incremento_minimo)
							{
								estado=2;
								posicion_fin=j;

								//A�adir caracter
								caracter.esquina_inferior_derecha.x=posicion_inicio + (posicion_fin-posicion_inicio)/2;
								caracter.esquina_inferior_derecha.y=(*palabra).esquina_inferior_derecha.y;
								//otra opcion seria coger la inicio para la nueva linea y la fin para la actual para coger la maxima zona posible de la linea)

								pila_caracteres->Apilar(caracter);

								//Empieza nuevo caracter
								caracter.esquina_superior_izquierda.x=posicion_inicio + (posicion_fin-posicion_inicio)/2;
								caracter.esquina_superior_izquierda.y=(*palabra).esquina_superior_izquierda.y;

								j=caracter.esquina_superior_izquierda.x + 1;
								fin=true;
							}
							else
							{
								if(pesos[j]<-incremento_minimo)
								{
									estado=0;
									bajada-=pesos[j];
								}
								else
									elementos_meseta++;
							}
						}
					break;
					case 2:
						if(pesos[j]<-incremento_minimo)
						{
							estado=0;
							bajada-=pesos[j];
						}
						//en cualquier otro caso no hay cambios
					break;
				}
			}
		}

		//A�adir caracter
		caracter.esquina_inferior_derecha.x=(*palabra).esquina_inferior_derecha.x;
		caracter.esquina_inferior_derecha.y=(*palabra).esquina_inferior_derecha.y;
		pila_caracteres->Apilar(caracter);

		(*palabra).numeroCaracteres=pila_caracteres->NumeroElementos();

		(*palabra).caracteres=new Caracter[(*palabra).numeroCaracteres];

		for(int i=(*palabra).numeroCaracteres-1; !pila_caracteres->EsVacia();i--)
			pila_caracteres->Desapilar(&(*palabra).caracteres[i]);

		delete pila_caracteres;
		delete proyeccion;
		delete pesos;

		/*ofstream archivo;
		
		archivo.open("./h", ios::app); 
		for(int k=punto_segmentacion_anterior+1; k<=(*palabra).esquina_inferior_derecha.x; k++)
		{
			pesos[k]=pesos[k+1]-pesos[k];
			
			
			String s = " --> ";

			archivo << k << s << pesos[k] << "\n"; // ... must add it back
			
		}

		archivo << "\n";*/

		
		return 0;
			
	}

	
	//======================================================================
	// NOMBRE: CorregirSlope
	//
	// DESCRIPCI�N: Corrige el Slope de los segmentos de todas las l�neas de una imagen de texto. (Slope -> Inclinaci�n de las palabras respecto al eje horizontal)
	//				Sino hemos obtenido antes los segmentos de la imagen, la correci�n no tendr� efecto alguno.
	//
	// ARGUMENTOS: -
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada
	//======================================================================
	int Texto::CorregirSlope()
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Corregimos el Slope de cada una de las l�neas de la imagen
		for(int l=0; l<numeroLineas; l++)
			CorregirSlope(lineas[l]);

		return 0;
	}


	//======================================================================
	// NOMBRE: CorregirSlope
	//
	// DESCRIPCI�N: Corrige el Slope de una l�nea de una imagen de texto. (Slope -> Inclinaci�n de las palabras respecto al eje horizontal)
	//				Sino hemos obtenido antes los segmentos de la imagen, la correci�n no tendr� efecto alguno.
	//
	// ARGUMENTOS: Linea linea	-> L�nea sobre la que se aplicar� la correci�n
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//======================================================================
	int Texto::CorregirSlope(Linea linea)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;		
		
		//Realizamos la correci�n para todos los segmentos de la l�nea
		for(int s=0; s<linea.numeroPalabras; s++)
		{			
			int angulo = 0;//�ngulo estimado de inclinaci�n del texto
			int desplazamiento = 3;//Incremento inicial a la hora de testear inclinaciones
			int sentido = 1;//Sentido en el que buscamos la inclinaci�n
			BYTE estado = 0;//0 -> inicializamos 'b�squeda'	   1 -> hemos cambiado el sentido en la prueba anterior    2 -> el cambio de sentido ha mejorado el resultado    3 -> hemos encontrado un m�ximo (posiblemente local :( )
					
			//Reservamos memoria para almacenar la proyecci�n horizontal de la imagen
			//Estamos reservando memoria de m�s (para todas las l�neas) pero a cambio nos ahorramos c�lculos (ajustes en los �ndices de los vectores)
			int *proyeccion = new int[bitmapinfoheader->biHeight];
						
			//Variables para guardar m�ximo actual y el anterior. Nuestro objetivo es maximizar este m�ximo
			double maximo_antiguo;
			double maximo_nuevo;

			//Calculamos la proyecci�n horizontal del segmento, as� como su m�ximo asociado
			proyeccionHorizontal(linea.palabras[s], proyeccion, &maximo_nuevo);

			while(estado!=3)
			{
				//Creamos una imagen auxiliar sobre la que testear la inclinaci�n
				Texto *texto_auxiliar = new Texto();

				//Guardamos el �ltimo m�ximo
				maximo_antiguo = maximo_nuevo;

				//Calculamos el siguiente �ngulo a testear
				angulo += sentido * desplazamiento;

				//Copiamos la imagen original en la auxiliar
				(*texto_auxiliar)=(*this);

				//Rotamos el segmento de la imagen auxiliar
				texto_auxiliar->Rotacion(linea.palabras[s], angulo);			

				//Calculamos la proyecci�n horizontal, as� como su m�ximo asociado
				texto_auxiliar->proyeccionHorizontal(linea.palabras[s], proyeccion, &maximo_nuevo);

				//Si el m�ximo ha empeorado (disminuido)
				if(maximo_nuevo<=maximo_antiguo)
				{
					//Si el salto de testeo es mayor que 1 lo disminuimos en una unidad
					if(fabs((double)desplazamiento)!=1)
						desplazamiento--;
					else//Si el desplazamiento es de 1
					{					
						switch(estado)
						{
							case 0:
								estado = 1;
							break;
							case 1:
								if(maximo_nuevo==maximo_antiguo)
									estado = 3;
							break;
							case 2:
								estado = 3;
						}
					}

					//Cambiamos el sentido
					sentido *= -1;						
				}
				else//El m�ximo ha mejorado (incrementado)
				{
					if(estado==1 && fabs((double)desplazamiento)==1)
						estado = 2;
					else//Si no venimos de un cambio de sentido 'reiniciamos' la b�squeda
						estado = 0;
				}

				//Borramos la imagen auxiliar usada
				delete texto_auxiliar;
			}

			//Dejamos el �ngulo que ha m�ximizado la desviaci�n, ya que el �ltimo calculado siempre es uno peor
			angulo += sentido * desplazamiento;

			//Comprobamos si el �ngulo es demasiado grande, si no lo es enderezamos el texto
			//Si el �ngulo es muy grande hay una alta probabilidad de que la detecci�n sea incorrecta por lo que no rotamos el texto
			if(fabs((double)angulo)<=60)
				Rotacion(linea.palabras[s], angulo);						
		}

		return 0;
	}

	
	//======================================================================
	// NOMBRE: CorregirSlant
	//
	// DESCRIPCI�N: Corrige el Slnt de los segmentos de todas las l�neas de una imagen de texto. (Slant -> 'Inclinaci�n' de las letras respecto al eje vertical)
	//				Sino hemos obtenido antes los segmentos de la imagen, la correci�n no tendr� efecto alguno.
	//
	// ARGUMENTOS: -
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada
	//======================================================================
	int Texto::CorregirSlant()
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;		

		//Corregimos el Slant de cada una de las l�neas de la imagen
		for(int l=0; l<numeroLineas; l++)
			CorregirSlant(lineas[l]);

		return 0;
	}


	//======================================================================
	// NOMBRE: CorregirSlant
	//
	// DESCRIPCI�N: Corrige el Slant de una l�nea de una imagen de texto. (Slant -> 'Inclinaci�n' de las letras respecto al eje vertical)
	//				Sino hemos obtenido antes los segmentos de la imagen, la correci�n no tendr� efecto alguno.
	//
	// ARGUMENTOS: Linea linea	-> L�nea sobre la que se aplicar� la correci�n
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//======================================================================
	int Texto::CorregirSlant(Linea linea)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;		
		
		//Realizamos la correci�n para todos los segmentos de la l�nea
		for(int s=0; s<linea.numeroPalabras; s++)
		{			
			int angulo = 0;//�ngulo estimado de inclinaci�n del texto
			int desplazamiento = 7;//Incremento inicial a la hora de testear inclinaciones
			int sentido = 1;//Sentido en el que buscamos la inclinaci�n
			BYTE estado = 0;//0 -> inicializamos 'b�squeda'	   1 -> hemos cambiado el sentido en la prueba anterior    2 -> el cambio de sentido ha mejorado el resultado    3 -> hemos encontrado un m�ximo (posiblemente local :( )
					
			//Reservamos memoria para almacenar la proyecci�n vertical de la imagen
			//Estamos reservando memoria de m�s (para todas las columnas) pero a cambio nos ahorramos c�lculos (ajustes en los �ndices de los vectores)
			int *proyeccion = new int[bitmapinfoheader->biWidth];
			
			//Variables para guardar la desviaci�n actual y la anterior. Nuestro objetivo es maximizar esta desviaci�n
			double desviacion_antigua;
			double desviacion_nueva;

			//Calculamos la proyecci�n vertical del segmento, as� como su desviaci�n asociada
			proyeccionVertical(linea.palabras[s], proyeccion, &desviacion_nueva);

			while(estado!=3 && fabs((double)angulo)<=50)//Establecemos 50 como correci�n m�xima
			{
				//Creamos una imagen auxiliar sobre la que testear la inclinaci�n
				Texto *texto_auxiliar = new Texto();

				//Guardamos la �ltima desviaci�n
				desviacion_antigua = desviacion_nueva;

				//Calculamos el siguiente �ngulo a testear
				angulo += sentido * desplazamiento;

				//Copiamos la imagen original en la auxiliar
				(*texto_auxiliar) = (*this);
				
				//Aplicamos Shear al segmento de la imagen auxiliar
				texto_auxiliar->Shear(linea.palabras[s], angulo);			

				//Calculamos la proyecci�n vertical del segmento, as� como su desviaci�n asociada
				texto_auxiliar->proyeccionVertical(linea.palabras[s], proyeccion, &desviacion_nueva);

				//Si la desviaci�n ha empeorado (disminuido)
				if(desviacion_nueva<=desviacion_antigua)
				{
					//Si el salto de testeo es mayor que 1 lo disminuimos en una unidad
					if(fabs((double)desplazamiento)!=1)
						desplazamiento--;
					else//Si el desplazamiento es de 1
					{					
						switch(estado)
						{
							case 0:
								estado = 1;
							break;
							case 1:
								if(desviacion_nueva==desviacion_antigua)
									estado = 3;
							break;
							case 2:
								estado = 3;
						}
					}

					//Cambiamos el sentido
					sentido *= -1;							
				}
				else//La desviaci�n ha mejorado (incrementado)
				{
					if(estado==1 && fabs((double)desplazamiento)==1)
						estado = 2;
					else//Si no venimos de un cambio de sentido 'reiniciamos' la b�squeda
						estado = 0;
				}

				//Borramos la imagen auxiliar usada
				delete texto_auxiliar;
			}

			//Dejamos el �ngulo que ha m�ximizado la desviaci�n, ya que el �ltimo calculado siempre es uno peor
			angulo+=sentido * desplazamiento;

			//'Enderezamos' el texto
			Shear(linea.palabras[s], angulo);					
		}

		return 0;
	}
//============================================================================
	// NOMBRE: Interseccion
	//
	// DESCRIPCI�N: Devuelve la intersecci�n de la imagen actual y la imagen pasada por par�metro. El resultado queda almacenado en la imagen actual. (Ambas de 256 colores y umbralizadas)
	//
	// ARGUMENTOS:	Texto *texto -> Imagen umbralizada con la que realizar la intersecci�n
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Las im�genes no est�n umbralizada
	//			2 -> Error. Las im�genes no tienen el mismo tama�o
	//============================================================================
	int Texto::Interseccion(Texto *texto)
	{
		//Comprobamos que las im�genes est�n umbralizadas
		if(!umbralizada || !texto->umbralizada)
			return 1;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=texto->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=texto->bitmapinfoheader->biWidth)
			return 2;

		//Realizamos la intersecci�n de las im�genes umbralizadas
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				//Si alguno de los p�xeles es distinto de 0 (0->true) el resultado es false (1->255)
				if(mapa_bits[posicion(i, j, 1)]!=0 || texto->mapa_bits[posicion(i, j, 1)]!=0)
					mapa_bits[posicion(i, j, 1)] = 255;
			}

		return 0;
	}


	//============================================================================
	// NOMBRE: Interseccion
	//
	// DESCRIPCI�N: Devuelve la intersecci�n de la imagen actual y la imagen pasada por par�metro. El resultado queda almacenado en la imagen actual. (Ambas de 256 colores y umbralizadas)
	//
	// ARGUMENTOS:	Texto *texto -> Imagen umbralizada con la que realizar la intersecci�n
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Las im�genes no est�n umbralizada
	//			2 -> Error. Las im�genes no tienen el mismo tama�o
	//============================================================================
	int Texto::InterseccionO(Texto *texto)
	{
		//Comprobamos que las im�genes est�n umbralizadas
		if(!umbralizada || !texto->umbralizada)
			return 1;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=texto->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=texto->bitmapinfoheader->biWidth)
			return 2;

		//Realizamos la intersecci�n de las im�genes umbralizadas
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
				{
					for(int i=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.y; i<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.y; i++)
						for(int j=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.x; j<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.x; j++)
						{
							//Si alguno de los p�xeles es distinto de 0 (0->true) el resultado es false (1->255)
							if(mapa_bits[posicion(i, j, 1)]!=0 || texto->mapa_bits[posicion(i, j, 1)]!=0)
								mapa_bits[posicion(i, j, 1)] = 255;
						}
				}

		return 0;
	}

	//============================================================================
	// NOMBRE: Diferencia
	//
	// DESCRIPCI�N: Devuelve la diferencia de la imagen actual y la imagen pasada por par�metro. El resultado queda almacenado en la imagen actual. (Ambas de 256 colores y umbralizadas)
	//
	// ARGUMENTOS:	Texto *texto -> Imagen umbralizada con la que realizar la diferencia
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Las im�genes no est�n umbralizada
	//			2 -> Error. Las im�genes no tienen el mismo tama�o
	//============================================================================
	int Texto::Diferencia(Texto *texto)
	{
		//Comprobamos que las im�genes est�n umbralizadas
		if(!umbralizada || !texto->umbralizada)
			return 1;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=texto->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=texto->bitmapinfoheader->biWidth)
			return 2;

		//Realizamos la diferencia de las im�genes umbralizadas
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				//Si el pixel es true (0) en la 2� imagen forzamos a que el resultado sea false (255), 
				//en otro caso el p�xel resultado es igual que el p�xel en la imagen original
				if(texto->mapa_bits[posicion(i, j, 1)]==0)
					mapa_bits[posicion(i, j, 1)] = 255;
			}

		return 0;
	}
	
	//============================================================================
	// NOMBRE: DiferenciaO
	//
	// DESCRIPCI�N: Devuelve la diferencia de la imagen actual y la imagen pasada por par�metro. El resultado queda almacenado en la imagen actual. (Ambas de 256 colores y umbralizadas)
	//
	// ARGUMENTOS:	Texto *texto -> Imagen umbralizada con la que realizar la diferencia
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. Las im�genes no est�n umbralizada
	//			2 -> Error. Las im�genes no tienen el mismo tama�o
	//============================================================================
	int Texto::DiferenciaO(Texto *texto)
	{
		//Comprobamos que las im�genes est�n umbralizadas
		if(!umbralizada || !texto->umbralizada)
			return 1;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=texto->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=texto->bitmapinfoheader->biWidth)
			return 2;

		//Realizamos la diferencia de las im�genes umbralizadas
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
				{
					for(int i=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.y; i<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.y; i++)
						for(int j=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.x; j<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.x; j++)
						{
							//Si el pixel es true (0) en la 2� imagen forzamos a que el resultado sea false (255), 
							//en otro caso el p�xel resultado es igual que el p�xel en la imagen original
							if(texto->mapa_bits[posicion(i, j, 1)]==0)
								mapa_bits[posicion(i, j, 1)] = 255;
						}
				}

		return 0;
	}


	//============================================================================
	// NOMBRE: Erosion
	//
	// DESCRIPCI�N: Obtiene la erosion de la imagen actual respecto a un determinado elemento estructural. El resultado queda almacenado en la imagen actual. (Imagen de 256 colores y umbralizada)
	//
	// ARGUMENTOS:	ElementoEstructural elemento -> Elemento estructural usado en la erosi�n
	//				bool imagen_complementaria	 -> Indica si debemos tratar la imagen como si fuera la complementaria (esto es para ahorrarnos la creaci�n de una imagen complementaria, ahorrando tiempo de CPU)
	//				bool elemento_complementario -> Indica si debemos tratar el elemento estructural como si fuera el complementaria (esto es para ahorrarnos la creaci�n de un elemento complementario, ahorrando tiempo de CPU)
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//============================================================================
	int Texto::Erosion(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;
	
		//Calculamos el tama�o de la paleta
		int tamano_paleta = 0;
		if(bitmapinfoheader->biBitCount<=8)
		{
			if(bitmapinfoheader->biClrUsed!=0)
				tamano_paleta = bitmapinfoheader->biClrUsed;
			else
				tamano_paleta = (int)pow(2.0, bitmapinfoheader->biBitCount);
		}

		bool contenidos;//Indica si los p�xeles del elemento estructural est�n inclu�dos en la imagen original

		//Creamos una imagen auxiliar en la que iremos guardando el resultado
		Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight, bitmapinfoheader->biBitCount, paleta, tamano_paleta);
		
		//Quitamos la primera columna y fila para ahorranos las comprobaciones de existencia de p�xeles al aplicar la m�scara (se perdia mucho tiempo de CPU tontamente)
		for(int i=1; i<bitmapinfoheader->biHeight - 1; i++)
			for(int j=1; j<bitmapinfoheader->biWidth - 1; j++)
			{
				//En principio suponemos que los p�xeles del elemento estructural est�n contenidos en la imagen original
				contenidos=true;

				//Comprobamos si el elemento estructural est� contenido en la imagen original
				for(int k=0;k<3 && contenidos;k++)
					for(int l=0;l<3 && contenidos;l++)
					{
						//Guardamos la posici�n para ahorrarnos c�lculos
						int posicion_imagen = posicion(i - elemento.centro.y + k, j - elemento.centro.x + l, 1);
						
						//Obtenemos el p�xel del elemento estructural (true -> 0 false -> 1 (255))
						bool pixel_elemento = (elemento.matriz[k][l]!=-1) && (((elemento.matriz[k][l]==1) && (!elemento_complementario)) || ((elemento.matriz[k][l]==0) && (elemento_complementario))) ;
						
						//Obtenemos el p�xel de la imagen (true -> 0 false -> 1 (255))
						bool pixel_imagen = ((mapa_bits[posicion_imagen]==0) && (!imagen_complementaria)) || ((mapa_bits[posicion_imagen]==255) && (imagen_complementaria));
						
						//Si tenemos un p�xel negro en el elemento y uno blanco en la imagen, el elemento no estar� contenido en ella	
						if(pixel_elemento && !pixel_imagen)
							contenidos=false;
					}

				//Colocamoss el resultado en la imagen auxiliar
				if(contenidos)
					imagen_auxiliar->mapa_bits[posicion(i, j, 1)] = 0;
				else
					imagen_auxiliar->mapa_bits[posicion(i, j, 1)] = 255;
			}			

		delete inicio_DIB;//boramos imagen oiginal

		//Sustituimos la imagen actual por la imagen rotada
		//Reasignaci�n punteos
		inicio_DIB = imagen_auxiliar->inicio_DIB;
		bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
		bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
		if(imagen_auxiliar->paleta!=NULL)
			paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
		else//No hay paleta
			paleta = NULL;
		mapa_bits=(BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
		//Fin reasignaci�n punteros
		
		return 0;
	}

	
	//NO LA COMENTO A�N PORQUE ES PROVISIONAL
	int Texto::ErosionO(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		int tamano_paleta=0;
			if(bitmapinfoheader->biBitCount<=8)
			{
				if(bitmapinfoheader->biClrUsed!=0)
					tamano_paleta=bitmapinfoheader->biClrUsed;
				else
					tamano_paleta=(int)pow(2.0, bitmapinfoheader->biBitCount);
			}

		bool contenidos;
		Texto *imagen_auxiliar=new Texto();
		(*imagen_auxiliar)=(*this); 
		
		for(int l=0;l<numeroLineas;l++)
			for(int p=0;p<lineas[l].numeroPalabras;p++)
				for(int c=0;c<lineas[l].palabras[p].numeroCaracteres;c++)
				{
					for(int i=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.y;i<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.y;i++)
						for(int j=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.x;j<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.x;j++)
						{
							contenidos=true;

							for(int k=0;k<3 && contenidos;k++)
								for(int l=0;l<3 && contenidos;l++)
								{
									int posicion_imagen = posicion(i-elemento.centro.y+k,j-elemento.centro.x+l,1);
									bool pixel_elemento = (elemento.matriz[k][l]!=-1) && (((elemento.matriz[k][l]==1) && (!elemento_complementario)) || ((elemento.matriz[k][l]==0) && (elemento_complementario))) ;
									bool pixel_imagen=imagen_complementaria;
									
									pixel_imagen = ((mapa_bits[posicion_imagen]==0) && (!imagen_complementaria)) || ((mapa_bits[posicion_imagen]==255) && (imagen_complementaria));
									
										
									if(pixel_elemento && !pixel_imagen)
										contenidos=false;
								}

							if(contenidos)
								imagen_auxiliar->mapa_bits[posicion(i,j,1)]=0;
							else
								imagen_auxiliar->mapa_bits[posicion(i,j,1)]=255;
						}
				}

			

			delete inicio_DIB;//boramos imagen oiginal

			//Reasignaci�n punteos
			inicio_DIB=imagen_auxiliar->inicio_DIB;
			bitmapinfoheader=(LPBITMAPINFOHEADER)inicio_DIB;
			bitmapinfo=(LPBITMAPINFO)bitmapinfoheader;
			if(imagen_auxiliar->paleta!=NULL)
				paleta=(RGBQUAD*)((BYTE*)inicio_DIB+sizeof(BITMAPINFOHEADER));
			else
				paleta=NULL;
			mapa_bits=(BYTE*)inicio_DIB+sizeof(BITMAPINFOHEADER)+tamano_paleta*sizeof(RGBQUAD);
			//Fin reasignaci�n punteros
			
			return 0;
	}	

	//============================================================================
	// NOMBRE: Transformaci�nAlAzar
	//
	// DESCRIPCI�N: Obtiene la transformaci�n al azar de la imagen actual respecto a un determinado elemento estructural. El resultado queda almacenado en la imagen actual. (Imagen 256 colores y umbralizada)
	//
	// ARGUMENTOS:	ElementoEstructural elemento -> Elemento estructural usado en la transformaci�n al azar
	//				bool imagen_complementaria	 -> Indica si debemos tratar la imagen como si fuera la complementaria (esto es para ahorrarnos la creaci�n de una imagen complementaria, ahorrando tiempo de CPU)
	//				bool elemento_complementario -> Indica si debemos tratar el elemento estructural como si fuera el complementaria (esto es para ahorrarnos la creaci�n de un elemento complementario, ahorrando tiempo de CPU)
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//============================================================================
	int Texto::TransformacionAlAzar(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Creamos una imagen auxiliar necesaria para hacer la erosi�n d el aimagen complementaria y elemento complementario
		Texto *imagen_auxiliar = new Texto();

		//Copiamos la imagen actual en la auxiliar
		(*imagen_auxiliar) = (*this);

		//Eros�n imagen y elemento originales
		Erosion(elemento, imagen_complementaria, elemento_complementario);

		//Erosi�n imagen y elemento complementarios
		imagen_auxiliar->Erosion(elemento, !imagen_complementaria, !elemento_complementario);

		//Calculamos la intersecci�n de las dos im�genes erosionadas para obtener el resultado final de la transformaci�n
		Interseccion(imagen_auxiliar);

		//Borramos la imagen auxiliar
		delete imagen_auxiliar;

		return 0;
	}

	//============================================================================
	// NOMBRE: Transformaci�nAlAzarO
	//
	// DESCRIPCI�N: Obtiene la transformaci�n al azar de la imagen actual respecto a un determinado elemento estructural. El resultado queda almacenado en la imagen actual. (Imagen 256 colores y umbralizada)
	//
	// ARGUMENTOS:	ElementoEstructural elemento -> Elemento estructural usado en la transformaci�n al azar
	//				bool imagen_complementaria	 -> Indica si debemos tratar la imagen como si fuera la complementaria (esto es para ahorrarnos la creaci�n de una imagen complementaria, ahorrando tiempo de CPU)
	//				bool elemento_complementario -> Indica si debemos tratar el elemento estructural como si fuera el complementaria (esto es para ahorrarnos la creaci�n de un elemento complementario, ahorrando tiempo de CPU)
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//============================================================================
	int Texto::TransformacionAlAzarO(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Creamos una imagen auxiliar necesaria para hacer la erosi�n d el aimagen complementaria y elemento complementario
		Texto *imagen_auxiliar = new Texto();

		//Copiamos la imagen actual en la auxiliar
		(*imagen_auxiliar) = (*this);

		//Eros�n imagen y elemento originales
		ErosionO(elemento, imagen_complementaria, elemento_complementario);

		//Erosi�n imagen y elemento complementarios
		imagen_auxiliar->ErosionO(elemento, !imagen_complementaria, !elemento_complementario);

		//Calculamos la intersecci�n de las dos im�genes erosionadas para obtener el resultado final de la transformaci�n
		InterseccionO(imagen_auxiliar);

		//Borramos la imagen auxiliar
		delete imagen_auxiliar;

		return 0;
	}

	
	//============================================================================
	// NOMBRE: Reducci�n
	//
	// DESCRIPCI�N: Obtiene la reducci�n de la imagen actual respecto a un determinado elemento estructural, es decir, el esqueleto de la imagen. El resultado queda almacenado en la imagen actual. (Imagen 256 colores y umbralizada)
	//
	// ARGUMENTOS:	ElementoEstructural elemento -> Elementos estructurales usados en la reducci�n (pasamos todos para no perder tiempo de CPU calcul�ndolos)
	//				bool imagen_complementaria	 -> Indica si debemos tratar la imagen como si fuera la complementaria (esto es para ahorrarnos la creaci�n de una imagen complementaria, ahorrando tiempo de CPU)
	//				bool elemento_complementario -> Indica si debemos tratar el elemento estructural como si fuera el complementaria (esto es para ahorrarnos la creaci�n de un elemento complementario, ahorrando tiempo de CPU)
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada			
	//============================================================================
	int Texto::Reduccion(ElementoEstructural *elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Creamos las dos im�genes auxiliares necesarias
		Texto *original = new Texto(); 
		Texto *transf = new Texto();

		//Inicializamos las im�genes auxiliares
		(*original) = (*this);
		(*transf) = (*this);

		//Indica si ya hemos encontrado el esqueleto de la imagen
		bool fin = false;
		
		//Calculamos el esqueleto
		while(!fin)
		{
			//Iteramos por los distintos elementos estructurales
			for(int e=0; e<8; e++)
			{
				if(numeroLineas!=0 && lineas[0].numeroPalabras!=0 && lineas[0].palabras[0].numeroCaracteres!=0)
				{
					transf->TransformacionAlAzarO(elemento[e], imagen_complementaria, elemento_complementario);
					DiferenciaO(transf);
				}
				else
				{
					transf->TransformacionAlAzar(elemento[e], imagen_complementaria, elemento_complementario);
					Diferencia(transf);
				}

				//Borramos la imagen auxiliar
				delete transf;

				//Establecemos el nuevo valor de
				transf = new Texto();
				(*transf) = (*this);				
			}

			//Si la imagen no ha cambiado en la �ltima iteraci�n hemos encontrado el esqueleto de la imagen
			if(EsIgual(original))
				fin = true;
			else
			{
				//Borramos el resultado de la iteraci�n anterior
				delete original;

				//Guardamos el resultado de esta iteraci�n
				original = new Texto();
				(*original) = (*this);
			}			
		}

		//Borramos las im�genes auxiliares
		delete transf;
		delete original;

		return 0;
	}


	//NO ESTOY SEGURO DE SI FUNCIONAR� BIEN EL CODIGO PARA NUESTRO CASO, POR AHORA NO LO IMPLEMENTO
	//============================================================================
	// NOMBRE: CodigoCadena
	//
	// DESCRIPCI�N: Obtiene la reducci�n de la imagen actual respecto a un determinado elemento estructural, es decir, el esqueleto de la imagen. El resultado queda almacenado en la imagen actual. (Imagen 256 colores y umbralizada)
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. El car�cter no pertenece a la imagen actual
	//============================================================================
	int Texto::CodigoCadena(Caracter *caracter)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el car�cter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		return 0;
	}


	//============================================================================
	// NOMBRE: NumeroHuecos
	//
	// DESCRIPCI�N: Obtiene el n�mero de huecos del car�cter pasado por par�metro dejando el valor en dicha estructura (en el campo numeroHuecos)
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. El car�cter no pertenece a la imagen actual
	//============================================================================
	int Texto::NumeroHuecos(Caracter *caracter)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el car�cter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Creamos un caracter auxiliar, ya que debemos modificarlo para hallar los huecos
		Texto *caracter_auxiliar=new Texto(caracter->esquina_inferior_derecha.x-caracter->esquina_superior_izquierda.x+1, caracter->esquina_inferior_derecha.y-caracter->esquina_superior_izquierda.y+1, 8, paleta, 256);

		//Indicamos que el car�cter est� umbralizado
		caracter_auxiliar->umbralizada = true;

		//Copiamos el car�cter original en el auxiliar
		for(int i=0; i<=caracter->esquina_inferior_derecha.y-caracter->esquina_superior_izquierda.y;i++)
			memcpy(&caracter_auxiliar->mapa_bits[caracter_auxiliar->posicion(i,0,1)], &mapa_bits[posicion(caracter->esquina_superior_izquierda.y + i, caracter->esquina_superior_izquierda.x, 1)], caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x + 1);
				
		
		//Rellenamos el exterior del car�cter de p�xeles negros
		//Para ello debemos poner semillas en las 4 esquinas del car�cter, siempre que �stas no sean ya un p�xel negro
		Coordenada semilla;
		
		//Esquina superior izquierda
		if(caracter_auxiliar->mapa_bits[caracter_auxiliar->posicion(0 ,0 ,1)]!=0)
		{
			semilla.x = 0;
			semilla.y = 0;
			caracter_auxiliar->rellenarHuecos(semilla);
		}

		//Esquina inferior derecha
		if(caracter_auxiliar->mapa_bits[caracter_auxiliar->posicion(caracter_auxiliar->bitmapinfoheader->biHeight - 1, caracter_auxiliar->bitmapinfoheader->biWidth - 1, 1)]!=0)
		{
			semilla.x = caracter_auxiliar->bitmapinfoheader->biWidth - 1;
			semilla.y = caracter_auxiliar->bitmapinfoheader->biHeight - 1;
			caracter_auxiliar->rellenarHuecos(semilla);
		}

		//Esquina inferior izquierda
		if(caracter_auxiliar->mapa_bits[caracter_auxiliar->posicion(caracter_auxiliar->bitmapinfoheader->biHeight - 1, 0, 1)]!=0)
		{
			semilla.x = 0;
			semilla.y = caracter_auxiliar->bitmapinfoheader->biHeight - 1;
			caracter_auxiliar->rellenarHuecos(semilla);
		}

		//Esquina superior derecha
		if(caracter_auxiliar->mapa_bits[caracter_auxiliar->posicion(0, caracter_auxiliar->bitmapinfoheader->biWidth - 1, 1)]!=0)
		{
			semilla.x = caracter_auxiliar->bitmapinfoheader->biWidth-1;
			semilla.y = 0;
			caracter_auxiliar->rellenarHuecos(semilla);
		}

		//Inicializamos los huecos del car�cter a 0
		int huecos = 0;

		//Calculamos los huecos buscando un p�xel blanco
		for(int i=0; i<caracter_auxiliar->bitmapinfoheader->biHeight; i++)
			for(int j=0; j<caracter_auxiliar->bitmapinfoheader->biWidth; j++)
			{
				if(caracter_auxiliar->mapa_bits[caracter_auxiliar->posicion(i, j, 1)]!=0)
				{
					//Hemos encontrado un hueco
					huecos++;
					
					//Rellenamos el hueco encontrado para no contabilizarlo m�s de una vez
					//Para ello establecemo sel p�xel blanco encontrado como semilla
					semilla.y = i;
					semilla.x = j;
					caracter_auxiliar->rellenarHuecos(semilla);
				}
			}

		//Asociamos el n�mero de huecos con el car�cter
		caracter->numeroHuecos = huecos;

		//Borramos el car�cter auxiliar
		delete caracter_auxiliar;

		return 0;
	}

	
	//============================================================================
	// NOMBRE: NumeroCruces
	//
	// DESCRIPCI�N: Obtiene el n�mero de cruces del car�cter pasado por par�metro dejando el valor en dicha estructura (en el campo numeroCruces)
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. El car�cter no pertenece a la imagen actual
	//============================================================================
	int Texto::NumeroCruces(Caracter *caracter)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el car�cter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos los cruces del car�cter a 0
		int cruces = 0;

		//Calculamos los cruces buscando un p�xel '8-conectado' con 3 o m�s p�xeles
		for(int i=caracter->esquina_superior_izquierda.y+1; i<caracter->esquina_inferior_derecha.y; i++)
			for(int j=caracter->esquina_superior_izquierda.x; j<caracter->esquina_inferior_derecha.x; j++)
			{
				if(conectividad(i,j)>=3)
				{
					//Hemos encontrado un cruce
					cruces++;
				}
			}

		//Asociamos el n�mero de cruces con el car�cter
		caracter->numeroCruces = cruces;
		
        return 0;
	}


	//============================================================================
	// NOMBRE: PuntosTerminales
	//
	// DESCRIPCI�N: Obtiene el n�mero de puntos terminales del car�cter pasado por par�metro dejando el valor en dicha estructura (en el campo numeroPuntosTerminales)
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. El car�cter no pertenece a la imagen actual
	//============================================================================
	int Texto::PuntosTerminales(Caracter *caracter)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el car�cter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos los puntos terminales del car�cter a 0
		int puntos = 0;

		//Calculamos los puntos terminales buscando un p�xel que sea punto final
		for(int i=caracter->esquina_superior_izquierda.y+1; i<caracter->esquina_inferior_derecha.y; i++)
			for(int j=caracter->esquina_superior_izquierda.x; j<caracter->esquina_inferior_derecha.x; j++)
			{
				if(mapa_bits[posicion(i, j, 1)]==0 && esPuntoFinal(i, j))
				{
					//Hemos encontrado un punto final
					puntos++;
				}
			}

		//Asociamos el n�mero de puntos terminales con el car�cter
		caracter->numeroPuntosTerminales = puntos;
		
        return 0;
	}


	//============================================================================
	// NOMBRE: CalcularCaracteristicas
	//
	// DESCRIPCI�N: Calcula diversas caracter�sticas del car�cter pasado por par�metro dejando los valores en dicha estructura. Estas caracter�sticas se obtendr�n para 9 secciones id�nticas del car�cter.
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. El car�cter no pertenece a la imagen actual
	//============================================================================
	int Texto::CalcularCaracteristicas(Caracter *caracter)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el car�cter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos a 0 el n�mero de p�xeles del car�cter
		caracter->numeroPixeles = 0;		

		//'Creamos' las secciones del car�cter
		caracter->caracteristicasDireccionalesB = new int*[9];
		caracter->caracteristicasDireccionalesR = new BYTE*[9];
		caracter->caracteristicasEstructurales = new BYTE*[9];
		caracter->caracteristicasConcavidad = new BYTE*[9];

		for(int s=0;s<9;s++)
		{
			caracter->caracteristicasDireccionalesB[s] = new int[12];
			caracter->caracteristicasDireccionalesR[s] = new BYTE[12];
			caracter->caracteristicasEstructurales[s] = new BYTE[12];
			caracter->caracteristicasConcavidad[s] = new BYTE[8];

			//Inicializamos caracter�sticas
			for(int c=0;c<12;c++)
			{
				caracter->caracteristicasDireccionalesB[s][c] = 0;
				caracter->caracteristicasDireccionalesR[s][c] = 0;
				caracter->caracteristicasEstructurales[s][c] = 0;
				
				if(c<8)
					caracter->caracteristicasConcavidad[s][c] = 0;
			}
		}

		//Creamos una matriz en la que guardaremos el 'gradiente' del car�cter (realmente guardaremos el intervalo al que pertenece de entre 12)
		BYTE **gradiente = new BYTE*[caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y + 1];
		for(int i=0; i<caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y + 1; i++)
			gradiente[i] = new BYTE[caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x + 1];

		//Calculamos el gradiente del car�cter
		Sobel(caracter, gradiente);

		//Calculamos las dimensiones de una secci�n
		int tamano_seccion_horizontal = (caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x + 1) / 3;
		int tamano_seccion_vertical = (caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y + 1) / 3;
		int tamano_seccion = tamano_seccion_vertical * tamano_seccion_horizontal;
				
		//Recorremos el gradiente (y el caracter)
		for(int i=0; i<=caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y; i++)
			for(int j=0; j<=caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x; j++)
			{
				//Calculamos en que secci�n estamos				
				//Primero hallamos la fila y columna
				int f, c;
				
				//Fila
				if(tamano_seccion_vertical==0)
					f = 0;
				else
					f = (i - 2) / tamano_seccion_vertical;//El -2 es para que en las �ltimas filas del car�cter salga la fila 3(2) y no la 4(3) y
				if(f<0)//Evitamos valores negativos
					f = 0;

				//Columna
				if(tamano_seccion_horizontal==0)
					c = 0;
				else
					c = (j - 2) / tamano_seccion_horizontal;//El -2 es para que en las �ltimas columnas del car�cter salga la secci�n 3(2) y no la 4(3)
				if(c<0)//Evitamos valores negativos
					c = 0;
				
				//Obtenemos la secci�n
				int seccion = f * 3 + c;

				//Calculamos la fila y columna reales en la imagen
				int fila_real = i + caracter->esquina_superior_izquierda.y;
				int columna_real = j + caracter->esquina_superior_izquierda.x;

				//Incrementamos en 1 el intervalo del gradiente correspondiente en la secci�n actual
				caracter->caracteristicasDireccionalesB[seccion][gradiente[i][j]]++;

				//Si el p�xel actual es negro
				if(mapa_bits[posicion(fila_real, columna_real, 1)]==0)
				{
					caracter->caracteristicasConcavidad[seccion][0]++;//Pixeles negros de la secci�n

					//Calculamos la caracter�sticas estructurales para todos los p�xeles menos primera fila y columna y �ltima fila y columna
					if(i>0 && i<caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y && j>0 && j<caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x)
					{
						//Caracteristicas Estructurales
						BYTE vecino1;
						BYTE vecino2;
						
						//Reglas 1 y 2 (Horizontal line)
						if(mapa_bits[posicion(fila_real, columna_real - 1, 1)]==0 && mapa_bits[posicion(fila_real, columna_real + 1, 1)]==0)
						{
							vecino1 = gradiente[i][j-1];
							vecino2 = gradiente[i][j+1];

							if(vecino1>=2 && vecino1<=4 && vecino2>=2 && vecino2<=4)
								caracter->caracteristicasEstructurales[seccion][0]++;
							else
							{
								if(vecino1>=8 && vecino1<=10 && vecino2>=8 && vecino2<=10)
									caracter->caracteristicasEstructurales[seccion][1]++;
							}
						}

						//Reglas 3 y 4 (Vertical line)
						if(mapa_bits[posicion(fila_real - 1, columna_real, 1)]==0 && mapa_bits[posicion(fila_real + 1, columna_real, 1)]==0)
						{
							vecino1 = gradiente[i-1][j];
							vecino2 = gradiente[i+1][j];

							if(vecino1>=5 && vecino1<=7 && vecino2>=5 && vecino2<=7)
								caracter->caracteristicasEstructurales[seccion][2]++;
							else
							{
								if((vecino1==0 || vecino1==1 || vecino1==11) && (vecino2==0 || vecino2==1 || vecino1==11))
									caracter->caracteristicasEstructurales[seccion][3]++;
							}
						}					

						//Reglas 5 y 6 (Diagonal Rising)
						if(mapa_bits[posicion(fila_real - 1, columna_real + 1, 1)]==0 && mapa_bits[posicion(fila_real + 1, columna_real - 1, 1)]==0)
						{
							vecino1 = gradiente[i-1][j+1];
							vecino2 = gradiente[i+1][j-1];

							if(vecino1>=4 && vecino1<=6 && vecino2>=4 && vecino2<=6)
								caracter->caracteristicasEstructurales[seccion][4]++;
							else
							{
								if((vecino1==0 || vecino1==10 || vecino1==11) && (vecino2==0 || vecino2==10 || vecino1==11))
									caracter->caracteristicasEstructurales[seccion][5]++;
							}
						}		

						//Reglas 7 y 8 (Diagonal Falling)
						if(mapa_bits[posicion(fila_real - 1, columna_real - 1, 1)]==0 && mapa_bits[posicion(fila_real + 1, columna_real + 1, 1)]==0)
						{
							vecino1 = gradiente[i-1][j-1];
							vecino2 = gradiente[i+1][j+1];

							if(vecino1>=1 && vecino1<=3 && vecino2>=1 && vecino2<=3)
								caracter->caracteristicasEstructurales[seccion][6]++;
							else
							{
								if(vecino1>=7 && vecino1<=9 && vecino2>=7 && vecino2<=9)
									caracter->caracteristicasEstructurales[seccion][7]++;
							}
						}	
					
						//Regla 9 (Corner 1)
						if(mapa_bits[posicion(fila_real - 1, columna_real, 1)]==0 && mapa_bits[posicion(fila_real, columna_real + 1, 1)]==0)
						{
							vecino1 = gradiente[i-1][j];
							vecino2 = gradiente[i][j+1];

							if(vecino1>=5 && vecino1<=7 && vecino2>=8 && vecino2<=10)
								caracter->caracteristicasEstructurales[seccion][8]++;					
						}				
					
						//Regla 10 (Corner 2)
						if(mapa_bits[posicion(fila_real + 1, columna_real, 1)]==0 && mapa_bits[posicion(fila_real, columna_real + 1, 1)]==0)
						{
							vecino1 = gradiente[i+1][j];
							vecino2 = gradiente[i][j+1];

							if(vecino1>=5 && vecino1<=7 && vecino2>=2 && vecino2<=4)
								caracter->caracteristicasEstructurales[seccion][9]++;					
						}	

						//Regla 11 (Corner 3)
						if(mapa_bits[posicion(fila_real - 1, columna_real, 1)]==0 && mapa_bits[posicion(fila_real, columna_real - 1, 1)]==0)
						{
							vecino1 = gradiente[i-1][j];
							vecino2 = gradiente[i][j-1];

							if(vecino1>=8 && vecino1<=10 && (vecino2==0 || vecino2==1 || vecino2==11))
								caracter->caracteristicasEstructurales[seccion][10]++;					
						}	

						//Regla 12 (Corner 4)
						if(mapa_bits[posicion(fila_real + 1, columna_real, 1)]==0 && mapa_bits[posicion(fila_real, columna_real - 1, 1)]==0)
						{
							vecino1 = gradiente[i+1][j];
							vecino2 = gradiente[i][j-1];

							if((vecino2==0 || vecino2==1 || vecino2==11) && vecino2>=2 && vecino2<=4 )
								caracter->caracteristicasEstructurales[seccion][11]++;					
						}
					}
				}
			}

		//Calculamos las caracter�sticas de concavidad y actualizamos las caracter�sticas que son porcentajes	
		for(int s=0; s<9; s++)
		{
			//Caracteristicas concavidad
			int pixeles_negros_seccion = caracter->caracteristicasConcavidad[s][0];

			//Actualizamos el n�mero de p�xeles del car�cter
			caracter->numeroPixeles += pixeles_negros_seccion;

			//Hacemos la comprobaci�n para evitar divisiones por 0
			if(pixeles_negros_seccion>0)
			{
				double porcentaje;
				//Calculamos el porcentaje de pixeles con la regla 1 � 2 en la regi�n
				porcentaje = ( ((caracter->caracteristicasEstructurales[s][0]*100)/pixeles_negros_seccion) + ((caracter->caracteristicasEstructurales[s][1]*100)/pixeles_negros_seccion) ) / 2;
				caracter->caracteristicasConcavidad[s][1] = (int)porcentaje;

				//Calculamos el porcentaje de pixeles con la regla 3 � 4 en la regi�n
				porcentaje = ( ((caracter->caracteristicasEstructurales[s][2]*100)/pixeles_negros_seccion) + ((caracter->caracteristicasEstructurales[s][3]*100)/pixeles_negros_seccion) ) / 2;
				caracter->caracteristicasConcavidad[s][2] = (int)porcentaje;

				//Calculamos el porcentaje de pixeles con la regla 5 � 6 en la regi�n
				porcentaje = (caracter->caracteristicasEstructurales[s][4]*100)/pixeles_negros_seccion + (caracter->caracteristicasEstructurales[s][5]*100)/pixeles_negros_seccion + (caracter->caracteristicasEstructurales[s][6]*100)/pixeles_negros_seccion + (caracter->caracteristicasEstructurales[s][7]*100)/pixeles_negros_seccion;
				porcentaje = porcentaje / 4;
				caracter->caracteristicasConcavidad[s][3] = (int)porcentaje;

				//Calculamos los porcentajes restantes
				caracter->caracteristicasConcavidad[s][4] = (caracter->caracteristicasEstructurales[s][8]*100)/pixeles_negros_seccion;
				caracter->caracteristicasConcavidad[s][5] = (caracter->caracteristicasEstructurales[s][9]*100)/pixeles_negros_seccion;
				caracter->caracteristicasConcavidad[s][6] = (caracter->caracteristicasEstructurales[s][10]*100)/pixeles_negros_seccion;
				caracter->caracteristicasConcavidad[s][7] = (caracter->caracteristicasEstructurales[s][11]*100)/pixeles_negros_seccion;
			}

			//Actualizamos lo necesario en las que son porcentajes
			for(int c=0; c<12; c++)
			{
				//Hacemos la comprobaci�n para evitar divisiones por 0
				if(tamano_seccion!=0)
					caracter->caracteristicasDireccionalesR[s][c] = (caracter->caracteristicasDireccionalesB[s][c]*100)/tamano_seccion;

				//Hacemos la comprobaci�n porque se ve que a veces se pasa del 100% debido a las distintas aproximaciones realizadas
				//COMPROBAR SI REALMENTE ES POR ESO O SI HAY ALGO MAL
				if(caracter->caracteristicasDireccionalesR[s][c]>100)
					caracter->caracteristicasDireccionalesR[s][c] = 100;
				
				//Hacemos la comprobaci�n para evitar divisiones por 0
				if(pixeles_negros_seccion!=0)
					caracter->caracteristicasEstructurales[s][c] = (caracter->caracteristicasEstructurales[s][c]*100)/pixeles_negros_seccion;				
			}

			if(tamano_seccion!=0)
				caracter->caracteristicasConcavidad[s][0] = (caracter->caracteristicasConcavidad[s][0]*100)/tamano_seccion;			

		}

		//Borramos la matriz de gradiente
		for(int i=0; i<=caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y; i++)
			delete gradiente[i];
		delete gradiente;
		
		return 0;
	}


	//============================================================================
	// NOMBRE: GetNumeroVecinosKNN
	//
	// DESCRIPCI�N: Establece el n�mero de vecinos en caso de usar el clasificador KNN.
	//
	// ARGUMENTOS: -
	//							
	// SALIDA: Un valor entero que indica la k elegida al usar el clasificador KNN
	//============================================================================
	int Texto::GetNumeroVecinosKNN()
	{
		return numeroVecinos;
	}


	//============================================================================
	// NOMBRE: SetNumeroVecinosKNN
	//
	// DESCRIPCI�N: Establece el n�mero de vecinos en caso de usar el clasificador KNN.
	//
	// ARGUMENTOS: int numero_vecinos -> Indica la k elegida al usar el clasificador KNN
	//							
	// SALIDA: -
	//============================================================================
	void Texto::SetNumeroVecinosKNN(int numero_vecinos)
	{
		numeroVecinos = numero_vecinos;
	}


	//============================================================================
	// NOMBRE: CrearClasificador
	//
	// DESCRIPCI�N: Inicializa el clasificador indicado por par�metro.
	//
	// ARGUMENTOS:	int clasificador -> Tipo de clasificador que queremos construir:
	//										0 -> KNN
	//										1 -> Naive Bayes
	//							
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. No existe el clasificador seleccionado	
	//			2 -> Error. Par�metro incorrecto para el clasificador seleccionado	
	//			3 -> Error. No se ha podido abrir o cerrar la BBDD
	//============================================================================
	int Texto::CrearClasificador(int clasificador_seleccionado)
	{
		//Creamos la bbdd si no existe
		if(bbdd == NULL)
			bbdd = new InterfazBBDD();
		
		//Abrimos la bbdd
		if(bbdd->AbrirBBDD(rutaBBDD)<0)
			return 3;//Error al abrir la base de datos

		//Creamos el clasificador
		switch(clasificador_seleccionado)
		{
			case CKNN:
				//Comprobamos que los par�metros sean correctos
				if(numeroVecinos<1)
					return 2;
				clasificador = new KNN(perfil, bbdd, numeroVecinos);
			break;
			case CBAYES:
				clasificador = new NaiveBayes(perfil, bbdd);
			default:
				return 1;
		}

		//Cerramos la bbdd
		if(bbdd->CerrarBBDD()<0)
			return 3;//Error al abrir la base de datos

		return 0;
	}


	//============================================================================
	// NOMBRE: Reconocer
	//
	// DESCRIPCI�N: Realiza el reconocimiento o aprendizaje de untexto dependiendo del modo establecido. En el caso de reconocimiento usaremos un determinado clasificador.
	//
	// ARGUMENTOS: int modo -> Indica si estamos en aprendizaje (0) o reconocimiento (>0), habiendo en este �ltimo caso varios tipos(POR AHORA A�N NO EST�N IMPLEMENTADOS)
	//							
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. Estamos en reconocimiento y no hay un clasificador seleccionado
	//			3 -> Error. Estamos en aprendizaje y el texto reconocido no coincide con la segmentaci�n
	//			4 -> Error. No se ha podido abrir o cerrar la BBDD
	//============================================================================
	int Texto::Reconocer(int modo)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;		
		
		//Comprobamos que haya un clasificador asignado en caso de estar en reconocimiento
		if(modo!=0 && clasificador==NULL)
			return 2;

		if(modo==0)
		{
			if(aprendeCaracteres()>0)
				return 3;

			//Creamos la bbdd si no existe
			if(bbdd == NULL)
				bbdd = new InterfazBBDD();
		}

		//Abrimos la bbdd
		if(bbdd->AbrirBBDD(rutaBBDD)<0)
			return 4;//Error al abrir la base de datos
		
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				ReconocerPalabra(&lineas[l].palabras[p], modo);

		string t ="";
		for(int l=0; l<numeroLineas; l++)
		{
			for(int p=0; p<lineas[l].numeroPalabras; p++)
			{
				for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
				{
					t += lineas[l].palabras[p].caracteres[c].reconocido;					
				}

				t+=" ";
			}

			t+="\n";
		}

		textoReconocido = new char[t.length()];
		strcpy(textoReconocido, (String)t.data());

		//Cerramos la bbdd
		if(bbdd->CerrarBBDD()<0)
			return 4;//Error al abrir la base de datos

		return 0;	
	}


	//============================================================================
	// NOMBRE: ReconocerPalabra
	//
	// DESCRIPCI�N: Realiza el reconocimiento o aprendizaje de una palabra dependiendo del modo establecido. En el caso de reconocimiento usaremos un determinado clasificador.
	//
	// ARGUMENTOS:	Palabra *palabra -> Puntero a una palabra de la imagen
	//				int modo		 -> Indica si estamos en aprendizaje (0) o reconocimiento (>0), habiendo en este �ltimo caso varios tipos(POR AHORA A�N NO EST�N IMPLEMENTADOS)
	//							
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. La palabra no pertenece a la imagen actual
	//			3 -> Error. Estamos en reconocimiento y no hay un clasificador seleccionado
	//============================================================================
	int Texto::ReconocerPalabra(Palabra *palabra, int modo)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que la palabra pertenece a la imagen
		if(palabra->esquina_superior_izquierda.x<0 || palabra->esquina_superior_izquierda.y<0 || palabra->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || palabra->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;
		
		//Comprobamos que haya un clasificador asignado en caso de estar en reconocimiento
		if(modo!=0 && clasificador==NULL)
			return 3;

		if(modo==0)
		{
			for(int c=0;c<palabra->numeroCaracteres;c++)
			{
				Caracter caracter_actual = palabra->caracteres[c];
				NumeroHuecos(&caracter_actual);
				NumeroCruces(&caracter_actual);
				PuntosTerminales(&caracter_actual);
				CalcularCaracteristicas(&caracter_actual);

				if(bbdd->idCaracter(caracter_actual.reconocido)<0)
					bbdd->InsertarCaracter(caracter_actual.reconocido);
				
				double ancho = caracter_actual.esquina_inferior_derecha.x - caracter_actual.esquina_superior_izquierda.x + 1;
				double alto = caracter_actual.esquina_inferior_derecha.y - caracter_actual.esquina_superior_izquierda.y + 1;
		
				bbdd->InsertarInstancia(perfil, caracter_actual.reconocido, caracter_actual.numeroHuecos, caracter_actual.numeroCruces, caracter_actual.numeroPuntosTerminales, ancho/alto, caracter_actual.caracteristicasDireccionalesB, caracter_actual.caracteristicasDireccionalesR, caracter_actual.caracteristicasEstructurales, caracter_actual.caracteristicasConcavidad);
			}			
		}

		if(modo==1)
		{//El modo 1
			for(int c=0;c<palabra->numeroCaracteres;c++)
			{
				int k=c+1;
				double acierto_normal;
				double acierto_fusionado;
				bool mejora=false;
				bool mejora_global=false;

				Caracter caracter_actual = palabra->caracteres[c];
				NumeroHuecos(&caracter_actual);
				NumeroCruces(&caracter_actual);
				PuntosTerminales(&caracter_actual);
				
				double ancho = caracter_actual.esquina_inferior_derecha.x - caracter_actual.esquina_superior_izquierda.x + 1;
				double alto = caracter_actual.esquina_inferior_derecha.y - caracter_actual.esquina_superior_izquierda.y + 1;
				caracter_actual.relacionAspecto = ancho/alto;

				CalcularCaracteristicas(&caracter_actual);

				if(caracter_actual.numeroPixeles!=0)
				{

					acierto_normal = clasificador->ClasificarCaracter(/*perfil, */&caracter_actual);

					if(c+1<palabra->numeroCaracteres)
					{
						do
						{	
							mejora = false;
							
							Caracter caracter_fusionado = palabra->caracteres[c];
							caracter_fusionado.esquina_inferior_derecha=palabra->caracteres[k].esquina_inferior_derecha;		

							if(palabra->caracteres[c].esquina_superior_izquierda.y>palabra->caracteres[k].esquina_superior_izquierda.y)
								caracter_fusionado.esquina_superior_izquierda.y=palabra->caracteres[k].esquina_superior_izquierda.y;

							if(palabra->caracteres[k].esquina_inferior_derecha.y<palabra->caracteres[c].esquina_inferior_derecha.y)
								caracter_fusionado.esquina_inferior_derecha.y=palabra->caracteres[c].esquina_inferior_derecha.y;

							NumeroHuecos(&caracter_fusionado);
							NumeroCruces(&caracter_fusionado);
							PuntosTerminales(&caracter_fusionado);
							double ancho = caracter_fusionado.esquina_inferior_derecha.x - caracter_fusionado.esquina_superior_izquierda.x + 1;
							double alto = caracter_fusionado.esquina_inferior_derecha.y - caracter_fusionado.esquina_superior_izquierda.y + 1;
							caracter_fusionado.relacionAspecto = ancho/alto;
							CalcularCaracteristicas(&caracter_fusionado);

							if(caracter_fusionado.numeroPixeles!=caracter_actual.numeroPixeles)
							{
								acierto_fusionado = clasificador->ClasificarCaracter(/*perfil, */&caracter_fusionado);
								
								if(acierto_fusionado>acierto_normal)
								{
									mejora = true;
									mejora_global = mejora_global || mejora;
									caracter_actual = caracter_fusionado;
									acierto_normal = acierto_fusionado;

									k++;
								}
							}
						}while(mejora && k<palabra->numeroCaracteres);
					}

					//ahora toca 'rehacer' la palabra
					if(!mejora_global)//no ha mejorado la fusi�n y no hay que tocar nada
					{
						palabra->caracteres[c] = caracter_actual;
					}
					else//debemos reestructurar la palabra
					{
						palabra->numeroCaracteres = palabra->numeroCaracteres - (k - (c+1));

						Caracter *palabra_nueva = new  Caracter[palabra->numeroCaracteres];

						for(int car=0;car<c;car++)
							palabra_nueva[car]=palabra->caracteres[car];

						palabra_nueva[c] = caracter_actual;

						for(int car=c+1;car<palabra->numeroCaracteres;car++, k++)
							palabra_nueva[car] = palabra->caracteres[k];

						delete palabra->caracteres;

						palabra->caracteres = palabra_nueva;
					}
				}
				else
				{
					palabra->numeroCaracteres--;
					
					Caracter *palabra_nueva = new  Caracter[palabra->numeroCaracteres];

					for(int car=0;car<c;car++)
						palabra_nueva[car]=palabra->caracteres[car];

					for(int car=c;car<palabra->numeroCaracteres;car++)
							palabra_nueva[car] = palabra->caracteres[car+1];

					delete palabra->caracteres;

					palabra->caracteres = palabra_nueva;

					c--;
				}
			}
		}

		//bbdd->CerrarBBDD();
		return 0;		
	}


	int Texto::GetLinea(int fila, int columna)
	{
		for(int l=0; l<numeroLineas; l++)
		{
			if(fila>=lineas[l].esquina_superior_izquierda.y && fila<=lineas[l].esquina_inferior_derecha.y && columna>=lineas[l].esquina_superior_izquierda.x && columna<=lineas[l].esquina_inferior_derecha.x)
				return l;
		}

		return -1;
	}


	int Texto::PintarLinea(int linea)
	{
		for(int i=lineas[linea].esquina_superior_izquierda.y; i<=lineas[linea].esquina_inferior_derecha.y; i++)
			for(int j=lineas[linea].esquina_superior_izquierda.x; j<=lineas[linea].esquina_inferior_derecha.x; j++)
				if(mapa_bits[posicion(i, j, 1)]!=0)
					mapa_bits[posicion(i, j, 1)] = 127;

		return 0;
	}

	int Texto::Fusionar(int linea1, int linea2)
	{
		//Comprobamos que la fusi�n sea v�lida
		if(linea1!=linea2 - 1 || linea1>=numeroLineas - 1 || linea1<0 ||linea2<0 || numeroLineas<=0)
			return 1;

		//PONER LO DE ESPACIADO COMO DEFINE!!!!!
		int espaciado = 15;//lineas[0].esquina_superior_izquierda.y;		
		
			//Redistribuimos las l�neas
		Linea *lineas_auxiliares = new Linea[numeroLineas - 1];

		for(int l=0; l<linea1; l++)
			lineas_auxiliares[l] = lineas[l];

		Linea fusionada;
		fusionada.esquina_superior_izquierda = lineas[linea1].esquina_superior_izquierda;
		fusionada.esquina_inferior_derecha = lineas[linea2].esquina_inferior_derecha;
		fusionada.esquina_inferior_derecha.y -= espaciado;

		lineas_auxiliares[linea1] = fusionada;

		for(int l=linea2; l<numeroLineas-1; l++)
		{
			lineas_auxiliares[l] = lineas[l+1];
			lineas_auxiliares[l].esquina_superior_izquierda.y -= espaciado;
			lineas_auxiliares[l].esquina_inferior_derecha.y -= espaciado;
		}
		
		//Creamos una nueva imagen con el tama�o necesario para guardar todas las l�neas halladas con una separaci�n interl�nea de 'espaciado' p�xeles
		Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight - espaciado, 8, paleta, 256); 
		
		//Creamos la estructura de l�neas de la imagen segmentada
		imagen_auxiliar->lineas = lineas_auxiliares;
		imagen_auxiliar->numeroLineas = numeroLineas - 1;

		//Copiamos las l�neas de la imagen original en la imagen segmentada
		int tamano_pixel = bitmapinfoheader->biBitCount / 8;

		//Calculamos las columnas de relleno
		int columnas_relleno = 0;
		if((bitmapinfoheader->biWidth*tamano_pixel) % 4!=0)
			columnas_relleno = 4 - ((bitmapinfoheader->biWidth * tamano_pixel) % 4);

		//Copiamos las l�neas de la imagen original en la imagen segmentada
		for(int l=0; l<linea1; l++)
		{
			//Asignamos los atributos de una l�nea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			/*//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado * (l + 1);
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y - inicio + espaciado * (l + 1);	*/			

			//Pintamos de negro la fila inferior y superior de la l�nea en la imagen segmentada
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la l�nea de la imagen original en la imagen segmentada
			memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[l].esquina_inferior_derecha.y, 0, 1)], (lineas[l].esquina_inferior_derecha.y - lineas[l].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));
		}

		imagen_auxiliar->lineas[linea1].palabras = NULL;
		imagen_auxiliar->lineas[linea1].numeroPalabras = 0;
		//Copiamos la l�nea de la imagen original en la imagen segmentada
		memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea1].esquina_superior_izquierda.y + (lineas[linea1].esquina_inferior_derecha.y - lineas[linea1].esquina_superior_izquierda.y + 1), 0, 1)], &mapa_bits[posicion(lineas[linea1].esquina_inferior_derecha.y, 0, 1)], (lineas[linea1].esquina_inferior_derecha.y - lineas[linea1].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));

		//Copiamos la l�nea de la imagen original en la imagen segmentada
		memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea1].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[linea2].esquina_inferior_derecha.y, 0, 1)], (lineas[linea2].esquina_inferior_derecha.y - lineas[linea2].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));

		//Pintamos de negro la fila inferior y superior de la l�nea en la imagen segmentada
		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea1].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea1].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
		
		for(int l=linea2; l<numeroLineas-1; l++)
		{
			//Asignamos los atributos de una l�nea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			/*//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado * (l + 1);
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y - inicio + espaciado * (l + 1);	*/			

			//Pintamos de negro la fila inferior y superior de la l�nea en la imagen segmentada
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la l�nea de la imagen original en la imagen segmentada
			memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[l+1].esquina_inferior_derecha.y, 0, 1)], (lineas[l+1].esquina_inferior_derecha.y - lineas[l+1].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));
		}

		delete inicio_DIB;//Borramos la imagen original
		delete lineas;

		//Sustituimos la imagen actual por la imagen segmentada
		//Reasignamos punteros
		inicio_DIB = imagen_auxiliar->inicio_DIB;
		bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
		bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
		paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));		
		mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
		lineas = imagen_auxiliar->lineas;
		numeroLineas = imagen_auxiliar->numeroLineas;
		//Fin reasignaci�n de punteros

		return 0;		
	}


	int Texto::Dividir(int linea)
	{
		//Comprobamos que la fusi�n sea v�lida
		if(linea>=numeroLineas || linea<0 || numeroLineas<=0)
			return 1;

		//PONER LO DE ESPACIADO COMO DEFINE!!!!!
		int espaciado = 15;//lineas[0].esquina_superior_izquierda.y;		
		
		//Redistribuimos las l�neas
		Linea *lineas_auxiliares = new Linea[numeroLineas + 1];

		for(int l=0; l<linea; l++)
			lineas_auxiliares[l] = lineas[l];

		Linea parte1, parte2;
		
		parte1.esquina_superior_izquierda = lineas[linea].esquina_superior_izquierda;
		parte1.esquina_inferior_derecha.x = bitmapinfoheader->biWidth - 1;
		parte1.esquina_inferior_derecha.y = lineas[linea].esquina_superior_izquierda.y + ((lineas[linea].esquina_inferior_derecha.y - lineas[linea].esquina_superior_izquierda.y + 1) / 2);
		
		parte2.esquina_superior_izquierda.x = 0;
		parte2.esquina_superior_izquierda.y = parte1.esquina_inferior_derecha.y + espaciado;
		parte2.esquina_inferior_derecha = lineas[linea].esquina_inferior_derecha;
		parte2.esquina_inferior_derecha.y += espaciado;

		lineas_auxiliares[linea] = parte1;
		lineas_auxiliares[linea + 1] = parte2;

		for(int l=linea + 2; l<numeroLineas+1; l++)
		{
			lineas_auxiliares[l] = lineas[l-1];
			lineas_auxiliares[l].esquina_superior_izquierda.y += espaciado;
			lineas_auxiliares[l].esquina_inferior_derecha.y += espaciado;
		}
		
		//Creamos una nueva imagen con el tama�o necesario para guardar todas las l�neas halladas con una separaci�n interl�nea de 'espaciado' p�xeles
		Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight + espaciado, 8, paleta, 256); 
		
		//Creamos la estructura de l�neas de la imagen segmentada
		imagen_auxiliar->lineas = lineas_auxiliares;
		imagen_auxiliar->numeroLineas = numeroLineas + 1;

		//Copiamos las l�neas de la imagen original en la imagen segmentada
		int tamano_pixel = bitmapinfoheader->biBitCount / 8;

		//Calculamos las columnas de relleno
		int columnas_relleno = 0;
		if((bitmapinfoheader->biWidth*tamano_pixel) % 4!=0)
			columnas_relleno = 4 - ((bitmapinfoheader->biWidth * tamano_pixel) % 4);

		//Copiamos las l�neas de la imagen original en la imagen segmentada
		for(int l=0; l<linea; l++)
		{
			//Asignamos los atributos de una l�nea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			/*//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado * (l + 1);
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y - inicio + espaciado * (l + 1);	*/			

			//Pintamos de negro la fila inferior y superior de la l�nea en la imagen segmentada
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la l�nea de la imagen original en la imagen segmentada
			memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[l].esquina_inferior_derecha.y, 0, 1)], (lineas[l].esquina_inferior_derecha.y - lineas[l].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));
		}

		//Copiamos la l�nea de la imagen original en la imagen segmentada
		imagen_auxiliar->lineas[linea].palabras = NULL;
		imagen_auxiliar->lineas[linea].numeroPalabras = 0;
		memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y, 0, 1)], (imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y - imagen_auxiliar->lineas[linea].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));

		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);

		//Copiamos la l�nea de la imagen original en la imagen segmentada
		imagen_auxiliar->lineas[linea + 1].palabras = NULL;
		imagen_auxiliar->lineas[linea + 1].numeroPalabras = 0;
		memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea + 1].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[linea].esquina_inferior_derecha.y, 0, 1)], (imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y - imagen_auxiliar->lineas[linea].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));

		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea + 1].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea + 1].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);


		for(int l=linea + 2; l<numeroLineas+1; l++)
		{
			//Asignamos los atributos de una l�nea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			/*//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado * (l + 1);
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y - inicio + espaciado * (l + 1);	*/			

			//Pintamos de negro la fila inferior y superior de la l�nea en la imagen segmentada
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la l�nea de la imagen original en la imagen segmentada
			memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[l-1].esquina_inferior_derecha.y, 0, 1)], (lineas[l-1].esquina_inferior_derecha.y - lineas[l-1].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));
		}

		delete inicio_DIB;//Borramos la imagen original
		delete lineas;

		//Sustituimos la imagen actual por la imagen segmentada
		//Reasignamos punteros
		inicio_DIB = imagen_auxiliar->inicio_DIB;
		bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
		bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
		paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));		
		mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
		lineas = imagen_auxiliar->lineas;
		numeroLineas = imagen_auxiliar->numeroLineas;
		//Fin reasignaci�n de punteros

		return 0;
	}

	int Texto::AprendeCaracter(Caracter caracter)
	{
		//comprobar q hay una bbdd cargada

		NumeroHuecos(&caracter);
		NumeroCruces(&caracter);
		PuntosTerminales(&caracter);
		CalcularCaracteristicas(&caracter);

		if(bbdd->idCaracter(caracter.reconocido)<0)
			bbdd->InsertarCaracter(caracter.reconocido);
				
		double ancho = caracter.esquina_inferior_derecha.x - caracter.esquina_superior_izquierda.x +1;
		double alto = caracter.esquina_inferior_derecha.y - caracter.esquina_superior_izquierda.y + 1;
		
		bbdd->InsertarInstancia(perfil, caracter.reconocido, caracter.numeroHuecos, caracter.numeroCruces, caracter.numeroPuntosTerminales, ancho/alto,caracter.caracteristicasDireccionalesB, caracter.caracteristicasDireccionalesR, caracter.caracteristicasEstructurales, caracter.caracteristicasConcavidad);

		return 0;

	}

	int Texto::CorregirPalabra(Palabra *reconocida, String corregida)
	{
		int caracter_segmentado_inicial, caracter_segmentado_final;
		int caracter_string_inicial, caracter_string_final;
		int caracter_corregida_inicial, caracter_corregida_final;

		caracter_segmentado_inicial = 0;
		caracter_string_inicial = 0;
		caracter_corregida_inicial = 0;

		while(caracter_segmentado_inicial<reconocida->numeroCaracteres && caracter_corregida_inicial<strlen(corregida))
		{
			if(reconocida->caracteres[caracter_segmentado_inicial].reconocido[caracter_string_inicial] == corregida[caracter_corregida_inicial])
			{
				caracter_corregida_inicial++;

				if(caracter_string_inicial<strlen(reconocida->caracteres[caracter_segmentado_inicial].reconocido) - 1)
					caracter_string_inicial++;
				else
				{
					caracter_segmentado_inicial++;
					caracter_string_inicial = 0;
				}
			}
			else
			{
				if(caracter_string_inicial!=0)
					return -1;
				
				bool fusion = false;

				/*if(strlen(reconocida->caracteres[caracter_segmentado_inicial].reconocido)!= 1)
					caracter_string_final = strlen(reconocida->caracteres[caracter_segmentado_inicial].reconocido) - 1;
				else
				{*/
					caracter_segmentado_final = caracter_segmentado_inicial + 1; 
					caracter_string_final  = 0;//strlen(reconocida->caracteres[caracter_segmentado_finall].reconocido) - 1;
				//}

				while(caracter_segmentado_final<reconocida->numeroCaracteres && !fusion)
				{
					for(caracter_corregida_final = caracter_corregida_inicial + 1; reconocida->caracteres[caracter_segmentado_final].reconocido[caracter_string_final] != corregida[caracter_corregida_final] && caracter_corregida_final<strlen(corregida); caracter_corregida_final++);

					if(caracter_corregida_final==strlen(corregida) || (caracter_corregida_final==strlen(corregida)-1 && caracter_segmentado_final!=reconocida->numeroCaracteres-1) || (caracter_segmentado_final==reconocida->numeroCaracteres-1 && caracter_corregida_final!=strlen(corregida)-1))//no hemos encontrado ninguno igual o mas largo reconocido
					{
						caracter_segmentado_final++; 
						caracter_string_final = 0;//strlen(reconocida->caracteres[caracter_segmentado_finall].reconocido) - 1;						
					}
					else//fusion
					{
						fusion=true;
						
						Caracter caracter_fusionado;

						caracter_fusionado.esquina_superior_izquierda = reconocida->caracteres[caracter_segmentado_inicial].esquina_superior_izquierda;
						caracter_fusionado.esquina_inferior_derecha = reconocida->caracteres[caracter_segmentado_final-1].esquina_inferior_derecha;

						if(reconocida->caracteres[caracter_segmentado_final-1].esquina_superior_izquierda.y<caracter_fusionado.esquina_superior_izquierda.y)
							caracter_fusionado.esquina_superior_izquierda.y = reconocida->caracteres[caracter_segmentado_final-1].esquina_superior_izquierda.y;

						if(reconocida->caracteres[caracter_segmentado_inicial].esquina_inferior_derecha.y>reconocida->caracteres[caracter_segmentado_final-1].esquina_inferior_derecha.y)
							caracter_fusionado.esquina_inferior_derecha.y = reconocida->caracteres[caracter_segmentado_inicial].esquina_inferior_derecha.y;

						/*NumeroHuecos(&caracter_fusionado);
						NumeroCruces(&caracter_fusionado);
						PuntosTerminales(&caracter_fusionado);
						CalcularCaracteristicas(&caracter_fusionado);*/

						caracter_fusionado.reconocido = new char[caracter_corregida_final-caracter_corregida_inicial + 1];
						strncpy(caracter_fusionado.reconocido, corregida+caracter_corregida_inicial, caracter_corregida_final-caracter_corregida_inicial);

						caracter_fusionado.reconocido[caracter_corregida_final-caracter_corregida_inicial] ='\0';
						/*if(bbdd->idCaracter(caracter_fusionado.reconocido)<0)
							bbdd->InsertarCaracter(caracter_fusionado.reconocido);
			
						bbdd->InsertarInstancia(perfil, caracter_fusionado.reconocido, caracter_fusionado.numeroHuecos, caracter_fusionado.numeroCruces, caracter_fusionado.numeroPuntosTerminales, caracter_fusionado.caracteristicasDireccionalesB, caracter_fusionado.caracteristicasDireccionalesR, caracter_fusionado.caracteristicasEstructurales, caracter_fusionado.caracteristicasConcavidad);
						*/

						AprendeCaracter(caracter_fusionado);

						caracter_corregida_inicial = caracter_corregida_final;
						caracter_segmentado_inicial = caracter_segmentado_final;
						caracter_string_inicial = 0;
					}					
				}

				if(caracter_segmentado_final==reconocida->numeroCaracteres)
				{
					Caracter caracter_fusionado;

					caracter_fusionado.esquina_superior_izquierda = reconocida->caracteres[caracter_segmentado_inicial].esquina_superior_izquierda;
					caracter_fusionado.esquina_inferior_derecha = reconocida->caracteres[caracter_segmentado_final-1].esquina_inferior_derecha;

					if(reconocida->caracteres[caracter_segmentado_final-1].esquina_superior_izquierda.y<caracter_fusionado.esquina_superior_izquierda.y)
							caracter_fusionado.esquina_superior_izquierda.y = reconocida->caracteres[caracter_segmentado_final-1].esquina_superior_izquierda.y;

					if(reconocida->caracteres[caracter_segmentado_inicial].esquina_inferior_derecha.y>reconocida->caracteres[caracter_segmentado_final-1].esquina_inferior_derecha.y)
						caracter_fusionado.esquina_inferior_derecha.y = reconocida->caracteres[caracter_segmentado_inicial].esquina_inferior_derecha.y;
					/*NumeroHuecos(&caracter_fusionado);
					NumeroCruces(&caracter_fusionado);
					PuntosTerminales(&caracter_fusionado);
					CalcularCaracteristicas(&caracter_fusionado);*/

					caracter_fusionado.reconocido = new char[strlen(corregida)-caracter_corregida_inicial + 1];
					strncpy(caracter_fusionado.reconocido, corregida+caracter_corregida_inicial, strlen(corregida)-caracter_corregida_inicial);

					caracter_fusionado.reconocido[strlen(corregida)-caracter_corregida_inicial] ='\0';
					/*if(bbdd->idCaracter(caracter_fusionado.reconocido)<0)
						bbdd->InsertarCaracter(caracter_fusionado.reconocido);
		
					bbdd->InsertarInstancia(perfil, caracter_fusionado.reconocido, caracter_fusionado.numeroHuecos, caracter_fusionado.numeroCruces, caracter_fusionado.numeroPuntosTerminales, caracter_fusionado.caracteristicasDireccionalesB, caracter_fusionado.caracteristicasDireccionalesR, caracter_fusionado.caracteristicasEstructurales, caracter_fusionado.caracteristicasConcavidad);
					*/

					AprendeCaracter(caracter_fusionado);

					caracter_corregida_inicial = caracter_corregida_final;
					caracter_segmentado_inicial = caracter_segmentado_final;
					caracter_string_inicial = 0;
				}
			}
		}

		return 0;
	}

	int Texto::CorregirTexto()
	{
		//Comprobamos que la imagen est� segmentada en caracteres
		if(lineas==NULL ||lineas[0].palabras==NULL || lineas[0].palabras[0].caracteres==NULL)
			return 1;

		//Comprobamos
		if(strlen(textoReconocido)<1)
			return 2;

		bbdd->AbrirBBDD(rutaBBDD);
		
		int k=0;

		for(int l=0; l<numeroLineas; l++)
		{
			for(int p=0; p<lineas[l].numeroPalabras; p++)
			{
				int inicio_palabra = k;

				//Buscamos un fin de palabra
				for(; textoReconocido[k]!=' ' && textoReconocido[k]!='\n' && textoReconocido[k]!='\0'; k++);

				//Comprobaci�n de textoReconocido 'correcto' (falta al menos una palabra corregida)
				if(inicio_palabra == k)
					return 3;
				
				//Si no es la �ltima palabra
				if(p!=lineas[l].numeroPalabras - 1)
				{
					//Comprobaci�n de textoReconocido 'correcto' (que no haya m�s palabras en una linea reconocidas que corregidas)
					if(textoReconocido[k]!=' ')
						return 3;

					k++;
				}
			}

			//Si no es la �ltima l�nea
			if(l!=numeroLineas - 1)
			{
				//Saltamos los posibles espacios del final
				while(textoReconocido[k]==' ')
					k++;

				//Comprobaci�n de textoReconocido 'correcto' (que no haya m�s l�neas reconocidas que corregidas)
				if(textoReconocido[k]!='\n')
					return 3;

				k++;
			}
		}

		//Saltamos los posibles espacios del final
		while(textoReconocido[k]==' ')
			k++;

		//Saltamos los posibles espacios del final
		while(textoReconocido[k]=='\n')
			k++;

		//Comprobaci�n de textoReconocido 'correcto' (que no haya m�s l�neas corregidas que reconocidas)
		if(textoReconocido[k]!='\0' && textoReconocido[k]!=10)
			return 3;

		//Hacemos la correcci�n
		for(int l=0, k=0; l<numeroLineas; l++)
		{
			for(int p=0; p<lineas[l].numeroPalabras; p++)
			{
				int inicio, fin;
				inicio=k;

				for(; textoReconocido[k]!=' ' && textoReconocido[k]!='\0' && textoReconocido[k]!='\n'; k++);

				fin=k-1;

				String corregida = new char[fin -inicio +2];
				strncpy(corregida, textoReconocido+inicio, fin-inicio+1);
				corregida[k-inicio]='\0';

				CorregirPalabra(&lineas[l].palabras[p], corregida);

				k++;
			}

			for(; textoReconocido[k]!='\n'; k++);

			k++;
		}

		bbdd->CerrarBBDD();

		return 0;
	}


	//======================================================================
	// NOMBRE: posicion
	//
	// DESCRIPCI�N: Calcula una posici�n dentro del mapa de bits.
	//
	// ARGUMENTOS:	int fila			-> Componente fila de la posici�n
	//				int columna			-> Componente columna de la posici�n
	//				int tamano_elemento -> Tama�o en bytes de un p�xel
	//				
	// SALIDA: Un valor entero que indica la posici�n del mapa de bits pedida
	//======================================================================
	int Texto::posicion(int fila, int columna, int tamano_elemento)
	{
		//Calculamos el tama�o de una fila
		int tamano_fila = bitmapinfoheader->biWidth * tamano_elemento;

		//Calculamos las columnas de relleno necesarias
		int columnas_relleno = 0;
		if(tamano_fila%4!=0)
			columnas_relleno = 4 - (tamano_fila % 4);

		//Devolvemos la posici�n del p�xel en el array de bytes
		return (((bitmapinfoheader->biHeight - 1) - fila) * (tamano_fila + columnas_relleno)) + columna * tamano_elemento;
	}


	//======================================================================
	// NOMBRE: RLSA
	//
	// DESCRIPCI�N: Aplica el algoritmo RLSA (suavizado por longitud de p�xeles horizontales) a la imagen con el umbral pasado por par�metro.
	//
	// ARGUMENTOS:	int umbral -> N�mero m�ximo de p�xeles de fondo entre dos p�xeles imagen para que el tramo se convierta todo en p�xeles imagen
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//======================================================================
	int Texto::RLSA(int umbral)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Variables necesarias
		BYTE *inicio_tramo;//Puntero al comienzo de tramo (direcci�n del p�xel de imagen anterior a unposible tramo de fondo)
		int pixeles_tramo;//N�mero de p�xeles del tramo de fondo
		bool tramo;//Indica si hemos encontrado un tramo

		//Recorremos la imagen
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
		{
			//Inicio de fila, restauramos las variables
			tramo = false;
			pixeles_tramo=0;

			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				if(mapa_bits[posicion(i, j, 1)]==0 && !tramo)
				{
					//Hemos encontrado el un posible inicio de tramo (sino pixeles_tramo ser� 0, con lo que no hay problema)
					pixeles_tramo = 0;
					tramo = true;
					inicio_tramo = &mapa_bits[posicion(i, j, 1)];
				}
				else
				{
					if(tramo)
					{
						//Estamos en un tramo
						if(mapa_bits[posicion(i,j,1)]==0)
						{
							//Fin de tramo
							//Si el n�mero de p�xeles del tramo es menor o igual que el umbral convertimos el tramo d efondo en p�xeles imagen
							//Observar que en el caso de que realmente no hubiera un tramo de fondo, pixeles_tramo ser� 0, con lo que no cambiamos el valor de ning�n p�xel
							if(pixeles_tramo<=umbral)
								memset(inicio_tramo + 1, 0, pixeles_tramo);

							//Adem�s de ser el fin de un tramo podemos estar ante el comienzo de otro
							tramo = true;
							pixeles_tramo = 0;
							inicio_tramo = &mapa_bits[posicion(i, j, 1)];
						}
						else
						{
							//Estamos en un tramo de fondo
							pixeles_tramo++;
						}
					}
				}
			}
		}

		return 0;
	}

	
	//PROVISIONAL, NO CREO QUE LO USE AL FINAL (SIA AL FINAL LO USO COMENTARLO BIEN)
	//======================================================================
	// NOMBRE: RLSAVertical
	//
	// DESCRIPCI�N: Aplica el algoritmo RLSA (suavizado por longitud de p�xeles VERTICALES) a la imagen con el umbral pasado por par�metro.
	//
	// ARGUMENTOS:	int umbral -> N�mero m�ximo de p�xeles de fondo entre dos p�xeles imagen para que el tramo se convierta todo en p�xeles imagen
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//======================================================================
	int Texto::RLSAVertical(Linea palabra, int umbral)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		int inicio_tramo;
		int pixeles_tramo;
		bool tramo=false;

		for(int j=palabra.esquina_superior_izquierda.x;j<=palabra.esquina_inferior_derecha.x;j++)
			for(int i=palabra.esquina_superior_izquierda.y;i<=palabra.esquina_inferior_derecha.y;i++)			
			{
				if(mapa_bits[posicion(i,j,1)]==0 && !tramo)
				{
					pixeles_tramo=0;
					tramo=true;
					inicio_tramo=i;
				}
				else
				{
					if(tramo)
					{
						if(mapa_bits[posicion(i,j,1)]==0)//Fin de tramo
						{
							if(pixeles_tramo<=umbral)
							{
								for(int fila=inicio_tramo+1; fila<=inicio_tramo+pixeles_tramo; fila++)
									mapa_bits[posicion(fila, j, 1)]=0;
							}

							tramo=true;
							pixeles_tramo=0;
							inicio_tramo=i;
						}
						else
						{
							pixeles_tramo++;
						}
					}
				}
			}

		return 0;
	}


	//======================================================================
	// NOMBRE: proyeccionHorizontal
	//
	// DESCRIPCI�N: Obtiene la proyecci�n horizontal de la imagen guard�ndola en el par�metro proyeccion.
	//
	// ARGUMENTOS:	int *proyeccion -> Vector donde guardaremos la proyecci�n de la imagen. Debe reservarse la memoria con anterioridad o dar� error.
	//								   Esto se ha hecho as� para que sea m�s f�cil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//								   aqu� probablemente olvidariamos liberar la memoria con frecuencia.
	//								   El tama�o del vector debe ser el n�mero de filas de la imagen.
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//======================================================================
	int Texto::proyeccionHorizontal(int *proyeccion)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		for(int i=0; i<bitmapinfoheader->biHeight; i++)
		{
			//Inicializamos la fila a 0
			proyeccion[i] = 0;
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				if(!mapa_bits[posicion(i, j, 1)])
				{	
					//Hemos encontrado un p�xel negro en la fila i
					proyeccion[i]++; 					
				}
			}
		} 		
		
		return 0;
	}


	//======================================================================
	// NOMBRE: proyeccionHorizontal
	//
	// DESCRIPCI�N: Obtiene la proyecci�n horizontal de la imagen guard�ndola en el par�metro proyeccion, as� como un valor proporcional a la desviacion que guardaremos en el par�metro pseudodesviacion.
	//
	// ARGUMENTOS:	int *proyeccion			 -> Vector donde guardaremos la proyecci�n de la imagen. Debe reservarse la memoria con anterioridad o dar� error.
	//											Esto se ha hecho as� para que sea m�s f�cil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//											aqu� probablemente olvidariamos liberar la memoria con frecuencia.
	//											El tama�o del vector debe ser el n�mero de filas de la imagen.
	//				double *pseudodesviacion -> En este par�metro por referencia devolvemos un valor proporcional a la desviaci�n de la proyecci�n
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//======================================================================
	int Texto::proyeccionHorizontal(int *proyeccion, double *pseudodesviacion)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Inicializamos a 0 la media y la pseudodesviacion
		double media = 0;
		(*pseudodesviacion) = 0;

		for(int i=0; i<bitmapinfoheader->biHeight; i++)
		{
			//Inicializamos la fila a 0
			proyeccion[i] = 0;
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				if(!mapa_bits[posicion(i, j, 1)])
				{
					//Hemos encontrado un p�xel negro en la fila i
					media++;
					
					proyeccion[i]++; 					
				}
			}
		}

		//Calculamos la media
		media /= bitmapinfoheader->biHeight;

		//Calculamos el valor proporcional a la desviaci�n para ahorrarnos los cuadrados y las ra�ces
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			(*pseudodesviacion) += fabs((double)proyeccion[i] - media);

		//'Normalizamos' la pseudodesviacion con el n�mero de filas de la imagen
		(*pseudodesviacion) /= bitmapinfoheader->biHeight;

		/*ofstream archivo;
		
		archivo.open(".\\proyecciones", ios::app); 
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
		{
			String s = " --> ";

			archivo << i << s << proyeccion[i] << "\n"; // ... must add it back
			
		}

		archivo << "\n";

		archivo.close();*/


		return 0;
	}

	
	//======================================================================
	// NOMBRE: proyeccionHorizontal
	//
	// DESCRIPCI�N: Obtiene la proyecci�n horizontal de una palabra de la imagen guard�ndola en el par�metro proyeccion, as� como el m�ximo de la misma que guardaremos en el par�metro maximo.
	// 
	// ARGUMENTOS: Palabra segmento -> Palabra de la que queremos calcular la proyecci�n
	//			   int *proyeccion	-> Vector donde guardaremos la proyecci�n de la imagen. Debe reservarse la memoria con anterioridad o dar� error.
	//								   Esto se ha hecho as� para que sea m�s f�cil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//								   aqu� probablemente olvidariamos liberar la memoria con frecuencia.
	//								   El tama�o del vector debe ser el n�mero de filas de la imagen (aunque gastemos m�s memoria tontamente es por ahorrarnos los c�lculos de los �ndices).
	//				double *maximo  -> En este par�metro por referencia devolvemos el valor m�ximo de la proyecci�n
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. La palabra no pertenece a la imagen actual
	//============================================================================
	int Texto::proyeccionHorizontal(Palabra segmento, int *proyeccion, double *maximo)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que la palabra pertenece a la imagen
		if(segmento.esquina_superior_izquierda.x<0 || segmento.esquina_superior_izquierda.y<0 || segmento.esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || segmento.esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos a 0 la media y el m�ximo
		double media = 0;
		(*maximo) = 0;

		for(int i=segmento.esquina_superior_izquierda.y; i<=segmento.esquina_inferior_derecha.y; i++)
		{
			//Inicializamos la fila a 0
			proyeccion[i] = 0;
			for(int j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++)
			{
				if(!mapa_bits[posicion(i, j, 1)])
				{	
					//Hemos encontrado un p�xel negro en la fila i
					proyeccion[i]++; 					
				}
			}

			//Actualizamos el valor m�ximo de la proyecci�n si es necesario
			if(proyeccion[i]>(*maximo))
				(*maximo)=proyeccion[i];
		}

		return 0;
	}


	//======================================================================
	// NOMBRE: proyeccionVertical
	//
	// DESCRIPCI�N: Obtiene la proyecci�n vertical de una palabra de la imagen guard�ndola en el par�metro proyeccion.
	// 
	// ARGUMENTOS: Palabra palabra -> Palabra de la que queremos calcular la proyecci�n
	//			   int *proyeccion -> Vector donde guardaremos la proyecci�n de la imagen. Debe reservarse la memoria con anterioridad o dar� error.
	//								  Esto se ha hecho as� para que sea m�s f�cil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//								  aqu� probablemente olvidariamos liberar la memoria con frecuencia.
	//								  El tama�o del vector debe ser el n�mero de columnas de la imagen (aunque gastemos m�s memoria tontamente es por ahorrarnos los c�lculos de los �ndices).
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. La palabra no pertenece a la imagen actual
	//============================================================================
	int Texto::proyeccionVertical(Palabra palabra, int *proyeccion)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que la palabra pertenece a la imagen
		if(palabra.esquina_superior_izquierda.x<0 || palabra.esquina_superior_izquierda.y<0 || palabra.esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || palabra.esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		for(int j=palabra.esquina_superior_izquierda.x; j<=palabra.esquina_inferior_derecha.x; j++)		
		{
			//Inicializamos la columna a 0
			proyeccion[j] = 0;
			for(int i=palabra.esquina_superior_izquierda.y; i<=palabra.esquina_inferior_derecha.y; i++)
			{
				if(!mapa_bits[posicion(i, j, 1)])
				{
					//Hemos encontrado un p�xel negro en la columna j
					proyeccion[j]++; 					
				}
			}				
		}

		/*ofstream archivo;
		
		archivo.open(".\\proyecciones", ios::app); 
		for(int j=palabra.esquina_superior_izquierda.x; j<=palabra.esquina_inferior_derecha.x; j++)
		{
			String s = " --> ";

			archivo << j << s << proyeccion[j] << "\n"; // ... must add it back
			
		}

		archivo << "\n";

		archivo.close();*/


		return 0;
	}

	
	//======================================================================
	// NOMBRE: proyeccionVertical
	//
	// DESCRIPCI�N: Obtiene la proyecci�n vertical de una palabra de la imagen guard�ndola en el par�metro proyeccion, as� como un valor proporcional a la desviacion que guardaremos en el par�metro pseudodesviacion.
	//
	// ARGUMENTOS: Palabra palabra			-> Palabra de la que queremos calcular la proyecci�n
	//			   int *proyeccion			-> Vector donde guardaremos la proyecci�n de la imagen. Debe reservarse la memoria con anterioridad o dar� error.
	//										   Esto se ha hecho as� para que sea m�s f�cil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//										   aqu� probablemente olvidariamos liberar la memoria con frecuencia.
	//										   El tama�o del vector debe ser el n�mero de columnas de la imagen (aunque gastemos m�s memoria tontamente es por ahorrarnos los c�lculos de los �ndices).
	//			   double *pseudodesviacion -> En este par�metro por referencia devolvemos un valor proporcional a la desviaci�n de la proyecci�n
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. La palabra no pertenece a la imagen actual
	//============================================================================
	int Texto::proyeccionVertical(Palabra segmento, int *proyeccion, double *pseudodesviacion)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que la palabra pertenece a la imagen
		if(segmento.esquina_superior_izquierda.x<0 || segmento.esquina_superior_izquierda.y<0 || segmento.esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || segmento.esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos a 0 la media y la pseudodesviacion
		double media = 0;
		(*pseudodesviacion) = 0;
		
		for(int j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++)		
		{
			//Inicializamos la columna a 0
			proyeccion[j] = 0;
			for(int i=segmento.esquina_superior_izquierda.y; i<=segmento.esquina_inferior_derecha.y; i++)
			{
				if(!mapa_bits[posicion(i, j, 1)])
				{		
					//Hemos encontrado un p�xel negro en la columna j
					media++; 

					proyeccion[j]++; 					
				}
			}			
		}

		//Calculamos la media
		media /= segmento.esquina_inferior_derecha.x-segmento.esquina_superior_izquierda.x+1;

		//Calculamos el valor proporcional a la desviaci�n para ahorrarnos los cuadrados y las ra�ces
		for(int j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++)
			(*pseudodesviacion) += fabs((double)proyeccion[j] - media);

		//'Normalizamos' la pseudodesviacion con el n�mero de columnas de la palabra
		(*pseudodesviacion) /= segmento.esquina_inferior_derecha.x - segmento.esquina_superior_izquierda.x + 1;

		return 0;
	}

	
	int Texto::proyeccionVertical(Palabra segmento, int *proyeccion, int *maximo)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que la palabra pertenece a la imagen
		if(segmento.esquina_superior_izquierda.x<0 || segmento.esquina_superior_izquierda.y<0 || segmento.esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || segmento.esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos a 0 la media y la pseudodesviacion
		(*maximo) = 0;
		
		for(int j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++)		
		{
			//Inicializamos la columna a 0
			proyeccion[j] = 0;
			for(int i=segmento.esquina_superior_izquierda.y; i<=segmento.esquina_inferior_derecha.y; i++)
			{
				if(!mapa_bits[posicion(i, j, 1)])
				{		
					//Hemos encontrado un p�xel negro en la columna j
					proyeccion[j]++; 					
				}
			}	

			if(proyeccion[j]>(*maximo))
				(*maximo) = proyeccion[j];
		}		

		return 0;
	}
	
	
	//======================================================================
	// NOMBRE: rellenarHuecos
	//
	// DESCRIPCI�N: Expande la semilla hasta rellenar el hueco al que �sta pertenece.
	// 
	// ARGUMENTOS:	Coordenada semilla -> P�xel de la imagen a partir del cual taparemos el hueco
	//				 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. La semilla no pertenece a la imagen actual
	//============================================================================
	int Texto::rellenarHuecos(Coordenada semilla)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que la semilla pertenece a la imagen
		if(semilla.x<0 || semilla.y<0 || semilla.x>=bitmapinfoheader->biWidth || semilla.y>=bitmapinfoheader->biHeight)
			return 2;
		
		//Creamos una pila en donde iremos guardando los vecinos de la semilla que est�n a blanco (!=0)
		//Una vez apilado, el vecino se convierte en una nueva 'semilla'
		Pila *pila_puntos = new Pila();

		//Colocamos la semilla inicial en la pila
		pila_puntos->Apilar(semilla);
		
		//Mientras tengamos 'semillas'
		while(!pila_puntos->EsVacia())
		{
            //Procesamos una 'semilla'
			Coordenada semilla_actual;
			pila_puntos->Desapilar(&semilla_actual);

			//'Pintamos' la 'semilla' de negro
			mapa_bits[posicion(semilla_actual.y, semilla_actual.x, 1)] = 0;

			//A�adimos en la pila aquellos 8-vecinos de la semilla que sean blancos (!=0)
			//Vecino izquierdo
            if(semilla_actual.x>0)
			{
				if(mapa_bits[posicion(semilla_actual.y, semilla_actual.x - 1, 1)]!=0)
				{
					Coordenada v;
					v.y = semilla_actual.y;
					v.x = semilla_actual.x - 1;
					
					pila_puntos->Apilar(v);
				}               
			}
			
			//Vecino derecho
			if(semilla_actual.x<bitmapinfoheader->biWidth - 1)
			{
				if(mapa_bits[posicion(semilla_actual.y, semilla_actual.x + 1, 1)]!=0)
				{
					Coordenada v;
					v.y = semilla_actual.y;
					v.x = semilla_actual.x + 1;
					
					pila_puntos->Apilar(v);
				}                
			}

			//Vecino superior
			if(semilla_actual.y>0)
			{
				if(mapa_bits[posicion(semilla_actual.y - 1, semilla_actual.x, 1)]!=0)
				{
					Coordenada v;
					v.y = semilla_actual.y - 1;
					v.x = semilla_actual.x;
					
					pila_puntos->Apilar(v);
				}                
			}

            //Vecino inferior
			if(semilla_actual.y<bitmapinfoheader->biHeight - 1)
			{
				if(mapa_bits[posicion(semilla_actual.y + 1, semilla_actual.x, 1)]!=0)
				{
					Coordenada v;
					v.y = semilla_actual.y + 1;
					v.x = semilla_actual.x;
					
					pila_puntos->Apilar(v);
				}                
			}

			//Vecino diagonal izquierda arriba
			if(semilla_actual.x>0 && semilla_actual.y>0)
			{
				if(mapa_bits[posicion(semilla_actual.y - 1, semilla_actual.x - 1, 1)]!=0)
				{
					Coordenada v;
					v.y = semilla_actual.y - 1;
					v.x = semilla_actual.x - 1;
					
					pila_puntos->Apilar(v);
				}                
			}

			//Vecino diagonal derecha arriba
			if(semilla_actual.x<bitmapinfoheader->biWidth - 1 && semilla_actual.y>0)
			{
				if(mapa_bits[posicion(semilla_actual.y - 1, semilla_actual.x + 1, 1)]!=0)
				{
					Coordenada v;
					v.y = semilla_actual.y - 1;
					v.x = semilla_actual.x + 1;
					
					pila_puntos->Apilar(v);
				}                
			}

			//Vecino diagonal derecha abajo
			if(semilla_actual.x>0 && semilla_actual.y<bitmapinfoheader->biHeight - 1)
			{
				if(mapa_bits[posicion(semilla_actual.y + 1, semilla_actual.x - 1, 1)]!=0)
				{
					Coordenada v;
					v.y = semilla_actual.y + 1;
					v.x = semilla_actual.x - 1;
					
					pila_puntos->Apilar(v);
				}                
			}

			//Vecino diagonal izquierda abajo
			if(semilla_actual.x<bitmapinfoheader->biWidth - 1 && semilla_actual.y<bitmapinfoheader->biHeight - 1)
			{
				if(mapa_bits[posicion(semilla_actual.y + 1, semilla_actual.x + 1, 1)]!=0)
				{
					Coordenada v;
					v.y = semilla_actual.y + 1;
					v.x = semilla_actual.x + 1;
					
					pila_puntos->Apilar(v);
				}                
			}
		}

		//Borramos la pila de 'semillas'
		delete pila_puntos;

		return 0;
	}


	//======================================================================
	// NOMBRE: conectividad
	//
	// DESCRIPCI�N: Devuelve el n�mero de conexiones que tiene el p�xel actual.
	// 
	// ARGUMENTOS: int fila    -> Coordenada y del p�xel a comprobar
	//			   int columna -> Coordenada x del p�xel a comprobar
	//				 
	// SALIDA: Un valor entero que indica:
	//			>=0 -> N�mero de conexiones del p�xel actual. Operaci�n realizada satisfactoriamente
	//			-1 -> Error. La imagen no est� umbralizada	
	//			-2 -> Error. El p�xel a comprobar no pertenece a la imagen
	//============================================================================
	int Texto::conectividad(int fila, int columna)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return -1;

		//Comprobamos que no nos salimos de la imagen
		if(fila<1 || columna<1 || fila>=bitmapinfoheader->biHeight - 1 || columna>=bitmapinfoheader->biWidth - 1)
			return -2;

        //Variables necesarias
		bool ultimo;//true->�ltimo p�xel era negro	false->�ltimo p�xel era blanco
		int cambios_color = 0;//Contador de los cambios de color (ser�n el doble que las conexiones)
		
		/*Ahora tenemos que contar cuantos cambios de color se producen alrededor del pixel central girando en sentido horario
		*															8	1	2
		*															7	0	3
		*															6	5	4
		*/
        //Pixel inicial (1)
		ultimo = mapa_bits[posicion(fila-1,columna,1)]==0;
       
        //Pasamos de 1 a 2
        if( (mapa_bits[posicion(fila-1,columna+1,1)]==0) != ultimo)
		{
			ultimo = !ultimo;
			cambios_color++;
		}

		//Pasamos de 2 a 3
        if( (mapa_bits[posicion(fila,columna+1,1)]==0) != ultimo)
		{
			ultimo = !ultimo;
			cambios_color++;
		}

		//Pasamos de 3 a 4
        if( (mapa_bits[posicion(fila+1,columna+1,1)]==0) != ultimo)
		{
			ultimo = !ultimo;
			cambios_color++;
		}

		//Pasamos de 4 a 5
        if( (mapa_bits[posicion(fila+1,columna,1)]==0) != ultimo)
		{
			ultimo = !ultimo;
			cambios_color++;
		}

		//Pasamos de 5 a 6
        if( (mapa_bits[posicion(fila+1,columna-1,1)]==0) != ultimo)
		{
			ultimo = !ultimo;
			cambios_color++;
		}

		//Pasamos de 6 a 7
        if( (mapa_bits[posicion(fila,columna-1,1)]==0) != ultimo)
		{
			ultimo = !ultimo;
			cambios_color++;
		}

		//Pasamos de 7 a 8
        if( (mapa_bits[posicion(fila-1,columna-1,1)]==0) != ultimo)
		{
			ultimo = !ultimo;
			cambios_color++;
		}
      
        //Pasamos de 8 a 1
        if( (mapa_bits[posicion(fila-1,columna,1)]==0) != ultimo)
		{
			ultimo = !ultimo;
			cambios_color++;
		}
		
		//Obtenemos el n�mero de conexiones
		cambios_color /= 2;

		return cambios_color;
	}

	
	//======================================================================
	// NOMBRE: esPuntoFinal
	//
	// DESCRIPCI�N: Comprueba si un determinado p�xel es un punto final.
	// 
	// ARGUMENTOS: int fila    -> Coordenada y del p�xel a comprobar
	//			   int columna -> Coordenada x del p�xel a comprobar
	//				 
	// SALIDA: Un valor booleano que indica:
	//			true -> El p�xel es un punto final. Operaci�n realizada satisfactoriamente
	//			false -> El p�xel no es un punto final o Error. La imagen no est� umbralizada o el p�xel a comprobar no pertenece a la imagen
	//============================================================================
	bool Texto::esPuntoFinal(int fila, int columna)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return false;

		//Comprobamos que no nos salimos de la imagen
		if(fila<1 || columna<1 || fila>=bitmapinfoheader->biHeight - 1 || columna>=bitmapinfoheader->biWidth - 1)
			return false;

		//Para ser punto final lo primero es que estemos ante un p�xel negro
		if(mapa_bits[posicion(fila, columna, 1)]!=0)
			return false;

		//Variable necesaria
		int vecinos = 0;//Contador de los vecinos del p�xel actual (consideraremos 8-conectividad)
		
		//Contamos los vecinos que tenga el p�xel actual
		//Vecino superior izquierdo
        if(mapa_bits[posicion(fila - 1, columna - 1, 1)]==0)
			vecinos++;

		//Vecino superior
		if(mapa_bits[posicion(fila - 1, columna, 1)]==0)
			vecinos++;

		//Vecino superior derecho
		if(mapa_bits[posicion(fila - 1, columna + 1, 1)]==0)
			vecinos++;

		//Vecino izquierdo
		if(mapa_bits[posicion(fila, columna - 1, 1)]==0)
			vecinos++;		

		//Vecino derecho
		if(mapa_bits[posicion(fila, columna + 1, 1)]==0)
			vecinos++;

		//Vecino inferior izquierdo
		if(mapa_bits[posicion(fila + 1, columna - 1, 1)]==0)
			vecinos++;

		//Vecino inferior
		if(mapa_bits[posicion(fila + 1, columna, 1)]==0)
			vecinos++;

		//Vecino inferior derecho
		if(mapa_bits[posicion(fila + 1, columna + 1, 1)]==0)
			vecinos++;

		//Estaremos ante un punto final si s�lo tiene un 8-vecino      
		return (vecinos==1);
	}


	//REVISAR LO DE SI OBLIGO A UMBRALIZADA O NO
	//======================================================================
	// NOMBRE: Sobel
	//
	// DESCRIPCI�N: Aplica Sobel a la porci�n de imagen del car�cter con el objetivo de calcular los gradientes de la misma. (Realmente solo guardaremos el rango de la direcci�n del gradiente)
	//				Estos valores se almacenar�n en el par�metro por referencia gradiente. La imagen no se ve modificada.
	//				Debe reservarse la memoria con anterioridad o dar� error. Esto se ha hecho as� para que sea m�s f�cil ver que luego hay 
	//				que borrarlo para ahorrar memoria, si reservaramos la memoria aqu� probablemente olvidariamos liberar la memoria con frecuencia.
	//				Las dimensiones de la matriz deben ser iguales que el tama�o del car�cter.
	// 
	// ARGUMENTOS: Caracter *caracter -> Puntero al car�cter del que queremos calcular el gradiente
	//			   BYTE **gradiente   -> Puntero a la matriz en la que guardaremos el gradiente
	//				 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente
	//			1 -> Error. La imagen no est� umbralizada	
	//			2 -> Error. El car�cter no pertenece a la imagen actual
	//============================================================================
	int Texto::Sobel(Caracter *caracter, BYTE **gradiente)
	{
		//Comprobamos que la imagen est� umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el car�cter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos las m�scaras de Sobel
		int mascara_horizontal[9] = {1,2,1,0,0,0,-1,-2,-1};
		int mascara_vertical[9] = {-1,0,1,-2,0,2,-1,0,1};

		int profundidad = bitmapinfoheader->biBitCount;

		if(profundidad==8)
		{
			//Recorremos el car�cter
			for(int i=0; i<caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y + 1; i++)
				for(int j=0; j<caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x + 1; j++)
				{
					//Variables necesarias
					int resultado_horizontal = 0;
					int resultado_vertical = 0;
					double angulo;//Direcci�n del gradiente
					bool fin = false;//Si una cordenada no existe nos saldremos

					//Recorremos la m�scara
					for(int n=0, k=i-1; k<=i+1 && !fin; k++)
						for(int t=j-1; t<=j+1 && !fin; t++, n++)
						{
							if(ExisteCoordenada(t,k))
							{
								resultado_horizontal += mapa_bits[posicion(k + caracter->esquina_superior_izquierda.y, t + caracter->esquina_superior_izquierda.x, 1)] * mascara_horizontal[n];
								resultado_vertical += mapa_bits[posicion(k + caracter->esquina_superior_izquierda.y, t + caracter->esquina_superior_izquierda.x, 1)] * mascara_vertical[n];
							}
							else
							{
								resultado_horizontal = mapa_bits[posicion(i + caracter->esquina_superior_izquierda.y, j, 1)];
								resultado_vertical = resultado_horizontal;
								fin = true;
							}
						}								
					
					//Obtenemos la direcci�n del gradiente
					if(resultado_horizontal==0)
					{
						//Casos especiales (para evitar divisi�n por 0)
						if(resultado_vertical<0)
							angulo = 270;
						else
							angulo = 90;
					}
					else
					{
						//Caso general
						angulo = atan(resultado_vertical / (double)resultado_horizontal);

						// Pasamos el �ngulo de radianes a grados
						angulo = (angulo * 180) / PI;

						if(resultado_horizontal<0)
							angulo = angulo + 180;
						else if(resultado_vertical<0)
							angulo = angulo + 360;
					}

					//Guardamos el rango de la direcci�n (�ngulo)
					//Tendremos 12 rangos de 30� cada uno
					gradiente[i][j] = (((int)(angulo)) - 1) / 30;				
				}
		}			
		
		return 0;
	}	

	
	//======================================================================
	// NOMBRE: ajustarCaracter
	//
	// DESCRIPCI�N: Elimina del caracter todas la sfilas y columnas de p�xeles blancos, obteniendo el menor rect�ngulo que incluya al car�cter.
	//			
	// ARGUMENTOS: Caracter *caracter -> Puntero al car�cter que queremos ajustar
	//							 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente	
	//			1 -> Error. El car�cter no pertenece a la imagen actual
	//============================================================================
	int Texto::ajustarCaracter(Caracter *caracter)
	{
		//Comprobamos que el car�cter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 1;

		//Guardamos las coordenadas originales
		Coordenada original_izquierda = caracter->esquina_superior_izquierda;
		Coordenada original_derecha = caracter->esquina_inferior_derecha;

		//Quitamos las filas blancas 
		//Filas superiores
		for(int i=caracter->esquina_superior_izquierda.y; i<=caracter->esquina_inferior_derecha.y; i++)
		{
			int j;

			for(j=caracter->esquina_superior_izquierda.x; j<=caracter->esquina_inferior_derecha.x && mapa_bits[posicion(i, j, 1)]==255; j++);

			if(j==caracter->esquina_inferior_derecha.x + 1)//La fila no tiene ning�n p�xel negro
				caracter->esquina_superior_izquierda.y++;
			else
				break;
		}

		if(caracter->esquina_superior_izquierda.y!=original_izquierda.y)//Para dar un p�xel m�s, especialmente �til en caracteres muy peque�os
			caracter->esquina_superior_izquierda.y--;

		//Filas inferiores
		for(int i=caracter->esquina_inferior_derecha.y; i>=caracter->esquina_superior_izquierda.y; i--)
		{
			int j;

			for(j=caracter->esquina_superior_izquierda.x; j<=caracter->esquina_inferior_derecha.x && mapa_bits[posicion(i,j,1)]==255; j++);

			if(j==caracter->esquina_inferior_derecha.x + 1)//La fila no tiene ning�n p�xel negro
				caracter->esquina_inferior_derecha.y--;
			else
				break;
		}	

		if(caracter->esquina_inferior_derecha.y!=original_derecha.y)//Para dar un p�xel m�s, especialmente �til en caracteres muy peque�os
			caracter->esquina_inferior_derecha.y++;

		//Quitamos las columnas blancas 
		//Columnas izquierda
		for(int j=caracter->esquina_superior_izquierda.x; j<=caracter->esquina_inferior_derecha.x; j++)
		{
			int i;

			for(i=caracter->esquina_superior_izquierda.y; i<=caracter->esquina_inferior_derecha.y && mapa_bits[posicion(i,j,1)]==255; i++);

			if(i==caracter->esquina_inferior_derecha.y + 1)//La columna no tiene ning�n p�xel negro
				caracter->esquina_superior_izquierda.x++;
			else
				break;
		}

		if(caracter->esquina_superior_izquierda.x!=original_izquierda.x)//Para dar un p�xel m�s, especialmente �til en caracteres muy peque�os
			caracter->esquina_superior_izquierda.x--;

		//Columnas derecha
		for(int j=caracter->esquina_inferior_derecha.x; j>=caracter->esquina_superior_izquierda.x; j--)
		{
			int i;

			for(i=caracter->esquina_superior_izquierda.y; i<=caracter->esquina_inferior_derecha.y && mapa_bits[posicion(i,j,1)]==255; i++);

			if(i==caracter->esquina_inferior_derecha.y + 1)//La columna no tiene ning�n p�xel negro
				caracter->esquina_inferior_derecha.x--;
			else
				break;
		}

		if(caracter->esquina_inferior_derecha.x!=original_derecha.x)//Para dar un p�xel m�s, especialmente �til en caracteres muy peque�os
			caracter->esquina_inferior_derecha.x++;
		
		return 0;
	}


	//======================================================================
	// NOMBRE: aprendeCaracteres
	//
	// DESCRIPCI�N: Aprende los caracteres.
	//			
	// ARGUMENTOS: -
	//							 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operaci�n realizada satisfactoriamente	
	//			1 -> Error. La imagen no est� segmentada en caracteres
	//			2 -> Error. No hay texto reconocido
	//			3 -> Error. El texto reonocido no coincide con la segmentaci�n
	//============================================================================
	int Texto::aprendeCaracteres()
	{
		//Comprobamos que la imagen est� segmentada en caracteres
		if(lineas==NULL ||lineas[0].palabras==NULL || lineas[0].palabras[0].caracteres==NULL)
			return 1;

		//Comprobamos
		if(strlen(textoReconocido)<1)
			return 2;

		for(int l=0, k=0; l<numeroLineas; l++)
		{
			for(int p=0; p<lineas[l].numeroPalabras; p++)
			{
				for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++, k++)
				{
					//Comprobaci�n de textoReconocido 'correcto'
					if(textoReconocido[k]==' ' || textoReconocido[k]=='\n')
						return 3;

					lineas[l].palabras[p].caracteres[c].reconocido = new char[2];
					lineas[l].palabras[p].caracteres[c].reconocido[0] = textoReconocido[k];	
					lineas[l].palabras[p].caracteres[c].reconocido[1] = '\0';
				}

				if(p!=lineas[l].numeroPalabras - 1)
				{
					//Comprobaci�n de textoReconocido 'correcto'
					if(textoReconocido[k]!=' ')
						return 3;

					k++;
				}
			}

			if(l!=numeroLineas - 1)
			{
				//Saltamos lo sposibles espacios del final
				while(textoReconocido[k]==' ')
					k++;

				//Comprobaci�n de textoReconocido 'correcto'
				if(textoReconocido[k]!='\n')
					return 3;

				k++;
			}
		}

		return 0;
	}

	
	/*
	*/
	int Texto::redondear(double valor)
	{
		if(fabs(valor - floor(valor))>0.5)
			return (int)ceil(valor);
		
		return (int)floor(valor);
	}

}

