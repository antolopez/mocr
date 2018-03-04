using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Reflection;
using Imagen;

namespace OCR
{
    public partial class ImagenActualForm : Form
    {    
        private Etapa etapaProcesado;
        private bool cerrarHabilitado = true;

        public ImagenActualForm(Etapa etapa)
        {
            InitializeComponent();     
     
            etapaProcesado = etapa;

            if(etapa == Etapa.preprocesado)
                Text = "Preprocesado";

            if(etapa == Etapa.segmentacion)
                Text = "Segmentación";            
        }

        public void CargarImagen(Bitmap textoInicial)
        {
            if(textoOriginalPictureBox.Image!=null)
                textoOriginalPictureBox.Image.Dispose();

            textoOriginalPictureBox.Image = new Bitmap(textoInicial);            
            
            //Actualizamos el tamaño de la ventana
            ClientSize = new Size(textoOriginalPictureBox.Image.Size.Width, textoOriginalPictureBox.Image.Size.Height);

            //Actualizamos el tamaño del PictureBox
            textoOriginalPictureBox.Size = textoOriginalPictureBox.Image.Size;
            
        }

        private void ImagenActualForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (etapaProcesado == Etapa.preprocesado)
            {
                ((PrincipalForm)MdiParent).textoActual.LiberarTextoManejado();
                textoOriginalPictureBox.Image = null;
                ((PrincipalForm)MdiParent).ImagenCerrada();
            }

            if (etapaProcesado == Etapa.segmentacion)
            {
                ((PrincipalForm)MdiParent).textoSegmentado.LiberarTextoManejado();
                textoOriginalPictureBox.Image = null;
                ((PrincipalForm)MdiParent).CerrarImagenSegmentada();
            }
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

        private void ImagenActualForm_Resize(object sender, EventArgs e)
        {
            habilitarBotonCerrar(cerrarHabilitado);
        }

        private void textoOriginalPictureBox_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}