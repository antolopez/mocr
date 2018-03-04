using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Imagen;
using System.Runtime.InteropServices;

namespace OCR.Segmentación
{
    public partial class SlopeForm : Form
    {
        private PrincipalForm formPadre;
        private TextoManejado copiaTexto;
       
        public SlopeForm(PrincipalForm Padre)
        {
            InitializeComponent();

            formPadre = (PrincipalForm)Padre;
            copiaTexto = formPadre.textoSegmentado;

            distanciaMinimaNumericUpDown.Value = formPadre.perfilActual.segmentacion.distanciaMinimaSlope;
        }

        private void distanciaMinimaNumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoSegmentado != copiaTexto)
                    formPadre.textoSegmentado.LiberarTextoManejado();

                formPadre.textoSegmentado = copiaTexto.Copia();

                formPadre.textoSegmentado.CorregirSlope((int)distanciaMinimaNumericUpDown.Value);

                formPadre.CargarImagenSegmentada();
            }
        }

        private void previsualizarCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoSegmentado != copiaTexto)
                    formPadre.textoSegmentado.LiberarTextoManejado();

                formPadre.textoSegmentado = copiaTexto.Copia();

                formPadre.textoSegmentado.CorregirSlope((int)distanciaMinimaNumericUpDown.Value);

                formPadre.CargarImagenSegmentada();
            }
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            formPadre.textoSegmentado = copiaTexto.Copia();           

            formPadre.deshabilitarMenus("Correción Slope");

            habilitarBotonCerrar(false);

            this.Enabled = false;

            corregirSlopeBackgroundWorker.RunWorkerAsync();             
        }

        private void cancelarButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void SlopeForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            formPadre.textoSegmentado = copiaTexto;

            formPadre.CargarImagenSegmentada();
        }

        private void corregirSlopeBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            formPadre.conometro.Start();

            formPadre.textoSegmentado.CorregirSlope((int)distanciaMinimaNumericUpDown.Value);

            formPadre.conometro.Stop();
        }

        private void corregirSlopeBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            formPadre.habilitarMenus("Correción Slope");

            if (formPadre.textoSegmentado != copiaTexto)
            {
                copiaTexto.LiberarTextoManejado();
                copiaTexto = formPadre.textoSegmentado;
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