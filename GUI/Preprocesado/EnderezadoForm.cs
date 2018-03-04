using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Imagen;
using System.Runtime.InteropServices;

namespace OCR.Preprocesado
{
    public partial class EnderezadoForm : Form
    {
        private PrincipalForm formPadre;
        private TextoManejado copiaTexto;
        private int direccion;
        private double grados;//Porque desde el hilo background no se puede acceder a la propiedad Value de la TrackBar

        public EnderezadoForm(PrincipalForm Padre)
        {
            InitializeComponent();

            formPadre = (PrincipalForm)Padre;
            copiaTexto = formPadre.textoActual;

            direccion = 1;

            if(formPadre.estadoImagen == EstadoImagen.cargada)//La imagen no está umbralizada aún
                automaticoRadioButton.Enabled = false;
        }

        private void derechaRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            direccion = 1;

            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();

                if (manualRadioButton.Checked)
                    formPadre.textoActual.Rotacion(direccion * gradosTrackBar.Value);
                else
                    formPadre.textoActual.Enderezar();

                formPadre.CargarImagen();
            }
        }

        private void izquierdaRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            direccion = -1;

            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();

                if (manualRadioButton.Checked)
                    formPadre.textoActual.Rotacion(direccion * gradosTrackBar.Value);
                else
                    formPadre.textoActual.Enderezar();

                formPadre.CargarImagen();
            }
        }

        private void gradosTrackBar_Scroll(object sender, EventArgs e)
        {
            gradosTextBox.Text = gradosTrackBar.Value.ToString();

            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();

                if (manualRadioButton.Checked)
                    formPadre.textoActual.Rotacion(direccion * gradosTrackBar.Value);
                else
                    formPadre.textoActual.Enderezar();

                formPadre.CargarImagen();
            }
        }

        private void manualRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            direccionGroupBox.Enabled = true;
            gradosGroupBox.Enabled = true;

            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();

                if (manualRadioButton.Checked)
                    formPadre.textoActual.Rotacion(direccion * gradosTrackBar.Value);
                else
                    formPadre.textoActual.Enderezar();

                formPadre.CargarImagen();
            }
        }

        private void automaticoRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            direccionGroupBox.Enabled = false;
            gradosGroupBox.Enabled = false;

            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();

                if (manualRadioButton.Checked)
                    formPadre.textoActual.Rotacion(direccion * gradosTrackBar.Value);
                else
                    formPadre.textoActual.Enderezar();

                formPadre.CargarImagen();
            }
        }

        private void previsualizarCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();

                if (manualRadioButton.Checked)
                    formPadre.textoActual.Rotacion(direccion * gradosTrackBar.Value);
                else
                    formPadre.textoActual.Enderezar();

                formPadre.CargarImagen();                
            }
        }        

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            formPadre.textoActual = copiaTexto.Copia();

            formPadre.deshabilitarMenus("Enderezado");

            grados = gradosTrackBar.Value;

            habilitarBotonCerrar(false);

            this.Enabled = false;

            enderezadoBackgroundWorker.RunWorkerAsync();             
        }

        private void cancelarButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void EnderezadoForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            formPadre.textoActual = copiaTexto;

            formPadre.CargarImagen();            
        }        

        private void enderezadoBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            formPadre.conometro.Start();

            if (manualRadioButton.Checked)
                formPadre.textoActual.Rotacion(direccion * grados);
            else
                formPadre.textoActual.Enderezar();

            formPadre.conometro.Stop();
        }

        private void enderezadoBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            formPadre.habilitarMenus("Enderezado");

            if (formPadre.textoActual != copiaTexto)
            {
                copiaTexto.LiberarTextoManejado();
                copiaTexto = formPadre.textoActual;
            }

            habilitarBotonCerrar(true);

            Close();   
        }

        //Importación de procedimientos externos almacenados
        //en la librería de Windows USER32.DLL
        //Quitar elementos del menú de sistema
        [DllImport("user32.Dll")]
        private static extern bool EnableMenuItem(IntPtr hMenu, int uIDEnableItem, int uEnable);
        //Obtener el menú de sistema
        [DllImport("User32.Dll")]
        private static extern IntPtr GetSystemMenu(IntPtr hWnd, bool bRevert);

        private void habilitarBotonCerrar(bool habilitado)
        {
            const int SC_CLOSE = 0x0F060;
            const int MF_BYCOMMAND = 0x0;
            const int MF_ENABLED = 0x0;
            const int MF_GRAYED = 0x1;
            const int MF_DISABLED = 0x2;

            IntPtr hSysMenu = GetSystemMenu(this.Handle, false);
            int flags = MF_BYCOMMAND;

            if (habilitado)
                flags |= MF_ENABLED;
            else
                flags |= MF_DISABLED | MF_GRAYED;

            EnableMenuItem(hSysMenu, SC_CLOSE, flags);
        }
    }
}