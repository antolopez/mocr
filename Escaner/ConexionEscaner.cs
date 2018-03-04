using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using TwainLib;

namespace Escaner
{
    public class ConexionEscaner
    {
        //Atributos
        private Twain escaner;
        private TwProtocol protocoloEscaner;
        private IntPtr handle;


        //Métodos

        //============================================================================
        // NOMBRE: ConexionEscaner
        //
        // DESCRIPCIÓN: Constructor que inicializa los atributos para la gestión del escáner.
        //
        // ARGUMENTOS: IntPtr handle -> Identificador de ventana al que está enlazado el control	
        //
        // SALIDA: -
        //============================================================================
        public ConexionEscaner(IntPtr handle)
        {
            escaner = new Twain();
            protocoloEscaner = new TwProtocol();
            this.handle = handle;
        }


        //============================================================================
        // NOMBRE: Inicializar
        //
        // DESCRIPCIÓN: Prepara el escáner para su uso.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: Booleano que indica si la inicialización del escáner se ha producido correctamente
        //============================================================================
        public bool Inicializar()
        {
            TwRC existeTwain = escaner.Init(handle);
            
            if (existeTwain == TwRC.Success)
                return true;//Existe escaner
            else
                return false;//No existe escaner o se ha producido algún error
        }


        //============================================================================
        // NOMBRE: SeleccionarEscaner
        //
        // DESCRIPCIÓN: Permite seleccionar un escáner de entre todos los instalados en el sistema.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: Booleano que indica si la selección del escáner se ha realizado correctamente
        //============================================================================
        public bool SeleccionarEscaner()
        {
            TwRC accionUsuario = escaner.Select();

            if (accionUsuario == TwRC.Success)
                return true;//Escaner seleccionado correctamente
            else
                return false;//El usuario ha cancelado la selección o bien se ha producido algún error
        }


        //============================================================================
        // NOMBRE: Adquirir
        //
        // DESCRIPCIÓN: Muestra la ventana de opciones del escáner.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: Booleano que indica si el escáner es accesible
        //============================================================================
        public bool Adquirir()
        {
            TwRC escanerAccesible = escaner.Acquire();

            if (escanerAccesible == TwRC.Success)
                return true;
            else
                return false;
        }


        //============================================================================
        // NOMBRE: TransferirImagenes
        //
        // DESCRIPCIÓN: Ordena la transferencia de la imágen/esMuestra la ventana de opciones del escáner.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: ArrayList que contiene todas las imágenes escaneadas
        //============================================================================
        public ArrayList TransferirImagenes()
        {
            return escaner.TransferPictures();
        }


        //============================================================================
        // NOMBRE: RecibirMensaje
        //
        // DESCRIPCIÓN: Recibe un mensaje devolviendo un comando.
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: Comando realizado/a realizar
        //============================================================================
        public TwainCommand RecibirMensaje(ref Message m)
        {
            return escaner.PassMessage(ref m);
        }


        //============================================================================
        // NOMBRE: CerrarConexion
        //
        // DESCRIPCIÓN: Cierra la conexión con el escáner (al terminar una transacción). 
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: -
        //============================================================================
        public void CerrarConexion()
        {
            escaner.CloseSrc();
        }


        //============================================================================
        // NOMBRE: Finalizar
        //
        // DESCRIPCIÓN: Elimina cualquier referencia al escáner(cuando ya no lo vamos a usar).
        //
        // ARGUMENTOS: -	
        //
        // SALIDA: -
        //============================================================================
        public void Finalizar()
        {
            escaner.Finish();
        }    
        
    }
}
