using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.Runtime.InteropServices;
using System.IO;
using System.Reflection;
using Escaner;
using TwainLib;
using Imagen;
using OCR.Perfiles;
using OCR.Ayuda;
using OCR.Preprocesado;
using OCR.Segmentación;
using BBDD;
using System.Threading;
using System.Diagnostics;

namespace OCR
{

    public enum EstadoImagen
    {
        nula,
        cargada,
        umbralizada,
        lineasSegmentadas,
        palabrasSegmentadas,
        caracteresSegmentados,
        reconocida
    }

    public enum Etapa
    {
        preprocesado,
        segmentacion,
        reconocimiento
    }

    public partial class PrincipalForm : Form, IMessageFilter
    {
        //Atributos
        public TextoManejado textoActual;//Imagen con el texto manuscrito a procesar
        public TextoManejado textoSegmentado;//Imagen en la que iremos mostrando el proceso de Segmentación
        public TextoManejado textoReconocido;
        public ImagenActualForm imagenPreprocesada;
        public ImagenActualForm imagenSegmentada;
        ConexionEscaner escaner;
        public Configuracion configuracionGeneral;
        public Perfil perfilActual;
        private bool mensajesActivados;//Para saber si estamos recibiendo mensajes del escaner o no
        public EstadoImagen estadoImagen;
        public EstadoImagen estadoImagenSegmentada;
        public Stopwatch conometro;
  
        
        //Dll´s importadas
        //Importamos funciones para poder cargar y escanear imágenes
        [DllImport("GdiPlus.dll", CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int GdipCreateBitmapFromGdiDib(IntPtr pBIH, IntPtr pPix, out IntPtr pBitmap);
        [DllImport("kernel32.dll", ExactSpelling = true)]
        internal static extern IntPtr GlobalLock(IntPtr handle);
        [DllImport("kernel32.dll", ExactSpelling = true)]
        internal static extern IntPtr GlobalFree(IntPtr handle);

        
        //Métodos

        //Constructor
        public PrincipalForm()
        {
            InitializeComponent();          
            
            //Inicializaciones
            textoActual = new TextoManejado();
            estadoImagen = EstadoImagen.nula;
            
            textoSegmentado = new TextoManejado();

            escaner = new ConexionEscaner(this.Handle);

            if (!escaner.Inicializar())
            {
                twainToolStripMenuItem.Enabled = false;
                twainToolStripButton.Enabled = false;
            }

            imagenPreprocesada = new ImagenActualForm(Etapa.preprocesado);
            imagenSegmentada = new ImagenActualForm(Etapa.segmentacion);

            conometro = new Stopwatch();

            perfilesLabel.Text = "Perfil actual:";
            TransparencyKey = Color.Red;

            tareaToolStripStatusLabel.Alignment = ToolStripItemAlignment.Right;
            barraEstadoProgressBar.Alignment = ToolStripItemAlignment.Right;

            CargarConfiguracion();

            ActualizarPerfiles();

            perfilesComboBox.SelectedItem = configuracionGeneral.perfilInicial;

            CargarPerfil();

            if(perfilActual.general.iniciarMaximizado)
                WindowState = FormWindowState.Maximized;           
        }   
     

        //Menú Archivo
        //Métodos Abrir Imagen
        private void abrirToolStripMenuItem_Click(object sender, EventArgs e)
        {
            abrirFileDialog.ShowDialog();
        }

        private void abrirFileDialog_FileOk(object sender, CancelEventArgs e)
        {
            if (abrirFileDialog.FileNames.Length > 0)//Se ha seleccionado un archivo
            {
                textoActual.LiberarTextoManejado();

                textoActual.Cargar(abrirFileDialog.FileName);

                estadoImagen = EstadoImagen.nula;

                CargarImagen();
            }
        }

        //Métodos Guardar Imagen
        private void guardarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            guardarFileDialog.ShowDialog();
        }

        private void guardarFileDialog_FileOk(object sender, CancelEventArgs e)
        {
            if (guardarFileDialog.FileNames.Length > 0)//Se ha seleccionado un archivo
            {
                textoActual.Guardar(guardarFileDialog.FileName);
            }
        }

        //Métodos Escanear Imagen
        private void twainToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (escaner.SeleccionarEscaner())
            {
                if (!mensajesActivados)
                {
                    this.Enabled = false;

                    mensajesActivados = true;

                    Application.AddMessageFilter(this);
                }

                if (!escaner.Adquirir())
                {
                    Terminar();

                    escaner.CerrarConexion();
                }
            }            
        }

        //Métodos auxiliares para escanear
        bool IMessageFilter.PreFilterMessage(ref Message m)
        {
            TwainCommand orden = escaner.RecibirMensaje(ref m);

            if (orden == TwainCommand.Not)
                return false;

            switch (orden)
            {
                case TwainCommand.CloseRequest:
                    {
                        Terminar();
                        escaner.CerrarConexion();
                        break;
                    }
                case TwainCommand.CloseOk:
                    {
                        Terminar();
                        escaner.CerrarConexion();
                        break;
                    }
                case TwainCommand.DeviceEvent:
                    {
                        break;
                    }
                case TwainCommand.TransferReady:
                    {
                        ArrayList imagenes = escaner.TransferirImagenes();
                        Terminar();
                        escaner.CerrarConexion();

                        for (int i = 0; i < imagenes.Count; i++)
                        {
                            //byte []nuevaImagen;
                            IntPtr imagen = (IntPtr)imagenes[i];

                            //Arreglar 
                            textoActual = new TextoManejado(GlobalLock(imagen));

                            textoActual = textoActual.Copia();

                            GlobalFree(imagen);

                            estadoImagen = EstadoImagen.nula;

                            CargarImagen();
                        }
                        break;
                    }
            }

            return true;
        }

        private void Terminar()
        {
            if (mensajesActivados)
            {
                Application.RemoveMessageFilter(this);
                mensajesActivados = false;
                this.Enabled = true;
                this.Activate();
            }
        }

        //Métodos Configuración
        private void configuracionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                ConfiguracionGeneralForm configuracion = new ConfiguracionGeneralForm(this);

                configuracion.ShowDialog();
            }
            catch (InvalidDataException)
            {
                MessageBox.Show("Formato de archivo de configuración incorrecto. ", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void CargarConfiguracion()
        {
            configuracionGeneral = new Configuracion();

            if (!configuracionGeneral.Cargar())
            {
                MessageBox.Show("No se encontró el archivo de configuración o tiene un formato incorrecto. ", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        //Fin Menú Archivo

        
        //Menú Editar
        //Métodos Perfiles
        private void perfilesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PerfilesForm perfiles = new PerfilesForm(this);

            perfiles.ShowDialog();

            ActualizarPerfiles();

            CargarPerfil();
        }

        private void perfilesComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            CargarPerfil();
        }

        private void ActualizarPerfiles()
        {
            String perfilSeleccionado = (String)perfilesComboBox.SelectedItem;

            perfilesComboBox.Items.Clear();

            //Cargamos los distintos perfiles disponibles en el combo box
            if (!Directory.Exists(Application.StartupPath + "\\Perfiles"))
                Directory.CreateDirectory(Application.StartupPath + "\\Perfiles");

            String[] perfiles = Directory.GetFiles(Application.StartupPath + "\\Perfiles");

            foreach (String perfil in perfiles)
                perfilesComboBox.Items.Add(perfil.Substring(perfil.LastIndexOf("\\") + 1));

            if (perfilSeleccionado != null && perfilesComboBox.Items.Contains(perfilSeleccionado))
                perfilesComboBox.SelectedItem = perfilSeleccionado;
            else
                perfilesComboBox.SelectedItem = "Predefinido";
        }

        private void CargarPerfil()
        {
            perfilActual = new Perfil();

            if (!perfilActual.Cargar(perfilesComboBox.SelectedItem.ToString()))
            {
                MessageBox.Show("Error al cargar el perfil. ", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        //Fin Menú Editar


        //Menú Preprocesado
        //Métodos Escalar
        private void escaladoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            EscaladoForm escalado = new EscaladoForm(this);

            escalado.ShowDialog();
        }

        //Métodos Umbralizar
        private void umbralizarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            UmbralizadoForm umbralizado = new UmbralizadoForm(this);

            umbralizado.ShowDialog();            
        }

        //Métodos Enderezar
        private void enderezarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            EnderezadoForm enderezado = new EnderezadoForm(this);

            enderezado.ShowDialog();
        }

        //Métodos Automático
        private void automaticoPreprocesadoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            deshabilitarMenus("Preprocesado");

            imagenPreprocesada.habilitarBotonCerrar(false);

            preprocesadoBackgroundWorker.RunWorkerAsync();                 
        }

        private void preprocesadoBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            conometro.Start();

            if (perfilActual.preprocesado.redimensionarImagen)
            {
                double factorAlto = 1.0;
                double factorAncho = 1.0;

                if (perfilActual.preprocesado.mantenerProporcion)
                {
                    if (perfilActual.preprocesado.cambiarAlto)
                    {
                        factorAlto = ((double)perfilActual.preprocesado.pixelesAlto) / textoActual.GetAlto();
                        factorAncho = textoActual.GetAncho() * factorAlto / textoActual.GetAlto();
                    }
                    else
                        if (perfilActual.preprocesado.cambiarAncho)
                        {
                            factorAncho = ((double)perfilActual.preprocesado.pixelesAncho) / textoActual.GetAncho();
                            factorAlto = textoActual.GetAlto() * factorAncho / textoActual.GetAncho();
                        }
                }
                else
                {
                    if (perfilActual.preprocesado.cambiarAlto)
                        factorAlto = ((double)perfilActual.preprocesado.pixelesAlto) / textoActual.GetAlto();

                    if (perfilActual.preprocesado.cambiarAncho)
                        factorAncho = ((double)perfilActual.preprocesado.pixelesAncho) / textoActual.GetAncho();
                }

                textoActual.Escalacion(factorAncho, factorAlto);
            }

            textoActual.Umbralizar(perfilActual.preprocesado.umbral);
            estadoImagen = EstadoImagen.umbralizada;            

            if (perfilActual.preprocesado.enderezadoAutomatico)
                textoActual.Enderezar();

            conometro.Stop();
        }

        private void preprocesadoBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            CargarImagen();

            imagenPreprocesada.habilitarBotonCerrar(true);

            habilitarMenus("Preprocesado");
        }

        //Métodos Preferencias Preprocesado
        private void preferenciasPreprocesadoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PerfilForm perfil = new PerfilForm(perfilActual, "Preprocesado");

            perfil.ShowDialog();
        }
        //Fin Menú Preprocesado


        //Menú Segmentación
        //Métodos Segmentar Líneas
        private void líneasToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LineasForm segmentacionLineas = new LineasForm(this);

            segmentacionLineas.ShowDialog();
        }

        //Métodos Corregir Slope
        private void correcionSlopeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SlopeForm corregirSlope = new SlopeForm(this);

            corregirSlope.ShowDialog();
        }

        //Métodos Corregir Slant
        private void correcionSlantToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SlantForm corregirSlant = new SlantForm(this);

            corregirSlant.ShowDialog();
        }

        //Métodos Segmentar Palabras
        private void palabrasToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PalabrasForm segmentarPalabras = new PalabrasForm(this);

            segmentarPalabras.ShowDialog();
        }

        //Métodos Segmentar Caracteres
        private void caracteresToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CaracteresForm segmentarCaracteres = new CaracteresForm(this);

            segmentarCaracteres.ShowDialog();
        }

        //Métodos Esqueleto
        private void esqueletoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            deshabilitarMenus("Esqueleto");
            
            imagenSegmentada.habilitarBotonCerrar(false);
            
            esqueletoBackgroundWorker.RunWorkerAsync();
        }

        private void esqueletoBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            conometro.Start();

            textoSegmentado.Esqueleto();

            conometro.Stop();
        }

        private void esqueletoBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            imagenSegmentada.habilitarBotonCerrar(true);

            CargarImagenSegmentada();

            habilitarMenus("Esqueleto");
        }

        //Métodos Segmentado Semiautomático
        private void semiautomaticoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(textoSegmentado!=null)
                textoSegmentado.LiberarTextoManejado();

            textoSegmentado = textoActual.Copia();

            deshabilitarMenus("Segmentado semiautomático");

            conometro.Start();

            //Segmentación líneas
            textoSegmentado.SegmentacionLineas(perfilActual.segmentacion.interlineadoMedio, perfilActual.segmentacion.incrementoMinimoLineas);
            estadoImagenSegmentada = EstadoImagen.lineasSegmentadas;

            ArreglarLineasForm arreglar = new ArreglarLineasForm(this);
            arreglar.ShowDialog();

            if (estadoImagenSegmentada != EstadoImagen.nula)
                imagenSegmentada.habilitarBotonCerrar(false);

            segmentadoSemiautomaticoBackgroundWorker.RunWorkerAsync();            
        }

        private void segmentadoSemiautomaticoBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            //Correción Slope
            if(perfilActual.segmentacion.corregirSlope)
                textoSegmentado.CorregirSlope(perfilActual.segmentacion.distanciaMinimaSlope);

            //Correción Slant
            if(perfilActual.segmentacion.corregirSlant)
                textoSegmentado.CorregirSlant(perfilActual.segmentacion.distanciaMinimaSlant);

            //Segmentado Palabras
            textoSegmentado.SegmentacionPalabras(perfilActual.segmentacion.espaciadoMedio, false);

            estadoImagenSegmentada = EstadoImagen.palabrasSegmentadas;

            //Segmentado Caracteres y Esqueleto
            if (perfilActual.segmentacion.precaracteres)
            {
                if (perfilActual.segmentacion.calcularEsqueleto)
                    textoSegmentado.Esqueleto();

                textoSegmentado.SegmentacionCaracteres(perfilActual.segmentacion.grosorMedioTrazo, perfilActual.segmentacion.incrementoMinimoCaracteres, false);
            }
            else
            {
                textoSegmentado.SegmentacionCaracteres(perfilActual.segmentacion.grosorMedioTrazo, perfilActual.segmentacion.incrementoMinimoCaracteres, false);

                if (perfilActual.segmentacion.calcularEsqueleto)
                    textoSegmentado.Esqueleto();
            }    
       
            estadoImagenSegmentada = EstadoImagen.caracteresSegmentados;

            conometro.Stop();
        }

        private void segmentadoSemiautomaticoBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            CargarImagenSegmentada();

            imagenSegmentada.habilitarBotonCerrar(true);

            habilitarMenus("Segmentado semiautomático");
        }

        //Métodos Segmentado Automático
        private void automaticoSegmentacionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (textoSegmentado != null)
                textoSegmentado.LiberarTextoManejado();

            textoSegmentado = textoActual.Copia();

            deshabilitarMenus("Segmentado automático");

            if(estadoImagenSegmentada != EstadoImagen.nula)
                imagenSegmentada.habilitarBotonCerrar(false);

            segmentadoAutomaticoBackgroundWorker.RunWorkerAsync();    
        }

        private void segmentadoAutomaticoBackgroundWorker_DoWork(object sender, DoWorkEventArgs e)
        {
            conometro.Start();

            //Segmentación líneas
            textoSegmentado.SegmentacionLineas(perfilActual.segmentacion.interlineadoMedio, perfilActual.segmentacion.incrementoMinimoLineas);
            estadoImagenSegmentada = EstadoImagen.lineasSegmentadas;

            //Correción Slope
            if (perfilActual.segmentacion.corregirSlope)
                textoSegmentado.CorregirSlope(perfilActual.segmentacion.distanciaMinimaSlope);

            //Correción Slant
            if (perfilActual.segmentacion.corregirSlant)
                textoSegmentado.CorregirSlant(perfilActual.segmentacion.distanciaMinimaSlant);

            //Segmentado Palabras
            textoSegmentado.SegmentacionPalabras(perfilActual.segmentacion.espaciadoMedio, false);

            estadoImagenSegmentada = EstadoImagen.palabrasSegmentadas;

            //Segmentado Caracteres y Esqueleto
            if (perfilActual.segmentacion.precaracteres)
            {
                if (perfilActual.segmentacion.calcularEsqueleto)
                    textoSegmentado.Esqueleto();

                textoSegmentado.SegmentacionCaracteres(perfilActual.segmentacion.grosorMedioTrazo, perfilActual.segmentacion.incrementoMinimoCaracteres, false);
            }
            else
            {
                textoSegmentado.SegmentacionCaracteres(perfilActual.segmentacion.grosorMedioTrazo, perfilActual.segmentacion.incrementoMinimoCaracteres, false);

                if (perfilActual.segmentacion.calcularEsqueleto)
                    textoSegmentado.Esqueleto();
            }

            estadoImagenSegmentada = EstadoImagen.caracteresSegmentados;

            conometro.Stop();
        }

        private void segmentadoAutomaticoBackgroundWorker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            CargarImagenSegmentada();

            imagenSegmentada.habilitarBotonCerrar(true);

            habilitarMenus("Segmentado automático");
        }

        //Métodos Preferencias Segmentación
        private void preferenciasSegmentacionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PerfilForm perfil = new PerfilForm(perfilActual, "Segmentacion");

            perfil.ShowDialog();
        }
        //Fin Menú Segmentación


        //Menú Reconocimiento
        //Métodos Reconocimiento Manual
        private void ManualToolStripMenuItem_Click(object sender, EventArgs e)
        {
            textoReconocido = textoSegmentado.Copia();

            TextoReconocidoForm t = new TextoReconocidoForm(this);
            t.MdiParent = this;
            t.Show();
        }

        //Métodos Preferencias Reconocimiento
        private void preferenciasReconocimientoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            PerfilForm perfil = new PerfilForm(perfilActual, "Reconocimiento");

            perfil.ShowDialog();
        }
        //Fin Menú Reconocimiento


        //Menú Ayuda
        //Métodos Acerca De
        private void acercaDeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AcercaDeForm acercaDe = new AcercaDeForm();

            acercaDe.ShowDialog();
        }
        //Fin Menú Ayuda

       
        


        //Métodos auxiliares
       
        //Método auxiliar para obtener un Bitmap a partir de un puntero
        public static Bitmap BitmapFromDIB(IntPtr pDIB, IntPtr pPix)
        {
            MethodInfo infoMetodo = typeof(Bitmap).GetMethod("FromGDIplus", BindingFlags.Static | BindingFlags.NonPublic);

            if (infoMetodo == null)
                return null; //Problema de permisos

            IntPtr pBmp = IntPtr.Zero;//Puntero a bitnap devuelto por la función de la librería Gdi
            int status = GdipCreateBitmapFromGdiDib(pDIB, pPix, out pBmp);

            if ((status == 0) && (pBmp != IntPtr.Zero))//Bitmap correcto
                return (Bitmap)infoMetodo.Invoke(null, new object[] { pBmp });
            else
                return null; //Fallo en la creación del Bitmap
        }

        //Métodos para cargar imágenes en el form
        public void CargarImagen()
        {
            IntPtr ptrDIB;//Puntero al DIB
            IntPtr ptrMapaBits;//Puntero al mapa de bits en sí (nos saltamos el bitmapinfoheader y la paleta, si la hay)

            unsafe//Zona no segura (transformación de punteros C++ a C#) 
            {
                ptrDIB = (IntPtr)textoActual.GetDIB();

                ptrMapaBits = (IntPtr)textoActual.GetMapaBits();
            }

            //Cargamos la imagen en un Form hijo
            if (imagenPreprocesada.IsDisposed)
                imagenPreprocesada = new ImagenActualForm(Etapa.preprocesado);
            imagenPreprocesada.CargarImagen(BitmapFromDIB(ptrDIB, ptrMapaBits));
            imagenPreprocesada.MdiParent = this;//Indicamos que PrincipalForm es el padre del formulario        
            imagenPreprocesada.Show();

            
            //Habilitado y deshabilitado de menús
            if (estadoImagen == EstadoImagen.nula)
            {
                estadoImagen = EstadoImagen.cargada;

                guardarToolStripMenuItem.Enabled = true;
                guardarImagenToolStripButton.Enabled = true;

                preprocesadoToolStripMenuItem.Enabled = false;
                manualPreprocesadoToolStripMenuItem.Enabled = false;
                escalarToolStripButton.Enabled = false;
                escaladoToolStripMenuItem.Enabled = false;
                umbralizarToolStripButton.Enabled = false;
                umbralizarToolStripMenuItem.Enabled = false;
                enderezarToolStripButton.Enabled = false;
                enderezarToolStripMenuItem.Enabled = false;
                automaticoPreprocesadoToolStripMenuItem.Enabled = false;

                lineasToolStripButton.Enabled = false;
                lineasToolStripMenuItem.Enabled = false;                
                semiautomaticoToolStripMenuItem.Enabled = false;
                automaticoSegmentacionToolStripMenuItem.Enabled = false;

                if (textoActual.GetProfundidad() == 8 || textoActual.GetProfundidad() == 24)
                {
                    preprocesadoToolStripMenuItem.Enabled = true;
                    manualPreprocesadoToolStripMenuItem.Enabled = true;
                    escalarToolStripButton.Enabled = true;
                    escaladoToolStripMenuItem.Enabled = true;
                    umbralizarToolStripButton.Enabled = true;
                    umbralizarToolStripMenuItem.Enabled = true;
                    enderezarToolStripButton.Enabled = true;
                    enderezarToolStripMenuItem.Enabled = true;   
                    automaticoPreprocesadoToolStripMenuItem.Enabled = true;

                    if (textoActual.EstaUmbralizado())
                    {
                        estadoImagen = EstadoImagen.umbralizada;           
             
                        umbralizarToolStripButton.Enabled = false;
                        umbralizarToolStripMenuItem.Enabled = false;

                        segmentaciónToolStripMenuItem.Enabled = true;
                        manualSegmentacionToolStripMenuItem.Enabled = true;
                        lineasToolStripButton.Enabled = true;
                        lineasToolStripMenuItem.Enabled = true;
                        semiautomaticoToolStripMenuItem.Enabled = true;
                        automaticoSegmentacionToolStripMenuItem.Enabled = true;
                    }
                }
            }
            else
            {
                if (estadoImagen == EstadoImagen.umbralizada)
                {
                    umbralizarToolStripButton.Enabled = false;
                    umbralizarToolStripMenuItem.Enabled = false;

                    segmentaciónToolStripMenuItem.Enabled = true;
                    manualSegmentacionToolStripMenuItem.Enabled = true;
                    lineasToolStripButton.Enabled = true;
                    lineasToolStripMenuItem.Enabled = true;
                    semiautomaticoToolStripMenuItem.Enabled = true;
                    automaticoSegmentacionToolStripMenuItem.Enabled = true;
                }
            }
        }

        public void CargarImagenSegmentada()
        {
            IntPtr ptrDIB;//Puntero al DIB
            IntPtr ptrMapaBits;//Puntero al mapa de bits en sí (nos saltamos el bitmapinfoheader y la paleta, si la hay)

            unsafe//Zona no segura (transformación de punteros C++ a C#) 
            {
                ptrDIB = (IntPtr)textoSegmentado.GetDIB();

                ptrMapaBits = (IntPtr)textoSegmentado.GetMapaBits();
            }

            //Cargamos la imagen en un Form hijo
            if (imagenSegmentada.IsDisposed)
                imagenSegmentada = new ImagenActualForm(Etapa.segmentacion);
            imagenSegmentada.CargarImagen(BitmapFromDIB(ptrDIB, ptrMapaBits));
            imagenSegmentada.MdiParent = this;//Indicamos que PrincipalForm es el padre del formulario        
            imagenSegmentada.Show();


            //Habilitado y deshabilitado de menús
            if(estadoImagenSegmentada == EstadoImagen.lineasSegmentadas)
            {
                correcionSlopeToolStripButton.Enabled = true;
                correcionSlopeToolStripMenuItem.Enabled = true;                
                correcionSlantToolStripButton.Enabled = true;
                correcionSlantToolStripMenuItem.Enabled = true;                
                palabrasToolStripButton.Enabled = true;
                palabrasToolStripMenuItem.Enabled = true;
                caracteresToolStripButton.Enabled = false;
                caracteresToolStripMenuItem.Enabled = false;
                esqueletoToolStripButton.Enabled = true;
                esqueletoToolStripMenuItem.Enabled = true;
                
                reconocimientoToolStripMenuItem.Enabled = false;
                reconocimientoToolStripButton.Enabled = false;
                ManualToolStripMenuItem.Enabled = false;
            }

            if(estadoImagenSegmentada == EstadoImagen.palabrasSegmentadas)
            {                
                correcionSlopeToolStripButton.Enabled = false;
                correcionSlopeToolStripMenuItem.Enabled = false;
                correcionSlantToolStripButton.Enabled = false;
                correcionSlantToolStripMenuItem.Enabled = false;                
                palabrasToolStripButton.Enabled = false;
                palabrasToolStripMenuItem.Enabled = false;                
                caracteresToolStripButton.Enabled = true;
                caracteresToolStripMenuItem.Enabled = true;
                esqueletoToolStripButton.Enabled = true;
                esqueletoToolStripMenuItem.Enabled = true;

                reconocimientoToolStripMenuItem.Enabled = false;
                reconocimientoToolStripButton.Enabled = false;
                ManualToolStripMenuItem.Enabled = false;                
            }

            if(estadoImagenSegmentada == EstadoImagen.caracteresSegmentados)
            {
                correcionSlopeToolStripButton.Enabled = false;
                correcionSlopeToolStripMenuItem.Enabled = false;
                correcionSlantToolStripButton.Enabled = false;
                correcionSlantToolStripMenuItem.Enabled = false;
                palabrasToolStripButton.Enabled = false;
                palabrasToolStripMenuItem.Enabled = false;
                caracteresToolStripButton.Enabled = false;
                caracteresToolStripMenuItem.Enabled = false;
                esqueletoToolStripButton.Enabled = true;
                esqueletoToolStripMenuItem.Enabled = true;

                reconocimientoToolStripMenuItem.Enabled = true;
                reconocimientoToolStripButton.Enabled = true;
                ManualToolStripMenuItem.Enabled = true;  
            }

        }

        //Métodos para cerrar las imágenes de los forms
        public void ImagenCerrada()
        {
            estadoImagen = EstadoImagen.nula;

            guardarToolStripMenuItem.Enabled = false;
            guardarImagenToolStripButton.Enabled = false;

            preprocesadoToolStripMenuItem.Enabled = false;
            escalarToolStripButton.Enabled = false;
            escaladoToolStripMenuItem.Enabled = false;
            umbralizarToolStripButton.Enabled = false;
            umbralizarToolStripMenuItem.Enabled = false;
            enderezarToolStripButton.Enabled = false;
            enderezarToolStripMenuItem.Enabled = false;

            if (!correcionSlopeToolStripButton.Enabled && !correcionSlantToolStripButton.Enabled && !palabrasToolStripButton.Enabled && !caracteresToolStripButton.Enabled && !esqueletoToolStripButton.Enabled)
            {
                segmentaciónToolStripMenuItem.Enabled = false;
                manualSegmentacionToolStripMenuItem.Enabled = false;
            }
            
            lineasToolStripButton.Enabled = false;
            lineasToolStripMenuItem.Enabled = false;            
            semiautomaticoToolStripMenuItem.Enabled = false;
            automaticoSegmentacionToolStripMenuItem.Enabled = false;
        }

        public void CerrarImagenSegmentada()
        {            
            estadoImagenSegmentada = EstadoImagen.nula;
            
            if(!lineasToolStripButton.Enabled) 
                segmentaciónToolStripMenuItem.Enabled = false;
            
            correcionSlopeToolStripButton.Enabled = false;
            correcionSlopeToolStripMenuItem.Enabled = false;
            correcionSlantToolStripButton.Enabled = false;
            correcionSlantToolStripMenuItem.Enabled = false;
            palabrasToolStripButton.Enabled = false;
            palabrasToolStripMenuItem.Enabled = false;
            caracteresToolStripButton.Enabled = false;
            caracteresToolStripMenuItem.Enabled = false;
            esqueletoToolStripButton.Enabled = false;
            esqueletoToolStripMenuItem.Enabled = false;

            reconocimientoToolStripMenuItem.Enabled = false;
            reconocimientoToolStripButton.Enabled = false;
            ManualToolStripMenuItem.Enabled = false;  
        }

        //Métodos para actualizar barra de estado y bloqueo de menús por hilos background
        public void actualizarBarraEstado(Boolean ejecutando, String tarea)
        {
            if (ejecutando)
            {
                estadoToolStripStatusLabel.Text = "Ejecutando";
                tareaToolStripStatusLabel.Text = tarea;
                barraEstadoProgressBar.Style = ProgressBarStyle.Marquee;
                barraEstadoProgressBar.Visible = true;
            }
            else
            {
                estadoToolStripStatusLabel.Text = "Listo";
                if (conometro.Elapsed.Minutes == 0)
                    tareaToolStripStatusLabel.Text = tarea + ": " + conometro.Elapsed.TotalSeconds.ToString("F4") + " s";
                else
                    tareaToolStripStatusLabel.Text = tarea + ": " + conometro.Elapsed.TotalMinutes.ToString("F4") + " min";
                conometro.Reset();

                barraEstadoProgressBar.Style = ProgressBarStyle.Blocks;
                barraEstadoProgressBar.Visible = false;
            }
        }

        public void deshabilitarMenus(String tarea)
        {
            menuStrip.Enabled = false;
            toolStrip.Enabled = false;

            actualizarBarraEstado(true, tarea);
        }

        public void habilitarMenus(String tarea)
        {
            menuStrip.Enabled = true;
            toolStrip.Enabled = true;

            actualizarBarraEstado(false, tarea);
        }     
        //Fin Métodos Auxiliares 
     

        //Evento Closing
        private void Principal_FormClosing(object sender, FormClosingEventArgs e)
        {
            //Finalizamos todo lo necesario al cerrar el Form
            escaner.Finalizar();
        }        
    }
}