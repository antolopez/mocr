//Tipos b�sicos
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef char *String;


//Estructuras necesarias para cargar/guardar un Bitmap

//En esta estructura necesitamos forzar su tama�o para poder leer bien los datos del fichero 
//(.NET modifica el tama�o de los campos para optimizar)
[System::Runtime::InteropServices::StructLayout(System::Runtime::InteropServices::LayoutKind::Explicit,Size=14)]
typedef value struct tagBITMAPFILEHEADER 
{
	[System::Runtime::InteropServices::FieldOffset(0)] WORD    bfType;
	[System::Runtime::InteropServices::FieldOffset(2)] DWORD   bfSize;
	[System::Runtime::InteropServices::FieldOffset(6)] WORD    bfReserved1;
	[System::Runtime::InteropServices::FieldOffset(8)] WORD    bfReserved2;
	[System::Runtime::InteropServices::FieldOffset(10)] DWORD   bfOffBits;
}BITMAPFILEHEADER, far *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

//En estas estructuras no hemos forzado el tama�o porque de por si est� optimizado, 
//aunque para asegurarnos seria mejor ponerlo se ha decidido dejarlo as� por claridad
typedef struct tagRGBQUAD 
{
	BYTE	rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    rgbReserved;
}RGBQUAD;

typedef struct tagBITMAPINFOHEADER
{
	DWORD	biSize;
    LONG    biWidth;
    LONG    biHeight;
    WORD    biPlanes;
    WORD    biBitCount;
    DWORD   biCompression;
    DWORD   biSizeImage;
    LONG    biXPelsPerMeter;
    LONG    biYPelsPerMeter;
    DWORD   biClrUsed;
    DWORD   biClrImportant;
}BITMAPINFOHEADER, far *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPINFO 
{
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO, far *LPBITMAPINFO, *PBITMAPINFO;




