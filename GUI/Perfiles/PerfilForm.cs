using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace OCR.Perfiles
{
    public partial class PerfilForm : Form
    {
        Perfil perfil;

        public PerfilForm()
        {
            InitializeComponent();            
        }

        public PerfilForm(Perfil perfilActual, String pestaña)
        {
            InitializeComponent();

            perfil = perfilActual;

            Text += ": " + perfil.ToString();

            //General
            iniciarMaximizadoCheckBox.Checked = perfil.general.iniciarMaximizado;

            //Preprocesado
            //Escalado
            redimensionarCheckBox.Checked = perfil.preprocesado.redimensionarImagen;
            proporcionesCheckBox.Checked = perfil.preprocesado.mantenerProporcion;
            anchoCheckBox.Checked = perfil.preprocesado.cambiarAncho;
            altoCheckBox.Checked = perfil.preprocesado.cambiarAlto;
            anchoNumericUpDown.Value = perfil.preprocesado.pixelesAncho;
            altoNumericUpDown.Value = perfil.preprocesado.pixelesAlto;

            if (!redimensionarCheckBox.Checked)
            {
                proporcionesCheckBox.Enabled = false;
                anchoCheckBox.Enabled = false;
                anchoNumericUpDown.Enabled = false;
                altoCheckBox.Enabled = false;
                altoNumericUpDown.Enabled = false;
            }

            //Umbralizado
            umbralNumericUpDown.Value = perfil.preprocesado.umbral;
            //Enderezado
            enderezadoAutomaticoRadioButton.Checked = perfil.preprocesado.enderezadoAutomatico;
            enderezadoManualRadioButton.Checked = !perfil.preprocesado.enderezadoAutomatico;

            //Segmentación
            //Extracción de líneas
            interlineadoMedioNumericUpDown.Value = perfil.segmentacion.interlineadoMedio;
            incrementoMinimoLineasNumericUpDown.Value = perfil.segmentacion.incrementoMinimoLineas;
            //Slope
            activarCorregirSlopeCheckBox.Checked = perfil.segmentacion.corregirSlope;
            distanciaMinimaSlopeNumericUpDown.Value = perfil.segmentacion.distanciaMinimaSlope;

            if(!activarCorregirSlopeCheckBox.Checked)
                distanciaMinimaSlopeNumericUpDown.Enabled = false;
            //Slant
            activarCorregirSlantCheckBox.Checked = perfil.segmentacion.corregirSlant;
            distanciaMinimaSlantNumericUpDown.Value = perfil.segmentacion.distanciaMinimaSlant;

            if (!activarCorregirSlantCheckBox.Checked)
                distanciaMinimaSlantNumericUpDown.Enabled = false;
            //Extracción de palabras
            espaciadoMedioNumericUpDown.Value = perfil.segmentacion.espaciadoMedio;
            //Extracción de caracteres
            grosorMedioTrazoNumericUpDown.Value = perfil.segmentacion.grosorMedioTrazo;
            incrementoMinimoCaracteresNumericUpDown.Value = (decimal)perfil.segmentacion.incrementoMinimoCaracteres;
            //Esqueleto
            activarEsqueletoCheckBox.Checked = perfil.segmentacion.calcularEsqueleto;
            esqueletoCheckBox.Checked = perfil.segmentacion.precaracteres;

            if(!perfil.segmentacion.calcularEsqueleto)
                esqueletoCheckBox.Enabled = false;
                        
            //Reconocimiento

            switch (pestaña)
            {
                case "General":
                    perfilTabControl.SelectedTab = perfilGeneralTabPage;
                break;

                case "Preprocesado":
                    perfilTabControl.SelectedTab = perfilPreprocesadoTabPage;
                break;

                case "Segmentacion":
                    perfilTabControl.SelectedTab = perfilSegmentacionTabPage;
                break;

                case "Reconocimiento":
                    perfilTabControl.SelectedTab = perfilReconocimientoTabPage;
                break;

                default:
                    perfilTabControl.SelectedTab = perfilGeneralTabPage;
                break;
            }
        }

        private void redimensionarCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (!redimensionarCheckBox.Checked)
            {
                proporcionesCheckBox.Enabled = false;
                anchoCheckBox.Enabled = false;
                anchoNumericUpDown.Enabled = false;
                altoCheckBox.Enabled = false;
                altoNumericUpDown.Enabled = false;
            }
            else
            {
                proporcionesCheckBox.Enabled = true;
                anchoCheckBox.Enabled = true;
                anchoNumericUpDown.Enabled = true;
                altoCheckBox.Enabled = true;
                altoNumericUpDown.Enabled = true;
            }
        }

        private void proporcionesCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (proporcionesCheckBox.Checked && anchoCheckBox.Checked && altoCheckBox.Checked)
                anchoCheckBox.Checked = false;
        }

        private void anchoCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if(proporcionesCheckBox.Checked)
                if(anchoCheckBox.Checked)
                    altoCheckBox.Checked = false;
        }

        private void altoCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (proporcionesCheckBox.Checked)
                if (altoCheckBox.Checked)
                    anchoCheckBox.Checked = false;
        }

        private void activarCorregirSlopeCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if(activarCorregirSlopeCheckBox.Checked)
                distanciaMinimaSlopeNumericUpDown.Enabled = true;
            else
                distanciaMinimaSlopeNumericUpDown.Enabled = false;
        }

        private void activarCorregirSlantCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (activarCorregirSlantCheckBox.Checked)
                distanciaMinimaSlantNumericUpDown.Enabled = true;
            else
                distanciaMinimaSlantNumericUpDown.Enabled = false;
        }

        private void activarEsqueletoCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if(activarEsqueletoCheckBox.Checked)
                esqueletoCheckBox.Enabled = true;
            else
                esqueletoCheckBox.Enabled = false;
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            //General
            perfil.general.iniciarMaximizado = iniciarMaximizadoCheckBox.Checked;

            //Preprocesado
            //Escalado
            perfil.preprocesado.redimensionarImagen = redimensionarCheckBox.Checked;
            perfil.preprocesado.mantenerProporcion = proporcionesCheckBox.Checked;
            perfil.preprocesado.cambiarAncho = anchoCheckBox.Checked;
            perfil.preprocesado.cambiarAlto = altoCheckBox.Checked;
            perfil.preprocesado.pixelesAncho = (int)anchoNumericUpDown.Value;
            perfil.preprocesado.pixelesAlto = (int)altoNumericUpDown.Value;
            //Umbralizado
            perfil.preprocesado.umbral = (int)umbralNumericUpDown.Value;
            //Enderezado
            perfil.preprocesado.enderezadoAutomatico = enderezadoAutomaticoRadioButton.Checked;
            
            //Segmentación
            //Extracción de líneas
            perfil.segmentacion.interlineadoMedio = (int)interlineadoMedioNumericUpDown.Value;
            perfil.segmentacion.incrementoMinimoLineas = (int)incrementoMinimoLineasNumericUpDown.Value;
            //Slope
            perfil.segmentacion.corregirSlope = activarCorregirSlopeCheckBox.Checked;
            perfil.segmentacion.distanciaMinimaSlope = (int)distanciaMinimaSlopeNumericUpDown.Value;
            //Slant            
            perfil.segmentacion.corregirSlant = activarCorregirSlantCheckBox.Checked;
            perfil.segmentacion.distanciaMinimaSlant = (int)distanciaMinimaSlantNumericUpDown.Value;
            //Extracción de palabras
            perfil.segmentacion.espaciadoMedio = (int)espaciadoMedioNumericUpDown.Value;
            //Extracción de caracteres
            perfil.segmentacion.grosorMedioTrazo = (int)grosorMedioTrazoNumericUpDown.Value;
            perfil.segmentacion.incrementoMinimoCaracteres = (double)incrementoMinimoCaracteresNumericUpDown.Value;
            //Esqueleto
            perfil.segmentacion.calcularEsqueleto = activarEsqueletoCheckBox.Checked;
            perfil.segmentacion.precaracteres = esqueletoCheckBox.Checked;

            //Reconocimiento


            if (!perfil.Guardar())
                MessageBox.Show("Error al guardar la configuración. ", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            Close();
        }

        
       

        
        
    }
}