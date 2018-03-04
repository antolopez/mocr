using System;
using System.Collections.Generic;
using System.Text;

namespace OCR.Perfiles
{
    public class Segmentacion
    {
        //Extracción de líneas
        public int interlineadoMedio = 10;
        public int incrementoMinimoLineas = 10;

        //Slope
        public bool corregirSlope = false;
        public int distanciaMinimaSlope = 100;

        //Slant
        public bool corregirSlant = false;
        public int distanciaMinimaSlant = 100;

        //Extracción de palabras
        public int espaciadoMedio = 30;

        //Extracción de caracteres
        public int grosorMedioTrazo = 5;
        public double incrementoMinimoCaracteres = 0.02;

        //Esqueleto
        public bool calcularEsqueleto = true;
        public bool precaracteres = false;

    }
}
