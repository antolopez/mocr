#pragma once

//Tipos básicos
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef char *String;


//Estructuras para la segmentación del texto en líneas, palabras y caracteres

//Identifica un punto de la imagen
typedef struct Coordenada
{
	int x;//Columna
	int y;//Fila
}Coordenada;

//Estructura con todo lo necesario para caracterizar un carácter (realmente un segmento, ya que permitimos aprender varios caracteres juntos)
typedef struct Caracter
{
	//Rectángulo que incluye al carácter
	Coordenada esquina_superior_izquierda;
	Coordenada esquina_inferior_derecha;

	int numeroPixeles;

	int numeroHuecos;
	int numeroCruces;
	int numeroPuntosTerminales;
	double relacionAspecto;

	int **caracteristicasDireccionalesB;
	BYTE **caracteristicasDireccionalesR;
	BYTE **caracteristicasEstructurales;
	BYTE **caracteristicasConcavidad;

	String reconocido;
}Caracter;

//Estructura con todo lo necesario para caracterizar una palabra
typedef struct Palabra
{
	Coordenada esquina_superior_izquierda;
	Coordenada esquina_inferior_derecha;

	int numeroCaracteres;
	Caracter *caracteres;
}Palabra;

//Estructura con todo lo necesario para carcaterizar una línea
typedef struct Linea
{
	Coordenada esquina_superior_izquierda;
	Coordenada esquina_inferior_derecha;

	int numeroPalabras;
	Palabra *palabras;
}Linea;


//Estructuras para las operaciones morfológicas

typedef struct ElementoEstructural
{
	Coordenada centro;
	int tamano;
	int **matriz;
}ElementoEstructural;


//Estructuras necesarias para cargar/guardar un Bitmap

typedef struct tagBITMAPFILEHEADER 
{
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER, far *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagRGBQUAD 
{
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFOHEADER
{
	DWORD      biSize;
    LONG       biWidth;
    LONG       biHeight;
    WORD       biPlanes;
    WORD       biBitCount;
    DWORD      biCompression;
    DWORD      biSizeImage;
    LONG       biXPelsPerMeter;
    LONG       biYPelsPerMeter;
    DWORD      biClrUsed;
    DWORD      biClrImportant;
} BITMAPINFOHEADER, far *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPINFO 
{
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO, far *LPBITMAPINFO, *PBITMAPINFO;

