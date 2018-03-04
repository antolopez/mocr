// Archivo DLL principal.

#include "stdafx.h"

#include "BMP.h"

//Bibliotecas necesarias
#include <fstream>
#include "math.h"

using namespace std;

namespace BMP
{

	//============================================================================
	// NOMBRE: CargarImagen
	//
	// DESCRIPCIÓN: Carga una imagen desde un archivo en disco en formato BMP, el cual debe estar
	//				sin ningún tipo de compresión y almacenado en 1, 4, 8, 16 ó 24 bits/píxel.
	//
	// ARGUMENTOS: String ruta_fichero -> Camino completo al archivo en disco
	//			   int *codigo_error   -> Un valor entero que indica:
	//										0 -> Operación realizada satisfactoriamente
	//										1 -> Error. Problemas al abrir el archivo
	//										2 -> Error. No es un archivo BMP
	//										3 -> Error. No está almacenada en 1, 4, 8, 16 ó 24 bits/píxel
	//										4 -> Error. El bitmap está comprimido	
	//
	// SALIDA: Un puntero a la imagen en formato DIB
	//============================================================================
	BYTE* ImagenIO::CargarImagen(String ruta_fichero, int *codigo_error)
	{
		ifstream archivo;

		archivo.open(ruta_fichero, ios::binary|ios::in);//Intentamos abrir el archivo en modo binario y lectura
		
		if(!archivo)//Comprobamos si ha habido problemas al abrir el archivo
		{
			*codigo_error = 1;
			return NULL;
		}

		BITMAPFILEHEADER fileheader;//Leemos la información de la cabecera del supuesto bitmap
		archivo.read((char *)&fileheader, sizeof(BITMAPFILEHEADER));
		
		if(fileheader.bfType!=0x4d42)//Comprobamos si el archivo es un BMP
		{
			*codigo_error = 2;
			return NULL;
		}

		BITMAPINFOHEADER infoheader;//Leemos la información del BITMAPINFOHEADER
		archivo.read((char *)&infoheader, sizeof(BITMAPINFOHEADER));

		//Comprobamos que la imagen cumple los requisitos impuestos
		if((infoheader.biBitCount!=1) && (infoheader.biBitCount!=4) && (infoheader.biBitCount!=8) && (infoheader.biBitCount!=16) && (infoheader.biBitCount!=24))
		{
			*codigo_error = 3;
			return NULL;
		}

		if(infoheader.biCompression!=BI_RGB)//Comprobamos que el BMP no esté comprimido
		{
			*codigo_error = 4;
			return NULL;
		}

		//Calculamos el tamaño del DIB
		archivo.seekg(0,ios::end);//Volvemos al prinipio del archivo
		DWORD tamano_DIB = (int)archivo.tellg() - sizeof(BITMAPFILEHEADER);//Tamaño del fichero menos el de FILEHEADER

		//Cargamos el DIB
		BYTE *imagen = new BYTE[tamano_DIB];//Reservamos memoria para el DIB
		archivo.seekg(sizeof(BITMAPFILEHEADER),ios::beg);//Nos saltamos la cabecera FILEHEADER
		archivo.read((char*)imagen, tamano_DIB);//Leemos la información a guardar en el DIB
		
		archivo.close();//Cerramos el archivo

		*codigo_error = 0;//Carga finalizada con éxito

		return imagen;		
	}


	//============================================================================
	// NOMBRE: GuardarImagen
	//
	// DESCRIPCIÓN: Guarda una imagen en un archivo de disco en formato BMP.
	//				Si el archivo ya existe se sobrescribe.
	//
	// ARGUMENTOS:	BYTE *imagen		-> Un puntero a la imagen en formato DIB
	//				String ruta_fichero -> Camino completo al archivo en disco
	//				int *codigo_error   -> Un valor entero que indica:
	//											0 -> Operación realizada satisfactoriamente
	//											1 -> Error. Problemas al abrir el archivo	
	// SALIDA: -			
	//
	//============================================================================

	void ImagenIO::GuardarImagen(BYTE *imagen, String ruta_fichero, int *codigo_error)
	{
		ofstream archivo;

		archivo.open(ruta_fichero,ios::binary|ios::out);//Intentamos abrir el archivo en modo binario y escritura

		if(!archivo)
		{
			*codigo_error = 1;
			return;
		}

		//Creamos la estructura bitmapfileheader
		BITMAPFILEHEADER bitmapfileheader;
		
		//Recogemos el bitmapinfoheader del DIB
		LPBITMAPINFOHEADER bitmapinfoheader = (LPBITMAPINFOHEADER)imagen;

		//Calculamos tamaño paleta
		int tamano_paleta = 0;
		if(bitmapinfoheader->biBitCount<=8)
		{
			if(bitmapinfoheader->biClrUsed!=0)
				tamano_paleta = bitmapinfoheader->biClrUsed * sizeof(RGBQUAD);
			else
				tamano_paleta = (int)pow(2.0,bitmapinfoheader->biBitCount) * sizeof(RGBQUAD);
		}
		//Fin cálculo tamaño paleta
		
		//Rellenamos el bitmapfileheader
		bitmapfileheader.bfType = 0x4d42;//Queremos guardar la imagen como un bmp
		bitmapfileheader.bfReserved1 = 0;
		bitmapfileheader.bfReserved2 = 0;
		bitmapfileheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + tamano_paleta + bitmapinfoheader->biSizeImage;
		bitmapfileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + tamano_paleta;
		//Fin estructura bitmapfileheader

		//Escribimos en el disco
		archivo.write((const char*)&bitmapfileheader, sizeof(BITMAPFILEHEADER));
		archivo.write((const char*)imagen, bitmapfileheader.bfSize - sizeof(BITMAPFILEHEADER));
		//Fin escribir en disco

		archivo.close();//Cerramos el archivo
	}
	
}


