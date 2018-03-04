// BMP.h

#pragma once

#include "definiciones.h"
#include "tipos.h"

namespace BMP
{
	public ref class ImagenIO
	{
		//Métodos estáticos que cargan o guardan desde/en disco una imagen en formato BMP
		public:
			
			static BYTE* CargarImagen(String ruta_fichero, int *codigo_error);
			static void GuardarImagen(BYTE* imagen, String ruta_fichero, int *codigo_error);
	};
}
