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
    public partial class EscaladoForm : Form
    {
        private PrincipalForm formPadre;
        private TextoManejado copiaTexto;

        public EscaladoForm(PrincipalForm Padre)
        {
            InitializeComponent();

            formPadre = (PrincipalForm)Padre;
            copiaTexto = formPadre.textoActual;

            altoNumericUpDown.Value = copiaTexto.GetAlto();
            anchoNumericUpDown.Value = copiaTexto.GetAncho();
            proporcionesCheckBox.Checked = formPadre.perfilActual.preprocesado.mantenerProporcion;
        }

        private void anchoNumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (proporcionesCheckBox.Checked)
                altoNumericUpDown.Value = (anchoNumericUpDown.Value * copiaTexto.GetAlto()) / copiaTexto.GetAncho();

            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();                

                formPadre.textoActual.Escalacion((double)anchoNumericUpDown.Value / copiaTexto.GetAncho(), (double)altoNumericUpDown.Value / copiaTexto.GetAlto());

                formPadre.CargarImagen();
            }
        }

        private void altoNumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (proporcionesCheckBox.Checked)
                anchoNumericUpDown.Value = (copiaTexto.GetAncho() * altoNumericUpDown.Value) / copiaTexto.GetAlto();

            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoActual != copiaTexto)
                    formPadre.textoActual.LiberarTextoManejado();

                formPadre.textoActual = copiaTexto.Copia();

                formPadre.textoActual.Escalacion((double)anchoNumericUpDown.Value / copiaTexto.GetAncho(), (double)altoNumericUpDown.Value / copiaTexto.GetAlto());

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

                formPadre.textoActual.Escalacion((double)anchoNumericUpDown.Value / copiaTexto.GetAncho(), (double)altoNumericUpDown.Value / copiaTexto.GetAlto());

                formPadre.CargarImagen();                              
            }
        }        

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            formPadre.textoActual = copiaTexto.Copia();

            formPadre.deshabilitarMenus("Escalado");

            habilitarBotonCerrar(false);

            this.Enabled = false;

            escaladoBackgroundWorker.RunWorkerAsync();            
        }

        private void cancelarButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void EscaladoForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            formPadre.textoActual = copiaTexto;

            formPadre.CargarImagen();
        }

        private void escaladoBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            formPadre.conometro.Start();

            formPadre.textoActual.Escalacion((double)anchoNumericUpDown.Value / copiaTexto.GetAncho(), (double)altoNumericUpDown.Value / copiaTexto.GetAlto());

            formPadre.conometro.Stop();
        }

        private void escaladoBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            formPadre.habilitarMenus("Escalado");

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