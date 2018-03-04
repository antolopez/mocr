#include "stdafx.h"
#include "TextoManejado.h"
#include <math.h>
#include "BBDD.h"
#using "BMP.dll"
//#using "RAW.dll"

using namespace System::Runtime::InteropServices;
//using namespace System::Reflection;
using namespace BMP;
//using namespace RAW;
namespace Imagen
{
	TextoManejado::TextoManejado(void)
	{
		texto = NULL;
	}

	TextoManejado::TextoManejado(IntPtr imagen)
	{
		texto = new Texto();	

		BYTE* ptrImagen = (BYTE*) imagen.ToPointer();

		texto->SetDIB(ptrImagen);
		
	}

	TextoManejado::~TextoManejado()
	{
		if(texto!=NULL)
			delete texto;
	}

	int TextoManejado::GetTamano()
	{
		return texto->GetTamano();
	}

	int TextoManejado::Cargar(System::String^ ruta)
	{
		texto = new Texto();

		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(ruta);

		char *rutaFichero= (char*)ptr.ToPointer();
		
		BYTE *imagen=NULL;
		
		int codigoError=0;
		 
		imagen=ImagenIO::CargarImagen(rutaFichero, &codigoError);
		
		texto->SetDIB(imagen);
		
		return 0;

	}

	int TextoManejado::Guardar(System::String^ ruta)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(ruta);

		char *rutaFichero= (char*)ptr.ToPointer();
		
		BYTE *imagen=NULL;
		
		int codigoError=0;		
		
		ImagenIO::GuardarImagen(texto->GetDIB(),rutaFichero, &codigoError);
		return 0;
	}

	BYTE* TextoManejado::GetDIB()
	{
		return texto->GetDIB();		
	}

	BYTE* TextoManejado::GetMapaBits()
	{
		return texto->GetMapaBits();
	}

	TextoManejado^ TextoManejado::Copia()
	{
		TextoManejado ^p = gcnew TextoManejado();

		p->texto = new Texto();

		*(p->texto)=*texto;

		return p;
	}

	void TextoManejado::LiberarTextoManejado(void)
	{
		if(texto!=NULL)
			delete texto;

		texto=NULL;
	}


	int TextoManejado::SegmentacionLineas(int interlineado_medio, int diferencia_minima)
	{		
		return 	texto->SegmentacionLineas(interlineado_medio, diferencia_minima, 100);
	}

	int TextoManejado::Umbralizar(int umbral)
	{
		return texto->Umbralizar(umbral, 0, 255);
	}

	int TextoManejado::GetAlto()
	{
		return texto->GetNumeroFilas();
	}

	int TextoManejado::GetAncho()
	{
		return texto->GetNumeroColumnas();
	}

	int TextoManejado::Escalacion(double factor_columnas, double factor_filas)
	{
		return texto->Escalacion(factor_columnas, factor_filas);
	}

	int TextoManejado::EscalacionLineal(double factor_columnas, double factor_filas)
	{
		return texto->EscalacionLineal(factor_columnas, factor_filas);
	}

	int TextoManejado::Rotacion(double grados)
	{
		return texto->Rotacion(1, grados);
	}

	int TextoManejado::Enderezar()
	{
		return texto->Enderezar();
	}

	bool TextoManejado::EstaUmbralizado()
	{
		return texto->EstaUmbralizada();
	}

	int TextoManejado::GetProfundidad()
	{
		return texto->GetProfundidad();
	}

	int TextoManejado::CorregirSlope(int distancia_minima)
	{
		texto->ObtenerSegmentos(distancia_minima);

		return texto->CorregirSlope();
	}
	int TextoManejado::CorregirSlant(int distancia_minima)
	{
		texto->ObtenerSegmentos(distancia_minima);

		return texto->CorregirSlant();
	}

	int TextoManejado::SegmentacionPalabras(int espaciado_medio, bool pintar_limites)
	{
		return texto->SegmentacionPalabras(espaciado_medio, pintar_limites);
	}

	int TextoManejado::SegmentacionCaracteres(int grosor_lapiz, double incremento_minimo, bool pintar_limites)
	{
		texto->SegmentacionCaracteres(grosor_lapiz, incremento_minimo, pintar_limites);

		return 0;
	}

	int TextoManejado::Esqueleto()
	{
		ElementoEstructural *e= new ElementoEstructural[8];

		for(int i=0;i<8;i++)
		{
			e[i].centro.x=1;
			e[i].centro.y=1;

			e[i].tamano=3;

			e[i].matriz= new int*[3];
			e[i].matriz[0]=new int[3];
			e[i].matriz[1]=new int[3];
			e[i].matriz[2]=new int[3];
		}
		
		e[0].matriz[0][0]=0;
		e[0].matriz[0][1]=0;
		e[0].matriz[0][2]=0;
		e[0].matriz[1][0]=-1;
		e[0].matriz[1][1]=1;
		e[0].matriz[1][2]=-1;
		e[0].matriz[2][0]=1;
		e[0].matriz[2][1]=1;
		e[0].matriz[2][2]=1;

		e[1].matriz[0][0]=-1;
		e[1].matriz[0][1]=0;
		e[1].matriz[0][2]=0;
		e[1].matriz[1][0]=1;
		e[1].matriz[1][1]=1;
		e[1].matriz[1][2]=0;
		e[1].matriz[2][0]=1;
		e[1].matriz[2][1]=1;
		e[1].matriz[2][2]=-1;


		e[2].matriz[0][0]=1;
		e[2].matriz[0][1]=-1;
		e[2].matriz[0][2]=0;
		e[2].matriz[1][0]=1;
		e[2].matriz[1][1]=1;
		e[2].matriz[1][2]=0;
		e[2].matriz[2][0]=1;
		e[2].matriz[2][1]=-1;
		e[2].matriz[2][2]=0;


		e[3].matriz[0][0]=1;
		e[3].matriz[0][1]=1;
		e[3].matriz[0][2]=-1;
		e[3].matriz[1][0]=1;
		e[3].matriz[1][1]=1;
		e[3].matriz[1][2]=0;
		e[3].matriz[2][0]=-1;
		e[3].matriz[2][1]=0;
		e[3].matriz[2][2]=0;


		e[4].matriz[0][0]=1;
		e[4].matriz[0][1]=1;
		e[4].matriz[0][2]=1;
		e[4].matriz[1][0]=-1;
		e[4].matriz[1][1]=1;
		e[4].matriz[1][2]=-1;
		e[4].matriz[2][0]=0;
		e[4].matriz[2][1]=0;
		e[4].matriz[2][2]=0;


		e[5].matriz[0][0]=-1;
		e[5].matriz[0][1]=1;
		e[5].matriz[0][2]=1;
		e[5].matriz[1][0]=0;
		e[5].matriz[1][1]=1;
		e[5].matriz[1][2]=1;
		e[5].matriz[2][0]=0;
		e[5].matriz[2][1]=0;
		e[5].matriz[2][2]=-1;

		e[6].matriz[0][0]=0;
		e[6].matriz[0][1]=-1;
		e[6].matriz[0][2]=1;
		e[6].matriz[1][0]=0;
		e[6].matriz[1][1]=1;
		e[6].matriz[1][2]=1;
		e[6].matriz[2][0]=0;
		e[6].matriz[2][1]=-1;
		e[6].matriz[2][2]=1;


		e[7].matriz[0][0]=0;
		e[7].matriz[0][1]=0;
		e[7].matriz[0][2]=-1;
		e[7].matriz[1][0]=0;
		e[7].matriz[1][1]=1;
		e[7].matriz[1][2]=1;
		e[7].matriz[2][0]=-1;
		e[7].matriz[2][1]=1;
		e[7].matriz[2][2]=1;


		texto->Reduccion(e, false, false);

		return 0;
	}

	System::String^ TextoManejado::Reconocer(bool modo)
	{
		int error;

		if(modo)
			error = texto->Reconocer(1);
		else
			error = texto->Reconocer(0);

		System::String^ s2;

		if(error==3)
			return s2;

		System::String^ s1 = gcnew System::String(texto->GetTextoReconocido());

		return s1;
	}

	void TextoManejado::SetNumeroVecinos(int numero_vecinos)
	{
		texto->SetNumeroVecinosKNN(numero_vecinos);
	}

	int TextoManejado::GetNumeroVecinos()
	{
		return texto->GetNumeroVecinosKNN();
	}

	void TextoManejado::SetPerfil(System::String ^perfil)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(perfil);

		char *p= (char*)ptr.ToPointer();

		texto->SetPerfil(p);
	}
	
	void TextoManejado::SetRutaBBDD(System::String ^ruta)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(ruta);

		char *r= (char*)ptr.ToPointer();

		texto->SetRutaBBDD(r);
	}

	int TextoManejado::CrearClasificador(int clasificador)
	{
		return texto->CrearClasificador(clasificador);
	}

	void TextoManejado::SetTextoReconocido(System::String ^texto_reconocido)
	{
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(texto_reconocido);

		char *t= (char*)ptr.ToPointer();

		texto->SetTextoReconocido(t);
	}

	int TextoManejado::InsertarPerfil(System::String ^ruta, System::String ^perfil)
	{
		InterfazBBDD *bbdd = new InterfazBBDD();
		
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(ruta);

		char *r= (char*)ptr.ToPointer();

		//Abrimos la bbdd
		if(bbdd->AbrirBBDD(r)<0)
			return 3;//Error al abrir la base de datos

		IntPtr ptr2 = Marshal::StringToCoTaskMemAnsi(perfil);

		char *t= (char*)ptr2.ToPointer();

		int codigo_error = bbdd->InsertarPerfil(t);

		bbdd->CerrarBBDD();

		return codigo_error;
	}

	int TextoManejado::BorrarPerfil(System::String ^ruta, System::String ^perfil)
	{
		InterfazBBDD *bbdd = new InterfazBBDD();
		
		IntPtr ptr = Marshal::StringToCoTaskMemAnsi(ruta);

		char *r= (char*)ptr.ToPointer();

		//Abrimos la bbdd
		if(bbdd->AbrirBBDD(r)<0)
			return 3;//Error al abrir la base de datos

		IntPtr ptr2 = Marshal::StringToCoTaskMemAnsi(perfil);

		char *t= (char*)ptr2.ToPointer();
		
		int codigo_error = bbdd->BorrarPerfil(t);

		bbdd->CerrarBBDD();

		return codigo_error;
	}


	int TextoManejado::GetLinea(int fila, int columna)
	{
		return texto->GetLinea(fila, columna);
	}


	int TextoManejado::PintarLinea(int linea)
	{
		return texto->PintarLinea(linea);
	}

	int TextoManejado::Fusionar(int linea1, int linea2)
	{
		return texto->Fusionar(linea1, linea2);
	}


	int TextoManejado::Dividir(int linea)
	{
		return texto->Dividir(linea);
	}

	int TextoManejado::CorregirTexto()
	{
		return texto->CorregirTexto();
	}


}