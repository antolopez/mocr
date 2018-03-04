using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace OCR.Perfiles
{
    public class Perfil
    {
        private String nombrePerfil;
        public General general;
        public Preprocesado preprocesado;
        public Segmentacion segmentacion;
        public Reconocimiento reconocimiento;


        public Perfil()
        {
            general = new General();
            preprocesado = new Preprocesado();
            segmentacion = new Segmentacion();
            reconocimiento = new Reconocimiento();
        }

        public String GetNombre()
        {
            return nombrePerfil;
        }

        public bool Cargar(String nombrePerfil)
        {
            this.nombrePerfil = nombrePerfil;

            String[] configuracion;
            try
            {
                configuracion = File.ReadAllLines(Application.StartupPath + "\\Perfiles\\" + nombrePerfil);
            }
            catch (Exception)
            {
                return false;
            }

            foreach (String linea in configuracion)
            {
                String lineaTrim = linea.Trim();//Quitamos espacios y tabuladores iniciales

                switch (lineaTrim)
                {
                    case "General":
                    break;

                    case "":
                    break;

                    case "iniciarMaximizado = true":
                        general.iniciarMaximizado = true;
                    break;

                    case "iniciarMaximizado = false":
                        general.iniciarMaximizado = false;
                    break;

                    case "Preprocesado":
                    break;

                    case "Escalado":
                    break;

                    case "redimensionarImagen = true":
                        preprocesado.redimensionarImagen = true;
                    break;

                    case "redimensionarImagen = false":
                        preprocesado.redimensionarImagen = false;
                    break;

                    case "mantenerProporcion = true":
                        preprocesado.mantenerProporcion = true;
                    break;

                    case "mantenerProporcion = false":
                        preprocesado.mantenerProporcion = false;
                    break;

                    case "cambiarAncho = true":
                        preprocesado.cambiarAncho = true;
                    break;

                    case "cambiarAncho = false":
                        preprocesado.cambiarAncho = false;
                    break;

                    case "cambiarAlto = true":
                        preprocesado.cambiarAlto = true;
                    break;

                    case "cambiarAlto = false":
                        preprocesado.cambiarAlto = false;
                    break;

                    case "Umbralizado":
                    break;

                    case "Enderezado":
                    break;

                    case "enderezadoAutomatico = true":
                        preprocesado.enderezadoAutomatico = true;
                    break;

                    case "enderezadoAutomatico = false":
                        preprocesado.enderezadoAutomatico = false;
                    break;

                    case "Segmentacion":
                    break;

                    case "Lineas":
                    break;

                    case "Slope":
                    break;

                    case "activarCorregirSlope = true":
                        segmentacion.corregirSlope = true;
                    break;

                    case "activarCorregirSlope = false":
                        segmentacion.corregirSlope = false;
                    break;

                    case "Slant":
                    break;

                    case "activarCorregirSlant = true":
                        segmentacion.corregirSlant = true;
                    break;

                    case "activarCorregirSlant = false":
                        segmentacion.corregirSlant = false;
                    break;
	
		            case "Palabras":
                    break;

                    case "Caracteres":
                    break;

                    case "Esqueleto":
                    break;

                    case "activarCalculoEsqueleto = true":
                        segmentacion.calcularEsqueleto = true;
                    break;

                    case "activarCalculoEsqueleto = false":
                        segmentacion.calcularEsqueleto = false;
                    break;

                    case "presegmentacionCaracteres = true":
                        segmentacion.precaracteres = true;
                    break;

                    case "presegmentacionCaracteres = false":
                        segmentacion.precaracteres = false;
                    break;

                    case "Reconocimiento":
                    break;


                    default:
                    {
                        //Escalado
                        if(lineaTrim.Length>15 && lineaTrim.Substring(0, 15) == "pixelesAncho = ")
                            preprocesado.pixelesAncho = Int32.Parse(lineaTrim.Substring(15));
                        else
                            if(lineaTrim.Length>14 && lineaTrim.Substring(0, 14) == "pixelesAlto = ")
                                preprocesado.pixelesAlto = Int32.Parse(lineaTrim.Substring(14));
                            else//Umbralizado
                                if(lineaTrim.Length>9 && lineaTrim.Substring(0, 9) == "umbral = ")
                                    preprocesado.umbral = Int32.Parse(lineaTrim.Substring(9));
                                else//Extracción líneas
                                    if(lineaTrim.Length>20 && lineaTrim.Substring(0, 20) == "interlineadoMedio = ")
                                        segmentacion.interlineadoMedio = Int32.Parse(lineaTrim.Substring(20));
                                    else
                                        if(lineaTrim.Length>25 && lineaTrim.Substring(0, 25) == "incrementoMinimoLineas = ")
                                            segmentacion.incrementoMinimoLineas = Int32.Parse(lineaTrim.Substring(25));
                                        else
                                            if(lineaTrim.Length>23 && lineaTrim.Substring(0, 23) == "distanciaMinimaSlope = ")
                                                segmentacion.distanciaMinimaSlope = Int32.Parse(lineaTrim.Substring(23));
                                            else
                                                if(lineaTrim.Length>23 && lineaTrim.Substring(0, 23)== "distanciaMinimaSlant = ")
                                                    segmentacion.distanciaMinimaSlant = Int32.Parse(lineaTrim.Substring(23));
                                                else
                                                    if(lineaTrim.Length>17 && lineaTrim.Substring(0, 17) == "espaciadoMedio = ")
                                                        segmentacion.espaciadoMedio = Int32.Parse(lineaTrim.Substring(17));
                                                    else
                                                        if(lineaTrim.Length>19 && lineaTrim.Substring(0, 19) == "grosorMedioTrazo = ")
                                                            segmentacion.grosorMedioTrazo = Int32.Parse(lineaTrim.Substring(19));
                                                        else
                                                            if(lineaTrim.Length>29 && lineaTrim.Substring(0, 29) == "incrementoMinimoCaracteres = ")
                                                                segmentacion.incrementoMinimoCaracteres = Double.Parse(lineaTrim.Substring(29));
                                                            else
                                                            {
                                                                //Cargamos perfil 'Predefinido'
                                                                nombrePerfil = "Predefinido";
                                                                general = new General();
                                                                preprocesado = new Preprocesado();
                                                                segmentacion = new Segmentacion();
                                                                reconocimiento = new Reconocimiento();

                                                                return false;
                                                            }                                                       
                        }
                        break;
                }
            }

            return true;
        }

        public bool Guardar()
        {
            String[] configuracion = new String[55];

            configuracion[0]  = "General";
            configuracion[1]  = "";
            configuracion[2]  = "\t\tiniciarMaximizado = " + general.iniciarMaximizado.ToString().ToLower();
            configuracion[3]  = "";
            configuracion[4]  = "Preprocesado";
            configuracion[5]  = "";
            configuracion[6]  = "\tEscalado";
            configuracion[7]  = "";
            configuracion[8]  = "\t\tredimensionarImagen = " + preprocesado.redimensionarImagen.ToString().ToLower();
            configuracion[9]  = "\t\tmantenerProporcion = " + preprocesado.mantenerProporcion.ToString().ToLower();
            configuracion[10] = "\t\tcambiarAncho = " + preprocesado.cambiarAncho.ToString().ToLower();
            configuracion[11] = "\t\tpixelesAncho = " + preprocesado.pixelesAncho.ToString();
            configuracion[12] = "\t\tcambiarAlto = " + preprocesado.cambiarAlto.ToString().ToLower();
            configuracion[13] = "\t\tpixelesAlto = " + preprocesado.pixelesAlto.ToString();
            configuracion[14] = "";
            configuracion[15] = "\tUmbralizado";
            configuracion[16] = "";
            configuracion[17] = "\t\tumbral = " + preprocesado.umbral.ToString();
            configuracion[18] = "";
            configuracion[19] = "\tEnderezado";
            configuracion[20] = "";
            configuracion[21] = "\t\tenderezadoAutomatico = " + preprocesado.enderezadoAutomatico.ToString().ToLower();
            configuracion[22] = "";
            configuracion[23] = "Segmentacion";
            configuracion[24] = "";
            configuracion[25] = "\tLineas";
            configuracion[26] = "";
            configuracion[27] = "\t\tinterlineadoMedio = " + segmentacion.interlineadoMedio.ToString();
            configuracion[28] = "\t\tincrementoMinimoLineas = " + segmentacion.incrementoMinimoLineas.ToString();
            configuracion[29] = "";
            configuracion[30] = "\tSlope";
            configuracion[31] = "";
            configuracion[32] = "\t\tactivarCorregirSlope = " + segmentacion.corregirSlope.ToString().ToLower();
            configuracion[33] = "\t\tdistanciaMinimaSlope = " + segmentacion.distanciaMinimaSlope.ToString();
            configuracion[34] = "";
            configuracion[35] = "\tSlant";
            configuracion[36] = "";
            configuracion[37] = "\t\tactivarCorregirSlant = " + segmentacion.corregirSlant.ToString().ToLower();
            configuracion[38] = "\t\tdistanciaMinimaSlant = " + segmentacion.distanciaMinimaSlant.ToString();
            configuracion[39] = "";
            configuracion[40] = "\tPalabras";
            configuracion[41] = "";
            configuracion[42] = "\t\tespaciadoMedio = " + segmentacion.espaciadoMedio.ToString();
            configuracion[43] = "";
            configuracion[44] = "\tCaracteres";
            configuracion[45] = "";
            configuracion[46] = "\t\tgrosorMedioTrazo = " + segmentacion.grosorMedioTrazo.ToString();
            configuracion[47] = "\t\tincrementoMinimoCaracteres = " + segmentacion.incrementoMinimoCaracteres.ToString();
            configuracion[48] = "";
            configuracion[49] = "\tEsqueleto";
            configuracion[50] = "";
            configuracion[51] = "\t\tactivarCalculoEsqueleto = " + segmentacion.calcularEsqueleto.ToString().ToLower();
            configuracion[52] = "\t\tpresegmentacionCaracteres = " + segmentacion.precaracteres.ToString().ToLower();
            configuracion[53] = "";
            configuracion[54] = "Reconocimiento";

            try
            {
                File.WriteAllLines(Application.StartupPath + "\\Perfiles\\" + nombrePerfil, configuracion);
            }
            catch (Exception)
            {
                return false;
            }

            return true;
        }

        public override string  ToString()
        { 	
            return nombrePerfil;
        }      

    }
}
