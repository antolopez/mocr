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
	// DESCRIPCIÓN: Constructor por defecto. 				
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

		//Elementos de segmentación a NULL
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
	// DESCRIPCIÓN: Constructor que crea una imagen no segmentada, con las dimensiones y colores especificados.				
	//
	// ARGUMENTOS:	int columnas			-> Nº de columnas de la imagen
	//				int filas				-> Nº de filas de la imagen
	//				int profundidad			-> Bits/píxel de la imagen
	//				RGBQUAD *paleta_origen	-> Puntero a la paleta que queremos copiar, si es NULL se reservará espacio para la paleta, pero no se copia
	//				int tamano_paleta		-> tamaño de la paleta que queremso copiar.
	//
	// SALIDA: -
	//============================================================================
	Texto::Texto(int columnas, int filas, int profundidad, RGBQUAD *paleta_origen, int tamano_paleta)
	{
		//Propiedades de la imagen
		nombreImagen = "";
		umbralizada = false;

		//Elementos de segmentación a NULL
		lineas = NULL;
		numeroLineas = 0;

		//Elementos de reconocimiento a NULL
		perfil = ""; 
		rutaBBDD = "";
		clasificador = NULL;
		bbdd = NULL;	
		textoReconocido = "";

		//Creamos la imagen
		//Calculamos el nº de bytes necesarios para el mapa de bits
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
		//Fin asignación punteros
		
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
	// DESCRIPCIÓN: Destructor. 				
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

		//ARREGLAR ESTO Y BORRAR LOS DEMÁS STRINGS
		//Borramos el nombre de la imagen
		//delete nombreImagen;

		//Borramos la información de segmentación
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
	// DESCRIPCIÓN: Sobrecarga operador =				
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
			
		//Copiamos la imagen en sí
		if(origen.inicio_DIB!= NULL)
		{
			//Calculamos tamaño paleta
			int tamano_paleta = 0;

			if(origen.bitmapinfoheader->biBitCount<=8)
			{
				if(origen.bitmapinfoheader->biClrUsed!=0)
					tamano_paleta = origen.bitmapinfoheader->biClrUsed;
				else
					tamano_paleta = (int)pow(2.0, origen.bitmapinfoheader->biBitCount);
			}
			//Fin cálculo tamaño paleta
						
			DWORD tamano_DIB = sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD) + origen.bitmapinfoheader->biSizeImage;

			//Borramos por si estamos reutilizando un aimagen anterior
			//Borramos el mapa de bits
			if(inicio_DIB!=NULL)
				delete inicio_DIB;

			//Borramos la información de segmentación
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

			//Asignación de punteros
			bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
			bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
			if(tamano_paleta!=0)
				paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
			else//No hay paleta
				paleta = NULL;

			mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
			//Fin de asignación de punteros
				
			memcpy(inicio_DIB, origen.inicio_DIB, tamano_DIB);//Copiamos el DIB

			//Copiamos la información de segmentación
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
	// DESCRIPCIÓN: Devuelve la dirección del DIB en memoria				
	//
	// ARGUMENTOS:	-
	//
	// SALIDA: Dirección del DIB en memoria	como BYTE*
	//============================================================================
	BYTE* Texto::GetDIB()
	{
		return inicio_DIB;
	}


	//============================================================================
	// NOMBRE: SetDIB
	//
	// DESCRIPCIÓN: Establece el DIB de una imagen				
	//
	// ARGUMENTOS:	BYTE *DIB -> Dirección del DIB a guardar
	//
	// SALIDA: -
	//============================================================================
	void Texto::SetDIB(BYTE *DIB)
	{
		//Establecemos los punteros iniciales
		inicio_DIB = DIB;
		bitmapinfo = (LPBITMAPINFO)DIB;
		bitmapinfoheader = (LPBITMAPINFOHEADER)DIB;

		//Si el tamaño de la imagen no está calculado lo hacemos nosotros
		if(bitmapinfoheader->biSizeImage==0)
		{
			int tamano_elemento = bitmapinfoheader->biBitCount / 8;
			int tamano_fila = bitmapinfoheader->biWidth * tamano_elemento;
			
			//Calculamos las columnas de relleno
			int columnas_relleno = 0;
			if(tamano_fila%4!=0)
				columnas_relleno=4-(tamano_fila%4);

			//Establecemos el tamaño de la imagen
			bitmapinfoheader->biSizeImage = bitmapinfoheader->biHeight * (tamano_fila + columnas_relleno);
		}
	
		//Calculos necesarios para conocer el tamaño de la paleta
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
	// DESCRIPCIÓN: Devuelve la dirección del mapa de bits en memoria				
	//
	// ARGUMENTOS:	-
	//
	// SALIDA: Dirección del mapa de bits en memoria como BYTE*
	//============================================================================
	BYTE* Texto::GetMapaBits()
	{
		return mapa_bits;
	}


	//============================================================================
	// NOMBRE: GetNombreImagen
	//
	// DESCRIPCIÓN: Retorna el nombre de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Una cadena de texto que indica el nombre de la imagen con extensión incluida			
	//============================================================================
	String Texto::GetNombreImagen()
	{
		return nombreImagen;
	}


	//============================================================================
	// NOMBRE: SetNombreImagen
	//
	// DESCRIPCIÓN: Modifica el nombre de la imagen.
	//
	// ARGUMENTOS: String nombre -> Nuevo nombre para la imagen (con extensión incluida)
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
	// DESCRIPCIÓN: Retorna el asociado al texto de la imagen.
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
	// DESCRIPCIÓN: Modifica el perfil asociado al texto de la imagen.
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
	// DESCRIPCIÓN: Retorna la ruta de la bbdd de los patrones.
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
	// DESCRIPCIÓN: Modifica la ruta de la bbdd de los patrones.
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
	// DESCRIPCIÓN: Retorna el texto que ha sido reconocido o aprendido.
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
	// DESCRIPCIÓN: Modifica el texto reconocido o aprendido.
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
	// DESCRIPCIÓN: Retorna el número de filas de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor entero que indica el número de filas			
	//============================================================================
	int Texto::GetNumeroFilas()
	{
		return bitmapinfoheader->biHeight;
	}


	//============================================================================
	// NOMBRE: GetNumeroColumnas
	//
	// DESCRIPCIÓN: Retorna el número de columnas de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor entero que indica el número de columnas			
	//============================================================================
	int Texto::GetNumeroColumnas()
	{
		return bitmapinfoheader->biWidth;
	}
	
	
	//============================================================================
	// NOMBRE: GetTamano
	//
	// DESCRIPCIÓN: Retorna el tamaño de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor entero que indica el tamaño de la imagen en bytes			
	//============================================================================
	int Texto::GetTamano()
	{
		return bitmapinfoheader->biSizeImage;
	}


	//============================================================================
	// NOMBRE: GetProfundidad
	//
	// DESCRIPCIÓN: Retorna la profundidad de la imagen.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor entero que indica la profundidad en bits/píxel			
	//============================================================================
	int Texto::GetProfundidad()
	{
		return bitmapinfoheader->biBitCount;
	}

	
	//============================================================================
	// NOMBRE: EstaUmbralizada
	//
	// DESCRIPCIÓN: Comprueba si la imagen está umbralizada.
	//
	// ARGUMENTOS: -
	//
	// SALIDA: Un valor booleano que indica:
	//			true  -> La imagen está umbralizada
	//			false -> La imagen no está umbralizada	
	//============================================================================
	bool Texto::EstaUmbralizada()
	{
		//En principio suponemos que la imagen no est´aumbralizada
		umbralizada = false;

		//Calculamos el número de lementos de la paleta
		int tamano_paleta = 0;

		if(bitmapinfoheader->biBitCount<=8)
		{
			if(bitmapinfoheader->biClrUsed!=0)
				tamano_paleta = bitmapinfoheader->biClrUsed;
			else
				tamano_paleta = (int)pow(2.0, bitmapinfoheader->biBitCount);
		}
		//Fin calculo tamaño paleta

		int i;

		for(i=0; i<tamano_paleta && paleta[i].rgbBlue==paleta[i].rgbGreen && paleta[i].rgbGreen==paleta[i].rgbRed; i++);

		if(i!=tamano_paleta || tamano_paleta==0)//tamano_paleta==0, por si la imagen no tiene paleta
			return false;

		//Está en niveles de gris, ahora comprobamos que esté umbralizada
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
				if(mapa_bits[posicion(i,j,1)]!=0 && mapa_bits[posicion(i,j,1)]!=255)
					return false;

		//La imagen está umbralizada
		//Actualizamos la propiedad de umbralizada
		umbralizada = true;
		return true;
	}


	//============================================================================
	// NOMBRE: ExisteCoordenada
	//
	// DESCRIPCIÓN: Comprueba si una coordenada pertenece a la imagen.
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
	// DESCRIPCIÓN: Comprueba si una coordenada pertenece al segmento.
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
	// DESCRIPCIÓN: Comprueba si la imagen (256 colores, si no se devolverá false automáticamente) actual es igual a la pasada por parámetro (en dimensiones y valores).
	//
	// ARGUMENTOS:	Texto *imagen -> Imagen de 256 colores, si no se devolverá false automáticamente
	//
	// SALIDA: Un valor booleano que indica:
	//			true  -> Las imágenes no son iguales
	//			false -> Las imágenes no son iguales o alguna no es de 8 bits de profundidad
	//============================================================================
	bool Texto::EsIgual(Texto *imagen)
	{
		//Comprobamos las profundidades de las imágenes
		if(bitmapinfoheader->biBitCount!=8 || imagen->bitmapinfoheader->biBitCount!=8)
			return false;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=imagen->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=imagen->bitmapinfoheader->biWidth)
			return false;

		//Comprobamos los valores del mapa de bits
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				//Al primer píxel distinto que encontremos devolvemos false
				if(mapa_bits[posicion(i, j, 1)]!=imagen->mapa_bits[posicion(i, j, 1)])
					return false;
			}

		//Las imágenes son iguales
		return true;
	}


	//======================================================================
	// NOMBRE: Escalacion
	//
	// DESCRIPCIÓN: Escala una imagen.
	//
	// ARGUMENTOS:	double factor_columnas  -> Factor de redimensionado del ancho de la imagen
	//				double factor_filas		-> Factor de redimensionado del alto de la imagen
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Factores negativos no permitidos
	//			2 -> Error. Imagen no es de 1, 4, 8 ó 24 bits/píxel
	//======================================================================
	int Texto::Escalacion(double factor_columnas, double factor_filas)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos que los factores de escalado sean positivos
		if(factor_columnas<0 || factor_filas<0)
			return 1;

		//Comprobamos que la profundidad de la imagen esté soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;

		if(factor_columnas!=1 || factor_filas!=1)//Si los dos son 1 no hay que escalar nada
		{
			int tamano_pixel = profundidad/8;

			//Calculamos el tamaño de la paleta
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

				//Reasignación punteos
				inicio_DIB = imagen_auxiliar->inicio_DIB;
				bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
				bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
				if(imagen_auxiliar->paleta!=NULL)
					paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
				else//No hay paleta
					paleta = NULL;
				mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
				//Fin reasignación punteros
		}

		return 0;
	}


	//======================================================================
	// NOMBRE: EscalacionLineal
	//
	// DESCRIPCIÓN: Escala una imagen con interpolación lineal.
	//
	// ARGUMENTOS:	double factor_columnas  -> Factor de redimensionado del ancho de la imagen
	//				double factor_filas		-> Factor de redimensionado del alto de la imagen
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Factores negativos no permitidos
	//			2 -> Error. Imagen no es de 1, 4, 8 ó 24 bits/píxel
	//======================================================================
	int Texto::EscalacionLineal(double factor_columnas, double factor_filas)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos que los factores de escalado sean positivos
		if(factor_columnas<0 || factor_filas<0)
			return 1;

		//Comprobamos que la profundidad de la imagen esté soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;

		if(factor_columnas!=1 || factor_filas!=1)//Si los dos son 1 no hay que escalar nada
		{
			int tamano_pixel = profundidad/8;

			//Calculamos el tamaño de la paleta
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

				//Reasignación punteos
				inicio_DIB = imagen_auxiliar->inicio_DIB;
				bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
				bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
				if(imagen_auxiliar->paleta!=NULL)
					paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
				else//No hay paleta
					paleta = NULL;
				mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
				//Fin reasignación punteros
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
	// DESCRIPCIÓN: Rota una imagen.
	//
	// ARGUMENTOS:	int modo	  ->  Indica modo de rotación
	//										TAMANO_ORIGINAL 0
	//										NUEVO_TAMANO	1
	//				double grados ->  Indica el nº de grados  si <0 ---> izquierda ; si >0 ---> derecha
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Grados no válidos
	//			2 -> Error. Imagen no es de 1, 4, 8 ó 24 bits/píxel
	//			3 -> Error.  Valor de Modo no válido
	//======================================================================
	int Texto::Rotacion(int modo, double grados)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos si el valor de los grados es válido 
		if(fabs(grados)>360)
			return 1;

		//Comprobamos que la profundidad de la imagen esté soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;
		
		if(grados!=0 && grados!=360)//Si son 0 grados o 360 no hacemos nada
		{
			int tamano_pixel = profundidad / 8;

			//Calculamos el tamaño de la paleta
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
				case TAMANO_ORIGINAL://Como no lo vamos a usar en la aplicación dejamos la versión sin optimizar completamente, pero mucho más clara
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
					
					//Realizamos la rotación
					for(int i=0; i<imagen_auxiliar->bitmapinfoheader->biHeight; i++)
					{
						//Calculamos los datos que nos valdrán para toda la fila (independientes de la columna)
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
													
							if(grados!=90 && grados!=180 && grados!=270)//Con esta división se obtienen mejores resultados
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
							else//REVISAR ---> INTENTAR MEJORAR CON APROXIMACIÓN O ALGO
								memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i, j, tamano_pixel)], 0, tamano_pixel);
						}
					}
						
					delete inicio_DIB;//boramos imagen oiginal

					//Sustituimos la imagen actual por la imagen rotada
					//Reasignación punteos
					inicio_DIB = imagen_auxiliar->inicio_DIB;
					bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
					bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
					if(imagen_auxiliar->paleta!=NULL)
						paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
					else//No hay paleta
						paleta = NULL;
					mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
					//Fin reasignación punteros
				}
				break;
				case NUEVO_TAMANO://Versión optimizada disminuyendo el número de cálculos necesarios
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
					
					//Realizamos la rotación
					for(int i=0; i<imagen_auxiliar->bitmapinfoheader->biHeight; i++)
					{
						//Calculamos los datos que nos valdrán para toda la fila (independientes de la columna)
						int y = i - centroy_imagen_nueva;
						double cosenoy = coseno * y;
						double senoy = seno * y;

						//Para una mayor optimización reducimos las llamadas a la función posición a una vez por fila
						for(int p = imagen_auxiliar->posicion(i, 0, tamano_pixel), j=0; j<imagen_auxiliar->bitmapinfoheader->biWidth; j++, p+=tamano_pixel)
						{
							//Calculo de los datos que dependen de la columna
							int x = j - centrox_imagen_nueva;
							
							//Coordenadas del pixel (i,j) de la imagen rotada en la imagen actual
							int coordenadaX = (int)(cosenox[j] + senoy);
							int coordenadaY = (int)(-senox[j] + cosenoy);

							coordenadaX += centrox;
							coordenadaY += centroy;
												
							//Eliminamos la llamada a ExisteCoordenada ya que es más eficiente poner el codigo directamente
							if((coordenadaX>=0) && (coordenadaX<bitmapinfoheader->biWidth) && (coordenadaY>=0) && (coordenadaY<bitmapinfoheader->biHeight))
							{
								//Si existe la coordenada en la imagen actual cogemos el valor del pixel
								int posicion_imagen_this=posicion(coordenadaY, coordenadaX, tamano_pixel);
								memcpy(&imagen_auxiliar->mapa_bits[p], &mapa_bits[posicion_imagen_this], tamano_pixel);								
							}
							else//REVISAR ---> INTENTAR MEJORAR CON APROXIMACIÓN O ALGO
								memset(&imagen_auxiliar->mapa_bits[p], 255, tamano_pixel);
						}
					}

					//Eliminamos los vectores usados para ahorrarnos cálculos
					delete cosenox;
					delete senox;
					
					delete inicio_DIB;//boramos imagen oiginal

					//Sustituimos la imagen actual por la imagen rotada
					//Reasignación punteos
					inicio_DIB = imagen_auxiliar->inicio_DIB;
					bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
					bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
					if(imagen_auxiliar->paleta!=NULL)
						paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
					else//No hay paleta
						paleta = NULL;
					mapa_bits = (BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
					//Fin reasignación punteros
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
	// DESCRIPCIÓN: Rota un segmento de la imagen respetando el tamaño del mismo. (Versión optimizada disminuyendo el número de cálculos necesarios)
	//
	// ARGUMENTOS:	Palabra segmento -> Segmento de la imagen
	//				double grados	 -> Indica el nº de grados  si <0 ---> izquierda ; si >0 ---> derecha
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Grados no válidos
	//			2 -> Error. Imagen no es de 1, 4, 8 ó 24 bits/píxel			
	//======================================================================
	int Texto::Rotacion(Palabra segmento, double grados)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos si el valor de los grados es válido 
		if(fabs(grados)>360)
			return 1;

		//Comprobamos que la profundidad de la imagen esté soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;
		
		if(grados!=0 && grados!=360)//Si son 0 grados o 360 no hacemos nada)
		{
			int tamano_pixel = profundidad / 8;

			//Calculamos el tamaño de la paleta
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
							
			//Realizamos la rotación
			for(int i=segmento.esquina_superior_izquierda.y; i<=segmento.esquina_inferior_derecha.y; i++)
			{
				//Calculamos los datos que nos valdrán para toda la fila (independientes de la columna)
				int y = i - centroy;
				double cosenoy = coseno * y;
				double senoy = seno * y;

				//Para una mayor optimización reducimos las llamadas a la función posición a una vez por fila
				for(int p = imagen_auxiliar->posicion(i - segmento.esquina_superior_izquierda.y, 0, tamano_pixel), j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++, p+=tamano_pixel)
				{
					//Calculo de los datos que dependen de la columna
					int x = j - centrox;
					
					//Coordenadas del pixel (i,j) de la imagen rotada en la imagen actual
					int coordenadaX = (int)(cosenox[j - segmento.esquina_superior_izquierda.x] + senoy);
					int coordenadaY = (int)(-senox[j - segmento.esquina_superior_izquierda.x] + cosenoy);

					coordenadaX += centrox;
					coordenadaY += centroy;
										
					//Eliminamos la llamada a ExisteCoordenada ya que es más eficiente poner el codigo directamente
					if((coordenadaX>=segmento.esquina_superior_izquierda.x) && (coordenadaX<=segmento.esquina_inferior_derecha.x) && (coordenadaY>=segmento.esquina_superior_izquierda.y) && (coordenadaY<=segmento.esquina_inferior_derecha.y))
					{
						//Si existe la coordenada en el segmento actual cogemos el valor del pixel
						int posicion_imagen_this=posicion(coordenadaY, coordenadaX, tamano_pixel);
						memcpy(&imagen_auxiliar->mapa_bits[p], &mapa_bits[posicion_imagen_this], tamano_pixel);								
					}
					else//REVISAR ---> INTENTAR MEJORAR CON APROXIMACIÓN O ALGO
						memset(&imagen_auxiliar->mapa_bits[p], 255, tamano_pixel);
				}
			}
			
			//Eliminamos los vectores usados para ahorrarnos cálculos
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
	// DESCRIPCIÓN: Aplica Shear un segmento de la imagen respetando el tamaño del mismo. (Versión optimizada disminuyendo el número de cálculos necesarios)
	//
	// ARGUMENTOS:	Palabra segmento -> Segmento de la imagen
	//				double grados	 -> Indica el nº de grados  si <0 ---> izquierda ; si >0 ---> derecha
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Grados no válidos
	//			2 -> Error. Imagen no es de 1, 4, 8 ó 24 bits/píxel			
	//======================================================================
	int Texto::Shear(Palabra segmento, double grados)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos si el valor de los grados es válido 
		if(fabs(grados)>360)
			return 1;

		//Comprobamos que la profundidad de la imagen esté soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;
		
		if(grados!=0 && grados!=360)//Si son 0 grados o 360 no hacemos nada)
		{
			int tamano_pixel = profundidad / 8;

			//Calculamos el tamaño de la paleta
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
				//Calculamos los datos que nos valdrán para toda la fila (independientes de la columna)
				int y = i - centroy;
				double tangentey = tangente * y;

				//Para una mayor optimización reducimos las llamadas a la función posición a una vez por fila
				for(int p = imagen_auxiliar->posicion(i - segmento.esquina_superior_izquierda.y, 0, tamano_pixel), j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++, p+=tamano_pixel)
				{
					//Calculo de los datos que dependen de la columna
					int x = j - centrox;
					
					//Coordenadas del pixel (i,j) de la imagen rotada en la imagen actual
					int coordenadaX = x + (int)(tangentey);
					int coordenadaY = i;

					coordenadaX += centrox;
															
					//Eliminamos la llamada a ExisteCoordenada ya que es más eficiente poner el codigo directamente
					if((coordenadaX>=segmento.esquina_superior_izquierda.x) && (coordenadaX<=segmento.esquina_inferior_derecha.x) && (coordenadaY>=segmento.esquina_superior_izquierda.y) && (coordenadaY<=segmento.esquina_inferior_derecha.y))
					{
						//Si existe la coordenada en el segmento actual cogemos el valor del pixel
						int posicion_imagen_this=posicion(coordenadaY, coordenadaX, tamano_pixel);
						memcpy(&imagen_auxiliar->mapa_bits[p], &mapa_bits[posicion_imagen_this], tamano_pixel);								
					}
					else//REVISAR ---> INTENTAR MEJORAR CON APROXIMACIÓN O ALGO
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
	// DESCRIPCIÓN: Endereza un texto automáticamente en caso de estar inclinado (dentro de lo posible).
	//
	// ARGUMENTOS: -
	//								
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada		
	//======================================================================
	int Texto::Enderezar()
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		int angulo = 0;//Ángulo estimado de inclinación del texto
		int desplazamiento = 3;//Incremento inicial a la hora de testear inclinaciones
		int sentido = 1;//Sentido en el que buscamos la inclinación
		BYTE estado = 0;//0 -> inicializamos 'búsqueda'	   1 -> hemos cambiado el sentido en la prueba anterior    2 -> el cambio de sentido ha mejorado el resultado    3 -> hemos encontrado un máximo (posiblemente local :( )

		//Creamos una imagen auxiliar sobre la que testear la inclinación
		Texto *texto_auxiliar = new Texto();		

		//Reservamos memoria para almacenar la proyección horizontal de la imagen
		int *proyeccion = new int[bitmapinfoheader->biHeight];
				
		//Variables para guardar la desviación actual y la anterior. Nuestro objetivo es maximizar esta desviación
		double desviacion_antigua;
		double desviacion_nueva;

		//Calculamos la proyección horizontal, así como su desviación asociada
		proyeccionHorizontal(proyeccion, &desviacion_nueva);

		while(estado!=3)
		{
			//Guardamos la última desviación
			desviacion_antigua = desviacion_nueva;

			//Calculamos el siguiente ángulo a testear
			angulo += sentido * desplazamiento;

			//Copiamos la imagen original en la auxiliar
			(*texto_auxiliar) = (*this);

			//Rotamos la imagen auxiliar
			texto_auxiliar->Rotacion(NUEVO_TAMANO, angulo);			

			//Borramos la proyección anterior y reservamos memoria para la nueva. No podemos aprovechar la anterior, porque al rotar la imagen podemos tener un número distinto de filas
			delete proyeccion;
			//Reservamos memoria para almacenar la proyección horizontal de la imagen
			proyeccion = new int[texto_auxiliar->bitmapinfoheader->biHeight];

			//Calculamos la proyección horizontal, así como su desviación asociada
			texto_auxiliar->proyeccionHorizontal(proyeccion, &desviacion_nueva);

			//Si la desviación ha empeorado (disminuido)
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
			else//La desviación ha mejorado (incrementado)
			{
				if(estado==1 && fabs((double)desplazamiento)==1)
					estado = 2;
				else//Si no venimos de un cambio de sentido 'reiniciamos' la búsqueda
					estado = 0;
			}
		}

		//Dejamos el ángulo que ha máximizado la desviación, ya que el último calculado siempre es uno peor
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
	// DESCRIPCIÓN: Umbralización binaria de una imagen.
	//
	// ARGUMENTOS:	int umbral		 -> Filtro
	//				int nivel_minimo -> Valor que se dará a los píxeles que sean menores que el umbral
	//				int nivel_maximo -> Valor que se dará a los píxeles que sean mayores o iguales que el umbral
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Umbral negativo o mayor de 255
	//			2 -> Error. Imagen no es de 1, 4, 8 ó 24 bits/píxel
	//			3 -> Error. Al menos uno de los valores asignados a los rangos es incorrecto.
	//======================================================================
	int Texto::Umbralizar(int umbral, int nivel_minimo, int nivel_maximo)
	{
		int profundidad = bitmapinfoheader->biBitCount;

		//Comprobamos que el umbral sea correcto
		if(umbral<0 || umbral>255)
			return 1;
		
		//Comprobamos que la profundidad de la imagen esté soportada
		if(profundidad!=1 && profundidad!=4 && profundidad!=8 && profundidad!=24 && profundidad!=32)
			return 2;		

		//Comprobamos que los niveles máximo y mínimo sean correctos
		if(nivel_minimo<0 || nivel_maximo>255)
			return 3;	
		
		if(profundidad==8)
		{
			//Recorremos la imagen
			for(int i=0; i<bitmapinfoheader->biHeight; i++)
				for(int j=0; j<bitmapinfoheader->biWidth; j++)
				{
					//Obtenemos la posición en la paleta del píxel actual
					int posicion_paleta = mapa_bits[posicion(i, j, 1)];
					
					//Valor en nivel de gris del píxel actual
					BYTE nivel_gris = (BYTE)((paleta[posicion_paleta].rgbBlue + paleta[posicion_paleta].rgbGreen + paleta[posicion_paleta].rgbRed) / 3);

					//Realizamos la umbralización
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
					//Obtenemos la posición del píxel actual en el array para ahorrar cálculos
					int posicion_pixel = posicion(i, j, 3);

					//Valor en nivel de gris del píxel actual
					BYTE nivel_gris = (BYTE)((mapa_bits[posicion_pixel] + mapa_bits[posicion_pixel + 1] + mapa_bits[posicion_pixel + 2]) / 3);
							
					//Realizamos la umbralización
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
				//Fin reasignación de punteros				
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
	// DESCRIPCIÓN: Segmentación de una imagen de texto en líneas.
	//
	// ARGUMENTOS: int interlineado_minimo	-> Separación mínima, en píxeles, que debe haber entre dos líneas consecutivas
	//			   int diferencia_minima	-> Diferencia de píxeles entre la fila actual y la anterior necesaria para que consideremos la presencia de un renglón
	//			   int espaciado_medio		-> Espacio medio entre palabras, en píxeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//======================================================================
	int Texto::SegmentacionLineas(int interlineado_medio, int diferencia_minima, int espaciado_medio)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Separación que dejaremos entre las líneas en la imagen una vez procesada
		int espaciado = 15;

		//Obtenemos las líneas de la imagen
		ObtenerLineas(interlineado_medio, diferencia_minima, espaciado_medio);

		//Quitamos las filas blancas de cada renglón, tanto por arriba como por abajo
		for(int l=0; l<numeroLineas; l++)
		{
			//Eliminamos las filas blancas superiores
			for(int i=lineas[l].esquina_superior_izquierda.y; i<=lineas[l].esquina_superior_izquierda.y ; i++)
			{
				int j;

				for(j=0; j<bitmapinfoheader->biWidth && mapa_bits[posicion(i,j,1)]==255; j++);

				if(j==bitmapinfoheader->biWidth)//No hay ningún píxel en la fila
					lineas[l].esquina_superior_izquierda.y++;
				else
					break;
			}

			//Eliminamos las filas blancas inferiores
			for(int i=lineas[l].esquina_inferior_derecha.y; i>=lineas[l].esquina_superior_izquierda.y ; i--)
			{
				int j;

				for(j=0; j<bitmapinfoheader->biWidth && mapa_bits[posicion(i,j,1)]==255; j++);

				if(j==bitmapinfoheader->biWidth)//No hay ningún píxel en la fila
					lineas[l].esquina_inferior_derecha.y--;
				else
					break;
			}			
		}

		//Creamos una nueva imagen con el tamaño necesario para guardar todas las líneas halladas con una separación interlínea de 'espaciado' píxeles
		Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight + espaciado * (numeroLineas + 1), 8, paleta, 256); 
		
		//Creamos la estructura de líneas de la imagen segmentada
		imagen_auxiliar->lineas = new Linea[numeroLineas];

		//Copiamos las líneas de la imagen original en la imagen segmentada
		int tamano_pixel = bitmapinfoheader->biBitCount / 8;

		//Calculamos las columnas de relleno
		int columnas_relleno = 0;
		if((bitmapinfoheader->biWidth*tamano_pixel) % 4!=0)
			columnas_relleno = 4 - ((bitmapinfoheader->biWidth * tamano_pixel) % 4);

		int inicio = 0;//Fila donde empieza la primera línea en l aimagen original
		if(numeroLineas>0)
			inicio = lineas[0].esquina_superior_izquierda.y; 
		
		//Copiamos las líneas de la imagen original en la imagen segmentada
		for(int l=0; l<numeroLineas; l++)
		{
			//Asignamos los atributos de una línea
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

			//Pintamos de negro una fila inferior y otra superior a la línea en la imagen segmentada (varias filas superiores e inferiores para mo interferir en el cálculo del esqueleto)
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la línea de la imagen original en la imagen segmentada
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
		//Fin reasignación de punteros

		return 0;
	}


	//======================================================================
	// NOMBRE: SegmentacionPalabras
	//
	// DESCRIPCIÓN: Segmentación de las líneas de una imagen de texto en palabras.
	//
	// ARGUMENTOS: int espaciado_medio	-> Espacio medio entre palabras, en píxeles
	//			   bool pintar_limites	-> Indica si queremos pintar en la imagen los límites de las distintas palabras
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//======================================================================
	int Texto::SegmentacionPalabras(int espaciado_medio, bool pintar_limites)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Obtenemos las palabras de cada una de las líneas de la imagen
		ObtenerSegmentos(espaciado_medio);

		//Pintamos los límites de las palabras encontradas
		if(pintar_limites)
		{
			for(int l=0; l<numeroLineas; l++)
				for(int p=0; p<lineas[l].numeroPalabras; p++)
					for(int i=lineas[l].palabras[p].esquina_superior_izquierda.y; i<=lineas[l].palabras[p].esquina_inferior_derecha.y; i++)
							mapa_bits[posicion(i, lineas[l].palabras[p].esquina_superior_izquierda.x, 1)] = 0;
		}

		return 0;
	}


	//####################################ESTA AÚN NO LA COMENTO PORQUE ES PROVISIONAL#################################
	//======================================================================
	// NOMBRE: SegmentacionCaracteres
	//
	// DESCRIPCIÓN: Segmentación de las palabras de una imagen de texto en caracteres.
	//
	// ARGUMENTOS: int interlineado_minimo	-> Separación mínima, en píxeles, que debe haber entre dos líneas consecutivas
	//			   int diferencia_minima	-> Diferencia de píxeles entre la fila actual y la anterior necesaria para que consideremos la presencia de un renglón
	//			   int espaciado_medio		-> Espacio medio entre palabras, en píxeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//======================================================================
	int Texto::SegmentacionCaracteres(int grosor_lapiz, double incremento_minimo, bool pintar_limites)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Obtenemos los caracteres de cada una de las palabras de cada una de las líneas de la imagen
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				ObtenerCaracteres(&lineas[l].palabras[p], grosor_lapiz, incremento_minimo);

		//Pintamos los límites de las palabras encontradas
		if(pintar_limites)
		{
			for(int l=0; l<numeroLineas; l++)
				for(int p=0; p<lineas[l].numeroPalabras; p++)
					for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
						for(int i=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.y; i<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.y; i++)
							mapa_bits[posicion(i, lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.x-1, 1)]=0;
		}

		//Eliminamos los espacios sobrantes del rectángulo que engloba al caracter (quitamos líneas y columnas en blanco)
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
					ajustarCaracter(&lineas[l].palabras[p].caracteres[c]);

		return 0;
	}	
	

	//======================================================================
	// NOMBRE: ObtenerLineas
	//
	// DESCRIPCIÓN: Obtiene las líneas de una imagen, guardándolas en la estructura correspondiente.
	//
	// ARGUMENTOS: int interlineado_minimo	-> Separación mínima, en píxeles, que debe haber entre dos líneas consecutivas
	//			   int diferencia_minima	-> Diferencia de píxeles entre la fila actual y la anterior necesaria para que consideremos la presencia de un renglón
	//			   int espaciado_medio		-> Espacio medio entre palabras, en píxeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//======================================================================
	int Texto::ObtenerLineas(int interlineado_medio, int diferencia_minima, int espaciado_medio)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Borramos las líneas encontradas anteriormente, en caso de que existan
		if(lineas!=NULL)
		{
			delete lineas;
			lineas = NULL;
			numeroLineas = 0;
		}

		//Creamos una imagen auxiliar sobre la que aplicaremos el algoritmo RLSA
		Texto *texto_auxiliar = new Texto();		
		(*texto_auxiliar) = (*this);

		//Aplicamos RLSA sobre la imagen auxiliar para crear unas 'líneas' consistentes
		texto_auxiliar->RLSA(espaciado_medio);

		//Reservamos espacio para la proyección horizontal de la imagen
		int *proyeccion = new int[bitmapinfoheader->biHeight];

		//Calculamos la proyección horizontal d el aimagen, sin calcular su desviación asociada
		texto_auxiliar->proyeccionHorizontal(proyeccion);

		//Calculamos la 'derivada' de la proyección (diferencia de cada posición con la anterior)
		for(int i=0; i<bitmapinfoheader->biHeight - 1; i++)
			proyeccion[i] = proyeccion[i+1] - proyeccion[i];

		//Buscaremos los valles en la proyección, ya que será en éstos donde haya una separación entre líneas
		//Variables necesarias
		int i;//Fila actual de la imagen
		int posicion_inicio;//Primera fila de un posible valle
		int posicion_fin;//Última fila de un posible valle
		int elementos_meseta = 0;//Contador de los píxeles de un valle de la proyeccion
		BYTE estado;//0 -> buscando valle(bajada)    1 -> buscando subida(valle)    2 -> buscando bajada(subida)
		Linea linea;//Variable para ir guardando la línea encontrada
		Pila *pila_lineas;//Pila para almacenar todas las líneas que vamos encontrando

		//Inicializacion
		posicion_inicio = -1;
		posicion_fin = -1;
		//Creamos la pila
		pila_lineas = new Pila();
		//Rellenamos la coordenada izquierda superior de la primera línea
		linea.esquina_superior_izquierda.x = 0;
		linea.esquina_superior_izquierda.y = 0;

		//Buscamos la primera subida, que nos indicará la presencia de un renglón
		for(i=0; fabs((double)proyeccion[i])<=diferencia_minima && i<bitmapinfoheader->biHeight; i++);
		
		//A partir de aqui buscamos una bajada, por lo que inicializamos el 'autómata' precisamente a ese estado		
		estado = 2;

		//Vamos recorriendo la proyección
		for(i; i<bitmapinfoheader->biHeight - 1; i++)
		{
			//'Autómata'
			switch(estado)
			{
				case 0://Si estamos en bajada
					//Si la proyección es mayor que la diferencia mínima estamos ante una subida en la proyección (mitad de un renglón)
					if(proyeccion[i]>diferencia_minima)
						estado = 2;
					else
					{
						if(fabs((double)proyeccion[i])<=diferencia_minima)
					    {
							//Hemos encontrado un 'valle' (realmente para que sea valle depende de la longitud de la meseta, así como que le siga una subida) 
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
						//Si no se ha superado el número mínimo de píxeles en el valle
						if(fabs((double)proyeccion[i])<=diferencia_minima)
						{
							//Si seguimos en una meseta incrementamos el número de píxeles en la misma
							elementos_meseta++;
							//El estado no varia
						}
						else//Sino no estabamos ante un valle y debemos seguir buscando uno
							estado = 0;
					}
					else//Se ha superado el número de píxeles mínimo para considerar un valle real
					{
						if(proyeccion[i]>diferencia_minima)
						{
							//Si hay una subida significa que empieza un nuevo renglón, por lo que tenemos que añadir el que acaba de terminar
							estado = 2;
							posicion_fin = i;

							//Añadimos la línea
							linea.esquina_inferior_derecha.x = bitmapinfoheader->biWidth - 1;
							linea.esquina_inferior_derecha.y = posicion_inicio + (posicion_fin - posicion_inicio) / 2;
							//NOTA ---> OTRA OPCION SERIA COGER LA INICIO PARA LA NUEVA LÍNEA Y LA FIN PARA LA ACTUAL, PARA COGER LA MÁXIMA ZONA POSIBLE DE LA LÍNEA)

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
							else//Seguimos en un posible valle, incrementamos el número de píxeles en el mismo
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

		//Añadimos la última línea
		linea.esquina_inferior_derecha.x = bitmapinfoheader->biWidth - 1;
		linea.esquina_inferior_derecha.y = bitmapinfoheader->biHeight - 1;

		pila_lineas->Apilar(linea);

		//'Copiamos' las líneas al atributo de nuestra clase Texto
		//Cogemos el número de líneas que hemos encontrado
		numeroLineas = pila_lineas->NumeroElementos();

		//Reservamos la memoria suficiente para almacenar las líneas encontradas
		lineas=new Linea[numeroLineas];

		//Pasamos las líneas encontradas de la pila al vector
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
	// DESCRIPCIÓN: Obtiene las palabras o segmentos de una imagen, guardándolas en la estructura correspondiente.
	//
	// ARGUMENTOS: int distancia_minima	-> Espacio medio entre caracteres, en píxeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//======================================================================
	int Texto::ObtenerSegmentos(int distancia_minima)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Creamos una imagen auxiliar sobre la que aplicaremos el algoritmo RLSA
		Texto *imagen_auxiliar = new Texto();
		(*imagen_auxiliar) = (*this);

		//Aplicamos RLSA sobre la imagen auxiliar para crear unas 'palabras' consistentes
		imagen_auxiliar->RLSA(distancia_minima);

		//Encontramos los segmentos para todas las líneas
		for(int l=0; l<numeroLineas; l++)
		{		
			//Borramos los segmentos encontrados anteriormente, en caso de que existan
			if(lineas[l].palabras!=NULL)
			{
				delete lineas[l].palabras;
				lineas[l].palabras = NULL;
				lineas[l].numeroPalabras = 0;
			}
			
			//Buscaremos las palabras de la línea actual, para ello buscamos las columnas sin pixeles negros
			//Variables necesarias
			bool pixel_negro_encontrado;//Indica si hay texto en la columna actual
			int i, j;//Fila y columna actuales		
			int posicion_inicio;//Primera columna sin píxeles negros
			int posicion_fin;//Última columna sin píxeles negros (realmente es la primera con píxel negro)
			Palabra segmento;//Variable para ir guardando la palabra encontrada
			Pila *pila_segmentos;//Pila para almacenar todos los segmentos que vamos encontrando

			//Inicializacion
			pixel_negro_encontrado = false;			
			posicion_inicio = -1;
			posicion_fin = -1;
			//Creamos la pila
			pila_segmentos = new Pila();
			//Rellenamos la coordenada izquierda superior de la primera línea, así como la fila de la inferior derecha
			segmento.esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			segmento.esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y;
			segmento.esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y;

			//Buscamos la primera columna con texto
			for(j=lineas[l].esquina_superior_izquierda.x; j<=lineas[l].esquina_inferior_derecha.x && !pixel_negro_encontrado; j++)
				for(int i=lineas[l].esquina_superior_izquierda.y; i<=lineas[l].esquina_inferior_derecha.y && !pixel_negro_encontrado; i++)
					if(imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(i,j,1)]==0)
						pixel_negro_encontrado = true;

			//Vamos recorriendo el renglón por columnas, hasta encontrar una en blanco
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

					//Comprobamos si hemos llegado al final de la línea y corregimos la posición fin
					if(posicion_fin>=bitmapinfoheader->biWidth)
						posicion_fin--;

					//Añadimos la palabra
					if(pixel_negro_encontrado)
						segmento.esquina_inferior_derecha.x = posicion_inicio + (posicion_fin - posicion_inicio + 1) / 2;
					else//Estamos en la última palabra del renglón
						segmento.esquina_inferior_derecha.x = lineas[0].esquina_inferior_derecha.x;					
					
					pila_segmentos->Apilar(segmento);

					//Empiezamos la nueva palabra
					segmento.esquina_superior_izquierda.x = segmento.esquina_inferior_derecha.x;
					//segmento.esquina_superior_izquierda.y --> no hace falta cambiarlo, es igual para todas las palabras
					//segmento.esquina_inferior_derecha.y --> no hace falta cambiarlo, es igual para todas las palabras
				}
			}

			//'Copiamos' las líneas al atributo de nuestra clase Texto
			//Cogemos el número de palabras que hemos encontrado en la línea l
			lineas[l].numeroPalabras = pila_segmentos->NumeroElementos();

			//Reservamos la memoria suficiente para almacenar las palabras encontradas en la línea l
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


	//####################################ESTA AÚN NO LA COMENTO PORQUE ES PROVISIONAL#################################
	//======================================================================
	// NOMBRE: ObtenerCaracteres
	//
	// DESCRIPCIÓN: Obtiene las caracteres de una palabra, guardándolos en la estructura correspondiente.
	//
	// ARGUMENTOS: int distancia_minima	-> Espacio medio entre caracteres, en píxeles
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//======================================================================
	int Texto::ObtenerCaracteres(Palabra *palabra, int grosor_lapiz, double incremento_minimo)
	{
		//Comprobamos que la imagen esté umbralizada
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

				//Calculo pixel más alto
				int i;
				for(i=(*palabra).esquina_superior_izquierda.y; mapa_bits[posicion(i, k, 1)]!=0 && i<=(*palabra).esquina_inferior_derecha.y; i++);
				
				int pixel_mas_alto;

				if(i<=(*palabra).esquina_inferior_derecha.y)
					pixel_mas_alto=altura_linea - (i-(*palabra).esquina_superior_izquierda.y);
				else
					pixel_mas_alto=0;

				//Calculo pixel más bajo
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

							//Añadir caracter
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
									//Añadir caracter
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

								//Añadir caracter
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

		//Añadir caracter
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
	// DESCRIPCIÓN: Corrige el Slope de los segmentos de todas las líneas de una imagen de texto. (Slope -> Inclinación de las palabras respecto al eje horizontal)
	//				Sino hemos obtenido antes los segmentos de la imagen, la correción no tendrá efecto alguno.
	//
	// ARGUMENTOS: -
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada
	//======================================================================
	int Texto::CorregirSlope()
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Corregimos el Slope de cada una de las líneas de la imagen
		for(int l=0; l<numeroLineas; l++)
			CorregirSlope(lineas[l]);

		return 0;
	}


	//======================================================================
	// NOMBRE: CorregirSlope
	//
	// DESCRIPCIÓN: Corrige el Slope de una línea de una imagen de texto. (Slope -> Inclinación de las palabras respecto al eje horizontal)
	//				Sino hemos obtenido antes los segmentos de la imagen, la correción no tendrá efecto alguno.
	//
	// ARGUMENTOS: Linea linea	-> Línea sobre la que se aplicará la correción
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//======================================================================
	int Texto::CorregirSlope(Linea linea)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;		
		
		//Realizamos la correción para todos los segmentos de la línea
		for(int s=0; s<linea.numeroPalabras; s++)
		{			
			int angulo = 0;//Ángulo estimado de inclinación del texto
			int desplazamiento = 3;//Incremento inicial a la hora de testear inclinaciones
			int sentido = 1;//Sentido en el que buscamos la inclinación
			BYTE estado = 0;//0 -> inicializamos 'búsqueda'	   1 -> hemos cambiado el sentido en la prueba anterior    2 -> el cambio de sentido ha mejorado el resultado    3 -> hemos encontrado un máximo (posiblemente local :( )
					
			//Reservamos memoria para almacenar la proyección horizontal de la imagen
			//Estamos reservando memoria de más (para todas las líneas) pero a cambio nos ahorramos cálculos (ajustes en los índices de los vectores)
			int *proyeccion = new int[bitmapinfoheader->biHeight];
						
			//Variables para guardar máximo actual y el anterior. Nuestro objetivo es maximizar este máximo
			double maximo_antiguo;
			double maximo_nuevo;

			//Calculamos la proyección horizontal del segmento, así como su máximo asociado
			proyeccionHorizontal(linea.palabras[s], proyeccion, &maximo_nuevo);

			while(estado!=3)
			{
				//Creamos una imagen auxiliar sobre la que testear la inclinación
				Texto *texto_auxiliar = new Texto();

				//Guardamos el último máximo
				maximo_antiguo = maximo_nuevo;

				//Calculamos el siguiente ángulo a testear
				angulo += sentido * desplazamiento;

				//Copiamos la imagen original en la auxiliar
				(*texto_auxiliar)=(*this);

				//Rotamos el segmento de la imagen auxiliar
				texto_auxiliar->Rotacion(linea.palabras[s], angulo);			

				//Calculamos la proyección horizontal, así como su máximo asociado
				texto_auxiliar->proyeccionHorizontal(linea.palabras[s], proyeccion, &maximo_nuevo);

				//Si el máximo ha empeorado (disminuido)
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
				else//El máximo ha mejorado (incrementado)
				{
					if(estado==1 && fabs((double)desplazamiento)==1)
						estado = 2;
					else//Si no venimos de un cambio de sentido 'reiniciamos' la búsqueda
						estado = 0;
				}

				//Borramos la imagen auxiliar usada
				delete texto_auxiliar;
			}

			//Dejamos el ángulo que ha máximizado la desviación, ya que el último calculado siempre es uno peor
			angulo += sentido * desplazamiento;

			//Comprobamos si el ángulo es demasiado grande, si no lo es enderezamos el texto
			//Si el ángulo es muy grande hay una alta probabilidad de que la detección sea incorrecta por lo que no rotamos el texto
			if(fabs((double)angulo)<=60)
				Rotacion(linea.palabras[s], angulo);						
		}

		return 0;
	}

	
	//======================================================================
	// NOMBRE: CorregirSlant
	//
	// DESCRIPCIÓN: Corrige el Slnt de los segmentos de todas las líneas de una imagen de texto. (Slant -> 'Inclinación' de las letras respecto al eje vertical)
	//				Sino hemos obtenido antes los segmentos de la imagen, la correción no tendrá efecto alguno.
	//
	// ARGUMENTOS: -
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada
	//======================================================================
	int Texto::CorregirSlant()
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;		

		//Corregimos el Slant de cada una de las líneas de la imagen
		for(int l=0; l<numeroLineas; l++)
			CorregirSlant(lineas[l]);

		return 0;
	}


	//======================================================================
	// NOMBRE: CorregirSlant
	//
	// DESCRIPCIÓN: Corrige el Slant de una línea de una imagen de texto. (Slant -> 'Inclinación' de las letras respecto al eje vertical)
	//				Sino hemos obtenido antes los segmentos de la imagen, la correción no tendrá efecto alguno.
	//
	// ARGUMENTOS: Linea linea	-> Línea sobre la que se aplicará la correción
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//======================================================================
	int Texto::CorregirSlant(Linea linea)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;		
		
		//Realizamos la correción para todos los segmentos de la línea
		for(int s=0; s<linea.numeroPalabras; s++)
		{			
			int angulo = 0;//Ángulo estimado de inclinación del texto
			int desplazamiento = 7;//Incremento inicial a la hora de testear inclinaciones
			int sentido = 1;//Sentido en el que buscamos la inclinación
			BYTE estado = 0;//0 -> inicializamos 'búsqueda'	   1 -> hemos cambiado el sentido en la prueba anterior    2 -> el cambio de sentido ha mejorado el resultado    3 -> hemos encontrado un máximo (posiblemente local :( )
					
			//Reservamos memoria para almacenar la proyección vertical de la imagen
			//Estamos reservando memoria de más (para todas las columnas) pero a cambio nos ahorramos cálculos (ajustes en los índices de los vectores)
			int *proyeccion = new int[bitmapinfoheader->biWidth];
			
			//Variables para guardar la desviación actual y la anterior. Nuestro objetivo es maximizar esta desviación
			double desviacion_antigua;
			double desviacion_nueva;

			//Calculamos la proyección vertical del segmento, así como su desviación asociada
			proyeccionVertical(linea.palabras[s], proyeccion, &desviacion_nueva);

			while(estado!=3 && fabs((double)angulo)<=50)//Establecemos 50 como correción máxima
			{
				//Creamos una imagen auxiliar sobre la que testear la inclinación
				Texto *texto_auxiliar = new Texto();

				//Guardamos la última desviación
				desviacion_antigua = desviacion_nueva;

				//Calculamos el siguiente ángulo a testear
				angulo += sentido * desplazamiento;

				//Copiamos la imagen original en la auxiliar
				(*texto_auxiliar) = (*this);
				
				//Aplicamos Shear al segmento de la imagen auxiliar
				texto_auxiliar->Shear(linea.palabras[s], angulo);			

				//Calculamos la proyección vertical del segmento, así como su desviación asociada
				texto_auxiliar->proyeccionVertical(linea.palabras[s], proyeccion, &desviacion_nueva);

				//Si la desviación ha empeorado (disminuido)
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
				else//La desviación ha mejorado (incrementado)
				{
					if(estado==1 && fabs((double)desplazamiento)==1)
						estado = 2;
					else//Si no venimos de un cambio de sentido 'reiniciamos' la búsqueda
						estado = 0;
				}

				//Borramos la imagen auxiliar usada
				delete texto_auxiliar;
			}

			//Dejamos el ángulo que ha máximizado la desviación, ya que el último calculado siempre es uno peor
			angulo+=sentido * desplazamiento;

			//'Enderezamos' el texto
			Shear(linea.palabras[s], angulo);					
		}

		return 0;
	}
//============================================================================
	// NOMBRE: Interseccion
	//
	// DESCRIPCIÓN: Devuelve la intersección de la imagen actual y la imagen pasada por parámetro. El resultado queda almacenado en la imagen actual. (Ambas de 256 colores y umbralizadas)
	//
	// ARGUMENTOS:	Texto *texto -> Imagen umbralizada con la que realizar la intersección
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Las imágenes no están umbralizada
	//			2 -> Error. Las imágenes no tienen el mismo tamaño
	//============================================================================
	int Texto::Interseccion(Texto *texto)
	{
		//Comprobamos que las imágenes estén umbralizadas
		if(!umbralizada || !texto->umbralizada)
			return 1;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=texto->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=texto->bitmapinfoheader->biWidth)
			return 2;

		//Realizamos la intersección de las imágenes umbralizadas
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				//Si alguno de los píxeles es distinto de 0 (0->true) el resultado es false (1->255)
				if(mapa_bits[posicion(i, j, 1)]!=0 || texto->mapa_bits[posicion(i, j, 1)]!=0)
					mapa_bits[posicion(i, j, 1)] = 255;
			}

		return 0;
	}


	//============================================================================
	// NOMBRE: Interseccion
	//
	// DESCRIPCIÓN: Devuelve la intersección de la imagen actual y la imagen pasada por parámetro. El resultado queda almacenado en la imagen actual. (Ambas de 256 colores y umbralizadas)
	//
	// ARGUMENTOS:	Texto *texto -> Imagen umbralizada con la que realizar la intersección
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Las imágenes no están umbralizada
	//			2 -> Error. Las imágenes no tienen el mismo tamaño
	//============================================================================
	int Texto::InterseccionO(Texto *texto)
	{
		//Comprobamos que las imágenes estén umbralizadas
		if(!umbralizada || !texto->umbralizada)
			return 1;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=texto->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=texto->bitmapinfoheader->biWidth)
			return 2;

		//Realizamos la intersección de las imágenes umbralizadas
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
				{
					for(int i=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.y; i<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.y; i++)
						for(int j=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.x; j<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.x; j++)
						{
							//Si alguno de los píxeles es distinto de 0 (0->true) el resultado es false (1->255)
							if(mapa_bits[posicion(i, j, 1)]!=0 || texto->mapa_bits[posicion(i, j, 1)]!=0)
								mapa_bits[posicion(i, j, 1)] = 255;
						}
				}

		return 0;
	}

	//============================================================================
	// NOMBRE: Diferencia
	//
	// DESCRIPCIÓN: Devuelve la diferencia de la imagen actual y la imagen pasada por parámetro. El resultado queda almacenado en la imagen actual. (Ambas de 256 colores y umbralizadas)
	//
	// ARGUMENTOS:	Texto *texto -> Imagen umbralizada con la que realizar la diferencia
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Las imágenes no están umbralizada
	//			2 -> Error. Las imágenes no tienen el mismo tamaño
	//============================================================================
	int Texto::Diferencia(Texto *texto)
	{
		//Comprobamos que las imágenes estén umbralizadas
		if(!umbralizada || !texto->umbralizada)
			return 1;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=texto->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=texto->bitmapinfoheader->biWidth)
			return 2;

		//Realizamos la diferencia de las imágenes umbralizadas
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			for(int j=0; j<bitmapinfoheader->biWidth; j++)
			{
				//Si el pixel es true (0) en la 2ª imagen forzamos a que el resultado sea false (255), 
				//en otro caso el píxel resultado es igual que el píxel en la imagen original
				if(texto->mapa_bits[posicion(i, j, 1)]==0)
					mapa_bits[posicion(i, j, 1)] = 255;
			}

		return 0;
	}
	
	//============================================================================
	// NOMBRE: DiferenciaO
	//
	// DESCRIPCIÓN: Devuelve la diferencia de la imagen actual y la imagen pasada por parámetro. El resultado queda almacenado en la imagen actual. (Ambas de 256 colores y umbralizadas)
	//
	// ARGUMENTOS:	Texto *texto -> Imagen umbralizada con la que realizar la diferencia
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. Las imágenes no están umbralizada
	//			2 -> Error. Las imágenes no tienen el mismo tamaño
	//============================================================================
	int Texto::DiferenciaO(Texto *texto)
	{
		//Comprobamos que las imágenes estén umbralizadas
		if(!umbralizada || !texto->umbralizada)
			return 1;

		//Comprobamos las dimensiones
		if(bitmapinfoheader->biHeight!=texto->bitmapinfoheader->biHeight || bitmapinfoheader->biWidth!=texto->bitmapinfoheader->biWidth)
			return 2;

		//Realizamos la diferencia de las imágenes umbralizadas
		for(int l=0; l<numeroLineas; l++)
			for(int p=0; p<lineas[l].numeroPalabras; p++)
				for(int c=0; c<lineas[l].palabras[p].numeroCaracteres; c++)
				{
					for(int i=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.y; i<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.y; i++)
						for(int j=lineas[l].palabras[p].caracteres[c].esquina_superior_izquierda.x; j<=lineas[l].palabras[p].caracteres[c].esquina_inferior_derecha.x; j++)
						{
							//Si el pixel es true (0) en la 2ª imagen forzamos a que el resultado sea false (255), 
							//en otro caso el píxel resultado es igual que el píxel en la imagen original
							if(texto->mapa_bits[posicion(i, j, 1)]==0)
								mapa_bits[posicion(i, j, 1)] = 255;
						}
				}

		return 0;
	}


	//============================================================================
	// NOMBRE: Erosion
	//
	// DESCRIPCIÓN: Obtiene la erosion de la imagen actual respecto a un determinado elemento estructural. El resultado queda almacenado en la imagen actual. (Imagen de 256 colores y umbralizada)
	//
	// ARGUMENTOS:	ElementoEstructural elemento -> Elemento estructural usado en la erosión
	//				bool imagen_complementaria	 -> Indica si debemos tratar la imagen como si fuera la complementaria (esto es para ahorrarnos la creación de una imagen complementaria, ahorrando tiempo de CPU)
	//				bool elemento_complementario -> Indica si debemos tratar el elemento estructural como si fuera el complementaria (esto es para ahorrarnos la creación de un elemento complementario, ahorrando tiempo de CPU)
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//============================================================================
	int Texto::Erosion(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;
	
		//Calculamos el tamaño de la paleta
		int tamano_paleta = 0;
		if(bitmapinfoheader->biBitCount<=8)
		{
			if(bitmapinfoheader->biClrUsed!=0)
				tamano_paleta = bitmapinfoheader->biClrUsed;
			else
				tamano_paleta = (int)pow(2.0, bitmapinfoheader->biBitCount);
		}

		bool contenidos;//Indica si los píxeles del elemento estructural están incluídos en la imagen original

		//Creamos una imagen auxiliar en la que iremos guardando el resultado
		Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight, bitmapinfoheader->biBitCount, paleta, tamano_paleta);
		
		//Quitamos la primera columna y fila para ahorranos las comprobaciones de existencia de píxeles al aplicar la máscara (se perdia mucho tiempo de CPU tontamente)
		for(int i=1; i<bitmapinfoheader->biHeight - 1; i++)
			for(int j=1; j<bitmapinfoheader->biWidth - 1; j++)
			{
				//En principio suponemos que los píxeles del elemento estructural están contenidos en la imagen original
				contenidos=true;

				//Comprobamos si el elemento estructural está contenido en la imagen original
				for(int k=0;k<3 && contenidos;k++)
					for(int l=0;l<3 && contenidos;l++)
					{
						//Guardamos la posición para ahorrarnos cálculos
						int posicion_imagen = posicion(i - elemento.centro.y + k, j - elemento.centro.x + l, 1);
						
						//Obtenemos el píxel del elemento estructural (true -> 0 false -> 1 (255))
						bool pixel_elemento = (elemento.matriz[k][l]!=-1) && (((elemento.matriz[k][l]==1) && (!elemento_complementario)) || ((elemento.matriz[k][l]==0) && (elemento_complementario))) ;
						
						//Obtenemos el píxel de la imagen (true -> 0 false -> 1 (255))
						bool pixel_imagen = ((mapa_bits[posicion_imagen]==0) && (!imagen_complementaria)) || ((mapa_bits[posicion_imagen]==255) && (imagen_complementaria));
						
						//Si tenemos un píxel negro en el elemento y uno blanco en la imagen, el elemento no estará contenido en ella	
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
		//Reasignación punteos
		inicio_DIB = imagen_auxiliar->inicio_DIB;
		bitmapinfoheader = (LPBITMAPINFOHEADER)inicio_DIB;
		bitmapinfo = (LPBITMAPINFO)bitmapinfoheader;
		if(imagen_auxiliar->paleta!=NULL)
			paleta = (RGBQUAD*)((BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER));
		else//No hay paleta
			paleta = NULL;
		mapa_bits=(BYTE*)inicio_DIB + sizeof(BITMAPINFOHEADER) + tamano_paleta * sizeof(RGBQUAD);
		//Fin reasignación punteros
		
		return 0;
	}

	
	//NO LA COMENTO AÚN PORQUE ES PROVISIONAL
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

			//Reasignación punteos
			inicio_DIB=imagen_auxiliar->inicio_DIB;
			bitmapinfoheader=(LPBITMAPINFOHEADER)inicio_DIB;
			bitmapinfo=(LPBITMAPINFO)bitmapinfoheader;
			if(imagen_auxiliar->paleta!=NULL)
				paleta=(RGBQUAD*)((BYTE*)inicio_DIB+sizeof(BITMAPINFOHEADER));
			else
				paleta=NULL;
			mapa_bits=(BYTE*)inicio_DIB+sizeof(BITMAPINFOHEADER)+tamano_paleta*sizeof(RGBQUAD);
			//Fin reasignación punteros
			
			return 0;
	}	

	//============================================================================
	// NOMBRE: TransformaciónAlAzar
	//
	// DESCRIPCIÓN: Obtiene la transformación al azar de la imagen actual respecto a un determinado elemento estructural. El resultado queda almacenado en la imagen actual. (Imagen 256 colores y umbralizada)
	//
	// ARGUMENTOS:	ElementoEstructural elemento -> Elemento estructural usado en la transformación al azar
	//				bool imagen_complementaria	 -> Indica si debemos tratar la imagen como si fuera la complementaria (esto es para ahorrarnos la creación de una imagen complementaria, ahorrando tiempo de CPU)
	//				bool elemento_complementario -> Indica si debemos tratar el elemento estructural como si fuera el complementaria (esto es para ahorrarnos la creación de un elemento complementario, ahorrando tiempo de CPU)
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//============================================================================
	int Texto::TransformacionAlAzar(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Creamos una imagen auxiliar necesaria para hacer la erosión d el aimagen complementaria y elemento complementario
		Texto *imagen_auxiliar = new Texto();

		//Copiamos la imagen actual en la auxiliar
		(*imagen_auxiliar) = (*this);

		//Erosón imagen y elemento originales
		Erosion(elemento, imagen_complementaria, elemento_complementario);

		//Erosión imagen y elemento complementarios
		imagen_auxiliar->Erosion(elemento, !imagen_complementaria, !elemento_complementario);

		//Calculamos la intersección de las dos imágenes erosionadas para obtener el resultado final de la transformación
		Interseccion(imagen_auxiliar);

		//Borramos la imagen auxiliar
		delete imagen_auxiliar;

		return 0;
	}

	//============================================================================
	// NOMBRE: TransformaciónAlAzarO
	//
	// DESCRIPCIÓN: Obtiene la transformación al azar de la imagen actual respecto a un determinado elemento estructural. El resultado queda almacenado en la imagen actual. (Imagen 256 colores y umbralizada)
	//
	// ARGUMENTOS:	ElementoEstructural elemento -> Elemento estructural usado en la transformación al azar
	//				bool imagen_complementaria	 -> Indica si debemos tratar la imagen como si fuera la complementaria (esto es para ahorrarnos la creación de una imagen complementaria, ahorrando tiempo de CPU)
	//				bool elemento_complementario -> Indica si debemos tratar el elemento estructural como si fuera el complementaria (esto es para ahorrarnos la creación de un elemento complementario, ahorrando tiempo de CPU)
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//============================================================================
	int Texto::TransformacionAlAzarO(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Creamos una imagen auxiliar necesaria para hacer la erosión d el aimagen complementaria y elemento complementario
		Texto *imagen_auxiliar = new Texto();

		//Copiamos la imagen actual en la auxiliar
		(*imagen_auxiliar) = (*this);

		//Erosón imagen y elemento originales
		ErosionO(elemento, imagen_complementaria, elemento_complementario);

		//Erosión imagen y elemento complementarios
		imagen_auxiliar->ErosionO(elemento, !imagen_complementaria, !elemento_complementario);

		//Calculamos la intersección de las dos imágenes erosionadas para obtener el resultado final de la transformación
		InterseccionO(imagen_auxiliar);

		//Borramos la imagen auxiliar
		delete imagen_auxiliar;

		return 0;
	}

	
	//============================================================================
	// NOMBRE: Reducción
	//
	// DESCRIPCIÓN: Obtiene la reducción de la imagen actual respecto a un determinado elemento estructural, es decir, el esqueleto de la imagen. El resultado queda almacenado en la imagen actual. (Imagen 256 colores y umbralizada)
	//
	// ARGUMENTOS:	ElementoEstructural elemento -> Elementos estructurales usados en la reducción (pasamos todos para no perder tiempo de CPU calculándolos)
	//				bool imagen_complementaria	 -> Indica si debemos tratar la imagen como si fuera la complementaria (esto es para ahorrarnos la creación de una imagen complementaria, ahorrando tiempo de CPU)
	//				bool elemento_complementario -> Indica si debemos tratar el elemento estructural como si fuera el complementaria (esto es para ahorrarnos la creación de un elemento complementario, ahorrando tiempo de CPU)
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada			
	//============================================================================
	int Texto::Reduccion(ElementoEstructural *elemento, bool imagen_complementaria, bool elemento_complementario)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Creamos las dos imágenes auxiliares necesarias
		Texto *original = new Texto(); 
		Texto *transf = new Texto();

		//Inicializamos las imágenes auxiliares
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

			//Si la imagen no ha cambiado en la última iteración hemos encontrado el esqueleto de la imagen
			if(EsIgual(original))
				fin = true;
			else
			{
				//Borramos el resultado de la iteración anterior
				delete original;

				//Guardamos el resultado de esta iteración
				original = new Texto();
				(*original) = (*this);
			}			
		}

		//Borramos las imágenes auxiliares
		delete transf;
		delete original;

		return 0;
	}


	//NO ESTOY SEGURO DE SI FUNCIONARÁ BIEN EL CODIGO PARA NUESTRO CASO, POR AHORA NO LO IMPLEMENTO
	//============================================================================
	// NOMBRE: CodigoCadena
	//
	// DESCRIPCIÓN: Obtiene la reducción de la imagen actual respecto a un determinado elemento estructural, es decir, el esqueleto de la imagen. El resultado queda almacenado en la imagen actual. (Imagen 256 colores y umbralizada)
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. El carácter no pertenece a la imagen actual
	//============================================================================
	int Texto::CodigoCadena(Caracter *caracter)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el carácter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		return 0;
	}


	//============================================================================
	// NOMBRE: NumeroHuecos
	//
	// DESCRIPCIÓN: Obtiene el número de huecos del carácter pasado por parámetro dejando el valor en dicha estructura (en el campo numeroHuecos)
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. El carácter no pertenece a la imagen actual
	//============================================================================
	int Texto::NumeroHuecos(Caracter *caracter)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el carácter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Creamos un caracter auxiliar, ya que debemos modificarlo para hallar los huecos
		Texto *caracter_auxiliar=new Texto(caracter->esquina_inferior_derecha.x-caracter->esquina_superior_izquierda.x+1, caracter->esquina_inferior_derecha.y-caracter->esquina_superior_izquierda.y+1, 8, paleta, 256);

		//Indicamos que el carácter está umbralizado
		caracter_auxiliar->umbralizada = true;

		//Copiamos el carácter original en el auxiliar
		for(int i=0; i<=caracter->esquina_inferior_derecha.y-caracter->esquina_superior_izquierda.y;i++)
			memcpy(&caracter_auxiliar->mapa_bits[caracter_auxiliar->posicion(i,0,1)], &mapa_bits[posicion(caracter->esquina_superior_izquierda.y + i, caracter->esquina_superior_izquierda.x, 1)], caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x + 1);
				
		
		//Rellenamos el exterior del carácter de píxeles negros
		//Para ello debemos poner semillas en las 4 esquinas del carácter, siempre que éstas no sean ya un píxel negro
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

		//Inicializamos los huecos del carácter a 0
		int huecos = 0;

		//Calculamos los huecos buscando un píxel blanco
		for(int i=0; i<caracter_auxiliar->bitmapinfoheader->biHeight; i++)
			for(int j=0; j<caracter_auxiliar->bitmapinfoheader->biWidth; j++)
			{
				if(caracter_auxiliar->mapa_bits[caracter_auxiliar->posicion(i, j, 1)]!=0)
				{
					//Hemos encontrado un hueco
					huecos++;
					
					//Rellenamos el hueco encontrado para no contabilizarlo más de una vez
					//Para ello establecemo sel píxel blanco encontrado como semilla
					semilla.y = i;
					semilla.x = j;
					caracter_auxiliar->rellenarHuecos(semilla);
				}
			}

		//Asociamos el número de huecos con el carácter
		caracter->numeroHuecos = huecos;

		//Borramos el carácter auxiliar
		delete caracter_auxiliar;

		return 0;
	}

	
	//============================================================================
	// NOMBRE: NumeroCruces
	//
	// DESCRIPCIÓN: Obtiene el número de cruces del carácter pasado por parámetro dejando el valor en dicha estructura (en el campo numeroCruces)
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. El carácter no pertenece a la imagen actual
	//============================================================================
	int Texto::NumeroCruces(Caracter *caracter)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el carácter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos los cruces del carácter a 0
		int cruces = 0;

		//Calculamos los cruces buscando un píxel '8-conectado' con 3 o más píxeles
		for(int i=caracter->esquina_superior_izquierda.y+1; i<caracter->esquina_inferior_derecha.y; i++)
			for(int j=caracter->esquina_superior_izquierda.x; j<caracter->esquina_inferior_derecha.x; j++)
			{
				if(conectividad(i,j)>=3)
				{
					//Hemos encontrado un cruce
					cruces++;
				}
			}

		//Asociamos el número de cruces con el carácter
		caracter->numeroCruces = cruces;
		
        return 0;
	}


	//============================================================================
	// NOMBRE: PuntosTerminales
	//
	// DESCRIPCIÓN: Obtiene el número de puntos terminales del carácter pasado por parámetro dejando el valor en dicha estructura (en el campo numeroPuntosTerminales)
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. El carácter no pertenece a la imagen actual
	//============================================================================
	int Texto::PuntosTerminales(Caracter *caracter)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el carácter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos los puntos terminales del carácter a 0
		int puntos = 0;

		//Calculamos los puntos terminales buscando un píxel que sea punto final
		for(int i=caracter->esquina_superior_izquierda.y+1; i<caracter->esquina_inferior_derecha.y; i++)
			for(int j=caracter->esquina_superior_izquierda.x; j<caracter->esquina_inferior_derecha.x; j++)
			{
				if(mapa_bits[posicion(i, j, 1)]==0 && esPuntoFinal(i, j))
				{
					//Hemos encontrado un punto final
					puntos++;
				}
			}

		//Asociamos el número de puntos terminales con el carácter
		caracter->numeroPuntosTerminales = puntos;
		
        return 0;
	}


	//============================================================================
	// NOMBRE: CalcularCaracteristicas
	//
	// DESCRIPCIÓN: Calcula diversas características del carácter pasado por parámetro dejando los valores en dicha estructura. Estas características se obtendrán para 9 secciones idénticas del carácter.
	//
	// ARGUMENTOS:	Caracter *caracter -> Puntero a un caracter de la imagen
	//
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. El carácter no pertenece a la imagen actual
	//============================================================================
	int Texto::CalcularCaracteristicas(Caracter *caracter)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el carácter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos a 0 el número de píxeles del carácter
		caracter->numeroPixeles = 0;		

		//'Creamos' las secciones del carácter
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

			//Inicializamos características
			for(int c=0;c<12;c++)
			{
				caracter->caracteristicasDireccionalesB[s][c] = 0;
				caracter->caracteristicasDireccionalesR[s][c] = 0;
				caracter->caracteristicasEstructurales[s][c] = 0;
				
				if(c<8)
					caracter->caracteristicasConcavidad[s][c] = 0;
			}
		}

		//Creamos una matriz en la que guardaremos el 'gradiente' del carácter (realmente guardaremos el intervalo al que pertenece de entre 12)
		BYTE **gradiente = new BYTE*[caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y + 1];
		for(int i=0; i<caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y + 1; i++)
			gradiente[i] = new BYTE[caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x + 1];

		//Calculamos el gradiente del carácter
		Sobel(caracter, gradiente);

		//Calculamos las dimensiones de una sección
		int tamano_seccion_horizontal = (caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x + 1) / 3;
		int tamano_seccion_vertical = (caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y + 1) / 3;
		int tamano_seccion = tamano_seccion_vertical * tamano_seccion_horizontal;
				
		//Recorremos el gradiente (y el caracter)
		for(int i=0; i<=caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y; i++)
			for(int j=0; j<=caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x; j++)
			{
				//Calculamos en que sección estamos				
				//Primero hallamos la fila y columna
				int f, c;
				
				//Fila
				if(tamano_seccion_vertical==0)
					f = 0;
				else
					f = (i - 2) / tamano_seccion_vertical;//El -2 es para que en las últimas filas del carácter salga la fila 3(2) y no la 4(3) y
				if(f<0)//Evitamos valores negativos
					f = 0;

				//Columna
				if(tamano_seccion_horizontal==0)
					c = 0;
				else
					c = (j - 2) / tamano_seccion_horizontal;//El -2 es para que en las últimas columnas del carácter salga la sección 3(2) y no la 4(3)
				if(c<0)//Evitamos valores negativos
					c = 0;
				
				//Obtenemos la sección
				int seccion = f * 3 + c;

				//Calculamos la fila y columna reales en la imagen
				int fila_real = i + caracter->esquina_superior_izquierda.y;
				int columna_real = j + caracter->esquina_superior_izquierda.x;

				//Incrementamos en 1 el intervalo del gradiente correspondiente en la sección actual
				caracter->caracteristicasDireccionalesB[seccion][gradiente[i][j]]++;

				//Si el píxel actual es negro
				if(mapa_bits[posicion(fila_real, columna_real, 1)]==0)
				{
					caracter->caracteristicasConcavidad[seccion][0]++;//Pixeles negros de la sección

					//Calculamos la características estructurales para todos los píxeles menos primera fila y columna y última fila y columna
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

		//Calculamos las características de concavidad y actualizamos las características que son porcentajes	
		for(int s=0; s<9; s++)
		{
			//Caracteristicas concavidad
			int pixeles_negros_seccion = caracter->caracteristicasConcavidad[s][0];

			//Actualizamos el número de píxeles del carácter
			caracter->numeroPixeles += pixeles_negros_seccion;

			//Hacemos la comprobación para evitar divisiones por 0
			if(pixeles_negros_seccion>0)
			{
				double porcentaje;
				//Calculamos el porcentaje de pixeles con la regla 1 ó 2 en la región
				porcentaje = ( ((caracter->caracteristicasEstructurales[s][0]*100)/pixeles_negros_seccion) + ((caracter->caracteristicasEstructurales[s][1]*100)/pixeles_negros_seccion) ) / 2;
				caracter->caracteristicasConcavidad[s][1] = (int)porcentaje;

				//Calculamos el porcentaje de pixeles con la regla 3 ó 4 en la región
				porcentaje = ( ((caracter->caracteristicasEstructurales[s][2]*100)/pixeles_negros_seccion) + ((caracter->caracteristicasEstructurales[s][3]*100)/pixeles_negros_seccion) ) / 2;
				caracter->caracteristicasConcavidad[s][2] = (int)porcentaje;

				//Calculamos el porcentaje de pixeles con la regla 5 ó 6 en la región
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
				//Hacemos la comprobación para evitar divisiones por 0
				if(tamano_seccion!=0)
					caracter->caracteristicasDireccionalesR[s][c] = (caracter->caracteristicasDireccionalesB[s][c]*100)/tamano_seccion;

				//Hacemos la comprobación porque se ve que a veces se pasa del 100% debido a las distintas aproximaciones realizadas
				//COMPROBAR SI REALMENTE ES POR ESO O SI HAY ALGO MAL
				if(caracter->caracteristicasDireccionalesR[s][c]>100)
					caracter->caracteristicasDireccionalesR[s][c] = 100;
				
				//Hacemos la comprobación para evitar divisiones por 0
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
	// DESCRIPCIÓN: Establece el número de vecinos en caso de usar el clasificador KNN.
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
	// DESCRIPCIÓN: Establece el número de vecinos en caso de usar el clasificador KNN.
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
	// DESCRIPCIÓN: Inicializa el clasificador indicado por parámetro.
	//
	// ARGUMENTOS:	int clasificador -> Tipo de clasificador que queremos construir:
	//										0 -> KNN
	//										1 -> Naive Bayes
	//							
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. No existe el clasificador seleccionado	
	//			2 -> Error. Parámetro incorrecto para el clasificador seleccionado	
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
				//Comprobamos que los parámetros sean correctos
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
	// DESCRIPCIÓN: Realiza el reconocimiento o aprendizaje de untexto dependiendo del modo establecido. En el caso de reconocimiento usaremos un determinado clasificador.
	//
	// ARGUMENTOS: int modo -> Indica si estamos en aprendizaje (0) o reconocimiento (>0), habiendo en este último caso varios tipos(POR AHORA AÚN NO ESTáN IMPLEMENTADOS)
	//							
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. Estamos en reconocimiento y no hay un clasificador seleccionado
	//			3 -> Error. Estamos en aprendizaje y el texto reconocido no coincide con la segmentación
	//			4 -> Error. No se ha podido abrir o cerrar la BBDD
	//============================================================================
	int Texto::Reconocer(int modo)
	{
		//Comprobamos que la imagen esté umbralizada
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
	// DESCRIPCIÓN: Realiza el reconocimiento o aprendizaje de una palabra dependiendo del modo establecido. En el caso de reconocimiento usaremos un determinado clasificador.
	//
	// ARGUMENTOS:	Palabra *palabra -> Puntero a una palabra de la imagen
	//				int modo		 -> Indica si estamos en aprendizaje (0) o reconocimiento (>0), habiendo en este último caso varios tipos(POR AHORA AÚN NO ESTáN IMPLEMENTADOS)
	//							
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. La palabra no pertenece a la imagen actual
	//			3 -> Error. Estamos en reconocimiento y no hay un clasificador seleccionado
	//============================================================================
	int Texto::ReconocerPalabra(Palabra *palabra, int modo)
	{
		//Comprobamos que la imagen esté umbralizada
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
					if(!mejora_global)//no ha mejorado la fusión y no hay que tocar nada
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
		//Comprobamos que la fusión sea válida
		if(linea1!=linea2 - 1 || linea1>=numeroLineas - 1 || linea1<0 ||linea2<0 || numeroLineas<=0)
			return 1;

		//PONER LO DE ESPACIADO COMO DEFINE!!!!!
		int espaciado = 15;//lineas[0].esquina_superior_izquierda.y;		
		
			//Redistribuimos las líneas
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
		
		//Creamos una nueva imagen con el tamaño necesario para guardar todas las líneas halladas con una separación interlínea de 'espaciado' píxeles
		Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight - espaciado, 8, paleta, 256); 
		
		//Creamos la estructura de líneas de la imagen segmentada
		imagen_auxiliar->lineas = lineas_auxiliares;
		imagen_auxiliar->numeroLineas = numeroLineas - 1;

		//Copiamos las líneas de la imagen original en la imagen segmentada
		int tamano_pixel = bitmapinfoheader->biBitCount / 8;

		//Calculamos las columnas de relleno
		int columnas_relleno = 0;
		if((bitmapinfoheader->biWidth*tamano_pixel) % 4!=0)
			columnas_relleno = 4 - ((bitmapinfoheader->biWidth * tamano_pixel) % 4);

		//Copiamos las líneas de la imagen original en la imagen segmentada
		for(int l=0; l<linea1; l++)
		{
			//Asignamos los atributos de una línea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			/*//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado * (l + 1);
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y - inicio + espaciado * (l + 1);	*/			

			//Pintamos de negro la fila inferior y superior de la línea en la imagen segmentada
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la línea de la imagen original en la imagen segmentada
			memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[l].esquina_inferior_derecha.y, 0, 1)], (lineas[l].esquina_inferior_derecha.y - lineas[l].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));
		}

		imagen_auxiliar->lineas[linea1].palabras = NULL;
		imagen_auxiliar->lineas[linea1].numeroPalabras = 0;
		//Copiamos la línea de la imagen original en la imagen segmentada
		memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea1].esquina_superior_izquierda.y + (lineas[linea1].esquina_inferior_derecha.y - lineas[linea1].esquina_superior_izquierda.y + 1), 0, 1)], &mapa_bits[posicion(lineas[linea1].esquina_inferior_derecha.y, 0, 1)], (lineas[linea1].esquina_inferior_derecha.y - lineas[linea1].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));

		//Copiamos la línea de la imagen original en la imagen segmentada
		memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea1].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[linea2].esquina_inferior_derecha.y, 0, 1)], (lineas[linea2].esquina_inferior_derecha.y - lineas[linea2].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));

		//Pintamos de negro la fila inferior y superior de la línea en la imagen segmentada
		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea1].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea1].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
		
		for(int l=linea2; l<numeroLineas-1; l++)
		{
			//Asignamos los atributos de una línea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			/*//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado * (l + 1);
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y - inicio + espaciado * (l + 1);	*/			

			//Pintamos de negro la fila inferior y superior de la línea en la imagen segmentada
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la línea de la imagen original en la imagen segmentada
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
		//Fin reasignación de punteros

		return 0;		
	}


	int Texto::Dividir(int linea)
	{
		//Comprobamos que la fusión sea válida
		if(linea>=numeroLineas || linea<0 || numeroLineas<=0)
			return 1;

		//PONER LO DE ESPACIADO COMO DEFINE!!!!!
		int espaciado = 15;//lineas[0].esquina_superior_izquierda.y;		
		
		//Redistribuimos las líneas
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
		
		//Creamos una nueva imagen con el tamaño necesario para guardar todas las líneas halladas con una separación interlínea de 'espaciado' píxeles
		Texto *imagen_auxiliar = new Texto(bitmapinfoheader->biWidth, bitmapinfoheader->biHeight + espaciado, 8, paleta, 256); 
		
		//Creamos la estructura de líneas de la imagen segmentada
		imagen_auxiliar->lineas = lineas_auxiliares;
		imagen_auxiliar->numeroLineas = numeroLineas + 1;

		//Copiamos las líneas de la imagen original en la imagen segmentada
		int tamano_pixel = bitmapinfoheader->biBitCount / 8;

		//Calculamos las columnas de relleno
		int columnas_relleno = 0;
		if((bitmapinfoheader->biWidth*tamano_pixel) % 4!=0)
			columnas_relleno = 4 - ((bitmapinfoheader->biWidth * tamano_pixel) % 4);

		//Copiamos las líneas de la imagen original en la imagen segmentada
		for(int l=0; l<linea; l++)
		{
			//Asignamos los atributos de una línea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			/*//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado * (l + 1);
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y - inicio + espaciado * (l + 1);	*/			

			//Pintamos de negro la fila inferior y superior de la línea en la imagen segmentada
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la línea de la imagen original en la imagen segmentada
			memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[l].esquina_inferior_derecha.y, 0, 1)], (lineas[l].esquina_inferior_derecha.y - lineas[l].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));
		}

		//Copiamos la línea de la imagen original en la imagen segmentada
		imagen_auxiliar->lineas[linea].palabras = NULL;
		imagen_auxiliar->lineas[linea].numeroPalabras = 0;
		memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y, 0, 1)], (imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y - imagen_auxiliar->lineas[linea].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));

		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);

		//Copiamos la línea de la imagen original en la imagen segmentada
		imagen_auxiliar->lineas[linea + 1].palabras = NULL;
		imagen_auxiliar->lineas[linea + 1].numeroPalabras = 0;
		memcpy(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea + 1].esquina_inferior_derecha.y, 0, 1)], &mapa_bits[posicion(lineas[linea].esquina_inferior_derecha.y, 0, 1)], (imagen_auxiliar->lineas[linea].esquina_inferior_derecha.y - imagen_auxiliar->lineas[linea].esquina_superior_izquierda.y + 1) * (bitmapinfoheader->biWidth + columnas_relleno));

		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea + 1].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
		memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[linea + 1].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);


		for(int l=linea + 2; l<numeroLineas+1; l++)
		{
			//Asignamos los atributos de una línea
			//Palabras
			imagen_auxiliar->lineas[l].palabras = NULL;
			imagen_auxiliar->lineas[l].numeroPalabras = 0;
			/*//Esquina superior izquierda
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.x = lineas[l].esquina_superior_izquierda.x;
			imagen_auxiliar->lineas[l].esquina_superior_izquierda.y = lineas[l].esquina_superior_izquierda.y - inicio + espaciado * (l + 1);
			//Esquina inferior derecha
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.x = lineas[l].esquina_inferior_derecha.x;
			imagen_auxiliar->lineas[l].esquina_inferior_derecha.y = lineas[l].esquina_inferior_derecha.y - inicio + espaciado * (l + 1);	*/			

			//Pintamos de negro la fila inferior y superior de la línea en la imagen segmentada
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_superior_izquierda.y - 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			memset(&imagen_auxiliar->mapa_bits[imagen_auxiliar->posicion(imagen_auxiliar->lineas[l].esquina_inferior_derecha.y + 3, 0, 1)], 0, imagen_auxiliar->bitmapinfoheader->biWidth);
			
			//Copiamos la línea de la imagen original en la imagen segmentada
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
		//Fin reasignación de punteros

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
		//Comprobamos que la imagen esté segmentada en caracteres
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

				//Comprobación de textoReconocido 'correcto' (falta al menos una palabra corregida)
				if(inicio_palabra == k)
					return 3;
				
				//Si no es la última palabra
				if(p!=lineas[l].numeroPalabras - 1)
				{
					//Comprobación de textoReconocido 'correcto' (que no haya más palabras en una linea reconocidas que corregidas)
					if(textoReconocido[k]!=' ')
						return 3;

					k++;
				}
			}

			//Si no es la última línea
			if(l!=numeroLineas - 1)
			{
				//Saltamos los posibles espacios del final
				while(textoReconocido[k]==' ')
					k++;

				//Comprobación de textoReconocido 'correcto' (que no haya más líneas reconocidas que corregidas)
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

		//Comprobación de textoReconocido 'correcto' (que no haya más líneas corregidas que reconocidas)
		if(textoReconocido[k]!='\0' && textoReconocido[k]!=10)
			return 3;

		//Hacemos la corrección
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
	// DESCRIPCIÓN: Calcula una posición dentro del mapa de bits.
	//
	// ARGUMENTOS:	int fila			-> Componente fila de la posición
	//				int columna			-> Componente columna de la posición
	//				int tamano_elemento -> Tamaño en bytes de un píxel
	//				
	// SALIDA: Un valor entero que indica la posición del mapa de bits pedida
	//======================================================================
	int Texto::posicion(int fila, int columna, int tamano_elemento)
	{
		//Calculamos el tamaño de una fila
		int tamano_fila = bitmapinfoheader->biWidth * tamano_elemento;

		//Calculamos las columnas de relleno necesarias
		int columnas_relleno = 0;
		if(tamano_fila%4!=0)
			columnas_relleno = 4 - (tamano_fila % 4);

		//Devolvemos la posición del píxel en el array de bytes
		return (((bitmapinfoheader->biHeight - 1) - fila) * (tamano_fila + columnas_relleno)) + columna * tamano_elemento;
	}


	//======================================================================
	// NOMBRE: RLSA
	//
	// DESCRIPCIÓN: Aplica el algoritmo RLSA (suavizado por longitud de píxeles horizontales) a la imagen con el umbral pasado por parámetro.
	//
	// ARGUMENTOS:	int umbral -> Número máximo de píxeles de fondo entre dos píxeles imagen para que el tramo se convierta todo en píxeles imagen
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//======================================================================
	int Texto::RLSA(int umbral)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Variables necesarias
		BYTE *inicio_tramo;//Puntero al comienzo de tramo (dirección del píxel de imagen anterior a unposible tramo de fondo)
		int pixeles_tramo;//Número de píxeles del tramo de fondo
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
					//Hemos encontrado el un posible inicio de tramo (sino pixeles_tramo será 0, con lo que no hay problema)
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
							//Si el número de píxeles del tramo es menor o igual que el umbral convertimos el tramo d efondo en píxeles imagen
							//Observar que en el caso de que realmente no hubiera un tramo de fondo, pixeles_tramo será 0, con lo que no cambiamos el valor de ningún píxel
							if(pixeles_tramo<=umbral)
								memset(inicio_tramo + 1, 0, pixeles_tramo);

							//Además de ser el fin de un tramo podemos estar ante el comienzo de otro
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
	// DESCRIPCIÓN: Aplica el algoritmo RLSA (suavizado por longitud de píxeles VERTICALES) a la imagen con el umbral pasado por parámetro.
	//
	// ARGUMENTOS:	int umbral -> Número máximo de píxeles de fondo entre dos píxeles imagen para que el tramo se convierta todo en píxeles imagen
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//======================================================================
	int Texto::RLSAVertical(Linea palabra, int umbral)
	{
		//Comprobamos que la imagen esté umbralizada
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
	// DESCRIPCIÓN: Obtiene la proyección horizontal de la imagen guardándola en el parámetro proyeccion.
	//
	// ARGUMENTOS:	int *proyeccion -> Vector donde guardaremos la proyección de la imagen. Debe reservarse la memoria con anterioridad o dará error.
	//								   Esto se ha hecho así para que sea más fácil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//								   aquí probablemente olvidariamos liberar la memoria con frecuencia.
	//								   El tamaño del vector debe ser el número de filas de la imagen.
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//======================================================================
	int Texto::proyeccionHorizontal(int *proyeccion)
	{
		//Comprobamos que la imagen esté umbralizada
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
					//Hemos encontrado un píxel negro en la fila i
					proyeccion[i]++; 					
				}
			}
		} 		
		
		return 0;
	}


	//======================================================================
	// NOMBRE: proyeccionHorizontal
	//
	// DESCRIPCIÓN: Obtiene la proyección horizontal de la imagen guardándola en el parámetro proyeccion, así como un valor proporcional a la desviacion que guardaremos en el parámetro pseudodesviacion.
	//
	// ARGUMENTOS:	int *proyeccion			 -> Vector donde guardaremos la proyección de la imagen. Debe reservarse la memoria con anterioridad o dará error.
	//											Esto se ha hecho así para que sea más fácil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//											aquí probablemente olvidariamos liberar la memoria con frecuencia.
	//											El tamaño del vector debe ser el número de filas de la imagen.
	//				double *pseudodesviacion -> En este parámetro por referencia devolvemos un valor proporcional a la desviación de la proyección
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//======================================================================
	int Texto::proyeccionHorizontal(int *proyeccion, double *pseudodesviacion)
	{
		//Comprobamos que la imagen esté umbralizada
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
					//Hemos encontrado un píxel negro en la fila i
					media++;
					
					proyeccion[i]++; 					
				}
			}
		}

		//Calculamos la media
		media /= bitmapinfoheader->biHeight;

		//Calculamos el valor proporcional a la desviación para ahorrarnos los cuadrados y las raíces
		for(int i=0; i<bitmapinfoheader->biHeight; i++)
			(*pseudodesviacion) += fabs((double)proyeccion[i] - media);

		//'Normalizamos' la pseudodesviacion con el número de filas de la imagen
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
	// DESCRIPCIÓN: Obtiene la proyección horizontal de una palabra de la imagen guardándola en el parámetro proyeccion, así como el máximo de la misma que guardaremos en el parámetro maximo.
	// 
	// ARGUMENTOS: Palabra segmento -> Palabra de la que queremos calcular la proyección
	//			   int *proyeccion	-> Vector donde guardaremos la proyección de la imagen. Debe reservarse la memoria con anterioridad o dará error.
	//								   Esto se ha hecho así para que sea más fácil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//								   aquí probablemente olvidariamos liberar la memoria con frecuencia.
	//								   El tamaño del vector debe ser el número de filas de la imagen (aunque gastemos más memoria tontamente es por ahorrarnos los cálculos de los índices).
	//				double *maximo  -> En este parámetro por referencia devolvemos el valor máximo de la proyección
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. La palabra no pertenece a la imagen actual
	//============================================================================
	int Texto::proyeccionHorizontal(Palabra segmento, int *proyeccion, double *maximo)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que la palabra pertenece a la imagen
		if(segmento.esquina_superior_izquierda.x<0 || segmento.esquina_superior_izquierda.y<0 || segmento.esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || segmento.esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos a 0 la media y el máximo
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
					//Hemos encontrado un píxel negro en la fila i
					proyeccion[i]++; 					
				}
			}

			//Actualizamos el valor máximo de la proyección si es necesario
			if(proyeccion[i]>(*maximo))
				(*maximo)=proyeccion[i];
		}

		return 0;
	}


	//======================================================================
	// NOMBRE: proyeccionVertical
	//
	// DESCRIPCIÓN: Obtiene la proyección vertical de una palabra de la imagen guardándola en el parámetro proyeccion.
	// 
	// ARGUMENTOS: Palabra palabra -> Palabra de la que queremos calcular la proyección
	//			   int *proyeccion -> Vector donde guardaremos la proyección de la imagen. Debe reservarse la memoria con anterioridad o dará error.
	//								  Esto se ha hecho así para que sea más fácil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//								  aquí probablemente olvidariamos liberar la memoria con frecuencia.
	//								  El tamaño del vector debe ser el número de columnas de la imagen (aunque gastemos más memoria tontamente es por ahorrarnos los cálculos de los índices).
	//				
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. La palabra no pertenece a la imagen actual
	//============================================================================
	int Texto::proyeccionVertical(Palabra palabra, int *proyeccion)
	{
		//Comprobamos que la imagen esté umbralizada
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
					//Hemos encontrado un píxel negro en la columna j
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
	// DESCRIPCIÓN: Obtiene la proyección vertical de una palabra de la imagen guardándola en el parámetro proyeccion, así como un valor proporcional a la desviacion que guardaremos en el parámetro pseudodesviacion.
	//
	// ARGUMENTOS: Palabra palabra			-> Palabra de la que queremos calcular la proyección
	//			   int *proyeccion			-> Vector donde guardaremos la proyección de la imagen. Debe reservarse la memoria con anterioridad o dará error.
	//										   Esto se ha hecho así para que sea más fácil ver que luego hay que borrarla para ahorrar memoria, si reservaramos la memoria 
	//										   aquí probablemente olvidariamos liberar la memoria con frecuencia.
	//										   El tamaño del vector debe ser el número de columnas de la imagen (aunque gastemos más memoria tontamente es por ahorrarnos los cálculos de los índices).
	//			   double *pseudodesviacion -> En este parámetro por referencia devolvemos un valor proporcional a la desviación de la proyección
	// 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. La palabra no pertenece a la imagen actual
	//============================================================================
	int Texto::proyeccionVertical(Palabra segmento, int *proyeccion, double *pseudodesviacion)
	{
		//Comprobamos que la imagen esté umbralizada
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
					//Hemos encontrado un píxel negro en la columna j
					media++; 

					proyeccion[j]++; 					
				}
			}			
		}

		//Calculamos la media
		media /= segmento.esquina_inferior_derecha.x-segmento.esquina_superior_izquierda.x+1;

		//Calculamos el valor proporcional a la desviación para ahorrarnos los cuadrados y las raíces
		for(int j=segmento.esquina_superior_izquierda.x; j<=segmento.esquina_inferior_derecha.x; j++)
			(*pseudodesviacion) += fabs((double)proyeccion[j] - media);

		//'Normalizamos' la pseudodesviacion con el número de columnas de la palabra
		(*pseudodesviacion) /= segmento.esquina_inferior_derecha.x - segmento.esquina_superior_izquierda.x + 1;

		return 0;
	}

	
	int Texto::proyeccionVertical(Palabra segmento, int *proyeccion, int *maximo)
	{
		//Comprobamos que la imagen esté umbralizada
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
					//Hemos encontrado un píxel negro en la columna j
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
	// DESCRIPCIÓN: Expande la semilla hasta rellenar el hueco al que ésta pertenece.
	// 
	// ARGUMENTOS:	Coordenada semilla -> Píxel de la imagen a partir del cual taparemos el hueco
	//				 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. La semilla no pertenece a la imagen actual
	//============================================================================
	int Texto::rellenarHuecos(Coordenada semilla)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que la semilla pertenece a la imagen
		if(semilla.x<0 || semilla.y<0 || semilla.x>=bitmapinfoheader->biWidth || semilla.y>=bitmapinfoheader->biHeight)
			return 2;
		
		//Creamos una pila en donde iremos guardando los vecinos de la semilla que estén a blanco (!=0)
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

			//Añadimos en la pila aquellos 8-vecinos de la semilla que sean blancos (!=0)
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
	// DESCRIPCIÓN: Devuelve el número de conexiones que tiene el píxel actual.
	// 
	// ARGUMENTOS: int fila    -> Coordenada y del píxel a comprobar
	//			   int columna -> Coordenada x del píxel a comprobar
	//				 
	// SALIDA: Un valor entero que indica:
	//			>=0 -> Número de conexiones del píxel actual. Operación realizada satisfactoriamente
	//			-1 -> Error. La imagen no está umbralizada	
	//			-2 -> Error. El píxel a comprobar no pertenece a la imagen
	//============================================================================
	int Texto::conectividad(int fila, int columna)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return -1;

		//Comprobamos que no nos salimos de la imagen
		if(fila<1 || columna<1 || fila>=bitmapinfoheader->biHeight - 1 || columna>=bitmapinfoheader->biWidth - 1)
			return -2;

        //Variables necesarias
		bool ultimo;//true->último píxel era negro	false->último píxel era blanco
		int cambios_color = 0;//Contador de los cambios de color (serán el doble que las conexiones)
		
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
		
		//Obtenemos el número de conexiones
		cambios_color /= 2;

		return cambios_color;
	}

	
	//======================================================================
	// NOMBRE: esPuntoFinal
	//
	// DESCRIPCIÓN: Comprueba si un determinado píxel es un punto final.
	// 
	// ARGUMENTOS: int fila    -> Coordenada y del píxel a comprobar
	//			   int columna -> Coordenada x del píxel a comprobar
	//				 
	// SALIDA: Un valor booleano que indica:
	//			true -> El píxel es un punto final. Operación realizada satisfactoriamente
	//			false -> El píxel no es un punto final o Error. La imagen no está umbralizada o el píxel a comprobar no pertenece a la imagen
	//============================================================================
	bool Texto::esPuntoFinal(int fila, int columna)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return false;

		//Comprobamos que no nos salimos de la imagen
		if(fila<1 || columna<1 || fila>=bitmapinfoheader->biHeight - 1 || columna>=bitmapinfoheader->biWidth - 1)
			return false;

		//Para ser punto final lo primero es que estemos ante un píxel negro
		if(mapa_bits[posicion(fila, columna, 1)]!=0)
			return false;

		//Variable necesaria
		int vecinos = 0;//Contador de los vecinos del píxel actual (consideraremos 8-conectividad)
		
		//Contamos los vecinos que tenga el píxel actual
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

		//Estaremos ante un punto final si sólo tiene un 8-vecino      
		return (vecinos==1);
	}


	//REVISAR LO DE SI OBLIGO A UMBRALIZADA O NO
	//======================================================================
	// NOMBRE: Sobel
	//
	// DESCRIPCIÓN: Aplica Sobel a la porción de imagen del carácter con el objetivo de calcular los gradientes de la misma. (Realmente solo guardaremos el rango de la dirección del gradiente)
	//				Estos valores se almacenarán en el parámetro por referencia gradiente. La imagen no se ve modificada.
	//				Debe reservarse la memoria con anterioridad o dará error. Esto se ha hecho así para que sea más fácil ver que luego hay 
	//				que borrarlo para ahorrar memoria, si reservaramos la memoria aquí probablemente olvidariamos liberar la memoria con frecuencia.
	//				Las dimensiones de la matriz deben ser iguales que el tamaño del carácter.
	// 
	// ARGUMENTOS: Caracter *caracter -> Puntero al carácter del que queremos calcular el gradiente
	//			   BYTE **gradiente   -> Puntero a la matriz en la que guardaremos el gradiente
	//				 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente
	//			1 -> Error. La imagen no está umbralizada	
	//			2 -> Error. El carácter no pertenece a la imagen actual
	//============================================================================
	int Texto::Sobel(Caracter *caracter, BYTE **gradiente)
	{
		//Comprobamos que la imagen esté umbralizada
		if(!umbralizada)
			return 1;

		//Comprobamos que el carácter pertenece a la imagen
		if(caracter->esquina_superior_izquierda.x<0 || caracter->esquina_superior_izquierda.y<0 || caracter->esquina_inferior_derecha.x>=bitmapinfoheader->biWidth || caracter->esquina_inferior_derecha.y>=bitmapinfoheader->biHeight)
			return 2;

		//Inicializamos las máscaras de Sobel
		int mascara_horizontal[9] = {1,2,1,0,0,0,-1,-2,-1};
		int mascara_vertical[9] = {-1,0,1,-2,0,2,-1,0,1};

		int profundidad = bitmapinfoheader->biBitCount;

		if(profundidad==8)
		{
			//Recorremos el carácter
			for(int i=0; i<caracter->esquina_inferior_derecha.y - caracter->esquina_superior_izquierda.y + 1; i++)
				for(int j=0; j<caracter->esquina_inferior_derecha.x - caracter->esquina_superior_izquierda.x + 1; j++)
				{
					//Variables necesarias
					int resultado_horizontal = 0;
					int resultado_vertical = 0;
					double angulo;//Dirección del gradiente
					bool fin = false;//Si una cordenada no existe nos saldremos

					//Recorremos la máscara
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
					
					//Obtenemos la dirección del gradiente
					if(resultado_horizontal==0)
					{
						//Casos especiales (para evitar división por 0)
						if(resultado_vertical<0)
							angulo = 270;
						else
							angulo = 90;
					}
					else
					{
						//Caso general
						angulo = atan(resultado_vertical / (double)resultado_horizontal);

						// Pasamos el ángulo de radianes a grados
						angulo = (angulo * 180) / PI;

						if(resultado_horizontal<0)
							angulo = angulo + 180;
						else if(resultado_vertical<0)
							angulo = angulo + 360;
					}

					//Guardamos el rango de la dirección (ángulo)
					//Tendremos 12 rangos de 30º cada uno
					gradiente[i][j] = (((int)(angulo)) - 1) / 30;				
				}
		}			
		
		return 0;
	}	

	
	//======================================================================
	// NOMBRE: ajustarCaracter
	//
	// DESCRIPCIÓN: Elimina del caracter todas la sfilas y columnas de píxeles blancos, obteniendo el menor rectángulo que incluya al carácter.
	//			
	// ARGUMENTOS: Caracter *caracter -> Puntero al carácter que queremos ajustar
	//							 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente	
	//			1 -> Error. El carácter no pertenece a la imagen actual
	//============================================================================
	int Texto::ajustarCaracter(Caracter *caracter)
	{
		//Comprobamos que el carácter pertenece a la imagen
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

			if(j==caracter->esquina_inferior_derecha.x + 1)//La fila no tiene ningún píxel negro
				caracter->esquina_superior_izquierda.y++;
			else
				break;
		}

		if(caracter->esquina_superior_izquierda.y!=original_izquierda.y)//Para dar un píxel más, especialmente útil en caracteres muy pequeños
			caracter->esquina_superior_izquierda.y--;

		//Filas inferiores
		for(int i=caracter->esquina_inferior_derecha.y; i>=caracter->esquina_superior_izquierda.y; i--)
		{
			int j;

			for(j=caracter->esquina_superior_izquierda.x; j<=caracter->esquina_inferior_derecha.x && mapa_bits[posicion(i,j,1)]==255; j++);

			if(j==caracter->esquina_inferior_derecha.x + 1)//La fila no tiene ningún píxel negro
				caracter->esquina_inferior_derecha.y--;
			else
				break;
		}	

		if(caracter->esquina_inferior_derecha.y!=original_derecha.y)//Para dar un píxel más, especialmente útil en caracteres muy pequeños
			caracter->esquina_inferior_derecha.y++;

		//Quitamos las columnas blancas 
		//Columnas izquierda
		for(int j=caracter->esquina_superior_izquierda.x; j<=caracter->esquina_inferior_derecha.x; j++)
		{
			int i;

			for(i=caracter->esquina_superior_izquierda.y; i<=caracter->esquina_inferior_derecha.y && mapa_bits[posicion(i,j,1)]==255; i++);

			if(i==caracter->esquina_inferior_derecha.y + 1)//La columna no tiene ningún píxel negro
				caracter->esquina_superior_izquierda.x++;
			else
				break;
		}

		if(caracter->esquina_superior_izquierda.x!=original_izquierda.x)//Para dar un píxel más, especialmente útil en caracteres muy pequeños
			caracter->esquina_superior_izquierda.x--;

		//Columnas derecha
		for(int j=caracter->esquina_inferior_derecha.x; j>=caracter->esquina_superior_izquierda.x; j--)
		{
			int i;

			for(i=caracter->esquina_superior_izquierda.y; i<=caracter->esquina_inferior_derecha.y && mapa_bits[posicion(i,j,1)]==255; i++);

			if(i==caracter->esquina_inferior_derecha.y + 1)//La columna no tiene ningún píxel negro
				caracter->esquina_inferior_derecha.x--;
			else
				break;
		}

		if(caracter->esquina_inferior_derecha.x!=original_derecha.x)//Para dar un píxel más, especialmente útil en caracteres muy pequeños
			caracter->esquina_inferior_derecha.x++;
		
		return 0;
	}


	//======================================================================
	// NOMBRE: aprendeCaracteres
	//
	// DESCRIPCIÓN: Aprende los caracteres.
	//			
	// ARGUMENTOS: -
	//							 
	// SALIDA: Un valor entero que indica:
	//			0 -> Operación realizada satisfactoriamente	
	//			1 -> Error. La imagen no está segmentada en caracteres
	//			2 -> Error. No hay texto reconocido
	//			3 -> Error. El texto reonocido no coincide con la segmentación
	//============================================================================
	int Texto::aprendeCaracteres()
	{
		//Comprobamos que la imagen esté segmentada en caracteres
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
					//Comprobación de textoReconocido 'correcto'
					if(textoReconocido[k]==' ' || textoReconocido[k]=='\n')
						return 3;

					lineas[l].palabras[p].caracteres[c].reconocido = new char[2];
					lineas[l].palabras[p].caracteres[c].reconocido[0] = textoReconocido[k];	
					lineas[l].palabras[p].caracteres[c].reconocido[1] = '\0';
				}

				if(p!=lineas[l].numeroPalabras - 1)
				{
					//Comprobación de textoReconocido 'correcto'
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

				//Comprobación de textoReconocido 'correcto'
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

