//TextoManejado.h
#pragma once

#include "Imagen.h"

using namespace System;

namespace Imagen
{
	public ref class TextoManejado
	{
		//Atributos
		private:
			Texto* texto;
			

		//Métodos
		public:
			
			//Constructores
			TextoManejado(void);
			TextoManejado(IntPtr imagen);

			//Destructor
			~TextoManejado(void);
			
			void LiberarTextoManejado(void);

			//Métodos propiedades imagen
			int GetTamano();
			int GetAlto();
			int GetAncho();
			int GetProfundidad();
			int Cargar(System::String^ ruta);
			int Guardar(System::String^ ruta);
			bool EstaUmbralizado();

			int Escalacion(double factor_columnas, double factor_filas);//Escala la imagen
			int EscalacionLineal(double factor_columnas, double factor_filas);//Escala la imagen
			
			int Rotacion(double grados);//Rota la iamgen
			int Umbralizar(int umbral);
			int Enderezar();
			int SegmentacionLineas(int interlineado_minimo, int distancia_minima);
			int SegmentacionPalabras(int espaciado_medio, bool pintar_limites);
			int SegmentacionCaracteres(int grosor_lapiz, double incremento_minimo, bool pintar_limites);
			int CorregirSlope(int distancia_minima);
			int CorregirSlant(int distancia_minima);
			
			int Esqueleto();
			
			BYTE* GetDIB();
			BYTE* GetMapaBits();

			TextoManejado^ Copia();

			void SetPerfil(System::String^ perfil);
			void SetRutaBBDD(System::String^ ruta);
			int GetNumeroVecinos();
			void SetNumeroVecinos(int numero_vecinos);

			System::String^ Reconocer(bool modo);
			int CrearClasificador(int clasificador);

			void SetTextoReconocido(System::String^ texto_reconocido);

			int InsertarPerfil(System::String ^ruta, System::String^perfil);
			int BorrarPerfil(System::String ^ruta, System::String^perfil);

			int GetLinea(int fila, int columna);
			int PintarLinea(int linea);
			int Fusionar(int linea1, int linea2);
			int Dividir(int linea);

			int CorregirTexto();			
	};
}