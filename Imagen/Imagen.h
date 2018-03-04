// Imagen.h

#pragma once

//Includes de código nuestro
#include "Pila.h"
#include "KNN.h"
#include "NaiveBayes.h"

using namespace BBDD;

namespace Imagen
{
	public class Texto
	{		
		//Atributos
		private:
			//Imagen
			BYTE *inicio_DIB; //Puntero al inicio del DIB
			LPBITMAPINFOHEADER bitmapinfoheader; //Puntero a la estructura BITMAPINFOHEADER
			LPBITMAPINFO bitmapinfo; //Puntero a la estructura BITMAPINFO
			RGBQUAD *paleta; //Paleta de colores
			BYTE *mapa_bits; //Cuerpo de la imagen
			bool umbralizada;//Indica si la imagen está umbralizada 
			String nombreImagen; //Nombre(con la extensión incluida) de la imagen

			//Segmentación imagen
			Linea *lineas;//Incluye todo lo necesario para conocer la segmentación de la imagen
			int numeroLineas;

			//Reconocimiento
			String perfil;//Identifica al escritor del texto actual 
			String rutaBBDD;//Ruta completa donde se encuentra almacenada la BBDD
			Clasificador *clasificador;//Instancia del clasificador que usaremos
			int numeroVecinos;//Indica la preferencia actual en el número de vecinos
			InterfazBBDD *bbdd;//'Conexión' con la BBDD	
			String textoReconocido;//Texto obtenido al final del reconocimiento (o texto pasado por el usuario si estamos en aprendizaje)


		//Métodos
		 public:
			//Constructores
			Texto(void);
			Texto(int columnas, int filas, int profundidad, RGBQUAD *paleta_origen, int tamano_paleta);			
	
			//Destructores
			~Texto(void);
			
			//Sobrecarga operadores
			Texto& operator=(const Texto&);
			
			//Métodos propiedades imagen
			BYTE* GetDIB();
			void SetDIB(BYTE *DIB);
			BYTE* GetMapaBits();			
			String GetNombreImagen();
			void SetNombreImagen(String nombre);
			String GetPerfil();
			void SetPerfil(String nombre);
			String GetRutaBBDD();
			void SetRutaBBDD(String ruta);
			String GetTextoReconocido();
			void SetTextoReconocido(String texto);
			int GetNumeroFilas();
			int GetNumeroColumnas(); 
			int GetTamano();
			int GetProfundidad();	
			
			bool EstaUmbralizada();
			//Fin Métodos propiedades imagen

			//Métodos comprobaciones
			bool ExisteCoordenada(int columna, int fila);
			bool ExisteCoordenada(int columna, int fila, Palabra segmento);
			bool EsIgual(Texto *texto);
			
			//Operaciones geométricas
			int Escalacion(double factor_columnas, double factor_filas);
			int EscalacionLineal(double factor_columnas, double factor_filas);
			int Rotacion(int modo, double grados);
			int Rotacion(Palabra segmento, double grados);
			int Shear(Palabra segmento, double grados);
			int Enderezar();

			//Operaciones de segmentación
			int Umbralizar(int umbral, int nivel_minimo, int nivel_maximo);
			int SegmentacionLineas(int interlineado_medio, int diferencia_minima, int espaciado_medio);
			int SegmentacionPalabras(int espaciado_medio, bool pintar_limites);
			int SegmentacionCaracteres(int grosor_lapiz, double incremento_minimo, bool pintar_limites);
			int ObtenerLineas(int interlineado_medio, int diferencia_minima, int espaciado_medio);
			int ObtenerSegmentos(int distancia_minima);
			int ObtenerCaracteres(Palabra *palabra, int grosor_lapiz, double incremento_minimo);
			
			//Operaciones de preprocesamiento
			int CorregirSlope();
			int CorregirSlope(Linea linea);
			int CorregirSlant();
			int CorregirSlant(Linea linea);

			//Operaciones de conjuntos
			int Interseccion(Texto *texto);
			int Diferencia(Texto *texto);
			int InterseccionO(Texto *texto);
			int DiferenciaO(Texto *texto);
			
			//Operaciones morfológicas
			int Erosion(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario);
			int ErosionO(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario);			
			int TransformacionAlAzar(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario);	
			int TransformacionAlAzarO(ElementoEstructural elemento, bool imagen_complementaria, bool elemento_complementario);
			int Reduccion(ElementoEstructural *elemento, bool imagen_complementaria, bool elemento_complementario);
			
			//Operaciones para obtener características de los 'caracteres'
			int CodigoCadena(Caracter *caracter);
			int NumeroHuecos(Caracter *caracter);
			int NumeroCruces(Caracter *caracter);
			int PuntosTerminales(Caracter *caracter);
			int CalcularCaracteristicas(Caracter *caracter);

			//Operaciones de reconocimiento
			int GetNumeroVecinosKNN();
			void SetNumeroVecinosKNN(int numero_vecinos);
			int CrearClasificador(int clasificador_seleccionado);
			int Reconocer(int modo);
			int ReconocerPalabra(Palabra *palabra, int modo);
			int AprendeCaracter(Caracter caracter);
			int CorregirTexto();
			int CorregirPalabra(Palabra *reconocida, String corregida);
			
			//Operaciones para arreglar segmentación en líneas
			int GetLinea(int fila, int columna);
			int PintarLinea(int linea);
			int Fusionar(int linea1, int linea2);
			int Dividir(int linea);			
			

		private:
			//Varios
			int posicion(int fila, int columna, int tamano_elemento);//Calcula una posición dentro del mapa de bits
			int RLSA(int umbral);
			int RLSAVertical(Linea palabra, int umbral);
			
			//Cálculo de proyecciones
			int proyeccionHorizontal(int *proyeccion);
			int proyeccionHorizontal(int *proyeccion, double *pseudodesviacion);
			int proyeccionHorizontal(Palabra segmento, int *proyeccion, double *maximo);
			int proyeccionVertical(Palabra palabra, int *proyeccion);
			int proyeccionVertical(Palabra segmento, int *proyeccion, double *pseudodesviacion);
			int proyeccionVertical(Palabra palabra, int *proyeccion, int *maximo);

			//Operaciones usadas en el cálculo de características
			int rellenarHuecos(Coordenada semilla);
			int conectividad(int fila, int columna);
			bool esPuntoFinal(int fila, int columna);
			int Sobel(Caracter *caracter, BYTE **gradiente);

			//Métodos usados para el entrenamiento/reconocimiento
			int ajustarCaracter(Caracter *caracter);
			int aprendeCaracteres();

			
			//Otros
			int redondear(double valor);
			
			

	};
}
