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
    public partial class PalabrasForm : Form
    {
        private PrincipalForm formPadre;
        private TextoManejado copiaTexto;

        public PalabrasForm(PrincipalForm Padre)
        {
            InitializeComponent();

            formPadre = (PrincipalForm)Padre;
            copiaTexto = formPadre.textoSegmentado;

            espaciadoMedioNumericUpDown.Value = formPadre.perfilActual.segmentacion.espaciadoMedio;
        }

        private void espaciadoMedioNumericUpDown_ValueChanged(object sender, EventArgs e)
        {
            if (previsualizarCheckBox.Checked)
            {
                if (formPadre.textoSegmentado != copiaTexto)
                    formPadre.textoSegmentado.LiberarTextoManejado();

                formPadre.textoSegmentado = copiaTexto.Copia();

                formPadre.textoSegmentado.SegmentacionPalabras((int)espaciadoMedioNumericUpDown.Value, true);

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

                formPadre.textoSegmentado.SegmentacionPalabras((int)espaciadoMedioNumericUpDown.Value, true);

                formPadre.CargarImagenSegmentada();
            }
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            formPadre.textoSegmentado = copiaTexto.Copia();

            formPadre.deshabilitarMenus("Segmentado palabras");

            habilitarBotonCerrar(false);

            this.Enabled = false;

            obtenerPalabrasBackgroundWorker.RunWorkerAsync();                
        }

        private void cancelarButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void PalabrasForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            formPadre.textoSegmentado = copiaTexto;

            formPadre.CargarImagenSegmentada();
        }

        private void obtenerPalabrasBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            formPadre.conometro.Start();

            //El false es para que no pinte los límites de las palabras
            formPadre.textoSegmentado.SegmentacionPalabras((int)espaciadoMedioNumericUpDown.Value, false);

            formPadre.conometro.Stop();
        }

        private void obtenerPalabrasBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            formPadre.habilitarMenus("Segmentado palabras");

            if (formPadre.textoSegmentado != copiaTexto)
            {
                copiaTexto.LiberarTextoManejado();
                copiaTexto = formPadre.textoSegmentado;
            }

            formPadre.estadoImagenSegmentada = EstadoImagen.palabrasSegmentadas;

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