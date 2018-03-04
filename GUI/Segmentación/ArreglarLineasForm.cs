using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Imagen;

namespace OCR.Segmentación
{
    public partial class ArreglarLineasForm : Form
    {
        private PrincipalForm formPadre;
        private TextoManejado copiaTexto;
        private TextoManejado textoEscalado;

        int lineaSeleccionada;
        
        bool fusionar;

        public ArreglarLineasForm(PrincipalForm Padre)
        {
            InitializeComponent();

            lineaSeleccionada = -1;
            fusionar = false;

            formPadre = (PrincipalForm)Padre;
            copiaTexto = formPadre.textoSegmentado.Copia();
            textoEscalado = copiaTexto.Copia();

            textoEscalado.EscalacionLineal(((double)lineasSegmentadasPictureBox.Size.Width) / copiaTexto.GetAncho(), ((double)lineasSegmentadasPictureBox.Size.Height) / copiaTexto.GetAlto());

            IntPtr ptrDIB;//Puntero al DIB
            IntPtr ptrMapaBits;//Puntero al mapa de bits en sí (nos saltamos el bitmapinfoheader y la paleta, si la hay)

            unsafe//Zona no segura (transformación de punteros C++ a C#) 
            {
                ptrDIB = (IntPtr)textoEscalado.GetDIB();

                ptrMapaBits = (IntPtr)textoEscalado.GetMapaBits();
            }

            lineasSegmentadasPictureBox.Image = PrincipalForm.BitmapFromDIB(ptrDIB, ptrMapaBits);
        }

        private void lineasSegmentadasPictureBox_MouseClick(object sender, MouseEventArgs e)
        {
            int linea = textoEscalado.GetLinea(e.Y, e.X);
                    
            textoEscalado.LiberarTextoManejado();

            if(fusionar)
            {
                if(lineaSeleccionada<linea)
                    copiaTexto.Fusionar(lineaSeleccionada, linea);
                else
                    copiaTexto.Fusionar(linea, lineaSeleccionada);
            }
            
            textoEscalado = copiaTexto.Copia();
            textoEscalado.EscalacionLineal(((double)lineasSegmentadasPictureBox.Size.Width) / copiaTexto.GetAncho(), ((double)lineasSegmentadasPictureBox.Size.Height) / copiaTexto.GetAlto());

            if (!fusionar && linea != -1 && linea != lineaSeleccionada)
            {
                textoEscalado.PintarLinea(linea);
                lineaSeleccionada = linea;
            }
            else
                lineaSeleccionada = -1;

            IntPtr ptrDIB;//Puntero al DIB
            IntPtr ptrMapaBits;//Puntero al mapa de bits en sí (nos saltamos el bitmapinfoheader y la paleta, si la hay)

            unsafe//Zona no segura (transformación de punteros C++ a C#) 
            {
                ptrDIB = (IntPtr)textoEscalado.GetDIB();

                ptrMapaBits = (IntPtr)textoEscalado.GetMapaBits();
            }

            lineasSegmentadasPictureBox.Image = PrincipalForm.BitmapFromDIB(ptrDIB, ptrMapaBits);
            
            fusionar = false;

                        
        }

        private void juntarButton_Click(object sender, EventArgs e)
        {
            if(lineaSeleccionada != -1)
                fusionar = true;
            else
                fusionar = false;
        }

        private void dividirButton_Click(object sender, EventArgs e)
        {
            copiaTexto.Dividir(lineaSeleccionada);

            textoEscalado.LiberarTextoManejado();

            textoEscalado = copiaTexto.Copia();
            textoEscalado.EscalacionLineal(((double)lineasSegmentadasPictureBox.Size.Width) / copiaTexto.GetAncho(), ((double)lineasSegmentadasPictureBox.Size.Height) / copiaTexto.GetAlto());

            IntPtr ptrDIB;//Puntero al DIB
            IntPtr ptrMapaBits;//Puntero al mapa de bits en sí (nos saltamos el bitmapinfoheader y la paleta, si la hay)

            unsafe//Zona no segura (transformación de punteros C++ a C#) 
            {
                ptrDIB = (IntPtr)textoEscalado.GetDIB();

                ptrMapaBits = (IntPtr)textoEscalado.GetMapaBits();
            }

            lineasSegmentadasPictureBox.Image = PrincipalForm.BitmapFromDIB(ptrDIB, ptrMapaBits);

            lineaSeleccionada = -1;
        }

        private void cancelarButton_Click(object sender, EventArgs e)
        {
            copiaTexto.LiberarTextoManejado();
            textoEscalado.LiberarTextoManejado();

            Close();
        }

        private void aceptarButton_Click(object sender, EventArgs e)
        {
            formPadre.textoSegmentado.LiberarTextoManejado();

            formPadre.textoSegmentado = copiaTexto;

            textoEscalado.LiberarTextoManejado();

            formPadre.CargarImagenSegmentada();

            Close();
        }
        
    }
}