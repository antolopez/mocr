using System;
using System.Collections.Generic;
using System.Text;

namespace OCR.Perfiles
{
    public class Preprocesado
    {
        //Escalado
        public bool redimensionarImagen = false;
        public bool mantenerProporcion = true;
        public bool cambiarAncho = false;
        public int pixelesAncho = 1000;
        public bool cambiarAlto = true;
        public int pixelesAlto = 1000;

        //Umbralizado
        public int umbral = 200;

        //Enderezado
        public bool enderezadoAutomatico = true;
    }
}
