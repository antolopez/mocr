using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace OCR
{
    public class Configuracion
    {
        public String perfilInicial;

        public bool Cargar()
        {
            String[] configuracion;
            try
            {
                configuracion = File.ReadAllLines(Application.StartupPath + "\\configuracion");
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
                    case "Perfiles":
                    break;

                    case "":
                    break;

                    default:
                    {
                        if (lineaTrim.Substring(0, 16) == "perfilInicial = ")
                        {
                            perfilInicial = lineaTrim.Substring(16);

                            //Cargamos los distintos perfiles disponibles en el combo box
                            if (!Directory.Exists(Application.StartupPath + "\\Perfiles"))
                                Directory.CreateDirectory(Application.StartupPath + "\\Perfiles");

                            String[] perfiles = Directory.GetFiles(Application.StartupPath + "\\Perfiles");

                            bool existePerfil = true;

                            foreach (String perfil in perfiles)
                            {
                                if (!perfil.Contains(perfilInicial))
                                {
                                    existePerfil = false;
                                }
                                else
                                {
                                    existePerfil = true;
                                    break;
                                }
                            }

                            if(!existePerfil)
                                perfilInicial = "Predefinido";
                        }
                        else
                        {
                            //Cargamos configuracion por defecto
                            perfilInicial = "Predefinido";

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
            String[] configuracion = new String[3];

            configuracion[0] = "Perfiles";
            configuracion[1] = "";
            configuracion[2] = "\tperfilInicial = " + perfilInicial;

            try
            {
                File.WriteAllLines(Application.StartupPath + "\\configuracion", configuracion);
            }
            catch (Exception)
            {
                return false;
            }

            return true;                
        }
    }
}
