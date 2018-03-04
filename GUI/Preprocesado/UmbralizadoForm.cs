using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Imagen;
using OCR.Perfiles;
using System.Runtime.InteropServices;

namespace OCR.Preprocesado
{
    public partial class UmbralizadoForm : Form
    {
        private PrincipalForm formPadre;
        private TextoManejado copiaTexto;
        private int umbral;//Porque desde el hilo background no se puede acceder a la propiedad Value de la TrackBar
       
        public UmbralizadoForm(PrincipalForm Padre)
        {
            InitializeComponent();    
            
            formPadre = (PrincipalForm)Padre;
            copiaTexto = formPadre.textoActual;  
          
            umbralTrackBar.Value = formPadre.perfilActual.preprocesado.umbral;
            umbralTextBox.Text = formPadre.perfilActual.preprocesado.umbral.ToString();
        }

        private void umbralTrackBar_Scroll(object sender, EventArgs e)
        {
            umbralTextBox.Text = umbralTrackBar.Value.ToString();
        }

        private void umbralTextBox_TextChanged(object sender, EventArgs e)
        {
            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();

                formPadre.textoActual.Umbralizar(umbralTrackBar.Value);

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

                formPadre.textoActual.Umbralizar(umbralTrackBar.Value);

                formPadre.CargarImagen();

            }
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            formPadre.textoActual = copiaTexto.Copia();
            
            formPadre.deshabilitarMenus("Umbralizado");

            umbral = umbralTrackBar.Value;

            habilitarBotonCerrar(false);

            this.Enabled = false;

            umbralizadoBackgroundWorker.RunWorkerAsync();
        }

        private void cancelarButton_Click(object sender, EventArgs e)
        {
            Close();            
        }        

        private void UmbralizadoForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            formPadre.textoActual = copiaTexto;

            formPadre.CargarImagen();            
        }        

        private void umbralizadoBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            formPadre.conometro.Start();

            formPadre.textoActual.Umbralizar(umbral);

            formPadre.conometro.Stop();
        }

        private void umbralizadoBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            formPadre.habilitarMenus("Umbralizado");

            if (formPadre.textoActual != copiaTexto)
            {
                copiaTexto.LiberarTextoManejado();
                copiaTexto = formPadre.textoActual;
            }

            formPadre.estadoImagen = EstadoImagen.umbralizada;

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