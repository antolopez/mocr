using System;
using System.Collections.Generic;
using System.Text;

namespace OCR.Perfiles
{
    public class Segmentacion
    {
        //Extracci�n de l�neas
        public int interlineadoMedio = 10;
        public int incrementoMinimoLineas = 10;

        //Slope
        public bool corregirSlope = false;
        public int distanciaMinimaSlope = 100;

        //Slant
        public bool corregirSlant = false;
        public int distanciaMinimaSlant = 100;

        //Extracci�n de palabras
        public int espaciadoMedio = 30;

        //Extracci�n de caracteres
        public int grosorMedioTrazo = 5;
        public double incrementoMinimoCaracteres = 0.02;

        //Esqueleto
        public bool calcularEsqueleto = true;
        public bool precaracteres = false;

    }
}
