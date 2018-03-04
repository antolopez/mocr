using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;
using System.Reflection;
using System.IO;

namespace OCR
{
    public partial class TextoReconocidoForm : Form
    {
        private PrincipalForm formPadre;
        private String texto;
        private String clasificadorSeleccionado;
        private bool cerrarHabilitado = true;

        public TextoReconocidoForm(Form padre)
        {
            InitializeComponent();

            this.formPadre = (PrincipalForm)padre;

            clasificadorComboBox.SelectedIndex = 0;
        }        

        private void ejecutarButton_Click(object sender, EventArgs e)
        {
            formPadre.textoReconocido.SetPerfil(formPadre.perfilActual.GetNombre());            
            formPadre.textoReconocido.SetRutaBBDD(Application.StartupPath + "\\BBDD\\bbdd.db");            
            formPadre.textoReconocido.SetTextoReconocido(textoReconocidoRichTextBox.Text);
            
            ejecutarButton.Enabled = false;
            configuracionButton.Enabled = false;
            corregirButton.Enabled = false;            
            ejecutarProgressBar.Style = ProgressBarStyle.Marquee;

            //Nos guardamos esto porque no deja hacerlo en el hilo background
            clasificadorSeleccionado = (String)clasificadorComboBox.SelectedItem;

            if(aprendizajeRadioButton.Checked)
                formPadre.actualizarBarraEstado(true, "Aprendizaje");
            else
                formPadre.actualizarBarraEstado(true, "Reconocimiento");

            habilitarBotonCerrar(false);

            try
            {
                ejecutarBackgroundWorker.RunWorkerAsync();
            }
            catch (Exception ex)
            {
                texto = ex.ToString();
            }
        }

        private void aprendizajeRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            clasificadorGroupBox.Enabled = false;
            corregirButton.Enabled = false;
        }

        private void reconocimientoRadioButton_CheckedChanged(object sender, EventArgs e)
        {
            clasificadorGroupBox.Enabled = true;                      
        }

        private void configuracionButton_Click(object sender, EventArgs e)
        {
            switch ((String)clasificadorComboBox.SelectedItem)
            {
                case "KNN":
                    OpcionesKNNForm opciones = new OpcionesKNNForm(formPadre.textoReconocido);
                    opciones.ShowDialog();
                    break;
                case "Naive Bayes":
                    
                    break;
            }
        }

        private void corregirButton_Click(object sender, EventArgs e)
        {
            formPadre.textoReconocido.SetTextoReconocido(textoReconocidoRichTextBox.Text);

            ejecutarButton.Enabled = false;
            corregirButton.Enabled = false;
            configuracionButton.Enabled = false;
            ejecutarProgressBar.Style = ProgressBarStyle.Marquee;

            formPadre.actualizarBarraEstado(true, "Corregir");

            habilitarBotonCerrar(false);

            corregirBackgroundWorker.RunWorkerAsync();            
        }

        private void exportarButton_Click(object sender, EventArgs e)
        {
            exportarTextoSaveFileDialog.ShowDialog();
        }

        private void exportarTextoSaveFileDialog_FileOk(object sender, CancelEventArgs e)
        {
            if (exportarTextoSaveFileDialog.FileNames.Length > 0)//Se ha seleccionado un archivo
                File.WriteAllText(exportarTextoSaveFileDialog.FileName, textoReconocidoRichTextBox.Text);
        }

        private void textoReconocidoRichTextBox_TextChanged(object sender, EventArgs e)
        {
            if(textoReconocidoRichTextBox.Text!="")
                exportarButton.Enabled = true;
            else
                exportarButton.Enabled = false;
        }

        private void ejecutarBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            formPadre.conometro.Start();

            Clasificador();
            Reconocer();

            formPadre.conometro.Stop();
        }

        private void ejecutarBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (texto != null)
                textoReconocidoRichTextBox.Text = texto;
            else
                if(aprendizajeRadioButton.Checked)
                    textoReconocidoRichTextBox.Text = "El texto insertado no coincide con la segmentación. No se ha realizado aprendizaje.";
                else
                    textoReconocidoRichTextBox.Text = "Error al realizar el reconocimiento";
                        
            if (reconocimientoRadioButton.Checked)
                corregirButton.Enabled = true;

            ejecutarButton.Enabled = true;
            configuracionButton.Enabled = true;
            ejecutarProgressBar.Style = ProgressBarStyle.Blocks;

            habilitarBotonCerrar(true);

            if (aprendizajeRadioButton.Checked)
                formPadre.actualizarBarraEstado(false, "Aprendizaje");
            else
                formPadre.actualizarBarraEstado(false, "Reconocimiento");
        }

        private void corregirBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            formPadre.conometro.Start();

            int codigo = formPadre.textoReconocido.CorregirTexto();

            formPadre.conometro.Stop();

            if (codigo == 3)
                textoReconocidoRichTextBox.Text = "El número de líneas y palabras segmentadas no coincide con la corrección. No se ha realizado aprendizaje.";
        }

        private void corregirBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            ejecutarButton.Enabled = true;
            configuracionButton.Enabled = true;
            ejecutarProgressBar.Style = ProgressBarStyle.Blocks;

            habilitarBotonCerrar(true);

            formPadre.actualizarBarraEstado(false, "Corregir");
        }

        private void Clasificador()
        {
            int clasificador = -1;

            switch (clasificadorSeleccionado)
            {
                case "KNN":
                    clasificador = 0;
                    break;
                case "Naive Bayes":
                    clasificador = 1;
                    break;
            }

            if (reconocimientoRadioButton.Checked)
                formPadre.textoReconocido.CrearClasificador(clasificador);
        }

        private void Reconocer()
        {
            texto = formPadre.textoReconocido.Reconocer(reconocimientoRadioButton.Checked);
        }

        //Importación de procedimientos externos almacenados
        //en la librería de Windows USER32.DLL
        //Quitar elementos del menú de sistema
        [DllImport("user32.Dll")]
        private static extern bool EnableMenuItem(IntPtr hMenu, int uIDEnableItem, int uEnable);
        //Obtener el menú de sistema
        [DllImport("User32.Dll")]
        private static extern IntPtr GetSystemMenu(IntPtr hWnd, bool bRevert);

        public void habilitarBotonCerrar(bool habilitado)
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

            cerrarHabilitado = habilitado;

            /*if(!habilitado)
                textoOriginalPictureBox.Image = null;*/
        }
    }
}